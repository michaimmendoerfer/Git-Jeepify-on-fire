#define NODE_NAME "Monitor-2"
#define NODE_TYPE MONITOR_ROUND

#define VERSION   "V 2.51"

#pragma region Includes
#include <Arduino.h>
#include "jeepify.h"
#include "main.h"
#include <TFT_eSPI.h>
#include <esp_now.h>
#include <WiFi.h>
#include <ArduinoJson.h>
#include "CST816D.h"
#include "pref_manager.h"
#include "PeerClass.h"
#include "LinkedList.h"
#include <lvgl.h>
#include "Ui\ui.h"
#include "Ui\ui_events.h" 
#pragma endregion Includes

#define TFT_HOR_RES   240
#define TFT_VER_RES   240
#define DRAW_BUF_SIZE (TFT_HOR_RES * TFT_VER_RES / 10 * (LV_COLOR_DEPTH / 8))

TFT_eSPI tft = TFT_eSPI(TFT_HOR_RES, TFT_VER_RES); /* TFT instance */
CST816D Touch(I2C_SDA, I2C_SCL, TP_RST, TP_INT);

#pragma region Globals
static lv_disp_draw_buf_t draw_buf;
static lv_color_t buf1[ TFT_HOR_RES * TFT_VER_RES / 10 ];

LinkedList<PeerClass*> PeerList = LinkedList<PeerClass*>();
LinkedList<PeriphClass*> PeriphList = LinkedList<PeriphClass*>();

PeerClass Self;

String jsondataBuf;

uint32_t TSPing          = 0;

volatile uint32_t TSMsgRcv  = 0;
volatile uint32_t TSMsgSnd  = 0;
volatile uint32_t TSMsgPDC  = 0;
volatile uint32_t TSMsgBat  = 0;
volatile uint32_t TSMsgVolt = 0;
volatile uint32_t TSMsgEich = 0;
volatile uint32_t TSMsgPair = 0;
volatile uint32_t TSPair    = 0;

lv_timer_t *WDButtonVars;

int ActiveMultiScreen;

#pragma endregion Globals
#pragma region Main
void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) 
{
    PeerClass *P;

    char* buff = (char*) incomingData;   
    StaticJsonDocument<500> doc; 
    String jsondata = String(buff); 
    
    String BufS; char Buf[50] = {};
    
    jsondataBuf = jsondata;
    PrepareJSON();
    
    DeserializationError error = deserializeJson(doc, jsondata);

    if (!error) // erfolgreich JSON
    {
        P = FindPeerByMAC(mac);
        TSMsgRcv = millis();

        if (P)      // Peer bekannt
        { 
            P->SetTSLastSeen(millis());
            Serial.print("bekannter Node: "); Serial.print(P->GetName()); Serial.print(" - "); Serial.println(P->GetTSLastSeen());
            
            //if (isBat(Peer)) TSMsgBat = TSMsgRcv;
            //if (isPDC(Peer)) TSMsgPDC = TSMsgRcv;

            if (doc["Pairing"] == "add me") { SendPairingConfirm(P); }
            else {
                for (int i=0; i<MAX_PERIPHERALS; i++) 
                {
                    if (doc.containsKey((const char*)P->GetPeriphName(i))) {
                        float TempSensor = (float)doc[P->GetPeriphName(i)];
                        
                        Serial.print(P->GetPeriphName(i)); Serial.print(" found = "); Serial.println(TempSensor);
                        
                        if (TempSensor != P->GetPeriphValue(i)) {
                            P->SetPeriphOldValue(i, P->GetPeriphValue(i));
                            P->SetPeriphValue(i, TempSensor);
                            P->SetPeriphChanged(i, true);
                        }
                    }

                    if (doc.containsKey("Status")) 
                    {
                        int Status = doc["Status"];
                        P->SetDebugMode ((bool) bitRead(Status, 0));
                        P->SetSleepMode ((bool) bitRead(Status, 1));
                        P->SetDemoMode  ((bool) bitRead(Status, 2));
                        P->SetPairMode  ((bool) bitRead(Status, 3));
                    } 
                } 
            }
        } 
        else           // Peer unbekannt, ggf Pairing
        {        
            if ((doc["Pairing"] == "add me") and (Self.GetPairMode())) // neuen Peer registrieren
            { 
                int Status = doc["Status"];

                P = new PeerClass();
                PeerList.add(P);

                String PeerName = doc["Node"];

                P->Setup(PeerName.c_str(), (int)doc["Type"], mac, (bool) bitRead(Status, 1), (bool) bitRead(Status, 0), (bool) bitRead(Status, 2), (bool) bitRead(Status, 3));
                P->SetTSLastSeen(millis());
                // Message-Bsp: "Node":"ESP32-1"; "T0":"1"; "N0":"Switch1"
                for (int Si=0; Si<MAX_PERIPHERALS; Si++) {
                    snprintf(Buf, sizeof(Buf), "T%d", Si);                          // Check for T0 (Type of Periph 0)
                    Serial.print("Check Pairing for: "); Serial.println(Buf);
                    
                    if (doc.containsKey(Buf)) 
                    {
                        Serial.print("Pairing found: "); Serial.println(Buf);       // Set Periph[0].Type
                        int  Type = doc[Buf];

                        snprintf(Buf, sizeof(Buf), "N%d", Si);                      // get N0 (Name of Periph 0)
                        String PName = doc[Buf];
                        P->PeriphSetup(Si, PName.c_str(), Type, false, false, 0, 0, 0, P->GetId());
                        PeriphList.add(P->GetPeriphPtr(Si));
                        
                        Serial.printf("%s->Periph[%d].Name is now: %s\n", P->GetName(), Si, P->GetPeriphName(Si));
                    } 
                }
                ReportAll();
                SavePeers();
                ReportAll();
                RegisterPeers();
                SendPairingConfirm(P);
                
                Self.SetPairMode(false); TSPair = 0;
            }
        }
    }
    else {        // Error bei JSON
      Serial.print(F("deserializeJson() failed: ")); 
      Serial.println(error.f_str());
      return;
    }
}
void setup() 
{
    Serial.begin(115000);

    Self.Setup("Monitor_2", MONITOR_ROUND, broadcastAddressAll, false, true, false, false);
    
    //TFT & LVGL
    tft.init();
    tft.setRotation(0);
    tft.setSwapBytes(true);
    tft.begin();
    Touch.begin();
      
    lv_init();
    
    lv_disp_draw_buf_init( &draw_buf, buf1, NULL, TFT_HOR_RES * TFT_VER_RES / 10 );

    //Display-Driver
    static lv_disp_drv_t disp_drv;
    lv_disp_drv_init( &disp_drv );
    disp_drv.hor_res = TFT_HOR_RES;
    disp_drv.ver_res = TFT_VER_RES;
    disp_drv.flush_cb = my_disp_flush;
    disp_drv.draw_buf = &draw_buf;
    lv_disp_drv_register( &disp_drv );

    //Touch-Driver
    static lv_indev_drv_t indev_drv;
    lv_indev_drv_init( &indev_drv );
    indev_drv.type = LV_INDEV_TYPE_POINTER;
    indev_drv.read_cb = my_touchpad_read;
    lv_indev_drv_register( &indev_drv );

    //ESP-Now
    WiFi.mode(WIFI_STA);
    if (esp_now_init() != ESP_OK) { Serial.println("Error initializing ESP-NOW"); return; }

    esp_now_register_send_cb(OnDataSent);
    esp_now_register_recv_cb(OnDataRecv);    

    //Get saved Peers  
    preferences.begin("JeepifyInit", true);
        Self.SetDebugMode(preferences.getBool("DebugMode", true));
        Self.SetSleepMode(preferences.getBool("SleepMode", false));
    preferences.end();
    
    GetPeers();
    RegisterPeers();
    ReportAll();
  
    static uint32_t user_data = 10;
    lv_timer_t * TimerPing = lv_timer_create(SendPing, PING_INTERVAL,  &user_data);

    ui_init(); 
}
void loop() 
{
  lv_timer_handler(); /* let the GUI do its work */
  delay(20);
}
#pragma endregion Main
void MultiScreenAddPeriph(PeriphClass *Periph, uint8_t Pos)
{
    Screen[ActiveMultiScreen].AddPeriph(Pos, Periph);
}

#pragma region Send-Things
void SendPing(lv_timer_t * timer) {
    StaticJsonDocument<500> doc;
    String jsondata;
    jsondata = "";  
    doc.clear();
    PeerClass *P;
    
    doc["Node"] = NODE_NAME;   
    doc["Order"] = "stay alive";

    if (Self.GetPairMode())
    {
        doc["Pairing"] = "aktiv";
    }

    serializeJson(doc, jsondata);  
    
    for (int i=0; i<PeerList.size(); i++)
    {
        P = PeerList.get(i);
        if (P->GetType() > 0) esp_now_send(P->GetBroadcastAddress(), (uint8_t *) jsondata.c_str(), 100);  
    }
}
void SendPairingConfirm(PeerClass *P) {
  StaticJsonDocument<500> doc;
  String jsondata;
  jsondata = "";  doc.clear();
  
  uint8_t *Broadcast = P->GetBroadcastAddress();
  
  doc["Node"]     = Self.GetName();   
  doc["Peer"]     = P->GetName();
  doc["Pairing"]  = "you are paired";
  doc["Type"]     = Self.GetType();
  doc["B0"]       = (uint8_t)Broadcast[0];
  doc["B1"]       = (uint8_t)Broadcast[1];
  doc["B2"]       = (uint8_t)Broadcast[2];
  doc["B3"]       = (uint8_t)Broadcast[3];
  doc["B4"]       = (uint8_t)Broadcast[4];
  doc["B5"]       = (uint8_t)Broadcast[5];

  serializeJson(doc, jsondata);  
  
  esp_now_send(P->GetBroadcastAddress(), (uint8_t *) jsondata.c_str(), 200); 
  if (Self.GetDebugMode())
  {
      Serial.print("Sent you are paired"); 
      Serial.println(jsondata);  
  }       
}
bool ToggleSwitch(PeerClass *P, int PerNr)
{
    StaticJsonDocument<500> doc;
    String jsondata;
    jsondata = "";  //clearing String after data is being sent
    doc.clear();
    
    doc["from"]  = NODE_NAME;   
    doc["Order"] = "ToggleSwitch";
    doc["Value"] = P->GetPeriphName(PerNr);
    
    serializeJson(doc, jsondata);  
    
    esp_now_send(P->GetBroadcastAddress(), (uint8_t *) jsondata.c_str(), 100);  //Sending "jsondata"  
    Serial.println(jsondata);
    
    jsondata = "";
    return true;
}
void SendCommand(PeerClass *P, String Cmd) {
  StaticJsonDocument<500> doc;
  String jsondata;
  jsondata = "";  //clearing String after data is being sent
  doc.clear();
  
  doc["from"]  = Self.GetName();   
  doc["Order"] = Cmd;
  
  serializeJson(doc, jsondata);  
  
  esp_now_send(P->GetBroadcastAddress(), (uint8_t *) jsondata.c_str(), 100);  //Sending "jsondata"  
  Serial.println(jsondata);
  
  jsondata = "";
}
#pragma endregion Send-Things

#pragma region System-Screens
void PrepareJSON() {
  if (jsondataBuf) {
    StaticJsonDocument<500> doc;
  
    DeserializationError error = deserializeJson(doc, jsondataBuf);
    if (doc["Node"] != NODE_NAME) { 
      lv_textarea_set_placeholder_text(ui_TxtJSON1, jsondataBuf.c_str());
      jsondataBuf = "";
    }
  }
}
#pragma endregion System-Screens
#pragma region Other
void WriteStringToCharArray(String S, char *C) {
  int   ArrayLength = S.length()+1;    //The +1 is for the 0x00h Terminator
  S.toCharArray(C,ArrayLength);
}

bool ToggleSleepMode() 
{
    preferences.begin("JeepifyInit", false);
        Self.SetSleepMode(!Self.GetSleepMode());
        if (preferences.getBool("SleepMode", false) != Self.GetSleepMode()) preferences.putBool("SleepMode", Self.GetSleepMode());
    preferences.end();
    return Self.GetSleepMode();
}
bool ToggleDebugMode() {
  preferences.begin("JeepifyInit", false);
      Self.SetDebugMode(!Self.GetDebugMode());
      if (preferences.getBool("DebugMode", false) != Self.GetDebugMode()) preferences.putBool("DebugMode", Self.GetDebugMode());
  preferences.end();
  return Self.GetDebugMode();
}
bool TogglePairMode() {
  if (Self.GetPairMode())
  {
      Self.SetPairMode(false);
      TSPair = 0;
  }
  else 
  {
      Self.SetPairMode(true);
      TSPair = millis();
  };

  if (Self.GetDebugMode()) { Serial.print("PairMode changed to: "); Serial.println(Self.GetPairMode()); }
  
  return Self.GetPairMode();
}
void CalibVolt() {
  StaticJsonDocument<500> doc;
  String jsondata;

  jsondata = "";  
  doc.clear();

  doc["Node"]  = Self.GetName();  
  doc["Order"] = "VoltCalib";
  doc["Value"] = lv_textarea_get_text(ui_TxtVolt);
  
  serializeJson(doc, jsondata);  

  esp_now_send(ActivePeer->GetBroadcastAddress(), (uint8_t *) jsondata.c_str(), 100);  //Sending "jsondata"  
  if (Self.GetDebugMode()) Serial.println(jsondata);
}

void PrintMAC(const uint8_t * mac_addr){
  char macStr[18];
  snprintf(macStr, sizeof(macStr), "%02x:%02x:%02x:%02x:%02x:%02x",
           mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5]);
  Serial.print(macStr);
}
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) { 
    if (Self.GetDebugMode()) {
        Serial.print("\r\nLast Packet Send Status:\t");
        Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
    }
}
void my_disp_flush( lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p )
{
    uint32_t w = ( area->x2 - area->x1 + 1 );
    uint32_t h = ( area->y2 - area->y1 + 1 );

    tft.startWrite();
    tft.setAddrWindow( area->x1, area->y1, w, h );
    tft.pushColors( ( uint16_t * )&color_p->full, w * h, true );
    tft.endWrite();

    lv_disp_flush_ready( disp );
}
void my_touchpad_read( lv_indev_drv_t * indev_driver, lv_indev_data_t * data ) {
    uint16_t touchX, touchY;
    uint8_t  Gesture;

    bool touched = Touch.getTouch( &touchX, &touchY, &Gesture);

    if( !touched ) {
        data->state = LV_INDEV_STATE_RELEASED;
    }
    else {
        data->state = LV_INDEV_STATE_PRESSED;

        data->point.x = TFT_HOR_RES - touchX;
        data->point.y = touchY;

        
        Serial.print( "Data x " );
        Serial.print( touchX );

        Serial.print( ", Data y " );
        Serial.println( touchY );
        
    }
}
#pragma endregion Other
//