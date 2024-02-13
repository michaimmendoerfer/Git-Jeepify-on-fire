#define NODE_NAME "Monitor-1"
#define NODE_TYPE MONITOR_ROUND

#define VERSION   "V 2.01"

#pragma region Includes
#include <Arduino.h>
#include <nvs_flash.h>
#include "../../jeepify.h"
#include <TFT_eSPI.h>
#include <esp_now.h>
#include <WiFi.h>
#include <ArduinoJson.h>
#include <Preferences.h>
#include "CST816D.h"
#include <lvgl.h>
#include "Ui\ui.h" 
#define TFT_HOR_RES   240
#define TFT_VER_RES   240
#define DRAW_BUF_SIZE (TFT_HOR_RES * TFT_VER_RES / 10 * (LV_COLOR_DEPTH / 8))

#pragma endregion Includes
#pragma region Function_Definitions
TFT_eSPI tft = TFT_eSPI(TFT_HOR_RES, TFT_VER_RES); /* TFT instance */
CST816D Touch(I2C_SDA, I2C_SCL, TP_RST, TP_INT);

void my_disp_flush( lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p );
void my_touchpad_read( lv_indev_drv_t * indev_driver, lv_indev_data_t * data );
static void btn_event_cb(lv_event_t * e);

void   OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status);
void   OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len);

void   SavePeers();
void   GetPeers();
void   RegisterPeers();
void   ClearPeers();
void   ClearInit();
void   ReportAll();

void   SendPing();
bool   ToggleSwitch(struct_Periph *Periph);
void   SendCommand(struct_Peer *Peer, String Cmd);
void   SendPairingConfirm(struct_Peer *Peer);

void   SetSleepMode(bool Mode);
void   SetDebugMode(bool Mode);

void   ShowMessage(String Msg);
void   ShowSingle(struct_Periph *Periph);
void   ShowMulti(struct_MultiScreen *ActiveScreen);
void   ShowEichenVolt();
void   ShowJSON();
void   ShowPeer();

void   PrintMAC(const uint8_t * mac_addr);
void WriteStringToCharArray(String S, char *C);
#pragma endregion Function_Definitions
#pragma region Globals
static lv_disp_draw_buf_t draw_buf;
static lv_color_t buf[ screenWidth * screenHeight / 10 ];

struct_MultiScreen Screen[MULTI_SCREENS];
int ActiveMultiScreen = 0; 
int PeriphToFill;


Preferences preferences;

struct_Peer   *ActivePeer, *ActivePDC, *ActiveBat, *ActiveSelection;
struct_Periph *ActiveSens, *ActiveSwitch, *ActivePeriph;

int PeerCount    =  0;

bool DebugMode = true;
bool SleepMode = false;
bool ReadyToPair = false;
bool ChangesSaved = true;

String jsondataBuf;

uint32_t TSPing          = 0;
uint32_t TSMsgStart      = 0;

volatile uint32_t TSMsgRcv  = 0;
volatile uint32_t TSMsgPDC  = 0;
volatile uint32_t TSMsgBat  = 0;
volatile uint32_t TSMsgVolt = 0;
volatile uint32_t TSMsgEich = 0;
volatile uint32_t TSMsgPair = 0;
volatile uint32_t TSPair    = 0;

bool MsgBatAktiv  = false;
bool MsgPDCAktiv  = false;
bool MsgVoltAktiv = false;
bool MsgEichAktiv = false;
bool MsgPairAktiv = false;


#pragma endregion Globals
#pragma region Main
void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
  char* buff = (char*) incomingData;   
  StaticJsonDocument<500> doc; 
  String jsondata = String(buff); 
  
  String BufS; char Buf[50] = {};
  
  Serial.print("Recieved from:"); PrintMAC(mac); Serial.println(); Serial.println(jsondata);
  jsondataBuf = jsondata;
  DeserializationError error = deserializeJson(doc, jsondata);

  if (!error) {
    struct_Peer *Peer = FindPeerByMAC(mac);
    TSMsgRcv = millis();

    if (Peer) { // Peer bekannt 
      Peer->TSLastSeen = millis();
      Serial.print("bekannter Node: "); Serial.print(Peer->Name); Serial.print(" - "); Serial.println(Peer->TSLastSeen);
      
      if (isBat(Peer)) TSMsgBat = TSMsgRcv;
      if (isPDC(Peer)) TSMsgPDC = TSMsgRcv;
      
      if (doc["Pairing"] == "add me") { SendPairingConfirm(Peer); }
      else {
        for (int i=0; i<MAX_PERIPHERALS; i++) {
          if (doc.containsKey(Peer->Periph[i].Name)) {
            float TempSensor = (float)doc[Peer->Periph[i].Name];
        
            if (TempSensor != Peer->Periph[i].Value) {
              Peer->Periph[i].Value = TempSensor;
              Peer->Periph[i].Changed = true;
            }
          }
          if (doc.containsKey("Status")) {
            int Status = doc["Status"];
            Peer->DebugMode   = (bool) bitRead(Status, 0);
            Peer->SleepMode   = (bool) bitRead(Status, 1);
            Peer->DemoMode    = (bool) bitRead(Status, 2);
            Peer->ReadyToPair = (bool) bitRead(Status, 3);
          } 
        } 
      }
    } 
    else {      // Peer unbekannt 
      if ((doc["Pairing"] == "add me") and (ReadyToPair)) { // neuen Peer registrieren
        Peer = FindEmptyPeer();
        
        if (Peer) {
          strcpy(Peer->Name, doc["Node"]);
          Peer->Type = doc["Type"];

          Peer->Id = FindHighestPeerId()+1; // PeerId starts with 1;
          
          Serial.print("vergebene Id=");    Serial.println(Peer->Id);
          //Serial.print("vergebener Slot="); Serial.println(Peer->Slot);

          for (int b = 0; b < 6; b++ ) Peer->BroadcastAddress[b] = mac[b];
          Serial.println();

          Peer->TSLastSeen = millis();
          
          for (int Si=0; Si<MAX_PERIPHERALS; Si++) {
            snprintf(Buf, sizeof(Buf), "T%d", Si);        // Type0
            if (doc.containsKey(Buf)) {
              Peer->Periph[Si].Type = doc[Buf];
              snprintf(Buf, sizeof(Buf), "N%d", Si);      // Name0
              strcpy(Peer->Periph[Si].Name, doc[Buf]);
              Peer->Periph[Si].Id = Si+1; //PeriphId starts with 1
              Peer->Periph[Si].PeerId = Peer->Id;
            }
          }   
          SavePeers();
          RegisterPeers();
          
          SendPairingConfirm(Peer);
          
          ReadyToPair = false; TSPair = 0; Mode = S_MENU;
        }
      }
    }
  }
  else {        // Error
    Serial.print(F("deserializeJson() failed: ")); 
    Serial.println(error.f_str());
    return;
  }
}
void setup() {
  tft.init();
  tft.setRotation(0);
  tft.setSwapBytes(true);
  tft.fillScreen(TFT_RED);
  delay(1000);
  tft.begin();
  
  String LVGL_Arduino = "Hello Arduino! ";
  LVGL_Arduino += String('V') + lv_version_major() + "." + lv_version_minor() + "." + lv_version_patch();

  Serial.begin(74880);
  Serial.println( LVGL_Arduino );

  lv_init();
  
  lv_disp_draw_buf_init( &draw_buf, buf, NULL, screenWidth * screenHeight / 10 );

  static lv_disp_drv_t disp_drv;
  lv_disp_drv_init( &disp_drv );
  /*Change the following line to your display resolution*/
  disp_drv.hor_res = TFT_HOR_RES;
  disp_drv.ver_res = TFT_VER_RES;
  disp_drv.flush_cb = my_disp_flush;
  disp_drv.draw_buf = &draw_buf;
  lv_disp_drv_register( &disp_drv );

  static lv_indev_drv_t indev_drv;
  lv_indev_drv_init( &indev_drv );
  indev_drv.type = LV_INDEV_TYPE_POINTER;
  indev_drv.read_cb = my_touchpad_read;
  lv_indev_drv_register( &indev_drv );

  //------------------
  ui_init();

  WiFi.mode(WIFI_STA);
  if (esp_now_init() != ESP_OK) { Serial.println("Error initializing ESP-NOW"); return; }

  esp_now_register_send_cb(OnDataSent);
  esp_now_register_recv_cb(OnDataRecv);    
  
  //TFT.drawString("ESP_Now launched", 120,120);
  
  preferences.begin("JeepifyInit", true);
  DebugMode = preferences.getBool("DebugMode", true);
  SleepMode = preferences.getBool("SleepMode", false);
  preferences.end();

  Touch.begin();
  //TFT.drawString("Touch ready", 120,140);
  
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
  
  //TFT.drawString("Peers registered", 120,160);
  
  if (PeerCount == 0) { Serial.println("PeerCount=0, RTP=True"); ReadyToPair = true; TSPair = millis();}

}
void loop() {
  lv_timer_handler(); /* let the GUI do its work */
  delay(5);
}
#pragma endregion Main
#pragma region Peer-Things
void ReportAll() {
  char Buf[100];
  String BufS;
  Serial.println("Report-All");
  preferences.begin("JeepifyPeers", true);
  
  for (int PNr=0; PNr< MAX_PEERS; PNr++) {
    snprintf(Buf, sizeof(Buf), "%d:%s(%d) - ID:%d -  ", PNr, P[PNr].Name, P[PNr].Type, P[PNr].Id);
    Serial.print(Buf);
    for (int Si=0; Si<MAX_PERIPHERALS; Si++) {
      snprintf(Buf, sizeof(Buf), "P%d:%s(%d), ", Si, P[PNr].Periph[Si].Name, P[PNr].Periph[Si].Type);
      Serial.print(Buf);
    }
    Serial.println();
  }
  
  for (int s=0; s<MULTI_SCREENS; s++) {
    (Screen[s].Used) ? Serial.println("used") : Serial.println("not used");
    if (Screen[s].Used) {
      snprintf(Buf, sizeof(Buf), "S%d:%s, Id=%d - ", s, Screen[s].Name, Screen[s].Id); Serial.println(Buf);
      for (int p=0; p<4; p++) {
        if (Screen[s].Periph[p]) {
          
          Serial.print(p);
          snprintf(Buf, sizeof(Buf), ": PeerId=%d, PeriphId=%d, PeriphName=%s", Screen[s].Periph[p]->PeerId, Screen[s].PeriphId[p], Screen[s].Periph[p]->Name);
          Serial.println(Buf);
        }
      }
      Serial.println();
    }
  }
  preferences.end();
}
void SavePeers() {
  Serial.println("SavePeers...");
  preferences.begin("JeepifyPeers", false);
  
  char Buf[50] = {}; String BufS;
  
  for (int PNr=0; PNr< MAX_PEERS; PNr++) {
    snprintf(Buf, sizeof(Buf), "P%d", PNr);
    Serial.print("Sizeof()"); Serial.print(PNr); Serial.print(") = "); Serial.println(sizeof(P[PNr]));
    preferences.putBytes(Buf, &P[PNr], sizeof(P[PNr]));
  }

  Serial.println("jetzt kommt Multi");

  for (int s=0; s<MULTI_SCREENS; s++) {
    snprintf(Buf, sizeof(Buf), "S%d", s);
    preferences.putBytes(Buf, &Screen[s], sizeof(Screen[s]));
  }
  preferences.end();
}
void GetPeers() {
  preferences.begin("JeepifyPeers", true);
  
  char Buf[50] = {}; 
  PeerCount = 0;

  for (int PNr=0; PNr<MAX_PEERS; PNr++) {
    snprintf(Buf, sizeof(Buf), "P%d", PNr); 
    //Serial.printf("Sizeof(P[%d]=", sizeof(P[PNr]));
    preferences.getBytes(Buf, &P[PNr], sizeof(P[PNr]));
    if (P[PNr].Id) PeerCount++;
    
    
    for (int SNr=0; SNr<MAX_PERIPHERALS; SNr++) {
      P[PNr].Periph[SNr].PeerId = P[PNr].Id;

      if (DebugMode) {
        snprintf(Buf, sizeof(Buf), "Periph %d: Name=%s, Type=%d, Id=%d, PeerId=%d", SNr, P[PNr].Periph[SNr].Name, P[PNr].Periph[SNr].Type, P[PNr].Periph[SNr].Id, P[PNr].Periph[SNr].PeerId);
        Serial.println(Buf);

        if (isSensor(&P[PNr].Periph[SNr]) and (ActiveSens   == NULL)) ActiveSens   = &P[PNr].Periph[SNr];
        if (isSwitch(&P[PNr].Periph[SNr]) and (ActiveSwitch == NULL)) ActiveSwitch = &P[PNr].Periph[SNr];
      }
    }
  }
  Serial.println("Multi-getpeer");
  for (int s=0; s<MULTI_SCREENS; s++) {
    snprintf(Buf, sizeof(Buf), "S%d", s);
    preferences.getBytes(Buf, &Screen[s], sizeof(Screen[s]));
    
    for (int p=0; p<PERIPH_PER_SCREEN; p++) {      
      if (Screen[s].PeerId[p]) {
        Screen[s].Peer[p]   = FindPeerById(Screen[s].PeerId[p]);
        Serial.print("Peer[p]=");   Serial.println(Screen[s].Peer[p]->Name);
      }
      if (Screen[s].PeriphId[p]) {
        Screen[s].Periph[p] = FindPeriphById(Screen[s].Peer[p], Screen[s].PeriphId[p]);
        Serial.print("Periph[p]="); Serial.println(Screen[s].Periph[p]->Name);
      }
    }  
  }
  preferences.end();
}
void RegisterPeers() {
  esp_now_peer_info_t peerInfo;
  peerInfo.channel = 1;
  peerInfo.encrypt = false;
  memset(&peerInfo, 0, sizeof(peerInfo));

  // Register BROADCAST
  for (int b=0; b<6; b++) peerInfo.peer_addr[b] = (uint8_t) broadcastAddressAll[b];
    if (esp_now_add_peer(&peerInfo) != ESP_OK) {
      PrintMAC(peerInfo.peer_addr); Serial.println(": Failed to add peer");
    }
    else {
      Serial.print (" ("); PrintMAC(peerInfo.peer_addr);  Serial.println(") added...");
    }

  // Register Peers
  for (int PNr=0; PNr<MAX_PEERS; PNr++) {
    if (P[PNr].Type > 0) {
      for (int b=0; b<6; b++) peerInfo.peer_addr[b] = (uint8_t) P[PNr].BroadcastAddress[b];
        if (esp_now_add_peer(&peerInfo) != ESP_OK) {
          PrintMAC(peerInfo.peer_addr); Serial.println(": Failed to add peer");
        }
        else {
          Serial.print("Peer: "); Serial.print(P[PNr].Name); 
          Serial.print (" ("); PrintMAC(peerInfo.peer_addr); Serial.println(") added...");
        }
    }
  }
}
void ClearPeers() {
  preferences.begin("JeepifyPeers", false);
    preferences.clear();
    Serial.println("JeepifyPeers cleared...");
  preferences.end();
}
void DeletePeer(struct_Peer *Peer) {
  for (int PNr=0; PNr<MAX_PEERS; PNr++) {
    if (P[PNr].Id == Peer->Id) {
      P[PNr].Name[0] = '\0';
      P[PNr].Id = 0;
      P[PNr].Type = 0;
      for (int i; i<6; i++) P[PNr].BroadcastAddress[i] = 0;
      P[PNr].TSLastSeen = 0;
      P[PNr].SleepMode = false;
      P[PNr].DebugMode = false;
    }
  }
  SendCommand(Peer, "Reset");
  
  for (int s=0; s<MULTI_SCREENS; s++) {
    for (int p=0; p<PERIPH_PER_SCREEN; p++) {
      if (Screen[s].PeerId[p] == Peer->Id) {
        Screen[s].PeerId[p] = 0;
        Screen[s].PeriphId[p] = 0;
        Screen[s].Periph[p] = NULL;
      }
    } 
  }
}
#pragma endregion Peer-Things
#pragma region Send-Things
void SendPing() {
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
#pragma region Sensor-Screens
#pragma endregion Sensor-Screens
#pragma region System-Screens
void ShowJSON() {
  StaticJsonDocument<500> doc;
  Serial.println("Show-JSON");
  if ((jsondataBuf != "") or (Mode != OldMode)) {
    ScreenChanged = true;
    OldMode = Mode;

    TFTBuffer.pushImage(0,0, 240, 240, JeepifyBackground);  
    TFTBuffer.loadFont(AA_FONT_SMALL);
    
    TFTBuffer.setTextColor(TFT_RUBICON, TFT_BLACK);
    TFTBuffer.setTextDatum(TC_DATUM);
    
    TFTBuffer.drawString("Debug JSON", 120, 200); 
    
    DeserializationError error = deserializeJson(doc, jsondataBuf);
    if (doc["Node"] != NODE_NAME) { 
      int sLength = jsondataBuf.length();

      if (sLength) {
        int len = 20;
        int Abstand = 20;
        int sLength = jsondataBuf.length();

        TFTBuffer.setTextDatum(MC_DATUM);
        TFTBuffer.setTextColor(TFT_WHITE, TFT_BLACK);
      
        for (int i=0 ; i<8 ; i++) {
          if ((i+1)*len < sLength)  TFTBuffer.drawString(jsondataBuf.substring(i*len, (i+1)*len), 120,50+i*Abstand); 
          else if (i*len < sLength) TFTBuffer.drawString(jsondataBuf.substring(i*len, sLength), 120,50+i*Abstand); 
        }
        jsondataBuf = "";
      }
    }
    TSScreenRefresh = millis();
  }
}
void ShowPeer() {
  if ((TSScreenRefresh - millis() > SCREEN_INTERVAL) or (Mode != OldMode)) {
    OldMode = Mode;
    ScreenChanged = true;

    TFTBuffer.pushImage(0,0, 240, 240, JeepifyBackground);  
    TFTBuffer.loadFont(AA_FONT_SMALL);
    
    TFTBuffer.setTextColor(TFT_RUBICON, TFT_BLACK);
    TFTBuffer.setTextDatum(TC_DATUM);
    
    TFTBuffer.drawString(ActivePeer->Name, 120, 200); 

    DrawButton(5);
    DrawButton(6);
    DrawButton(7);
    DrawButton(8);
    DrawButton(9);
    DrawButton(10, ActivePeer->SleepMode);
    DrawButton(14, ActivePeer->DemoMode);

    TSScreenRefresh = millis();
  }
}
void ShowPeers() {
    
    for (int PNr=0 ; PNr<MAX_PEERS ; PNr++) {
      if (P[PNr].Type) {
        TFTBuffer.setTextColor(TFT_WHITE, TFT_BLACK);
        TFTBuffer.drawString(P[PNr].Name, 10, 80+PNr*Abstand);
        switch (P[PNr].Type) {
          case SWITCH_1_WAY:   ZName = "1-way PDC";   break;
          case SWITCH_2_WAY:   ZName = "2-way PDC";   break;
          case SWITCH_4_WAY:   ZName = "4-way PDC";   break;
          case SWITCH_8_WAY:   ZName = "8-way PDC";   break;
          case PDC_SENSOR_MIX: ZName = "PDC-MIX";     break;
          case BATTERY_SENSOR: ZName = "Batt.-Sens."; break;
        }
        TFTBuffer.drawString(ZName, 105, 80+PNr*Abstand);
        
        if (millis()- P[PNr].TSLastSeen > OFFLINE_INTERVAL) { 
          TFTBuffer.setTextColor(TFT_DARKGREY,  TFT_BLACK); 
          TFTBuffer.drawString("off", 200, 80+PNr*Abstand);
        }
        else { 
          TFTBuffer.setTextColor(TFT_DARKGREEN, TFT_BLACK); 
          TFTBuffer.drawString("on",  200, 80+PNr*Abstand); 
        }
      }
    }
    TSScreenRefresh = millis();
  }
}
#pragma endregion System-Screens

#pragma region Touch-Things
#pragma endregion Touch-Things
#pragma region Other
void WriteStringToCharArray(String S, char *C) {
  int   ArrayLength = S.length()+1;    //The +1 is for the 0x00h Terminator
  S.toCharArray(C,ArrayLength);
}
void SetSleepMode(bool Mode) {
  preferences.begin("JeepifyInit", false);
    SleepMode = Mode;
    if (preferences.getBool("SleepMode", false) != SleepMode) preferences.putBool("SleepMode", SleepMode);
  preferences.end();
}
void SetDebugMode(bool Mode) {
  preferences.begin("JeepifyInit", false);
    DebugMode = Mode;
    if (preferences.getBool("DebugMode", false) != DebugMode) preferences.putBool("DebugMode", DebugMode);
  preferences.end();
}

void AddVolt(int i) {
  StaticJsonDocument<500> doc;
  String jsondata;
  jsondata = "";  //clearing String after data is being sent
  doc.clear();

  if ((i>=0) and (i<=9)) {
    switch (VoltCount) {
      case 0: VoltCalib  = (i)*10;          VoltCount++; break;
      case 1: VoltCalib += (i);             VoltCount++; break;
      case 2: VoltCalib += (float) (i)/10;  VoltCount++; break;
      case 3: VoltCalib += (float) (i)/100; VoltCount++; break;
    }
  } 
  else if (i==11) VoltCount++;
  
  if (VoltCount == 4) {
    char buf[10];
    dtostrf(VoltCalib, 5, 2, buf);

    doc["Node"] = NODE_NAME;   
    doc["Order"] = "VoltCalib";
    doc["Value"] = buf;
    
    serializeJson(doc, jsondata);  
  
    esp_now_send(ActivePeer->BroadcastAddress, (uint8_t *) jsondata.c_str(), 100);  //Sending "jsondata"  
    Serial.println(jsondata);
  
    Serial.println(jsondata);

    TSMsgVolt = millis();
    Mode = S_MENU;
  }
}
void PrintMAC(const uint8_t * mac_addr){
  char macStr[18];
  snprintf(macStr, sizeof(macStr), "%02x:%02x:%02x:%02x:%02x:%02x",
           mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5]);
  Serial.print(macStr);
}
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) { 
  //Serial.print("\r\nLast Packet Send Status:\t");
  //Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
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
