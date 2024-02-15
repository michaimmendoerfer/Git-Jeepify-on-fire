// This file was generated by SquareLine Studio
// SquareLine Studio version: SquareLine Studio 1.3.4
// LVGL version: 8.3.6
// Project name: Jeepify

#include "ui.h"
#include <Arduino.h>

extern uint32_t TSPair;
extern bool ReadyToPair;

extern bool ToggleDebugMode();
extern bool ToggleSleepMode();
extern bool TogglePairMode();
extern void SavePeers();

void ShowPeer(lv_event_t * e)
{
	
}

void Ui_Set_TogglePair(lv_event_t * e)
{
	if (TogglePairMode()) {
		lv_obj_add_state(ui_BtnSet2, LV_STATE_CHECKED);
	}
	else {
		lv_obj_add_state(ui_BtnSet2, V_STATE_DEFAULT);
	}
}

void Ui_Set_Restart(lv_event_t * e)
{
	ESP.restart();
}

void Ui_Set_Reset(lv_event_t * e)
{
	// Your code here
}

void Ui_Set_ToggleDebug(lv_event_t * e)
{
	if (ToggleDebugMode()) {
		lv_obj_add_state(ui_BtnSet7, LV_STATE_CHECKED);
	}
	else {
		lv_obj_add_state(ui_BtnSet7, V_STATE_DEFAULT);
	}
}
void Ui_SavePeers(lv_event_t * e)
{
	SavePeers();
}

void Ui_Peers_Prepare(lv_event_t * e)
{
	// Your code here
}

void Ui_JSON_Prepare(lv_event_t * e)
{
	// Your code here
}

void UI_Set_Prepare(lv_event_t * e)
{
	// Your code here
}

void Ui_Single_Next(lv_event_t * e)
{
	// Your code here
}

void Ui_Single_Last(lv_event_t * e)
{
	// Your code here
}
