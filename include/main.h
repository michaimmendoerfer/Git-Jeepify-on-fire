#ifndef MAIN_H
#define MAIN_H

#include <esp_now.h>
#include <WiFi.h>
#include <lvgl.h>
#include "PeerClass.h"
#include "LinkedList.h"
#include <Preferences.h>

#define TFT_HOR_RES   240
#define TFT_VER_RES   240

void   PrintMAC(const uint8_t * mac_addr);

//void   OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status);
//void   OnDataRecv(const esp_now_recv_info *info, const uint8_t* incomingData, int len);
void   OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status);
void   OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len);

void   my_disp_flush( lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p );
void   my_touchpad_read( lv_indev_drv_t * indev_driver, lv_indev_data_t * data );

void   SendPing(lv_timer_t * timer);
bool   ToggleSwitch(PeerClass *P, int PerNr);
bool   ToggleSwitch(PeriphClass *Periph);

void   SendCommand(PeerClass *P, int Cmd, String Value="");
void   SendPairingConfirm(PeerClass *Peer);

void   ShowMessageBox(const char * Titel, const char *Txt, int delay, int opa=255);

//void   CheckButtonVars();
bool   ToggleSleepMode();
bool   ToggleDebugMode();
bool   TogglePairMode();

void   CalibVolt();
void   CalibAmp();
void   PrepareJSON();
void   PrintMAC(const uint8_t * mac_addr);

void   InitWebServer();

extern volatile uint32_t TSMsgRcv;
extern volatile uint32_t TSMsgSnd;
extern volatile uint32_t TSPair;

extern PeerClass Self;

extern int ActiveMultiScreen;
extern const char *_Version;
extern void ToggleWebServer();
extern bool WebServerActive;


#endif