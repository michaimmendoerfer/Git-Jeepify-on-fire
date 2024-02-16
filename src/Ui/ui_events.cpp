// This file was generated by SquareLine Studio
// SquareLine Studio version: SquareLine Studio 1.3.4
// LVGL version: 8.3.6
// Project name: Jeepify

#include <Arduino.h>
#include <String.h>
#include "ui.h"
#include "peers.h"
#include "Jeepify.h"

extern bool ReadyToPair;
extern bool DebugMode;
extern bool ChangesSaved;

extern struct_Peer *ActivePeer;
extern struct_Peer *ActiveSelection;
extern struct_Periph *ActiveSens;
extern struct_Periph *ActiveSwitch;

extern struct_Peer   P[MAX_PEERS];

extern bool ToggleDebugMode();
extern bool ToggleSleepMode();
extern bool TogglePairMode();


extern void SavePeers();
extern void PrepareJSON();

void ShowPeer(lv_event_t * e)
{
	
}

void Ui_Set_TogglePair(lv_event_t * e)
{
	bool TempPairMode = TogglePairMode();
	if (TempPairmode) {
		_ui_state_modify(ui_BtnSet2, LV_STATE_CHECKED, _UI_MODIFY_STATE_ADD);
	}
	else {
		_ui_state_modify(ui_BtnSet2, LV_STATE_DEFAULT, _UI_MODIFY_STATE_ADD);
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
		lv_obj_add_state(ui_BtnSet7, LV_STATE_DEFAULT);
	}
}

void Ui_SavePeers(lv_event_t * e)
{
    SavePeers();
}

void Ui_Peers_Prepare(lv_event_t * e)
{
	String Options = "";

	for (int PNr=0 ; PNr<MAX_PEERS ; PNr++) {
    if (P[PNr].Type) {
           
      if (Options) Options += "\n";
      
      if (millis()- P[PNr].TSLastSeen > OFFLINE_INTERVAL) Options += "off: <";
      else Options += "on:  <"; 
        
      Options += P[PNr].Name;

      switch (P[PNr].Type) {
        case SWITCH_1_WAY:   Options += "> PDC-1"; break;
        case SWITCH_2_WAY:   Options += "> PDC-2"; break;
        case SWITCH_4_WAY:   Options += "> PDC-4"; break;
        case SWITCH_8_WAY:   Options += "> PDC-8"; break;
        case PDC_SENSOR_MIX: Options += "> MIX";   break;
        case BATTERY_SENSOR: Options += "> Sens";  break;
		default:		     Options += "> ???";   break;
      }
    }
  }
  //lv_roller_set_options(ui_RollerPeers1, Options.c_str(), LV_ROLLER_MODE_NORMAL);
  lv_roller_set_options(ui_RollerPeers1, "on:  <EPS32-1> PDC-2\noff: <EPS32-2> PDC-2", LV_ROLLER_MODE_NORMAL);
}

void Ui_Peers_Selected(lv_event_t * e)
{
	char buf[100];
	
	lv_roller_get_selected_str(ui_RollerPeers1, buf, 100);
	sscanf(buf, "<%s>", buf);

	ActivePeer = FindPeerByName(buf);

	if (ActivePeer) _ui_screen_change(&ui_ScrPeer, LV_SCR_LOAD_ANIM_FADE_ON, 50, 0, &ui_ScrPeer_screen_init);
}

void Ui_JSON_Prepare(lv_event_t * e)
{
	PrepareJSON();
}

void UI_Set_Prepare(lv_event_t * e)
{
	if (ReadyToPair) {
		if (!lv_obj_has_state(ui_BtnSet2, LV_STATE_CHECKED)) {
			lv_obj_add_state(ui_BtnSet2, LV_STATE_CHECKED);
			lv_event_send(ui_BtnSet2, LV_EVENT_REFRESH, NULL);
		}
	}
	else {
		if (lv_obj_has_state(ui_BtnSet2, LV_STATE_CHECKED)) {
			lv_obj_clear_state(ui_BtnSet2, LV_STATE_CHECKED);
			lv_event_send(ui_BtnSet2, LV_EVENT_REFRESH, NULL);
		}
	}
	
	if (DebugMode) {
		if (!lv_obj_has_state(ui_BtnSet7, LV_STATE_CHECKED)) {
			lv_obj_add_state(ui_BtnSet7, LV_STATE_CHECKED);
			lv_event_send(ui_BtnSet7, LV_EVENT_REFRESH, NULL);
		}
	}
	else {
		if (lv_obj_has_state(ui_BtnSet7, LV_STATE_CHECKED)) {
			lv_obj_clear_state(ui_BtnSet7, LV_STATE_CHECKED);
			lv_event_send(ui_BtnSet7, LV_EVENT_REFRESH, NULL);
		}
	}

	if (!ChangesSaved) {
		if (!lv_obj_has_state(ui_BtnSet8, LV_STATE_CHECKED)) {
			lv_obj_add_state(ui_BtnSet8, LV_STATE_CHECKED);
			lv_event_send(ui_BtnSet8, LV_EVENT_REFRESH, NULL);
		}
	}
	else {
		if (lv_obj_has_state(ui_BtnSet8, LV_STATE_CHECKED)) {
			lv_obj_clear_state(ui_BtnSet8, LV_STATE_CHECKED);
			lv_event_send(ui_BtnSet8, LV_EVENT_REFRESH, NULL);
		}
	}
}

void Ui_Single_Next(lv_event_t * e)
{
	if (!ActivePeer) ActivePeer = FindFirstPeer();
	if (!ActiveSens) ActiveSens = FindFirstPeriph()
}

void Ui_Single_Last(lv_event_t * e)
{
	// Your code here
}

void Ui_Peer_Prepare(lv_event_t * e)
{
	// Your code here
}

void Ui_Peer_Restart(lv_event_t * e)
{
	// Your code here
}

void Ui_Peer_Reset(lv_event_t * e)
{
	// Your code here
}

void Ui_Peer_ToggleSleep(lv_event_t * e)
{
	// Your code here
}

void Ui_Multi_Next(lv_event_t * e)
{
	// Your code here
}

void Ui_Multi_Last(lv_event_t * e)
{
	// Your code here
}

void Ui_Peer_Next(lv_event_t * e)
{
	// Your code here
}

void Ui_Peer_Last(lv_event_t * e)
{
	// Your code here
}

void Ui_Peer_Eichen(lv_event_t * e)
{
	// Your code here
}

void Ui_Peer_Volt(lv_event_t * e)
{
	// Your code here
}

void Ui_Single_Prepare(lv_event_t * e)
{
	/*
	//Write codes screen_meter_1
	ui_SingleMeter1 = lv_meter_create(ui_ScrPeer);
	// add scale screen_meter_1_scale_1
	lv_meter_scale_t *screen_meter_1_scale_1 = lv_meter_add_scale(ui_SingleMeter1);
	lv_meter_set_scale_ticks(ui_SingleMeter1, screen_meter_1_scale_1, 41, 2, 10, lv_color_hex(0xff0000));
	lv_meter_set_scale_major_ticks(ui_SingleMeter1, screen_meter_1_scale_1, 8, 5, 15, lv_color_hex(0xffff00), 5);
	lv_meter_set_scale_range(ui_SingleMeter1, screen_meter_1_scale_1, 0, 40, 270, 135);

	// add needle line for screen_meter_1_scale_1.
	ui->screen_meter_1_scale_1_ndline_0 = lv_meter_add_needle_line(ui->screen_meter_1, screen_meter_1_scale_1, 3, lv_color_hex(0x000000), -15);
	lv_meter_set_indicator_value(ui->screen_meter_1, ui->screen_meter_1_scale_1_ndline_0, 0);

	// add needle images for screen_meter_1_scale_1.
	ui->screen_meter_1_scale_1_ndimg_0 = lv_meter_add_needle_img(ui->screen_meter_1, screen_meter_1_scale_1, &__alpha_60x4, 0, 0);
	lv_meter_set_indicator_value(ui->screen_meter_1, ui->screen_meter_1_scale_1_ndimg_0, 20);
	lv_obj_set_pos(ui->screen_meter_1, 20, 20);
	lv_obj_set_size(ui->screen_meter_1, 194, 194);

	//Write style for screen_meter_1, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_bg_opa(ui->screen_meter_1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->screen_meter_1, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->screen_meter_1, 100, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_width(ui->screen_meter_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_meter_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write style for screen_meter_1, Part: LV_PART_TICKS, State: LV_STATE_DEFAULT.
	lv_obj_set_style_text_color(ui->screen_meter_1, lv_color_hex(0xff0000), LV_PART_TICKS|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->screen_meter_1, &lv_font_montserratMedium_12, LV_PART_TICKS|LV_STATE_DEFAULT);

	//Write style for screen_meter_1, Part: LV_PART_INDICATOR, State: LV_STATE_DEFAULT.
	lv_obj_set_style_bg_opa(ui->screen_meter_1, 255, LV_PART_INDICATOR|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->screen_meter_1, lv_color_hex(0x000000), LV_PART_INDICATOR|LV_STATE_DEFAULT);

	//Update current screen layout.
	lv_obj_update_layout(ui->screen);
	*/
}
