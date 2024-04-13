// This file was generated by SquareLine Studio
// SquareLine Studio version: SquareLine Studio 1.3.4
// LVGL version: 8.3.6
// Project name: Jeepify

#include <Arduino.h>
#include <String.h>
#include <nvs_flash.h>
#include "ui.h"
#include "lv_meter.h"
#include "PeerClass.h"
#include "pref_manager.h"
#include "LinkedList.h"
#include "Jeepify.h"
#include "ui_events.h"
#include "main.h"

#pragma region Global_Definitions
lv_obj_t *SingleMeter;
lv_meter_indicator_t * SingleIndic;
lv_meter_indicator_t * SingleIndicNeedle;
lv_meter_scale_t * scale;
uint8_t MultiPosToChange;

//PeerClass   *ActivePeerSingle;
PeriphClass *ActivePeriphSingle;
PeriphClass *ActivePeriphSwitch;

lv_obj_t *Ui_LedSnd;
lv_obj_t *Ui_LedRcv;
lv_obj_t *Ui_LedPair;

_lv_obj_t *MultiComponent[PERIPH_PER_SCREEN];

lv_timer_t *SingleTimer;
lv_timer_t *MultiTimer;
lv_timer_t *SwitchTimer;

LV_IMG_DECLARE(ui_img_btn_off_png);   
LV_IMG_DECLARE(ui_img_btn_png);      

void GenerateSingleMeter(void);
void Keyboard_cb(lv_event_t * event);

void SingleUpdateTimer(lv_timer_t * timer);
void MultiUpdateTimer(lv_timer_t * timer);
void SwitchUpdateTimer(lv_timer_t * timer);

void Ui_Multi_Button_Clicked(lv_event_t * e);
void Ui_Multi_Sensor_Clicked(lv_event_t * e);
#pragma endregion Global_Definitions

#pragma region Screen_Peer
void Ui_Peer_Prepare()
{
	if (ActivePeer) {
		lv_label_set_text_static(ui_LblPeerName, ActivePeer->GetName());
		switch (ActivePeer->GetType())
		{
			case SWITCH_1_WAY:	lv_img_set_src(ui_ImgPeerType, &ui_img_1253518904); break;
			case SWITCH_2_WAY:	lv_img_set_src(ui_ImgPeerType, &ui_img_horstrelais2_png); break;
			case SWITCH_4_WAY:	lv_img_set_src(ui_ImgPeerType, &ui_img_ansgarmodule_4_png); break;
			case MONITOR_ROUND:	lv_img_set_src(ui_ImgPeerType, &ui_img_rolfmodule_round_png); break;
			case MONITOR_BIG:	lv_img_set_src(ui_ImgPeerType, &ui_img_friedermodule_disp_png); break;
		}	

		if (ActivePeer->GetSleepMode()) {
			lv_obj_add_state(ui_BtnPeer3, LV_STATE_CHECKED);
		}
		else {
			lv_obj_clear_state(ui_BtnPeer3, LV_STATE_CHECKED);
		}
		if (ActivePeer->GetDemoMode()) {
			lv_obj_add_state(ui_BtnPeer6, LV_STATE_CHECKED);
		}
		else {
			lv_obj_clear_state(ui_BtnPeer6, LV_STATE_CHECKED);
		}
	}
}
void Ui_Peer_Loaded(lv_event_t * e)
{
	if (!ActivePeer) ActivePeer = FindFirstPeer(MODULE_ALL);

	Ui_Peer_Prepare();
}

void Ui_Peer_Restart(lv_event_t * e)
{
	if (ActivePeer) SendCommand(ActivePeer, "Restart");
}

void Ui_Peer_Reset(lv_event_t * e)
{
	if (ActivePeer) SendCommand(ActivePeer, "Reset");
}

void Ui_Peer_ToggleSleep(lv_event_t * e)
{
	if (ActivePeer) SendCommand(ActivePeer, "SleepMode Toggle");
}

void Ui_Peer_ToggleDemo(lv_event_t * e)
{
	if (ActivePeer) SendCommand(ActivePeer, "DemoMode Toggle");
}

void Ui_Peer_Next(lv_event_t * e)
{
	Serial.println("Peer-Next");
	ActivePeer = FindNextPeer(ActivePeer, MODULE_ALL, true); 
	if (ActivePeer) Ui_Peer_Prepare();
}
void Ui_Peer_Last(lv_event_t * e)
{
	ActivePeer = FindPrevPeer(ActivePeer, MODULE_ALL, true); 
	if (ActivePeer) Ui_Peer_Prepare();
}
#pragma endregion Screen_Peer
#pragma region Screen_Settings
void UI_Set_Prepare(lv_event_t * e)
{
	if (Self.GetPairMode()) {
		lv_obj_add_state(ui_BtnSet2, LV_STATE_CHECKED);
	}
	else {
		lv_obj_clear_state(ui_BtnSet2, LV_STATE_CHECKED);
	}

	if (Self.GetDebugMode()) {
		lv_obj_add_state(ui_BtnSet7, LV_STATE_CHECKED);
	}
	else {
		lv_obj_clear_state(ui_BtnSet7, LV_STATE_CHECKED);
	}

	if (Self.GetChanged()) {
		lv_obj_add_state(ui_BtnSet8, LV_STATE_CHECKED);
	}
	else {
		lv_obj_clear_state(ui_BtnSet8, LV_STATE_CHECKED);
	}
}

void Ui_Set_TogglePair(lv_event_t * e)
{
	if (TogglePairMode()) {
		lv_obj_add_state(ui_BtnSet2, LV_STATE_CHECKED);
	}
	else {
		lv_obj_clear_state(ui_BtnSet2, LV_STATE_CHECKED);
	}
}

void Ui_Set_Restart(lv_event_t * e)
{
	ESP.restart();
}

void Ui_Set_Reset(lv_event_t * e)
{
	nvs_flash_erase(); nvs_flash_init();
	ESP.restart();
}

void Ui_Set_ToggleDebug(lv_event_t * e)
{
	if (ToggleDebugMode()) {
		lv_obj_add_state(ui_BtnSet7, LV_STATE_CHECKED);
	}
	else {
		lv_obj_clear_state(ui_BtnSet7, LV_STATE_CHECKED);
	}
}

void Ui_Set_SavePeers(lv_event_t * e)
{
    SavePeers();
	if (Self.GetChanged()) {
		lv_obj_add_state(ui_BtnSet8, LV_STATE_CHECKED);
	}
	else {
		lv_obj_clear_state(ui_BtnSet8, LV_STATE_CHECKED);
	}
}
#pragma endregion Screen_Settings
#pragma region Screen_Peers
void Ui_Peers_Prepare(lv_event_t * e)
{
	String Options = "";
	PeerClass *P;

    for(int i = 0; i < PeerList.size(); i++){

		P = PeerList.get(i);

		if (Options != "") Options += "\n";
			
		if (millis()- P->GetTSLastSeen() > OFFLINE_INTERVAL) Options += "off: <";
		else Options += "on:  <"; 
			
		Options += P->GetName();

		switch (P->GetType()) {
			case SWITCH_1_WAY:   Options += "> PDC-1"; break;
			case SWITCH_2_WAY:   Options += "> PDC-2"; break;
			case SWITCH_4_WAY:   Options += "> PDC-4"; break;
			case SWITCH_8_WAY:   Options += "> PDC-8"; break;
			case PDC_SENSOR_MIX: Options += "> MIX";   break;
			case BATTERY_SENSOR: Options += "> Sens";  break;
			default:		     Options += "> ???";   break;
		}
	}
	
	lv_roller_set_options(ui_RollerPeers1, Options.c_str(), LV_ROLLER_MODE_NORMAL);
}

void Ui_Peers_Selected(lv_event_t * e)
{
	char buf[100];
	char SelectedName[50];
	
	lv_roller_get_selected_str(ui_RollerPeers1, buf, 100);
	Serial.print("Selected: ");
	Serial.println(buf);

  	char *Start = strchr(buf,'<'); 
	char *End = strchr(buf,'>'); 
	
  	memcpy(SelectedName, Start+1, End-Start-1);
	SelectedName[End-Start-1] = 0;
	Serial.println(SelectedName);

	PeerClass *TempPeer = FindPeerByName(SelectedName);

	if ((TempPeer) and (strcmp(SelectedName, "") != 0)) {
		ActivePeer = TempPeer;
		_ui_screen_change(&ui_ScrPeer, LV_SCR_LOAD_ANIM_FADE_ON, 50, 0, &ui_ScrPeer_screen_init);
	}
}
#pragma endregion Screen_Peers
#pragma region Screen_JSON
void Ui_JSON_Prepare(lv_event_t * e)
{
	PrepareJSON();
}
#pragma endregion Screen_JSON
#pragma region Screen_SingleMeter
void Ui_Single_Next(lv_event_t * e)
{	
	if (ActivePeriphSingle) 
	{
		ActivePeriphSingle = FindNextPeriph(NULL, ActivePeriphSingle, SENS_TYPE_SENS, true);
	}
	
	if (ActivePeriphSingle)
	{
		_ui_screen_change(&ui_ScrSingle, LV_SCR_LOAD_ANIM_NONE, 50, 0, &ui_ScrSingle_screen_init);
	}
}

void Ui_Single_Last(lv_event_t * e)
{
	if (ActivePeriphSingle) 
	{
		ActivePeriphSingle = FindPrevPeriph(NULL, ActivePeriphSingle, SENS_TYPE_SENS, true);
	}
	
	if (ActivePeriphSingle)
	{
		_ui_screen_change(&ui_ScrSingle, LV_SCR_LOAD_ANIM_NONE, 50, 0, &ui_ScrSingle_screen_init);
	}
}

void Ui_Single_Prepare(lv_event_t * e)
{
	Serial.println("Single-Prepare");
	
	if (!ActivePeriphSingle) ActivePeriphSingle = FindFirstPeriph(NULL, SENS_TYPE_SENS);
		
	if (ActivePeriphSingle)
	{
		lv_label_set_text(ui_LblSinglePeriph, ActivePeriphSingle->GetName());
		lv_label_set_text(ui_LblSinglePeer, FindPeerById(ActivePeriphSingle->GetPeerId())->GetName());
	}
	else
	{
		lv_label_set_text(ui_LblSinglePeriph, "n.n.");
		lv_label_set_text(ui_LblSinglePeer, "n.n.");
	}
	
	if (ActivePeriphSingle)
	{
		Serial.println("ActivePeriphSingle true");
		uint32_t user_data = 10;

		GenerateSingleMeter();
		Serial.println("Scale Generated");
		
		if (SingleTimer) 
		{
			lv_timer_resume(SingleTimer);
			
			Serial.println("SingleTimer resumed");
		}
		else 
		{
			SingleTimer = lv_timer_create(SingleUpdateTimer, 500,  &user_data);
			Serial.println("SingleTimer created");
		}

		Serial.println((unsigned)SingleTimer);
	}
}

void SingleUpdateTimer(lv_timer_t * timer)
{
	char buf[10];
	int nk = 0;
	float value;

	Serial.println("SinglUpdateTimer");
	
	if (ActivePeriphSingle)
	{
		value = ActivePeriphSingle->GetValue();
		//if (DebugMode) Serial.printf("Sensor: %s: %f\n", ActiveSens->Name, value);

		if      (value<10)  nk = 2;
		else if (value<100) nk = 1;
		else                nk = 0;

		if (value == -99) strcpy(buf, "--"); 
		else dtostrf(value, 0, nk, buf);

		if (ActivePeriphSingle->GetType() == SENS_TYPE_AMP)  strcat(buf, " A");
		if (ActivePeriphSingle->GetType() == SENS_TYPE_VOLT) strcat(buf, " V");
		
		lv_meter_set_indicator_value(SingleMeter, SingleIndicNeedle, value*10);
		lv_label_set_text(ui_LblSingleValue, buf);
	}
}

void Ui_Single_Leave(lv_event_t * e)
{
	lv_timer_del(SingleTimer);
	SingleTimer = NULL;

	Serial.println("SingleTimer deleted");
    
	lv_obj_del(SingleMeter);
	
	
	SingleMeter       = NULL;
	scale             = NULL;
	SingleIndicNeedle = NULL;
}

static void SingleMeter_cb(lv_event_t * e) {

	lv_obj_draw_part_dsc_t	*dsc  = (lv_obj_draw_part_dsc_t *)lv_event_get_param(e);
	double					value;

	if( dsc->text != NULL ) {		// Filter major ticks...
		value = dsc->value / 10;
		snprintf(dsc->text, sizeof(dsc->text), "%5.1f", value);
	}

}
void GenerateSingleMeter(void)
{
	SingleMeter = lv_meter_create(ui_ScrSingle);
	lv_obj_center(SingleMeter);
	lv_obj_set_style_bg_color(SingleMeter, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(SingleMeter, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
	lv_obj_set_size(SingleMeter, 240,	240);
	scale = lv_meter_add_scale(SingleMeter);
	
	lv_obj_move_background(SingleMeter);
	lv_obj_set_style_text_color(SingleMeter, lv_color_hex(0xdbdbdb), LV_PART_TICKS);
	
	SingleIndicNeedle = lv_meter_add_needle_line(SingleMeter, scale, 4, lv_palette_main(LV_PALETTE_GREY), -10);
	
	if ((ActivePeriphSingle) and (ActivePeriphSingle->GetType() == SENS_TYPE_AMP))
	{
		lv_meter_set_scale_ticks(SingleMeter, scale, 41, 2, 10, lv_palette_main(LV_PALETTE_GREY));
    	lv_meter_set_scale_major_ticks(SingleMeter, scale, 5, 4, 15, lv_color_black(), 15);
    	lv_meter_set_scale_range(SingleMeter, scale, 0, 400, 240, 150);
	
		//Add a green arc to the start
		SingleIndic = lv_meter_add_scale_lines(SingleMeter, scale, lv_palette_main(LV_PALETTE_GREEN), lv_palette_main(LV_PALETTE_GREEN), false, 0);
    	lv_meter_set_indicator_start_value(SingleMeter, SingleIndic, 0);
    	lv_meter_set_indicator_end_value(SingleMeter, SingleIndic, 250);

		SingleIndic = lv_meter_add_arc(SingleMeter, scale, 3, lv_palette_main(LV_PALETTE_RED), 0);
    	lv_meter_set_indicator_start_value(SingleMeter, SingleIndic, 300);
    	lv_meter_set_indicator_end_value(SingleMeter, SingleIndic, 400);

		//Make the tick lines red at the end of the scale
		SingleIndic = lv_meter_add_scale_lines(SingleMeter, scale, lv_palette_main(LV_PALETTE_RED), lv_palette_main(LV_PALETTE_RED), false, 0);
		lv_meter_set_indicator_start_value(SingleMeter, SingleIndic, 300);
		lv_meter_set_indicator_end_value(SingleMeter, SingleIndic, 400);

		lv_obj_add_event_cb(SingleMeter, SingleMeter_cb, LV_EVENT_DRAW_PART_BEGIN, NULL);
	}
	else if ((ActivePeriphSingle) and (ActivePeriphSingle->GetType() == SENS_TYPE_VOLT))
	{	
		lv_meter_set_scale_ticks(SingleMeter, scale, 31, 2, 10, lv_palette_main(LV_PALETTE_GREY));
    	lv_meter_set_scale_major_ticks(SingleMeter, scale, 5, 4, 15, lv_color_black(), 15);
    	lv_meter_set_scale_range(SingleMeter, scale, 90, 150, 240, 150);
	
		SingleIndic = lv_meter_add_scale_lines(SingleMeter, scale, lv_palette_main(LV_PALETTE_RED), lv_palette_main(LV_PALETTE_RED), false, 0);
    	lv_meter_set_indicator_start_value(SingleMeter, SingleIndic, 90);
    	lv_meter_set_indicator_end_value(SingleMeter, SingleIndic, 112);
		
		//Add a green arc to the start
		SingleIndic = lv_meter_add_scale_lines(SingleMeter, scale, lv_palette_main(LV_PALETTE_GREEN), lv_palette_main(LV_PALETTE_GREEN), false, 0);
    	lv_meter_set_indicator_start_value(SingleMeter, SingleIndic, 112);
    	lv_meter_set_indicator_end_value(SingleMeter, SingleIndic, 144);

		SingleIndic = lv_meter_add_arc(SingleMeter, scale, 3, lv_palette_main(LV_PALETTE_RED), 0);
    	lv_meter_set_indicator_start_value(SingleMeter, SingleIndic, 144);
    	lv_meter_set_indicator_end_value(SingleMeter, SingleIndic, 150);

		//Make the tick lines red at the end of the scale
		SingleIndic = lv_meter_add_scale_lines(SingleMeter, scale, lv_palette_main(LV_PALETTE_RED), lv_palette_main(LV_PALETTE_RED), false, 0);
		lv_meter_set_indicator_start_value(SingleMeter, SingleIndic, 144);
		lv_meter_set_indicator_end_value(SingleMeter, SingleIndic, 150);

		//Add draw callback to override default values
		lv_obj_add_event_cb(SingleMeter, SingleMeter_cb, LV_EVENT_DRAW_PART_BEGIN, NULL);
	}
}
#pragma endregion Screen_SingleMeter
#pragma region Screen_MultiMeter
void Ui_Multi_Loaded(lv_event_t * e)
{
	static uint32_t user_data = 10;
		
	lv_label_set_text(ui_LblMultiScreenName,Screen[ActiveMultiScreen].GetName());
					
	for (int Pos=0; Pos<PERIPH_PER_SCREEN; Pos++)
	{
		int x; int y;
		switch (Pos) {
			case 0: x=-50; y=-50; break;
			case 1: x= 50; y=-50; break;
			case 2: x=-50; y= 45; break;
			case 3: x= 50; y= 45; break;
		}

		if (Screen[ActiveMultiScreen].GetPeriph(Pos))
		{
			lv_obj_add_flag(lv_obj_get_child(lv_scr_act(), Pos+1), LV_OBJ_FLAG_HIDDEN);
			if (Screen[ActiveMultiScreen].GetPeriph(Pos)->IsSensor())
			{	
				lv_obj_t *ui_ButtonSensorSmall = ui_ButtonSensorSmall_create(ui_ScrMulti);
				lv_obj_set_x(ui_ButtonSensorSmall, x);
				lv_obj_set_y(ui_ButtonSensorSmall, y);

				MultiComponent[Pos] = ui_ButtonSensorSmall;
				
				lv_obj_t *SensButton           = ui_comp_get_child(ui_ButtonSensorSmall, UI_COMP_BUTTONSENSORSMALL_BUTTONSENSORSMALL);
				lv_obj_t *SensButtonPeerName   = ui_comp_get_child(ui_ButtonSensorSmall, UI_COMP_BUTTONSENSORSMALL_LBLSENSSMALLPEER);
				lv_obj_t *SensButtonPeriphName = ui_comp_get_child(ui_ButtonSensorSmall, UI_COMP_BUTTONSENSORSMALL_LBLSENSSMALLPERIPH);
				lv_obj_t *SensButtonPos        = ui_comp_get_child(ui_ButtonSensorSmall, UI_COMP_BUTTONSENSORSMALL_LBLSENSPOS);
				
				lv_label_set_text_fmt(SensButtonPeerName,   "%.6s", FindPeerById(Screen[ActiveMultiScreen].GetPeriph(Pos)->GetPeerId())->GetName());
				lv_label_set_text_fmt(SensButtonPeriphName, "%.6s", Screen[ActiveMultiScreen].GetPeriph(Pos)->GetName());
				lv_label_set_text_fmt(SensButtonPos, "%d", Pos);

				lv_obj_add_event_cb(ui_ButtonSensorSmall, Ui_Multi_Sensor_Clicked, LV_EVENT_ALL, NULL);  
			}
			else if (Screen[ActiveMultiScreen].GetPeriph(Pos)->IsSwitch())
			{
				lv_obj_t *ui_ButtonSwitchSmall = ui_ButtonSwitchSmall_create(ui_ScrMulti);
				lv_obj_set_x(ui_ButtonSwitchSmall, x);
				lv_obj_set_y(ui_ButtonSwitchSmall, y);

				MultiComponent[Pos] = ui_ButtonSwitchSmall;
				
				lv_obj_t *SwitchButton           = ui_comp_get_child(ui_ButtonSwitchSmall, UI_COMP_BUTTONSWITCHSMALL_BUTTONSWITCHSMALL);
				lv_obj_t *SwitchButtonPeerName   = ui_comp_get_child(ui_ButtonSwitchSmall, UI_COMP_BUTTONSWITCHSMALL_LBLPEER);
				lv_obj_t *SwitchButtonPeriphName = ui_comp_get_child(ui_ButtonSwitchSmall, UI_COMP_BUTTONSWITCHSMALL_LBLPERIPH);
				lv_obj_t *SwitchButtonPos        = ui_comp_get_child(ui_ButtonSwitchSmall, UI_COMP_BUTTONSWITCHSMALL_LBLPOSITION);
				
				lv_label_set_text_fmt(SwitchButtonPeerName,   "%.6s", FindPeerById(Screen[ActiveMultiScreen].GetPeriph(Pos)->GetPeerId())->GetName());
				lv_label_set_text_fmt(SwitchButtonPeriphName, "%.6s", Screen[ActiveMultiScreen].GetPeriph(Pos)->GetName());
				lv_label_set_text_fmt(SwitchButtonPos, "%d", Pos);

				if (Screen[ActiveMultiScreen].GetPeriph(Pos)->GetValue() == 1) 
					lv_imgbtn_set_state(SwitchButton, LV_IMGBTN_STATE_CHECKED_RELEASED);

				lv_obj_add_event_cb(ui_ButtonSwitchSmall, Ui_Multi_Button_Clicked, LV_EVENT_ALL, NULL);  
			}
		}
		else
		{
			lv_obj_clear_flag(lv_obj_get_child(lv_scr_act(), Pos+1), LV_OBJ_FLAG_HIDDEN);
		}
	}
	if (MultiTimer) 
	{
		lv_timer_resume(MultiTimer);
		Serial.println("MultiTimer resumed");
	}
	else 
	{
		MultiTimer = lv_timer_create(MultiUpdateTimer, 500,  &user_data);
		Serial.println("MultiTimer created");
	}
}
void MultiUpdateTimer(lv_timer_t * timer)
{
	_lv_obj_t *ComponentValue;
	_lv_obj_t *ComponentARC;
	
	static char ValueBuf[10];
	static int nk = 0;
	static float value;
	lv_color_t bg;

	Serial.printf("MultiTimer - Screen[%d] \n\r",ActiveMultiScreen);
	
	for (int Pos=0; Pos<4; Pos++) 
	{
		if (Screen[ActiveMultiScreen].GetPeriphId(Pos) > 0)
		{
			value = Screen[ActiveMultiScreen].GetPeriph(Pos)->GetValue();
		
			if      (value<10)  nk = 2;
			else if (value<100) nk = 1;
			else                nk = 0;

			if (value == -99) strcpy(ValueBuf, "--"); 
			else dtostrf(value, 0, nk, ValueBuf);

			switch (Screen[ActiveMultiScreen].GetPeriph(Pos)->GetType()) 
			{
				case SENS_TYPE_AMP:
					ComponentValue = ui_comp_get_child(MultiComponent[Pos], UI_COMP_BUTTONSENSORSMALL_LBLSENSSMALLVALUE);
			
					strcat(ValueBuf, " A");
					
					if 		(value < 20) bg = lv_color_hex(0x135A25);
					else if (value < 25) bg = lv_color_hex(0x7C7E26);
					else 				 bg = lv_color_hex(0x88182C);

					lv_obj_set_style_bg_color(MultiComponent[Pos], bg, LV_PART_MAIN | LV_STATE_DEFAULT);
					lv_label_set_text(ComponentValue, ValueBuf);

					ComponentARC = ui_comp_get_child(MultiComponent[Pos], UI_COMP_BUTTONSENSORSMALL_ARC2);
					lv_arc_set_range(ComponentARC, 0, 400);
					lv_arc_set_value(ComponentARC, value*10);
					
					break;
				case SENS_TYPE_VOLT:
					ComponentValue = ui_comp_get_child(MultiComponent[Pos], UI_COMP_BUTTONSENSORSMALL_LBLSENSSMALLVALUE);
					
					strcat(ValueBuf, " V");
					
					if 		(value < 13)   bg = lv_color_hex(0x135A25);
					else if (value < 14.4) bg = lv_color_hex(0x7C7E26);
					else 				   bg = lv_color_hex(0x88182C);

					lv_obj_set_style_bg_color(MultiComponent[Pos], bg, LV_PART_MAIN | LV_STATE_DEFAULT);
					lv_label_set_text(ComponentValue, ValueBuf);

					ComponentARC = ui_comp_get_child(MultiComponent[Pos], UI_COMP_BUTTONSENSORSMALL_ARC2);
					lv_arc_set_range(ComponentARC, 90, 150);
					lv_arc_set_value(ComponentARC, value*10);

					break;
				case SENS_TYPE_SWITCH:
					lv_obj_t *Button = ui_comp_get_child(MultiComponent[Pos], UI_COMP_BUTTONSWITCHSMALL_BUTTONSWITCHSMALL);
						
					if (value == 1) 
					{
						Serial.printf("Button %d ist 1", Pos);
						lv_imgbtn_set_state(Button, LV_IMGBTN_STATE_CHECKED_RELEASED);
					}
					else
					{
						Serial.printf("Button %d ist 0", Pos);
						lv_imgbtn_set_state(Button, LV_IMGBTN_STATE_RELEASED);
					}

					break;
			}
		}
	}
}
void Ui_Multi_Button_Clicked(lv_event_t * e)
{
	lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t * target = lv_event_get_target(e);
    
	if(event_code == LV_EVENT_CLICKED) {
        lv_obj_t *Button = ui_comp_get_child(target, UI_COMP_BUTTONSWITCHSMALL_LBLPOSITION);
	
		int Pos = atoi(lv_label_get_text(Button));

		ToggleSwitch(Screen[ActiveMultiScreen].GetPeer(Pos), Pos);
    }	
	if(event_code == LV_EVENT_LONG_PRESSED) {
        lv_obj_t *Button = ui_comp_get_child(target, UI_COMP_BUTTONSWITCHSMALL_LBLPOSITION);
	
		MultiPosToChange = atoi(lv_label_get_text(Button));

		_ui_screen_change(&ui_ScrPeriph, LV_SCR_LOAD_ANIM_NONE, 0, 0, &ui_ScrPeriph_screen_init);
    }
}
void Ui_Multi_Sensor_Clicked(lv_event_t * e)
{
	lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t * target = lv_event_get_target(e);
    
	if(event_code == LV_EVENT_CLICKED) {
        lv_obj_t *Sensor = ui_comp_get_child(target, UI_COMP_BUTTONSWITCHSMALL_LBLPOSITION);
	
		int Pos = atoi(lv_label_get_text(Sensor));
		ActivePeriph = Screen[ActiveMultiScreen].GetPeriph(Pos);
		ActivePeer   = Screen[ActiveMultiScreen].GetPeer(Pos);
		
		_ui_screen_change(&ui_ScrSingle, LV_SCR_LOAD_ANIM_FADE_ON, 50, 0, &ui_ScrSingle_screen_init);
    }	
	if(event_code == LV_EVENT_LONG_PRESSED) {
        lv_obj_t *Button = ui_comp_get_child(target, UI_COMP_BUTTONSWITCHSMALL_LBLPOSITION);
	
		MultiPosToChange = atoi(lv_label_get_text(Button));

		_ui_screen_change(&ui_ScrPeriph, LV_SCR_LOAD_ANIM_NONE, 0, 0, &ui_ScrPeriph_screen_init);
    }
}
void Ui_Multi_Set_Panel1(lv_event_t * e)
{
	MultiPosToChange = 0;
	_ui_screen_change(&ui_ScrPeriph, LV_SCR_LOAD_ANIM_NONE, 0, 0, &ui_ScrPeriph_screen_init);
}
void Ui_Multi_Set_Panel2(lv_event_t * e)
{
	MultiPosToChange = 1;
	_ui_screen_change(&ui_ScrPeriph, LV_SCR_LOAD_ANIM_NONE, 0, 0, &ui_ScrPeriph_screen_init);
}
void Ui_Multi_Set_Panel3(lv_event_t * e)
{
	MultiPosToChange = 2;
	_ui_screen_change(&ui_ScrPeriph, LV_SCR_LOAD_ANIM_NONE, 0, 0, &ui_ScrPeriph_screen_init);
}
void Ui_Multi_Set_Panel4(lv_event_t * e)
{
	MultiPosToChange = 3;
	_ui_screen_change(&ui_ScrPeriph, LV_SCR_LOAD_ANIM_NONE, 0, 0, &ui_ScrPeriph_screen_init);
}
void Ui_Multi_Unload(lv_event_t * e)
{
	if (MultiTimer) lv_timer_del(MultiTimer);
	MultiTimer = NULL;
	Serial.println("MultiTimer deleted");
	
	for (int Pos = 0; Pos<PERIPH_PER_SCREEN; Pos++)
	{
		if (MultiComponent[Pos])
		{
			lv_obj_del(MultiComponent[Pos]);
			MultiComponent[Pos] = NULL;
		}
	}
}
void Ui_Multi_Next(lv_event_t * e)
{
	ActiveMultiScreen++;
	if (ActiveMultiScreen == MULTI_SCREENS) ActiveMultiScreen = 0;
	_ui_screen_change(&ui_ScrMulti, LV_SCR_LOAD_ANIM_NONE, 50, 0, &ui_ScrMulti_screen_init);
}
void Ui_Multi_Prev(lv_event_t * e)
{
	ActiveMultiScreen--;
	if (ActiveMultiScreen == -1) ActiveMultiScreen = MULTI_SCREENS-1;
	_ui_screen_change(&ui_ScrMulti, LV_SCR_LOAD_ANIM_NONE, 50, 0, &ui_ScrMulti_screen_init);
}
#pragma endregion Screen_MultiMeter
#pragma region Screen_Switch
void SwitchUpdateTimer(lv_timer_t * timer)
{
	if (ActivePeriphSwitch){
		if (ActivePeriphSwitch->GetValue() == 1)
		{
			lv_obj_set_style_bg_img_src(ui_ScrSwitch, &ui_img_btn_png, LV_PART_MAIN | LV_STATE_DEFAULT);
		}
		else
		{
			lv_obj_set_style_bg_img_src(ui_ScrSwitch, &ui_img_btn_off_png, LV_PART_MAIN | LV_STATE_DEFAULT);
		}
	}
}
void Ui_Switch_Next(lv_event_t * e)
{
	if (ActivePeriphSwitch) 
	{
		ActivePeriphSwitch = FindNextPeriph(NULL, ActivePeriphSwitch, SENS_TYPE_SWITCH, true);
	}
	
	if (ActivePeriphSwitch)
	{
		_ui_screen_change(&ui_ScrSwitch, LV_SCR_LOAD_ANIM_NONE, 50, 0, &ui_ScrSwitch_screen_init);
	}
}
void Ui_Switch_Long(lv_event_t * e)
{
	ToggleSwitch(ActivePeriphSwitch);
}
void Ui_Switch_Prev(lv_event_t * e)
{
	if (ActivePeriphSwitch) 
	{
		ActivePeriphSwitch = FindPrevPeriph(NULL, ActivePeriphSwitch, SENS_TYPE_SWITCH, true);
	}
	
	if (ActivePeriphSwitch)
	{
		_ui_screen_change(&ui_ScrSwitch, LV_SCR_LOAD_ANIM_NONE, 50, 0, &ui_ScrSwitch_screen_init);
	}
}
void Ui_Switch_Loaded(lv_event_t * e)
{
	if (!ActivePeriphSwitch) 
	{
		Serial.println("No ActivePeriphSwitch");
		ActivePeriphSwitch = FindNextPeriph(NULL, NULL, SENS_TYPE_SWITCH, true);
	}	
	if (ActivePeriphSwitch)
	{
		Serial.println(ActivePeriphSwitch->GetName());
		
		lv_label_set_text(ui_LblSwitchPeriph, ActivePeriphSwitch->GetName());
		lv_label_set_text(ui_LblSwitchPeer, FindPeerById(ActivePeriphSwitch->GetPeerId())->GetName());
	}
	else
	{
		lv_obj_set_style_bg_img_src(ui_ScrSwitch, &ui_img_btn_off_png, LV_PART_MAIN | LV_STATE_DEFAULT);

		lv_label_set_text(ui_LblSwitchPeriph, "n.n.");
		lv_label_set_text(ui_LblSwitchPeer, "n.n.");
	}

	static uint32_t user_data = 10;
	if (!SwitchTimer) { 
		SwitchTimer = lv_timer_create(SwitchUpdateTimer, 500,  &user_data);
	}
	else {
		lv_timer_resume(SwitchTimer);
	}
}
void Ui_Switch_Leave(lv_event_t * e)
{
	if (SwitchTimer) 
	{
		lv_timer_del(SwitchTimer);
		SwitchTimer = NULL;
	}

}
#pragma endregion Screen_Switch
#pragma region Screen_PeriphChoice
void Ui_PeriphChoice_Next(lv_event_t * e)
{
	if (ActivePeriph) {
		PeriphClass *PeriphT = FindNextPeriph(NULL, ActivePeriph, SENS_TYPE_ALL, true);
		if (PeriphT) 
		{
			ActivePeriph = PeriphT;
			Ui_Periph_Choice_Loaded(e);
		}
	}
}

void Ui_PeriphChoice_Last(lv_event_t * e)
{
	if (ActivePeriph) {
		PeriphClass *PeriphT = FindPrevPeriph(NULL, ActivePeriph, SENS_TYPE_ALL, true);
		if (PeriphT) 
		{
			ActivePeriph = PeriphT;
			Ui_Periph_Choice_Loaded(e);
		}
	}
}

void Ui_PeriphChoice_Click(lv_event_t * e)
{
	Screen[ActiveMultiScreen].AddPeriph(MultiPosToChange, ActivePeriph);
	_ui_screen_change(&ui_ScrMulti, LV_SCR_LOAD_ANIM_FADE_ON, 50, 0, &ui_ScrMulti_screen_init);
}

void Ui_Periph_Choice_Loaded(lv_event_t * e)
{
	Serial.println("PeriphChoice loaded");

	PeerClass *P;
	if (!ActivePeriph) {
		ActivePeriph = FindFirstPeriph(NULL, SENS_TYPE_ALL);
	}
	Serial.printf("aktueller Periph: %s", ActivePeriph->GetName());

	if (ActivePeriph) {
		Serial.println("ActivePeriph in PeriphCoice");

		P = FindPeerById(ActivePeriph->GetPeerId());

		lv_label_set_text(ui_LblPeriphChoicePeriph, ActivePeriph->GetName());
		lv_label_set_text(ui_LblPeriphChoicePeer, P->GetName());
	
		if (millis()- P->GetTSLastSeen() > OFFLINE_INTERVAL) 
			lv_label_set_text(ui_LblPeriphChoiceOnline, "Offline");
      	else 
			lv_label_set_text(ui_LblPeriphChoiceOnline, "Online");

		switch (ActivePeriph->GetType()) {
			case SENS_TYPE_SWITCH:	lv_label_set_text(ui_LblPeriphChoiceType, "Switch"); 
									lv_img_set_src(ui_ImgPeerChoice, &ui_img_menubtn2_png);
									break;
			case SENS_TYPE_AMP:		lv_label_set_text(ui_LblPeriphChoiceType, "Amp-Sensor"); 
									lv_img_set_src(ui_ImgPeerChoice, &ui_img_menubtn1_png);
									break;
			case SENS_TYPE_VOLT:	lv_label_set_text(ui_LblPeriphChoiceType, "Volt-Sensor"); 
									lv_img_set_src(ui_ImgPeerChoice, &ui_img_menubtn1_png);
									break;
			default:				lv_label_set_text(ui_LblPeriphChoiceType, "unknown type"); break;
		}
		Serial.println("ActivePeriph in PeriphCoice fertig");
	}
	Serial.println("PeriphCoice ende");
}

#pragma endregion Screen_PeriphChoice
#pragma region System_TimerAndInit
void TopUpdateTimer(lv_timer_t * timer)
{
	if ((TSMsgSnd) and (millis() - TSMsgSnd < MSGLIGHT_INTERVAL)) {
		lv_led_on(Ui_LedSnd);
	}
	else {
		lv_led_off(Ui_LedSnd);
		TSMsgSnd = 0;

	}

	if ((TSMsgRcv) and (millis() - TSMsgRcv < MSGLIGHT_INTERVAL)) {
		lv_led_on(Ui_LedRcv);
	}
	else {
		lv_led_off(Ui_LedRcv);
		TSMsgRcv = 0;
	}

	if ((TSPair)  and (millis() - TSPair < PAIR_INTERVAL)){
		lv_led_on(Ui_LedPair);
	}
	else {
		lv_led_off(Ui_LedPair);
		TSPair = 0;
	}
}

void Ui_Init_Custom(lv_event_t * e)
{
	//LED-Layer
	static uint32_t user_data = 10; 
	char LEDSize = 10;
	lv_timer_t * timer = lv_timer_create(TopUpdateTimer, 100,  &user_data);

	Ui_LedRcv  = lv_led_create(lv_layer_top());
	lv_obj_set_size(Ui_LedRcv, LEDSize, LEDSize);
    lv_obj_align(Ui_LedRcv, LV_ALIGN_CENTER, 0, 110);
    lv_led_set_color(Ui_LedRcv, lv_palette_main(LV_PALETTE_GREEN));
	lv_led_off(Ui_LedRcv);

    Ui_LedSnd  = lv_led_create(lv_layer_top());
	lv_obj_set_size(Ui_LedSnd, LEDSize, LEDSize);
    lv_obj_align(Ui_LedSnd, LV_ALIGN_CENTER, -20, 107);
    lv_led_set_color(Ui_LedSnd, lv_palette_main(LV_PALETTE_BLUE));

    Ui_LedPair  = lv_led_create(lv_layer_top());
    lv_obj_set_size(Ui_LedPair, LEDSize, LEDSize);
	lv_obj_align(Ui_LedPair, LV_ALIGN_CENTER, 20, 107);
    lv_led_set_color(Ui_LedPair, lv_palette_main(LV_PALETTE_RED));

	//Keyboard
	static const char* kb_map[] = { "1", "2", "3", "\n",
								"4", "5", "6", "\n",
								"7", "8", "9", "\n",
								"." ,"0", LV_SYMBOL_BACKSPACE,"\n",
								LV_SYMBOL_CLOSE, LV_SYMBOL_OK, "" };
	static const lv_btnmatrix_ctrl_t kb_ctrl[] = {1,1,1,1,1,1,1,1,1,1,1,1,1,1};
    
	lv_keyboard_set_map(ui_Keyboard, LV_KEYBOARD_MODE_USER_1, kb_map, kb_ctrl);
    lv_keyboard_set_mode(ui_Keyboard, LV_KEYBOARD_MODE_USER_1);
	lv_obj_add_event_cb(ui_Keyboard, Keyboard_cb, LV_EVENT_READY, NULL);

	lv_label_set_text(ui_LblMenuVersion, _Version);
}

void Keyboard_cb(lv_event_t * event)
{
    CalibVolt();
	_ui_screen_change(&ui_ScrMenu, LV_SCR_LOAD_ANIM_FADE_ON, 50, 0, &ui_ScrMenu_screen_init);
}
#pragma endregion System_TimerAndInit
#pragma region System_Eichen
void Ui_Eichen_Start(lv_event_t * e)
{
	SendCommand(ActivePeer, "Eichen");
	TSMsgSnd = millis();
	_ui_screen_change(&ui_ScrMenu, LV_SCR_LOAD_ANIM_FADE_ON, 50, 0, &ui_ScrMenu_screen_init);
}

void Ui_Volt_Prepare(lv_event_t * e)
{
	if (ActivePeer) lv_label_set_text(ui_LblVoltPeer, ActivePeer->GetName());
}
#pragma endregion System_Eichen

void Ui_Volt_Start(lv_event_t * e)
{
	CalibVolt();
	_ui_screen_change(&ui_ScrMenu, LV_SCR_LOAD_ANIM_FADE_ON, 50, 0, &ui_ScrMenu_screen_init);
}

void Ui_Periph_Choice_prepare(lv_event_t * e)
{
	// Your code here
}
