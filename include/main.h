#ifndef MAIN_H
#define MAIN_H

#include <esp_now.h>
#include <WiFi.h>
#include <lvgl.h>
#include "PeerClass.h"

void   PrintMAC(const uint8_t * mac_addr);

void   OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status);
void   OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len);

void   my_disp_flush( lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p );
void   my_touchpad_read( lv_indev_drv_t * indev_driver, lv_indev_data_t * data );

void   SendPing(lv_timer_t * timer);
bool   ToggleSwitch(PeerClass *P, int PerNr);
void   SendCommand(PeerClass *Peer, String Cmd);
void   SendPairingConfirm(PeerClass *Peer);
void   MultiScreenAddPeriph(struct_Periph *Periph, uint8_t Pos);

void   CheckButtonVars();
bool   ToggleSleepMode();
bool   ToggleDebugMode();
bool   TogglePairMode();

void   CalibVolt();
void   PrepareJSON();
void   PrintMAC(const uint8_t * mac_addr);
void   WriteStringToCharArray(String S, char *C);

extern bool DebugMode;
extern bool SleepMode;
extern bool ReadyToPair;
extern bool ChangesSaved;

extern volatile uint32_t TSMsgRcv;
extern volatile uint32_t TSMsgSnd;
extern volatile uint32_t TSPair;

extern PeerClass Peer[MAX_PEERS];
#endif