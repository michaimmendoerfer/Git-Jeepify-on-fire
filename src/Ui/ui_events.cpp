// This file was generated by SquareLine Studio
// SquareLine Studio version: SquareLine Studio 1.3.4
// LVGL version: 8.3.6
// Project name: Jeepify

#include <Arduino.h>
#include <String.h>
#include "ui.h"
#include "lv_meter.h"
#include "peers.h"
#include "Jeepify.h"
#include "ui_events.h"


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
extern void SendCommand(struct_Peer *Peer, String Cmd);

static lv_obj_t *SingleMeter;
lv_meter_indicator_t * SingleIndic;

lv_obj_t *Ui_LedSnd;
lv_obj_t *Ui_LedRcv;
lv_obj_t *Ui_LedPair;

void SingleUpdateTimer(lv_timer_t * timer);
void TopUpdateTimer(lv_timer_t * timer);

extern volatile uint32_t TSMsgRcv;
extern volatile uint32_t TSMsgSnd;
extern volatile uint32_t TSPair;

/* Screen: Peer*/
void Ui_Peer_Prepare(lv_event_t * e)
{
	lv_label_set_text_static(ui_LblPeer1, ActivePeer->Name);
	
	if (ActivePeer->SleepMode) {
		lv_obj_add_state(ui_BtnPeer3, LV_STATE_CHECKED);
	}
	else {
		lv_obj_clear_state(ui_BtnPeer3, LV_STATE_CHECKED);
	}
	if (ActivePeer->DemoMode) {
		lv_obj_add_state(ui_BtnPeer6, LV_STATE_CHECKED);
	}
	else {
		lv_obj_clear_state(ui_BtnPeer6, LV_STATE_CHECKED);
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
	ActivePeer = FindNextPeer(ActivePeer, MODULE_ALL); 
	_ui_screen_change(&ui_ScrPeer, LV_SCR_LOAD_ANIM_FADE_ON, 50, 0, &ui_ScrPeer_screen_init);
}
void Ui_Peer_Last(lv_event_t * e)
{
	ActivePeer = FindPrevPeer(ActivePeer, MODULE_ALL); 
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

/* ScreenÖ: Settings*/
void UI_Set_Prepare(lv_event_t * e)
{
	if (ReadyToPair) {
		lv_obj_add_state(ui_BtnSet2, LV_STATE_CHECKED);
	}
	else {
		lv_obj_clear_state(ui_BtnSet2, LV_STATE_CHECKED);
	}

	if (DebugMode) {
		lv_obj_add_state(ui_BtnSet7, LV_STATE_CHECKED);
	}
	else {
		lv_obj_clear_state(ui_BtnSet7, LV_STATE_CHECKED);
	}

	if (!ChangesSaved) {
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
	// Your code here
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

void Ui_SavePeers(lv_event_t * e)
{
    SavePeers();
	lv_obj_clear_state(ui_BtnSet8, LV_STATE_CHECKED);
}

/* Screen: Peers*/
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

	struct_Peer *TempPeer = FindPeerByName(SelectedName);

	if ((TempPeer) and (strcmp(SelectedName, "") != 0)) {
		ActivePeer = TempPeer;
		_ui_screen_change(&ui_ScrPeer, LV_SCR_LOAD_ANIM_FADE_ON, 50, 0, &ui_ScrPeer_screen_init);
	}
}

/*Screen: JSON*/

//kann weg...
void Ui_JSON_Prepare(lv_event_t * e)
{
	PrepareJSON();
}

/*Screen: SingleMeter*/
void Ui_Single_Next(lv_event_t * e)
{
	if (ActiveSens) ActiveSens = FindNextPeriph(ActiveSens, SENS_TYPE_SENS, false);
	_ui_screen_change(&ui_ScrSingle, LV_SCR_LOAD_ANIM_FADE_ON, 50, 0, &ui_ScrSingle_screen_init);

}

void Ui_Single_Last(lv_event_t * e)
{
	if (ActiveSens) ActiveSens = FindPrevPeriph(ActiveSens, SENS_TYPE_SENS, false);
	_ui_screen_change(&ui_ScrSingle, LV_SCR_LOAD_ANIM_FADE_ON, 50, 0, &ui_ScrSingle_screen_init);
}

void Ui_Single_Prepare(lv_event_t * e)
{		
	if (!ActivePeer) ActivePeer = FindFirstPeer(MODULE_ALL);
	if (ActivePeer) {
		if (!ActiveSens) ActiveSens = FindFirstPeriph(ActivePeer, SENS_TYPE_SENS, false);
		if (ActiveSens) {
			Serial.println("Single-Prepare");
			if (!SingleMeter) {
				SingleMeter = lv_meter_create(ui_ScrSingle);
				lv_obj_center(SingleMeter);
				lv_obj_set_size(SingleMeter, 240,	240);

				int Ticks, BigTicks, ScaleMax, ScaleMin, GreenEnd, YellowEnd;

				switch (ActiveSens->Type) {
					case SENS_TYPE_AMP:		ScaleMin  = 0;
											ScaleMax  = 35;
											Ticks     = 36;
											BigTicks  = 5;
											GreenEnd  = 20;
											YellowEnd = 25;
											break;
					case SENS_TYPE_VOLT:	ScaleMin  = 10;
											ScaleMax  = 15;
											Ticks     = 26;
											BigTicks  = 5;
											GreenEnd  = 14;
											YellowEnd = 25;
											break;
				}
				/*Add a scale first*/
				lv_meter_scale_t * scale = lv_meter_add_scale(SingleMeter);
				lv_meter_set_scale_range(SingleMeter, scale, ScaleMin, ScaleMax, 300, 120);
				lv_meter_set_scale_ticks(SingleMeter, scale, Ticks, 2, 10, lv_palette_main(LV_PALETTE_GREY));
				lv_meter_set_scale_major_ticks(SingleMeter, scale, BigTicks, 4, 15, lv_color_black(), 10);

				/*Add a green arc to the start*/
				SingleIndic = lv_meter_add_arc(SingleMeter, scale, 3, lv_palette_main(LV_PALETTE_GREEN), 0);
				lv_meter_set_indicator_start_value(SingleMeter, SingleIndic, 0);
				lv_meter_set_indicator_end_value(SingleMeter, SingleIndic, GreenEnd);

				/*Make the tick lines green at the start of the scale*/
				SingleIndic = lv_meter_add_scale_lines(SingleMeter, scale, lv_palette_main(LV_PALETTE_GREEN), lv_palette_main(LV_PALETTE_GREEN), false, 0);
				lv_meter_set_indicator_start_value(SingleMeter, SingleIndic, 0);
				lv_meter_set_indicator_end_value(SingleMeter, SingleIndic, GreenEnd);
				
				/*Add a yelow arc to the start*/
				SingleIndic = lv_meter_add_arc(SingleMeter, scale, 3, lv_palette_main(LV_PALETTE_YELLOW), 0);
				lv_meter_set_indicator_start_value(SingleMeter, SingleIndic, GreenEnd);
				lv_meter_set_indicator_end_value(SingleMeter, SingleIndic, YellowEnd);

				/*Make the tick lines yellow at the start of the scale*/
				SingleIndic = lv_meter_add_scale_lines(SingleMeter, scale, lv_palette_main(LV_PALETTE_YELLOW), lv_palette_main(LV_PALETTE_YELLOW), false, 0);
				lv_meter_set_indicator_start_value(SingleMeter, SingleIndic, GreenEnd);
				lv_meter_set_indicator_end_value(SingleMeter, SingleIndic, YellowEnd);

				/*Add a red arc to the end*/
				SingleIndic = lv_meter_add_arc(SingleMeter, scale, 3, lv_palette_main(LV_PALETTE_RED), 0);
				lv_meter_set_indicator_start_value(SingleMeter, SingleIndic, YellowEnd);
				lv_meter_set_indicator_end_value(SingleMeter, SingleIndic, ScaleMax);

				/*Make the tick lines red at the end of the scale*/
				SingleIndic = lv_meter_add_scale_lines(SingleMeter, scale, lv_palette_main(LV_PALETTE_RED), lv_palette_main(LV_PALETTE_RED), false, 0);
				lv_meter_set_indicator_start_value(SingleMeter, SingleIndic, YellowEnd);
				lv_meter_set_indicator_end_value(SingleMeter, SingleIndic, ScaleMax);

				/*Add a needle line indicator*/
				SingleIndic = lv_meter_add_needle_line(SingleMeter, scale, 4, lv_palette_main(LV_PALETTE_GREY), -10);
				
				static uint32_t user_data = 10;
				lv_timer_t * timer = lv_timer_create(SingleUpdateTimer, 500,  &user_data);

				lv_obj_move_background(SingleMeter);
			}
		}
	}
}

void SingleUpdateTimer(lv_timer_t * timer)
{
	if (ActiveSens){
		
		char buf[10];
		int nk = 0;
		float value = ActiveSens->Value;

		if      (value<10)  nk = 2;
    	else if (value<100) nk = 1;
    	else                nk = 0;

    	if (value == -99) strcpy(buf, "--"); 
    	else dtostrf(value, 0, nk, buf);

		lv_meter_set_indicator_value(SingleMeter, SingleIndic, ActiveSens->Value);
		lv_label_set_text(ui_LblSingleValue, buf);
		Serial.print("Single Needle updated with:"); 
		Serial.println(buf);
	}
	else {
		float RandomValue = random(35);
		lv_meter_set_indicator_value(SingleMeter, SingleIndic, RandomValue);
		Serial.print("Single Needle updated with:"); 
		Serial.println(RandomValue);
	}
}
void Ui_Single_Unload(lv_event_t * e)
{
	// Your code here
}

/* Screen: MultiMeter*/
void Ui_Multi_Next(lv_event_t * e)
{
	// Your code here
}

void Ui_Multi_Last(lv_event_t * e)
{
	// Your code here
}

void Ui_Init_Custom(lv_event_t * e)
{
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
    //lv_led_set_brightness(LedPair, 150);
    lv_led_set_color(Ui_LedPair, lv_palette_main(LV_PALETTE_RED));
	
}

/* Timer*/
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