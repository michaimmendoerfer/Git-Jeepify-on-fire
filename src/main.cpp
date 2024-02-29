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
#include <Preferences.h>
#include "CST816D.h"
#include "peers.h"
#include "pref_manager.h"
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

bool DebugMode = true;
bool SleepMode = false;
bool ReadyToPair = false;
bool ChangesSaved = true;

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

#pragma endregion Globals
#pragma region Main
void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) 
{
    struct_Peer *Peer = NULL;

    char* buff = (char*) incomingData;   
    StaticJsonDocument<500> doc; 
    String jsondata = String(buff); 
    
    String BufS; char Buf[50] = {};
    
    jsondataBuf = jsondata;
    PrepareJSON();
    
    DeserializationError error = deserializeJson(doc, jsondata);

    if (!error) // erfolgreich JSON
    {
        Peer = FindPeerByMAC(mac);
        TSMsgRcv = millis();

        if (Peer)      // Peer bekannt
        { 
            Peer->TSLastSeen = millis();
            Serial.print("bekannter Node: "); Serial.print(Peer->Name); Serial.print(" - "); Serial.println(Peer->TSLastSeen);
            
            if (isBat(Peer)) TSMsgBat = TSMsgRcv;
            if (isPDC(Peer)) TSMsgPDC = TSMsgRcv;

            if (doc["Pairing"] == "add me") { SendPairingConfirm(Peer); }
            else {
                for (int i=0; i<MAX_PERIPHERALS; i++) 
                {
                    if (doc.containsKey(Peer->Periph[i].Name)) {
                        float TempSensor = (float)doc[Peer->Periph[i].Name];
                        
                        Serial.print(Peer->Periph[i].Name); Serial.print(" found = "); Serial.println(TempSensor);
                        
                        if (TempSensor != Peer->Periph[i].Value) {
                            Peer->Periph[i].Value = TempSensor;
                            Peer->Periph[i].Changed = true;
                        }
                    }

                    if (doc.containsKey("Status")) 
                    {
                        int Status = doc["Status"];
                        Peer->DebugMode   = (bool) bitRead(Status, 0);
                        Peer->SleepMode   = (bool) bitRead(Status, 1);
                        Peer->DemoMode    = (bool) bitRead(Status, 2);
                        Peer->ReadyToPair = (bool) bitRead(Status, 3);
                    } 
                } 
            }
        } 
        else           // Peer unbekannt 
        {        
            if ((doc["Pairing"] == "add me") and (ReadyToPair)) // neuen Peer registrieren
            { 
                Peer = FindEmptyPeer();
                
                if (Peer) {
                    strcpy(Peer->Name, doc["Node"]);
                    Peer->Type = doc["Type"];

                    Peer->Id = FindHighestPeerId()+1; // PeerId starts with 1;
                    
                    Serial.print("vergebene Id=");    Serial.println(Peer->Id);
                    
                    for (int b = 0; b < 6; b++ ) Peer->BroadcastAddress[b] = mac[b];
                    Serial.println();

                    Peer->TSLastSeen = millis();
                    
                    for (int Si=0; Si<MAX_PERIPHERALS; Si++) {
                        snprintf(Buf, sizeof(Buf), "T%d", Si);        
                        Serial.print("Check Pairing for: "); Serial.println(Buf);
                        
                        if (doc.containsKey(Buf)) 
                        {
                            Serial.print("Pairing found: "); Serial.println(Buf);
                            Peer->Periph[Si].Type = doc[Buf];
                            
                            snprintf(Buf, sizeof(Buf), "N%d", Si);      // N0
                            strcpy(Peer->Periph[Si].Name, doc[Buf]);
                            Serial.printf("Peer->Periph[%s].Name is now: %s\n", Si, Peer->Periph[Si].Name);
                        
                            Peer->Periph[Si].Id = Si+1; //PeriphId starts with 1
                            Peer->Periph[Si].PeerId = Peer->Id;
                        }
                    }   
                    // alle Periphs eingetragen in Peer->Periph[SNr] (Name+Type)
                    
                    //Report
                    Serial.printf("Zeiger-Aufruf: %s: ", Peer->Name);
                    for (int i=0; i<MAX_PERIPHERALS; i++) 
                    {
                        Serial.printf("%d-%s (%d) - &Periph.Name: ", i, Peer->Periph[i].Name, Peer->Periph[i].Type) ;
                        Serial.println((unsigned)(&Peer.Periph[i].Name[0]), HEX);
                    }
                    
                    ReportAll();
                    SavePeers();
                    RegisterPeers();
                    SendPairingConfirm(Peer);
                    
                    ReadyToPair = false; TSPair = 0;
                }
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
  DebugMode = preferences.getBool("DebugMode", true);
  SleepMode = preferences.getBool("SleepMode", false);
  preferences.end();

  for (int s=0; s<MULTI_SCREENS; s++) {
    Screen[s].Id = s;
    snprintf(Screen[s].Name, sizeof(Screen[s].Name), "Scr-%d", s);
    Screen[s].Used = false;
    for (int p=0; p<PERIPH_PER_SCREEN; p++) {
      Screen[s].PeriphId[p] = 0;
      Screen[s].Periph[p]   = NULL;
      Screen[s].PeerId[p]   = 0;
      Screen[s].Peer[p]     = NULL;
    }
  }

  for (int PNr=0; PNr<MAX_PEERS; PNr++) {
    P[PNr].Id = 0;
    P[PNr].Type = 0;
    P[PNr].Name[0] = '\0';
    P[PNr].PNumber = PNr;
    for (int i; i<6; i++) P[PNr].BroadcastAddress[i] = 0;
    for (int SNr=0; SNr<MAX_PERIPHERALS; SNr++) {
      P[PNr].Periph[SNr].Name[0] = '\0';
      P[PNr].Periph[SNr].Id      = 0;
      P[PNr].Periph[SNr].Type    = 0;
      P[PNr].Periph[SNr].PeerId  = 0;
    }
  }

  GetPeers();
  RegisterPeers();
  ReportAll();
  
  if (GetPeerCount() == 0) { Serial.println("PeerCount=0, RTP=True"); ReadyToPair = true; TSPair = millis();}
    
  static uint32_t user_data = 10;
  lv_timer_t * TimerPing = lv_timer_create(SendPing, PING_INTERVAL,  &user_data);

  ui_init(); 
}
void loop() 
{
  lv_timer_handler(); /* let the GUI do its work */
  delay(5);
}
#pragma endregion Main
void MultiScreenAddPeriph(struct_Periph *Periph, uint8_t Pos)
{
    Screen[ActiveMultiScreen].Periph[Pos]   = Periph;
    Screen[ActiveMultiScreen].PeriphId[Pos] = Periph->Id;
    Screen[ActiveMultiScreen].Peer[Pos]     = FindPeerById(Periph->PeerId);
    Screen[ActiveMultiScreen].PeerId[Pos]   = Periph->PeerId;
    Screen[ActiveMultiScreen].Used          = true;
}
#pragma region Timer-Thing
#pragma endregion Timer-Things
#pragma region Send-Things
void SendPing(lv_timer_t * timer) {
  StaticJsonDocument<500> doc;
  String jsondata;
  jsondata = "";  
  doc.clear();
  
  doc["Node"] = NODE_NAME;   
  doc["Order"] = "stay alive";

  if (ReadyToPair) {
    doc["Pairing"] = "aktiv";
  }

  serializeJson(doc, jsondata);  
  for (int PNr=0; PNr<MAX_PEERS; PNr++) {
    if (P[PNr].Type) esp_now_send(P[PNr].BroadcastAddress, (uint8_t *) jsondata.c_str(), 100);  
  }
}
void SendPairingConfirm(struct_Peer *Peer) {
  StaticJsonDocument<500> doc;
  String jsondata;
  jsondata = "";  doc.clear();
              
  doc["Node"]     = NODE_NAME;   
  doc["Peer"]     = Peer->Name;
  doc["Pairing"]  = "you are paired";
  doc["Type"]     = MONITOR_ROUND;
  doc["B0"]       = Peer->BroadcastAddress[0];
  doc["B1"]       = Peer->BroadcastAddress[1];
  doc["B2"]       = Peer->BroadcastAddress[2];
  doc["B3"]       = Peer->BroadcastAddress[3];
  doc["B4"]       = Peer->BroadcastAddress[4];
  doc["B5"]       = Peer->BroadcastAddress[5];

  serializeJson(doc, jsondata);  
  Serial.println("prepared... sending you are paired");
  Serial.println(jsondata);
  esp_now_send(Peer->BroadcastAddress, (uint8_t *) jsondata.c_str(), 200); 
  Serial.print("Sent you are paired"); 
  Serial.println(jsondata);         
}
bool ToggleSwitch(struct_Periph *Periph) {
   if (!Periph) return false;
  StaticJsonDocument<500> doc;
  String jsondata;
  jsondata = "";  //clearing String after data is being sent
  doc.clear();
  
  doc["from"]  = NODE_NAME;   
  doc["Order"] = "ToggleSwitch";
  doc["Value"] = Periph->Name;
  
  serializeJson(doc, jsondata);  
  
  esp_now_send(FindPeerById(Periph->PeerId)->BroadcastAddress, (uint8_t *) jsondata.c_str(), 100);  //Sending "jsondata"  
  Serial.println(jsondata);
  
  jsondata = "";
  return true;
}
void SendCommand(struct_Peer *Peer, String Cmd) {
  StaticJsonDocument<500> doc;
  String jsondata;
  jsondata = "";  //clearing String after data is being sent
  doc.clear();
  
  doc["from"] = NODE_NAME;   
  doc["Order"] = Cmd;
  
  serializeJson(doc, jsondata);  
  
  esp_now_send(Peer->BroadcastAddress, (uint8_t *) jsondata.c_str(), 100);  //Sending "jsondata"  
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

bool ToggleSleepMode() {
  preferences.begin("JeepifyInit", false);
    SleepMode = !SleepMode;
    if (preferences.getBool("SleepMode", false) != SleepMode) preferences.putBool("SleepMode", SleepMode);
  preferences.end();
  return SleepMode;
}

bool ToggleDebugMode() {
  preferences.begin("JeepifyInit", false);
    DebugMode = !DebugMode;
    if (preferences.getBool("DebugMode", false) != DebugMode) preferences.putBool("DebugMode", DebugMode);
  preferences.end();
  if (DebugMode) { Serial.print("DebugMode changed to: "); Serial.println((bool)DebugMode);}
  return DebugMode;
}

bool TogglePairMode() {
  if (ReadyToPair) {
    ReadyToPair = false;
    TSPair = 0;
  }
  else {
    ReadyToPair = true;
    TSPair = millis();
  };

  if (DebugMode) { Serial.print("ReadyToPair changed to: "); Serial.println(ReadyToPair); }
  
  return ReadyToPair;
}

void CalibVolt() {
  StaticJsonDocument<500> doc;
  String jsondata;

  jsondata = "";  
  doc.clear();

  doc["Node"] = NODE_NAME;   
  doc["Order"] = "VoltCalib";
  doc["Value"] = lv_textarea_get_text(ui_TxtVolt);
  
  serializeJson(doc, jsondata);  

  esp_now_send(ActivePeer->BroadcastAddress, (uint8_t *) jsondata.c_str(), 100);  //Sending "jsondata"  
  if (DebugMode) Serial.println(jsondata);
}

void PrintMAC(const uint8_t * mac_addr){
  char macStr[18];
  snprintf(macStr, sizeof(macStr), "%02x:%02x:%02x:%02x:%02x:%02x",
           mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5]);
  Serial.print(macStr);
}
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) { 
  if (DebugMode) {
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
