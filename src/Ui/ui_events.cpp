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

lv_obj_t *SingleMeter;
lv_meter_indicator_t * SingleIndic;
lv_meter_indicator_t * SingleIndicNeedle;
lv_meter_scale_t * scale;
uint8_t MultiPosToChange;
PeerClass   *ActivePeerSingle;
PeriphClass *ActivePeriphSingle;

lv_obj_t *Ui_LedSnd;
lv_obj_t *Ui_LedRcv;
lv_obj_t *Ui_LedPair;

lv_timer_t *SingleTimer;
lv_timer_t *MultiTimer;
lv_timer_t *SwitchTimer;

void GenerateSingleScale(void);
void Keyboard_cb(lv_event_t * event);

#pragma region Screen_Peer
void Ui_Peer_Prepare(lv_event_t * e)
{
	if (ActivePeer) {
		lv_label_set_text_static(ui_LblPeerName, ActivePeer->GetName());
	
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

void Ui_Peer_Restart(lv_event_t * e)
{
	SendCommand(ActivePeer, "Restart");
}

void Ui_Peer_Reset(lv_event_t * e)
{
	SendCommand(ActivePeer, "Reset");
}

void Ui_Peer_ToggleSleep(lv_event_t * e)
{
	SendCommand(ActivePeer, "SleepMode Toggle");
}

void Ui_Peer_ToggleDemo(lv_event_t * e)
{
	SendCommand(ActivePeer, "DemoMode Toggle");
}

void Ui_Peer_Next(lv_event_t * e)
{
	Serial.println("Peer-Next");
	ActivePeer = FindNextPeer(ActivePeer, MODULE_ALL, true); 
	_ui_screen_change(&ui_ScrPeer, LV_SCR_LOAD_ANIM_FADE_ON, 50, 0, &ui_ScrPeer_screen_init);
}
void Ui_Peer_Last(lv_event_t * e)
{
	ActivePeer = FindPrevPeer(ActivePeer, MODULE_ALL, true); 
	_ui_screen_change(&ui_ScrPeer, LV_SCR_LOAD_ANIM_FADE_ON, 50, 0, &ui_ScrPeer_screen_init);
}

void Ui_Peer_Eichen(lv_event_t * e)
{
	SendCommand(ActivePeer, "Eichen");
}

void Ui_Peer_Volt(lv_event_t * e)
{
	// Your code here
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
	if (ActiveSens) {
		ActiveSens = FindNextPeriph(ActiveSens, SENS_TYPE_SENS, false);
		// no Sens found in ActivePeer
		if (!ActiveSens) 
		{ 
			// find next peer
			ActivePeer = FindNextPeer(ActivePeer, MODULE_ALL, true); 
			if (ActivePeer) ActiveSens = FindFirstPeriph(ActivePeer, SENS_TYPE_SENS);
		}
	}
	else {
		ActiveSens = FindFirstPeriph(ActivePeer, SENS_TYPE_SENS);
	}
	
	if (ActiveSens)
	{
		GenerateSingleScale();

		lv_label_set_text(ui_LblSinglePeer, ActivePeer->GetName());
		lv_label_set_text(ui_LblSinglePeriph, ActiveSens->GetName());
		
		lv_label_set_text(ui_LblSingleValue, "---");
	}
}

void Ui_Single_Last(lv_event_t * e)
{
	if (ActiveSens) {
		ActiveSens = FindPrevPeriph(ActiveSens, SENS_TYPE_SENS, false);
	}
	else {
		ActiveSens = FindFirstPeriph(ActivePeer, SENS_TYPE_SENS);
	}
	
	GenerateSingleScale();

	if (ActivePeer) lv_label_set_text(ui_LblSinglePeer, ActivePeer->GetName());
	if (ActiveSens) lv_label_set_text(ui_LblSinglePeriph, ActiveSens->GetName());
	//lv_meter_set_indicator_value(SingleMeter, SingleIndic, -10);
	lv_label_set_text(ui_LblSingleValue, "---");
}

void Ui_Single_Prepare(lv_event_t * e)
{
	Serial.println("Single-Prepare");
	
	if (ActivePeerSingle) 
	{	lv_label_set_text(ui_LblSinglePeer, ActivePeer->GetName());
	}
	else
	{
		ActivePeerSingle = FindFirstPeer(MODULE_ALL);
	}
	
	if (ActivePeerSingle) 
	{ 	
		if (ActivePeriphSingle) 
		{
			lv_label_set_text(ui_LblSinglePeriph, ActivePeerSingle->GetName());
		}
		else
		{
			ActivePeriphSingle = FindFirstPeriph(ActivePeerSingle, SENS_TYPE_SENS);
		}
	}
	/*
	if (ActivePeriphSingle)
	{
		Serial.println("ActivePeriphSingle true");
		uint32_t user_data = 10;

		GenerateSingleScale();
		Serial.println("Scale Generated");
		
		
		//SingleTimer = lv_timer_create(SingleUpdateTimer, 500,  &user_data);
	}
	*/
}

void SingleUpdateTimer(lv_timer_t * timer)
{
		char buf[10];
		int nk = 0;
		float value;
	Serial.println("SinglUpdateTimer");
	if (ActivePeriphSingle){
		/*value = ActivePeriphSingle->Value;
		//if (DebugMode) Serial.printf("Sensor: %s: %f\n", ActiveSens->Name, value);

		if      (value<10)  nk = 2;
		else if (value<100) nk = 1;
		else                nk = 0;

		if (value == -99) strcpy(buf, "--"); 
		else dtostrf(value, 0, nk, buf);

		if (ActivePeriphSingle->Type == SENS_TYPE_AMP)  strcat(buf, " A");
		if (ActivePeriphSingle->Type == SENS_TYPE_VOLT) strcat(buf, " V");
		
		lv_meter_set_indicator_value(SingleMeter, SingleIndicNeedle, value*10);
		lv_label_set_text(ui_LblSingleValue, buf);
		*/
	}
}

void Ui_Single_Leave(lv_event_t * e)
{
	//lv_timer_del(SingleTimer);
}

static void SingleMeter_cb(lv_event_t * e) {

	lv_obj_draw_part_dsc_t	*dsc  = (lv_obj_draw_part_dsc_t *)lv_event_get_param(e);
	double					value;

	if( dsc->text != NULL ) {		// Filter major ticks...
		value = dsc->value / 10;
		snprintf(dsc->text, sizeof(dsc->text), "%5.1f", value);
	}

}
void GenerateSingleScale(void)
{
	if ((ActivePeriphSingle) and (ActivePeriphSingle->GetType() == SENS_TYPE_AMP))
	{
		lv_meter_set_scale_ticks(SingleMeter, scale, 41, 2, 10, lv_palette_main(LV_PALETTE_GREY));
    	lv_meter_set_scale_major_ticks(SingleMeter, scale, 5, 4, 15, lv_color_black(), 15);
    	lv_meter_set_scale_range(SingleMeter, scale, 0, 400, 240, 150);
	
		/*Add a green arc to the start*/
		SingleIndic = lv_meter_add_scale_lines(SingleMeter, scale, lv_palette_main(LV_PALETTE_GREEN), lv_palette_main(LV_PALETTE_GREEN), false, 0);
    	lv_meter_set_indicator_start_value(SingleMeter, SingleIndic, 0);
    	lv_meter_set_indicator_end_value(SingleMeter, SingleIndic, 250);

		SingleIndic = lv_meter_add_arc(SingleMeter, scale, 3, lv_palette_main(LV_PALETTE_RED), 0);
    	lv_meter_set_indicator_start_value(SingleMeter, SingleIndic, 300);
    	lv_meter_set_indicator_end_value(SingleMeter, SingleIndic, 400);

		/*Make the tick lines red at the end of the scale*/
		SingleIndic = lv_meter_add_scale_lines(SingleMeter, scale, lv_palette_main(LV_PALETTE_RED), lv_palette_main(LV_PALETTE_RED), false, 0);
		lv_meter_set_indicator_start_value(SingleMeter, SingleIndic, 300);
		lv_meter_set_indicator_end_value(SingleMeter, SingleIndic, 400);

		//SingleIndicNeedle = lv_meter_add_needle_line(SingleMeter, scale, 4, lv_palette_main(LV_PALETTE_GREY), -10);

		/*Add draw callback to override default values*/
		lv_obj_add_event_cb(SingleMeter, SingleMeter_cb, LV_EVENT_DRAW_PART_BEGIN, NULL);
	}

	if ((ActivePeriphSingle) and (ActivePeriphSingle->GetType() == SENS_TYPE_VOLT))
	{	
		//lv_meter_set_scale_ticks(SingleMeter, scale, 31, 2, 10, lv_palette_main(LV_PALETTE_GREY));
    	//lv_meter_set_scale_major_ticks(SingleMeter, scale, 5, 4, 15, lv_color_black(), 15);
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

		//SingleIndicNeedle = lv_meter_add_needle_line(SingleMeter, scale, 4, lv_palette_main(LV_PALETTE_GREY), -10);

		//Add draw callback to override default values
		lv_obj_add_event_cb(SingleMeter, SingleMeter_cb, LV_EVENT_DRAW_PART_BEGIN, NULL);
	}
}
#pragma endregion Screen_SingleMeter
#pragma region Screen_MultiMeter
void Ui_Multi_Next(lv_event_t * e)
{
	/*if (ActiveMultiScreen < MULTI_SCREENS-1)
		ActiveMultiScreen++;
	else ActiveMultiScreen = 0;
	_ui_screen_change(&ui_ScrMulti, LV_SCR_LOAD_ANIM_FADE_ON, 50, 0, &ui_ScrMulti_screen_init);
	*/
ReportAll();
}

void Ui_Multi_Last(lv_event_t * e)
{
	if (ActiveMultiScreen > 0)
		ActiveMultiScreen--;
	else ActiveMultiScreen = MULTI_SCREENS-1;

	_ui_screen_change(&ui_ScrMulti, LV_SCR_LOAD_ANIM_FADE_ON, 50, 0, &ui_ScrMulti_screen_init);
}
void Ui_Multi_SetPanel1(lv_event_t * e)
{
	MultiPosToChange = 0;
	_ui_screen_change(&ui_ScrPeriph, LV_SCR_LOAD_ANIM_FADE_ON, 50, 0, &ui_ScrPeriph_screen_init);
}

void Ui_Multi_SetPanel2(lv_event_t * e)
{
	MultiPosToChange = 1;
	_ui_screen_change(&ui_ScrPeriph, LV_SCR_LOAD_ANIM_FADE_ON, 50, 0, &ui_ScrPeriph_screen_init);
}

void Ui_Multi_SetPanel3(lv_event_t * e)
{
	MultiPosToChange = 2;
	_ui_screen_change(&ui_ScrPeriph, LV_SCR_LOAD_ANIM_FADE_ON, 50, 0, &ui_ScrPeriph_screen_init);
}

void Ui_Multi_SetPanel4(lv_event_t * e)
{
	MultiPosToChange = 3;
	_ui_screen_change(&ui_ScrPeriph, LV_SCR_LOAD_ANIM_FADE_ON, 50, 0, &ui_ScrPeriph_screen_init);
}

void Ui_Multi_Prepare(lv_event_t * e)
{
	Serial.println("Multi-Prepare");
	
	/*static uint32_t user_data = 10;
	
	lv_label_set_text(ui_LblMultiName, Screen[ActiveMultiScreen].Name);

	for (int i=0; i<4; i++) {
		Serial.println("for beginn");
		if (Screen[ActiveMultiScreen].Periph[i]->Type > 0) Ui_Multi_Set_Tile(i);
		Serial.println("for nach tile");
		
		Serial.println(Screen[ActiveMultiScreen].Periph[i]->Name);
	}
	*/
	//ReportScreen(0);

	//MultiTimer = lv_timer_create(MultiUpdateTimer, 500,  &user_data);
}
void Ui_Multi_Set_Tile(uint8_t Pos)
{
	PeriphClass *Periph = Screen[ActiveMultiScreen].GetPeriph(Pos);

	Serial.print("Multi-Set_tile : "); Serial.println(Pos);
	Serial.print("AktiveMultiScreen : "); Serial.println(ActiveMultiScreen);
	Serial.print("Screen[ActiveMultiScreen].Periph[Pos]->Type: "); Serial.println(Periph->GetType());
	
	lv_obj_t *TileActive; 
	lv_obj_t *TileInActive; 
	
	switch (Periph->GetType()) {
		case SENS_TYPE_AMP:
			Serial.println("AMP - Tile (in)active holen");
			TileActive   = lv_obj_get_child(ui_ScrMulti, Pos+4);
			TileInActive = lv_obj_get_child(ui_ScrMulti, Pos);
			break;
		case SENS_TYPE_VOLT:
			Serial.println("VOLT - Tile (in)active holen");
			TileActive   = lv_obj_get_child(ui_ScrMulti, Pos+4);
			TileInActive = lv_obj_get_child(ui_ScrMulti, Pos);
			break;
		case SENS_TYPE_SWITCH:
			Serial.println("SWITCH - Tile (in)active holen");
			TileActive   = lv_obj_get_child(ui_ScrMulti, Pos);
			TileInActive = lv_obj_get_child(ui_ScrMulti, Pos+4);
			break;
		default:
			Serial.println("kein Type");
			break;
	}
		
	lv_obj_clear_flag(TileActive, LV_OBJ_FLAG_HIDDEN);
	lv_obj_add_flag(TileInActive, LV_OBJ_FLAG_HIDDEN);

	lv_label_set_text(lv_obj_get_child(TileActive, 1), Periph->GetName());
	lv_label_set_text(lv_obj_get_child(TileActive, 0), Screen[ActiveMultiScreen].GetPeer(Pos)->GetName());
}
void MultiUpdateTimer(lv_timer_t * timer)
{
	char buf[10];
	int nk = 0;
	float value;
	lv_color_t bg;

	Serial.print("MultiTimer - ");
	Serial.println("ActiveMultiScreen = "); Serial.println(ActiveMultiScreen);
	//ReportScreen(ActiveMultiScreen);
	
	for (int i=0; i<4; i++) {
		lv_obj_t *TileActive; 
		
		value = Screen[ActiveMultiScreen].GetPeriph(i)->GetValue();
		
		if      (value<10)  nk = 2;
		else if (value<100) nk = 1;
		else                nk = 0;

		if (value == -99) strcpy(buf, "--"); 
		else dtostrf(value, 0, nk, buf);

		switch (Screen[ActiveMultiScreen].GetPeriph(i)->GetType()) 
		{
			case SENS_TYPE_AMP:
				TileActive = lv_obj_get_child(ui_ScrMulti, i+4);
				
				strcat(buf, " A");
				
				if 		(value < 20) bg = lv_color_hex(0x135A25);
				else if (value < 25) bg = lv_color_hex(0x7C7E26);
				else 				 bg = lv_color_hex(0x88182C);

				lv_obj_set_style_bg_color(TileActive, bg, LV_PART_MAIN | LV_STATE_DEFAULT);
				lv_label_set_text(lv_obj_get_child(TileActive, 2), buf);
				
				break;
			case SENS_TYPE_VOLT:
				TileActive = lv_obj_get_child(ui_ScrMulti, i+4);
				
				strcat(buf, " V");
				
				if 		(value < 13)   bg = lv_color_hex(0x135A25);
				else if (value < 14.4) bg = lv_color_hex(0x7C7E26);
				else 				   bg = lv_color_hex(0x88182C);

				lv_obj_set_style_bg_color(TileActive, bg, LV_PART_MAIN | LV_STATE_DEFAULT);
				lv_label_set_text(lv_obj_get_child(TileActive, 2), buf);

				break;
			case SENS_TYPE_SWITCH:
				TileActive = lv_obj_get_child(ui_ScrMulti, i);
			
				if (value == 1) 
				{
					lv_obj_add_state(TileActive, LV_STATE_CHECKED);
				}
				else
				{
					lv_obj_clear_state(TileActive, LV_STATE_CHECKED);
				}

				break;
		}

		Serial.print("MultiValueUpdate: setze Pos[");
		Serial.print(i); 
		Serial.print("]-");
		Serial.print(Screen[ActiveMultiScreen].GetPeriph(i)->GetName());
		Serial.print("auf: ");
		Serial.println(buf);
	}
}

void Ui_Multi_Leave(lv_event_t * e)
{
	lv_timer_del(MultiTimer);
}

#pragma endregion Screen_MultiMeter
#pragma region Screen_Switch
void SwitchUpdateTimer(lv_timer_t * timer)
{
	/*if (ActiveSwitch){
		if (ActiveSwitch->Value == 1) lv_imgbtn_set_state(ui_BtnImgSwitch, LV_IMGBTN_STATE_CHECKED_RELEASED);
		if (ActiveSwitch->Value == 0) lv_imgbtn_set_state(ui_BtnImgSwitch, LV_IMGBTN_STATE_RELEASED);
	}*/
}

void Ui_Switch_Next(lv_event_t * e)
{
	/*if (ActiveSwitch) {
		ActiveSwitch = FindNextPeriph(ActiveSwitch, SENS_TYPE_SWITCH, false);
	}
	else {
		ActiveSwitch = FindFirstPeriph(ActivePeer, SENS_TYPE_SWITCH, false);
	}
	
	if (ActivePeer) lv_label_set_text(ui_LblSwitchPeer, ActivePeer->Name);
	if (ActiveSwitch) lv_label_set_text(ui_LblSwitchPeriph, ActiveSwitch->Name);
	*/
}

void Ui_Switch_Last(lv_event_t * e)
{
		/*
	if (ActiveSwitch) {
		ActiveSwitch = FindPrevPeriph(ActiveSwitch, SENS_TYPE_SWITCH, false);
	}
	else {
		ActiveSwitch = FindFirstPeriph(ActivePeer, SENS_TYPE_SWITCH, false);
	}
	
	if (ActivePeer) lv_label_set_text(ui_LblSwitchPeer, ActivePeer->Name);
	if (ActiveSwitch) lv_label_set_text(ui_LblSwitchPeriph, ActiveSwitch->Name);
	*/
}

void Ui_Switch_Prepare(lv_event_t * e)
{
	/*
	if (DebugMode) Serial.println("Switch-Prepare");

	if (ActivePeer) lv_label_set_text(ui_LblSwitchPeer, ActivePeer->Name);
	if (ActiveSwitch) lv_label_set_text(ui_LblSwitchPeriph, ActiveSwitch->Name);
	
	static uint32_t user_data = 10;
	if (!SwitchTimer) { 
		SwitchTimer = lv_timer_create(SwitchUpdateTimer, 500,  &user_data);
	}
	else {
		lv_timer_resume(SwitchTimer);
	}
	*/
}

void Ui_Switch_Leave(lv_event_t * e)
{
	if (SwitchTimer) lv_timer_pause(SwitchTimer);
}

#pragma endregion Screen_Switch
#pragma region Screen_PeriphChoice
void Ui_PeriphChoice_Next(lv_event_t * e)
{
	if (ActivePeriph) {
		ActivePeriph = FindNextPeriph(ActivePeriph, SENS_TYPE_ALL, false);
		Ui_Periph_Choice_Prepare(e);
	}
}

void Ui_PeriphChoice_Last(lv_event_t * e)
{
	if (ActivePeriph) {
		ActivePeriph = FindPrevPeriph(ActivePeriph, SENS_TYPE_ALL, false);
		Ui_Periph_Choice_Prepare(e);
	}
}

void Ui_PeriphChoice_Click(lv_event_t * e)
{
	Screen[ActiveMultiScreen].AddPeriph(MultiPosToChange, ActivePeriph);
	_ui_screen_change(&ui_ScrMulti, LV_SCR_LOAD_ANIM_FADE_ON, 50, 0, &ui_ScrMulti_screen_init);
}

void Ui_Periph_Choice_Prepare(lv_event_t * e)
{
	if (!ActivePeer) ActivePeer = FindFirstPeer(MODULE_ALL);
	if (ActivePeer) {
		lv_label_set_text(ui_LblPeriphChoicePeer, ActivePeer->GetName());
	
		if (!ActivePeriph) {
			ActivePeriph = FindFirstPeriph(ActivePeer, SENS_TYPE_ALL);
		}
	}
	
	if (ActivePeriph) {
		lv_label_set_text(ui_LblPeriphChoicePeriph, ActivePeriph->GetName());
		
		if (millis()- ActivePeer->GetTSLastSeen() > OFFLINE_INTERVAL) 
			lv_label_set_text(ui_LblPeriphChoiceOnline, "Offline");
      	else 
			lv_label_set_text(ui_LblPeriphChoiceOnline, "Online");

		switch (ActivePeriph->GetType()) {
			case SENS_TYPE_SWITCH:	lv_label_set_text(ui_LblPeriphChoiceType, "Switch"); break;
			case SENS_TYPE_AMP:		lv_label_set_text(ui_LblPeriphChoiceType, "Amp-Sensor"); break;
			case SENS_TYPE_VOLT:	lv_label_set_text(ui_LblPeriphChoiceType, "Volt-Sensor"); break;
			default:				lv_label_set_text(ui_LblPeriphChoiceType, "unknown type"); break;
		}
	}
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

	//SingleMeter
	SingleMeter = lv_meter_create(ui_ScrSingle);
	lv_obj_center(SingleMeter);
	lv_obj_set_style_bg_color(SingleMeter, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(SingleMeter, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
	lv_obj_set_size(SingleMeter, 240,	240);
	scale = lv_meter_add_scale(SingleMeter);

	if (ActivePeer) lv_label_set_text(ui_LblSinglePeer, ActivePeer->GetName());
	if (ActiveSens) lv_label_set_text(ui_LblSinglePeriph, ActiveSens->GetName());
	 
	if (SingleTimer) lv_timer_pause(SingleTimer);
	
	if (Self.GetDebugMode()) { Serial.print("Type:"); if (ActiveSens) Serial.println(ActiveSens->GetType()); }
	
	lv_obj_move_background(SingleMeter);
	lv_obj_set_style_text_color(SingleMeter, lv_color_hex(0xdbdbdb), LV_PART_TICKS);
	SingleIndicNeedle = lv_meter_add_needle_line(SingleMeter, scale, 4, lv_palette_main(LV_PALETTE_GREY), -10);
	
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
	_ui_screen_change(&ui_ScrMenu, LV_SCR_LOAD_ANIM_FADE_ON, 50, 0, &ui_ScrMenu_screen_init);
}

void Ui_Volt_Prepare(lv_event_t * e)
{
	if (ActivePeer) lv_label_set_text(ui_LblVoltPeer, ActivePeer->GetName());
}
#pragma endregion System_Eichen

