#define NODE_NAME       "M240_1"
#define NODE_TYPE       MONITOR_ROUND

#define DEBUG(...) if (Self.GetDebugMode()) Serial.printf(__VA_ARGS__)
//#define KILL_NVS 1

const char *_Version = "V 3.81";
const char *_Name = "M240_1";
const char _Protokoll_Version[] = "2.00";

#pragma region Includes
#include <Arduino.h>
#include "Jeepify.h"
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
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <nvs_flash.h>
#pragma endregion Includes

const char *T[MAX_PERIPHERALS] = {"T0", "T1", "T2", "T3", "T4", "T5", "T6", "T7", "T8"};
const char *N[MAX_PERIPHERALS] = {"N0", "N1", "N2", "N3", "N4", "N5", "N6", "N7", "N8"};
const char *B[MAX_PERIPHERALS] = {"Br0", "Br1", "Br2", "Br3", "Br4", "B5r", "B6r", "B7r", "B8r"};
const char *ArrNullwert[MAX_PERIPHERALS] = {"NW0", "NW1", "NW2", "NW3", "NW4", "NW5", "NW6", "NW7", "NW8"};
const char *ArrVperAmp[MAX_PERIPHERALS] = {"VpA0", "VpA1", "VpA2", "VpA3", "VpA4", "VpA5", "VpA6", "VpA7", "VpA8"};
const char *ArrVin[MAX_PERIPHERALS] = {"Vin0", "Vin1", "Vin2", "Vin3", "Vin4", "Vin5", "Vin6", "Vin7", "Vin8"};
const char *ArrPeriph[MAX_PERIPHERALS]   = {"Per0", "Per1", "Per2", "Per3", "Per4", "Per5", "Per6", "Per6", "Per7"};

#define DRAW_BUF_SIZE (TFT_HOR_RES * TFT_VER_RES / 20 * (LV_COLOR_DEPTH / 8))

TFT_eSPI tft = TFT_eSPI(TFT_HOR_RES, TFT_VER_RES); /* TFT instance */
CST816D Touch(I2C_SDA, I2C_SCL, TP_RST, TP_INT);

#pragma region Globals
static lv_disp_draw_buf_t draw_buf;
static lv_color_t buf1[ TFT_HOR_RES * TFT_VER_RES / 20 ];

int PeerCount;
Preferences preferences;
uint8_t broadcastAddressAll[6] = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff};

struct ConfirmStruct {
    uint8_t  Address[10];
    char     Message[300];
    uint32_t TSMessage;
    int      Try;
    bool     Confirmed;
};
#define JEEPIFY_SEND_MAX_TRIES 10

MyLinkedList<ConfirmStruct*> ConfirmList = MyLinkedList<ConfirmStruct*>();

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
volatile uint32_t TSConfirm = 0;

lv_timer_t *WDButtonVars;

int ActiveMultiScreen;
bool WebServerActive = true;

#pragma endregion Globals
#pragma region WebServer
AsyncWebServer server(80);
const char* ssid = "Jeepify_Monitor";
const char* password = "";

const char* PARAM_MESSAGE = "message";

PeerClass   *ActiveWebPeer = NULL;
PeriphClass *ActiveWebPeriph = NULL;

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="en">
<head>
<meta charset="UTF-8">
<meta name="viewport" content="width=device-width, initial-scale=1.0">
<title>ESP32 Form</title>
<style>
body{ margin: 0;padding: 0;font-family: Arial, Helvetica, sans-serif;background-color: #2c257a;}
.box{ width: 70%%; padding: 40px; position: absolute; top: 50%%; left: 50%%; transform: translate(-50%%,-50%%); background-color: #191919; color: white; text-align: center; border-radius: 24px; box-shadow: 0px 1px 32px 0px rgba(0,227,197,0.59);}
h1{ text-transform: uppercase; font-weight: 500;}
input{ border: 0; display: block; background: none; margin: 20px auto; text-align: center; border: 2px solid #4834d4; padding: 14px 10px; width: 30%%; outline: none; border-radius: 24px; color: white; font-size: smaller; transition: 0.3s;}
input:focus{ width: 90%%; border-color:#22a6b3 ;}
input[type='submit']{ border: 0; display: block; background: none; margin: 20px auto; text-align: center; border: 2px solid #22a6b3; padding: 14px 10px; width: 90px; outline: none; border-radius: 24px; color: white; transition: 0.3s; cursor: pointer;}
input[type='submit']:hover{ background-color: #22a6b3;}
</style>
</head>
<body>
<form action="/get" class="box" id="values">
<h1>Settings (%PeerName%)</h1>
<div class="part">
<input name="PeerName" type="text" placeholder="%PeerName%">
</div>
<div class="part">
<input name="PeriphName" type="%TYPE%" placeholder="%PeriphName%">
</div>
<div class="part">
<input name="Nullwert" type="%TYPE%" placeholder="%Nullwert%">
</div>
<div class="part">
<input name="VperAmp" type="%TYPE%" placeholder="%VperAmp%">
</div>
<div class="part">
<input name="Vin" type="%TYPE%" placeholder="%Vin%">
<input type="submit" name="message" value="upd module" />
</div>
<div class="part">
<table align=center>
  <tr>
    <td><div class="part"><input type="submit" name="message" value="prev" /></div></td>
    <td><div class="part"><input type="submit" name="message" value="module" /></div></td>
    <td><div class="part"><input type="submit" name="message" value="next" /></div></td>
  </tr>
</table>
</div>
</form>
</body></html>
)rawliteral";

void notFound(AsyncWebServerRequest *request) {
  request->send(404, "text/plain", "Not found");
}

String processor(const String& var)
{
    char Buf[10];
    
    if (var == "TYPE")        if (ActiveWebPeriph) return "text";
                              else return "hidden";
    if (var == "PeerName")    return ActiveWebPeer->GetName();
    if (var == "PeriphName")  if (ActiveWebPeriph) return ActiveWebPeriph->GetName();
    if (var == "Nullwert")    if (ActiveWebPeriph) { dtostrf(ActiveWebPeriph->GetNullwert(), 0, 3, Buf); return String(Buf); }
    if (var == "VperAmp")     if (ActiveWebPeriph) { dtostrf(ActiveWebPeriph->GetVperAmp(), 0, 3, Buf); return String(Buf); }
    if (var == "Vin")         if (ActiveWebPeriph) return String(ActiveWebPeriph->GetVin());
    
    return String();
}
bool SendWebPeriphNameChange()
{
    JsonDocument doc; String jsondata; 

    doc["from"]    = NODE_NAME;   
    doc["Order"]   = SEND_CMD_UPDATE_NAME;
    doc["NewName"] = ActiveWebPeriph->GetName();
    doc["Pos"]     = ActiveWebPeriph->GetPos();
    
    serializeJson(doc, jsondata);  
    
    TSMsgSnd = millis();
    esp_now_send(ActiveWebPeer->GetBroadcastAddress(), (uint8_t *) jsondata.c_str(), 100);  //Sending "jsondata"  
    DEBUG ("%s\n\r", jsondata);

    return true;
}
bool SendWebPeerNameChange()
{
    JsonDocument doc; String jsondata; 
    
    doc["from"]    = NODE_NAME;   
    doc["Order"]   = SEND_CMD_UPDATE_NAME;
    doc["NewName"] = ActiveWebPeer->GetName();
    doc["Pos"]     = 99;
    
    serializeJson(doc, jsondata);  
    
    TSMsgSnd = millis();
    esp_now_send(ActiveWebPeer->GetBroadcastAddress(), (uint8_t *) jsondata.c_str(), 100);  //Sending "jsondata"  
    DEBUG ("%s\n\r", jsondata);
    return true;
}
bool SendWebVinChange()
{
    JsonDocument doc; String jsondata; 
    
    doc["from"]    = NODE_NAME;   
    doc["Order"]   = SEND_CMD_UPDATE_VIN;
    doc["Value"]   = ActiveWebPeriph->GetVin();
    doc["Pos"]     = ActiveWebPeriph->GetPos();
    
    serializeJson(doc, jsondata);  
    
    TSMsgSnd = millis();
    esp_now_send(ActiveWebPeer->GetBroadcastAddress(), (uint8_t *) jsondata.c_str(), 100);  //Sending "jsondata"  
    DEBUG ("%s\n\r", jsondata);
    
    return true;
}
bool SendWebVperAmpChange()
{
    JsonDocument doc; String jsondata; 
    
    doc["from"]    = NODE_NAME;   
    doc["Order"]   = SEND_CMD_UPDATE_VPERAMP;
    doc["Value"]   = ActiveWebPeriph->GetVperAmp();
    doc["Pos"]     = ActiveWebPeriph->GetPos();
    
    serializeJson(doc, jsondata);  
    
    TSMsgSnd = millis();
    esp_now_send(ActiveWebPeer->GetBroadcastAddress(), (uint8_t *) jsondata.c_str(), 100);  //Sending "jsondata"  
    DEBUG ("%s\n\r", jsondata);

    return true;
}
bool SendWebNullwertChange()
{
    JsonDocument doc; String jsondata; 
    
    doc["from"]    = NODE_NAME;   
    doc["Order"]   = SEND_CMD_UPDATE_NULLWERT;
    doc["Value"]   = ActiveWebPeriph->GetNullwert();
    doc["Pos"]     = ActiveWebPeriph->GetPos();
    
    serializeJson(doc, jsondata);  
    
    TSMsgSnd = millis();
    esp_now_send(ActiveWebPeer->GetBroadcastAddress(), (uint8_t *) jsondata.c_str(), 100);  //Sending "jsondata"  
    DEBUG ("%s\n\r", jsondata);

    return true;
}

void InitWebServer()
{
    Serial.printf("create AP = %d", WiFi.softAP(ssid, password));
    WiFi.setTxPower(WIFI_POWER_8_5dBm);
    int txPower = WiFi.getTxPower();
    Serial.print("TX power: ");
    Serial.println(txPower);

    IPAddress IP = WiFi.softAPIP();
    Serial.print("AP IP address: ");
    Serial.println(IP);
    
    ActiveWebPeer = &Self;
    ActiveWebPeriph = NULL;
    
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send_P(200, "text/html", index_html, processor);
    });
    
    server.on("/get", HTTP_GET, [] (AsyncWebServerRequest *request) {
        String message;
        String WebBuffer;

        bool SaveNeeded  = false;

        if (request->hasParam(PARAM_MESSAGE)) {
            message = request->getParam(PARAM_MESSAGE)->value();
            if (message == "update module") 
            {
                if (request->hasParam("PeerName"))
                {
                  WebBuffer = request->getParam("PeerName")->value();
                  if (WebBuffer != "")
                    { 
                        DEBUG ("Received from web: NewPeerName = %s\n\r", WebBuffer.c_str());  
                        if (ActiveWebPeer) 
                        {   
                            SaveNeeded = true;
                            ActiveWebPeer->SetName(WebBuffer.c_str());
                            if (ActiveWebPeer != &Self) SendWebPeerNameChange();
                        }
                    }
                }
                
                if (request->hasParam("PeriphName"))
                {
                  WebBuffer = request->getParam("PeriphName")->value();
                  if (WebBuffer != "")
                    { 
                        DEBUG ("Received from web: NewPeriphName = %s\n\r", WebBuffer.c_str());  
                        if (ActiveWebPeriph) 
                        {
                            SaveNeeded = true;
                            ActiveWebPeriph->SetName(WebBuffer.c_str());
                            if (ActiveWebPeer != &Self) SendWebPeriphNameChange();
                        }
                    }
                }

                if (request->hasParam("Nullwert"))
                {
                  WebBuffer = request->getParam("Nullwert")->value();
                  if (WebBuffer != "")
                    { 
                        DEBUG ("Received from web: NewNullwert = %s\n\r", WebBuffer.c_str());  
                        if (ActiveWebPeriph) 
                        {
                            SaveNeeded = true;
                            ActiveWebPeriph->SetNullwert(atof(WebBuffer.c_str()));
                            if (ActiveWebPeer != &Self) SendWebNullwertChange();
                        }
                    }
                }
                
                if (request->hasParam("VperAmp"))
                {
                  WebBuffer = request->getParam("VperAmp")->value();
                  if (WebBuffer != "")
                    { 
                        DEBUG ("Received from web: NewVperAmp = %s\n\r", WebBuffer.c_str());  
                        if (ActiveWebPeriph) 
                        {
                            SaveNeeded = true;
                            ActiveWebPeriph->SetVperAmp(atof(WebBuffer.c_str()));
                            if (ActiveWebPeer != &Self) SendWebVperAmpChange();
                        }
                    }
                }
                if (request->hasParam("Vin"))
                {
                  WebBuffer = request->getParam("Vin")->value();
                  if (WebBuffer != "")
                    { 
                        DEBUG ("Received from web: NewVin = %s\n\r", WebBuffer.c_str());  
                        if (ActiveWebPeriph) 
                        {
                            SaveNeeded = true;
                            ActiveWebPeriph->SetVperAmp(atoi(WebBuffer.c_str()));
                            if (ActiveWebPeer != &Self) SendWebVinChange();
                        }
                    }
                }  
            }
            
            if (message == "module") 
            {
                DEBUG ("Module aufgerufen\n\r");
                ActiveWebPeer   = &Self;
                ActiveWebPeriph = NULL;
                Serial.printf("aktueller Name = %s\n\r", ActiveWebPeer->GetName());
            }
            if (message == "prev") 
            {
                DEBUG ("Prev aufgerufen\n\r");
                if (ActiveWebPeer == &Self) 
                {
                    PeerClass *TempP = FindFirstPeer(MODULE_ALL);
                    if (TempP) ActiveWebPeer = TempP;
                    ActiveWebPeriph = FindFirstPeriph(ActiveWebPeer, SENS_TYPE_ALL);  
                }
                else
                {
                    ActiveWebPeriph = FindPrevPeriph(NULL, ActiveWebPeriph, SENS_TYPE_ALL, true);
                    ActiveWebPeer   = FindPeerById(ActiveWebPeriph->GetPeerId());
                }

            }
            if (message == "next") 
            {
                DEBUG ("Next aufgerufen\n\r");
                if (ActiveWebPeer == &Self) 
                {
                    PeerClass *TempP = FindFirstPeer(MODULE_ALL);
                    if (TempP) ActiveWebPeer = TempP;
                    ActiveWebPeriph = FindFirstPeriph(ActiveWebPeer, SENS_TYPE_ALL);  
                }
                else
                {
                    ActiveWebPeriph = FindNextPeriph(NULL, ActiveWebPeriph, SENS_TYPE_ALL, true);
                    ActiveWebPeer   = FindPeerById(ActiveWebPeriph->GetPeerId());
                }
            }
        } else {
            message = "No message sent";
        }
        request->send_P(200, "text/html", index_html, processor);
        
        if (SaveNeeded)
        {   
            if (ActiveWebPeer != &Self) SavePeers();
            else 
            {
                preferences.begin("JeepifyInit", false);
                preferences.putString("ModuleName", Self.GetName());
                preferences.end();
                Serial.println("Neuer Module Name gespeichert");
            }
            SaveNeeded = false;
        }
    });

  server.onNotFound(notFound);
}
void ToggleWebServer()
{   
    WebServerActive = !WebServerActive;
    if (WebServerActive) 
    {
        ActiveWebPeer = PeerList.get(0);
        ActiveWebPeriph = PeriphList.get(0);
        Serial.println("Server startet");
        server.begin();
    }
    else 
    {
        Serial.println("Server beendet");
        server.end();
    }
}
#pragma endregion WebServer

#pragma region Main
//void OnDataRecv(const esp_now_recv_info *info, const uint8_t* incomingData, int len)
void   OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len)
{
    PeerClass *P;
    
    char* buff = (char*) incomingData;   
    JsonDocument doc; 
    String jsondata = String(buff); 
    
    String BufS; char Buf[50] = {};
    bool SaveNeeded = false;
    bool NewPeer    = false;
    char buf[100];
    
    jsondataBuf = jsondata;
    PrepareJSON();
    Serial.println(jsondata);
    DeserializationError error = deserializeJson(doc, jsondata);

    if (!error) // erfolgreich JSON
    {
        strcpy(buf, doc["Node"]);

        char       *_PeerName    = strtok(buf, ";");
        uint32_t    _Uptime      = atoi(strtok(NULL, ";"));
        int         _Status      = atoi(strtok(NULL, ";"));
        int         _Type        = (int) (doc["Type"]);
        const char *_PeerVersion = doc["Version"];
        int         _Order       = (int)doc["Order"];   
        int         _TSConfirm   = (int)doc["TSConfirm"];
        //P = FindPeerByMAC(info->src_addr);
        P = FindPeerByMAC(mac);
        if (P)
        {
            if ((P) and (Self.GetDebugMode()) and (millis() - P->GetTSLastSeen() > OFFLINE_INTERVAL)) ShowMessageBox("Peer online", P->GetName(), 1000, 200);
            P->SetTSLastSeen(millis());
        }

        TSMsgRcv = millis();

        switch (_Order)
        {
            case SEND_CMD_PAIR_ME:
                // new Peer wants to pair and module too - create it
                if ((!P) and Self.GetPairMode())
                {
                    P = new PeerClass();
                    PeerList.add(P);
                    SaveNeeded = true;
                    NewPeer    = true;
                    Self.SetPairMode(false); TSPair = 0;
                    
                    P->Setup(_PeerName, _Type, _PeerVersion, mac, (bool) bitRead(_Status, 1), (bool) bitRead(_Status, 0), (bool) bitRead(_Status, 2), (bool) bitRead(_Status, 3));
                    if (Self.GetDebugMode()) ShowMessageBox("Peer added...", doc["Node"], 2000, 150);
                    SendPairingConfirm(P); 

                    for (int Si=0; Si<MAX_PERIPHERALS; Si++) 
                    {
                        DEBUG ("Check Pairing for: %s\n\r", ArrPeriph[Si]);
                        
                        if (doc[ArrPeriph[Si]].is<JsonVariant>())
                        {
                            strcpy(buf, doc[ArrPeriph[Si]]);
                            int   _PeriphType = atoi(strtok(buf, ";"));
                            char *_PeriphName = strtok(NULL, ";");
                            P->PeriphSetup(Si, _PeriphName, _PeriphType, false, 0,0,0,0, 0, 0, 0, P->GetId());
                            P->SetPeriphChanged(Si, true);
                            PeriphList.add(P->GetPeriphPtr(Si));
                            SaveNeeded = true;
                            DEBUG ("%s->Periph[%d].Name is now: %s\n\r", P->GetName(), Si, P->GetPeriphName(Si));
                        }
                    }
                }
                break;
            case SEND_CMD_STATUS:
                if (P)
                {
                    // check for module name change
                    if (strcmp(_PeerName, P->GetName())) P->SetName(_PeerName);

                    for (int Si=0; Si<MAX_PERIPHERALS; Si++) 
                    {
                        DEBUG ("Check values of: %s\n\r", ArrPeriph[Si]);
                        
                        if (doc[ArrPeriph[Si]].is<JsonVariant>())
                        {
                            strcpy(buf, doc[ArrPeriph[Si]]);
                            int   _PeriphType = atoi(strtok(buf, ";"));
                            char *_PeriphName = strtok(NULL, ";");
                            float _Value0     = atof(strtok(NULL, ";"));
                            float _Value1     = atof(strtok(NULL, ";"));
                            float _Value2     = atof(strtok(NULL, ";"));
                            float _Value3     = atof(strtok(NULL, ";"));

                            // check for periph name change
                            if (strcmp(_PeriphName, P->GetPeriphName(Si))) P->SetPeriphName(Si, _PeriphName);
                            
                            P->SetPeriphOldValue(Si, P->GetPeriphValue(Si, 0), 0);
                            P->SetPeriphValue(Si, _Value0, 0);
                            P->SetPeriphOldValue(Si, P->GetPeriphValue(Si, 1), 1);
                            P->SetPeriphValue(Si, _Value1, 1);
                            P->SetPeriphOldValue(Si, P->GetPeriphValue(Si, 2), 2);
                            P->SetPeriphValue(Si, _Value2, 2);
                            P->SetPeriphOldValue(Si, P->GetPeriphValue(Si, 3), 3);
                            P->SetPeriphValue(Si, _Value3, 3);
                            
                            P->SetPeriphChanged(Si, false); //werte wieder uptodate

                            if (_Status)
                            {
                                P->SetDebugMode ((bool) bitRead(_Status, 0));
                                P->SetSleepMode ((bool) bitRead(_Status, 1));
                                P->SetDemoMode  ((bool) bitRead(_Status, 2));
                                P->SetPairMode  ((bool) bitRead(_Status, 3));
                            } 
                            
                            DEBUG ("%s->%s values are: %.2f - %.2f - %.2f - %.2f\n\r", P->GetName(), P->GetPeriphName(Si), 
                                P->GetPeriphValue(Si, 0), P->GetPeriphValue(Si, 1), P->GetPeriphValue(Si, 2), P->GetPeriphValue(Si, 3));
                        }
                    }
                }
                break;
    
            case SEND_CMD_CONFIRM:
                if ((P) and (doc["TSConfirm"].is<JsonVariant>()))
                {                    
                    DEBUG ("Confirm (%lu) empfangen von %s\n\r", _TSConfirm, P->GetName());
                    for (int i=0; i<ConfirmList.size(); i++)
                    {
                        ConfirmStruct *TempConfirm;
                        TempConfirm = ConfirmList.get(i);
                        DEBUG ("empfangener TS ist: %lu - durchsuchter TS (List[%d]) ist: %lu\n\r", _TSConfirm, i, TempConfirm->TSMessage);
                        if (TempConfirm->TSMessage == _TSConfirm)
                        {
                            TempConfirm->Confirmed = true;
                            DEBUG ("Found at list[%d] - DELETED\n\r", i);
                        }
                    }
                }
                break;
        } 
        
        if (SaveNeeded)
        {
            SavePeers();
            SaveNeeded = false;
            if (Self.GetDebugMode()) ShowMessageBox("Saving...", "complete", 1000, 200);
        }
    }
    else // Error bei JSON
    {        
        Serial.print(F("deserializeJson() failed: ")); 
        Serial.println(error.f_str());
        Serial.printf("jsondata was: %s\n\r", jsondata);
        return;
    }
}

void setup() 
{
    #ifdef ARDUINO_USB_CDC_ON_BOOT
        delay(2000);
    #endif
    Serial.begin(115200);
    
    //TFT & LVGL
    tft.init();
    tft.setRotation(0);
    tft.setSwapBytes(false);
    tft.begin();
    Touch.begin();

    lv_init();
    
    lv_disp_draw_buf_init( &draw_buf, buf1, NULL, TFT_HOR_RES * TFT_VER_RES / 20 );
 
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
    
    Self.Setup(_Name, MONITOR_ROUND, _Version, broadcastAddressAll, false, true, false, false);

    #ifdef KILL_NVS
        nvs_flash_erase(); nvs_flash_init(); while(1);
        while(1)
        {}
    #endif
    
    WiFi.mode(WIFI_AP_STA);

    InitWebServer();
    
    WebServerActive = !WebServerActive;
    ToggleWebServer();
    
    if (esp_now_init() != ESP_OK) { Serial.println("Error initializing ESP-NOW"); return; }

    esp_now_register_send_cb(OnDataSent);
    esp_now_register_recv_cb(OnDataRecv);    

    //Get saved Peers  
    preferences.begin("JeepifyInit", true);
        Self.SetDebugMode(preferences.getBool("DebugMode", true));
        Self.SetSleepMode(preferences.getBool("SleepMode", false));
        Self.SetName(preferences.getString("ModuleName", NODE_NAME).c_str());
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
  delay(5);
}
#pragma endregion Main

#pragma region Send-Things
esp_err_t  JeepifySend(PeerClass *P, const uint8_t *data, size_t len, uint32_t TSConfirm, bool ConfirmNeeded = false)
{
    esp_err_t SendStatus = esp_now_send(P->GetBroadcastAddress(), data, len);
    
    Serial.printf("SendStatus was %d, ConfirmNeeded = %d\n\r", SendStatus, (int) ConfirmNeeded);
    if (ConfirmNeeded)
    {   
        Serial.println("Creating Confirmstruct");
        ConfirmStruct *Confirm = new ConfirmStruct;
        memcpy(Confirm->Address, P->GetBroadcastAddress(), 6);
        strcpy(Confirm->Message, (const char *)data);
        Confirm->Confirmed = false;
        Confirm->TSMessage = TSConfirm;
        Confirm->Try = 1;
        
        Serial.println("adding to confirmlist");
        
        ConfirmList.add(Confirm);

        //DEBUG("added Msg: %s to ConfirmList\n\r", Confirm->Message);   
    }
    Serial.println("JeepifySend ende");
        
    return SendStatus;
}
void SendPing(lv_timer_t * timer) {
    JsonDocument doc; String jsondata;

    PeerClass *P;
    
    doc["Node"] = NODE_NAME;   
    doc["Order"] = SEND_CMD_STAY_ALIVE;

    if (Self.GetPairMode())
    {
        doc["Pairing"] = "aktiv";
    }

    serializeJson(doc, jsondata);  
    
    for (int i=0; i<PeerList.size(); i++)
    {
        P = PeerList.get(i);
        //PrintMAC(P->GetBroadcastAddress());
        //Serial.printf(" - Sende Stay Alive an: %s\n\r", P->GetName());
        
        if (P->GetType() > 0) esp_now_send(P->GetBroadcastAddress(), (uint8_t *) jsondata.c_str(), 100);  
    }

    if (ConfirmList.size() > 0)
    { 
        for (int i=ConfirmList.size()-1; i>=0; i--)
        {
            ConfirmStruct *Confirm = ConfirmList.get(i);
            Confirm->Try++;
            
            if (Confirm->Confirmed == true)
            {
                char TxtBuf[100];
                if (Self.GetDebugMode()) sprintf(TxtBuf, "SUCCESS - Message to %s successful confirmed after %d tries!", FindPeerByMAC(Confirm->Address)->GetName(), Confirm->Try);
                if (Self.GetDebugMode()) ShowMessageBox("SUCCESS", TxtBuf, 1000, 200);
                ConfirmList.remove(i);
                delete Confirm;
            }
            else if (Confirm->Try == JEEPIFY_SEND_MAX_TRIES+1)
            {
                char TxtBuf[100];
                if (Self.GetDebugMode()) sprintf(TxtBuf, "FAILED - Message to %s deleted after %d tries!", FindPeerByMAC(Confirm->Address)->GetName(), Confirm->Try);
                if (Self.GetDebugMode()) ShowMessageBox("FAILED", TxtBuf, 1000, 200);
                ConfirmList.remove(i);
                delete Confirm;
            }
            else
            {
                DEBUG ("%d: reSending Msg: %s from ConfirmList Try: %d\n\r", millis(), Confirm->Message, Confirm->Try);
                esp_err_t SendStatus = esp_now_send(Confirm->Address, (uint8_t*) Confirm->Message, 200); 
            }     
        }
    }
}
void SendPairingConfirm(PeerClass *P) {
  JsonDocument doc; String jsondata; 
  
  uint8_t *Broadcast = P->GetBroadcastAddress();
  
  doc["Node"]     = Self.GetName();   
  doc["Peer"]     = P->GetName();
  doc["Order"]    = SEND_CMD_YOU_ARE_PAIRED;
  doc["Type"]     = Self.GetType();
  doc["B0"]       = (uint8_t)Broadcast[0]; doc["B1"] = (uint8_t)Broadcast[1]; doc["B2"] = (uint8_t)Broadcast[2];
  doc["B3"]       = (uint8_t)Broadcast[3]; doc["B4"] = (uint8_t)Broadcast[4]; doc["B5"] = (uint8_t)Broadcast[5];

  serializeJson(doc, jsondata);  
  
  TSMsgSnd = millis();
  esp_now_send(broadcastAddressAll, (uint8_t *) jsondata.c_str(), 200); 
  DEBUG ("Sent you are paired\n\r%s\n\r", jsondata);  
     
}
bool ToggleSwitch(PeerClass *P, int PerNr)
{
    return ToggleSwitch(P->GetPeriphPtr(PerNr));
}
bool ToggleSwitch(PeriphClass *Periph)
{
    JsonDocument doc; String jsondata; 
    
    doc["From"]         = Self.GetName();  
    doc["Order"]        = SEND_CMD_SWITCH_TOGGLE;
    doc["PeriphName"]   = Periph->GetName();
    doc["PeriphPos"]    = Periph->GetPos();

    serializeJson(doc, jsondata);  
    
    TSMsgSnd = millis();
    esp_now_send(FindPeerById(Periph->GetPeerId())->GetBroadcastAddress(), (uint8_t *) jsondata.c_str(), 100);  //Sending "jsondata"  
    DEBUG ("%s", jsondata);
    
    return true;
}
void SendCommand(PeerClass *P, int Cmd, String Value) {
  JsonDocument doc; String jsondata; 
  
  doc["From"]  = Self.GetName();   
  doc["Order"] = Cmd;
  if (Value != "") doc["Value"] = Value;
  
  serializeJson(doc, jsondata);  
  
  TSMsgSnd = millis();
  esp_now_send(P->GetBroadcastAddress(), (uint8_t *) jsondata.c_str(), 100);  //Sending "jsondata"  
  DEBUG ("%s", jsondata);
}
#pragma endregion Send-Things
#pragma region System-Screens
void PrepareJSON() {
  if (jsondataBuf) {
    JsonDocument doc;
  
    DeserializationError error = deserializeJson(doc, jsondataBuf);
    if (doc["Node"] != NODE_NAME) { 
      lv_textarea_set_placeholder_text(ui_TxtJSON1, jsondataBuf.c_str());
      jsondataBuf = "";
    }
  }
}
#pragma endregion System-Screens
#pragma region Other
void ShowMessageBox(const char * Titel, const char *Txt, int delay, int opa)
{
    static const char * btns[] = {""};

    lv_obj_t *MsgBox = lv_msgbox_create(lv_scr_act(), Titel, Txt, NULL, false);
    lv_obj_set_style_bg_color(MsgBox, lv_color_hex(0xAD0808), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(MsgBox, opa, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(MsgBox, lv_color_hex(0xDBDBDB), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(MsgBox, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(MsgBox, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(MsgBox, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(MsgBox, lv_color_hex(0xDBDBDB), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(MsgBox, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_width(MsgBox, lv_pct(80));
    lv_obj_set_style_text_font(lv_msgbox_get_title(MsgBox), &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
    
    lv_obj_center(MsgBox);
    lv_obj_del_delayed(MsgBox, delay);
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

  DEBUG ("PairMode changed to: %d", Self.GetPairMode()); 
  
  return Self.GetPairMode();
}
void CalibVolt() {
    JsonDocument doc; String jsondata;
    
    uint32_t TSConfirm = millis();

    doc["Node"]  = Self.GetName();  
    doc["Order"] = SEND_CMD_VOLTAGE_CALIB;
    doc["NewVoltage"] = lv_textarea_get_text(ui_TxtVolt);
    doc["TSConfirm"] = TSConfirm;
    
    serializeJson(doc, jsondata);  

    JeepifySend(ActivePeer, (uint8_t *) jsondata.c_str(), 100, TSConfirm, true);  
    Serial.println("zurück von JeepifySend-warte");
    delay(1000);
    Serial.println("zurück von JeepifySend");
        
    DEBUG ("%s", jsondata.c_str());
}
void CalibAmp() 
{
    Serial.println("CalibAmp beginnd");
    JsonDocument doc; String jsondata;

    uint32_t TSConfirm = millis();

    doc["Node"]  = Self.GetName();  
    doc["Order"] = SEND_CMD_CURRENT_CALIB;
    doc["TSConfirm"] = TSConfirm;
    
    serializeJson(doc, jsondata);  
    JeepifySend(ActivePeer, (uint8_t *) jsondata.c_str(), 100, TSConfirm, true);  

    DEBUG ("%s", jsondata.c_str());
}

void PrintMAC(const uint8_t * mac_addr){
  char macStr[18];
  snprintf(macStr, sizeof(macStr), "%02x:%02x:%02x:%02x:%02x:%02x",
           mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5]);
  Serial.print(macStr);
}
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) { 
    if (Self.GetDebugMode())  
    {
        //Serial.print("Last Packet Send Status: ");
        //Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
        
        if (status == ESP_NOW_SEND_SUCCESS)
        {
            //Serial.println("Message send SUCCESS");
        }
        else 
        {
            Serial.println("Message send FAILED");
        }
    }
}
//
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
    }
}
#pragma endregion Other
//