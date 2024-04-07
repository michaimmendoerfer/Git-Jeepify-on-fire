// This file was generated by SquareLine Studio
// SquareLine Studio version: SquareLine Studio 1.3.4
// LVGL version: 8.3.6
// Project name: Jeepify

#ifndef _JEEPIFY_UI_H
#define _JEEPIFY_UI_H

#ifdef __cplusplus
extern "C" {
#endif

#if defined __has_include
#if __has_include("lvgl.h")
#include "lvgl.h"
#elif __has_include("lvgl/lvgl.h")
#include "lvgl/lvgl.h"
#else
#include "lvgl.h"
#endif
#else
#include "lvgl.h"
#endif

#include "ui_helpers.h"
#include "components/ui_comp.h"
#include "components/ui_comp_hook.h"
#include "ui_events.h"
// SCREEN: ui_ScrMenu
void ui_ScrMenu_screen_init(void);
extern lv_obj_t * ui_ScrMenu;
void ui_event_BtnMenu1(lv_event_t * e);
extern lv_obj_t * ui_BtnMenu1;
extern lv_obj_t * ui_BtnMenu2;
void ui_event_BtnMenu3(lv_event_t * e);
extern lv_obj_t * ui_BtnMenu3;
void ui_event_BtnMenu4(lv_event_t * e);
extern lv_obj_t * ui_BtnMenu4;
extern lv_obj_t * ui_ImgRubicon;
// SCREEN: ui_ScrSettings
void ui_ScrSettings_screen_init(void);
void ui_event_ScrSettings(lv_event_t * e);
extern lv_obj_t * ui_ScrSettings;
void ui_event_BtnSet1(lv_event_t * e);
extern lv_obj_t * ui_BtnSet1;
extern lv_obj_t * ui_BtnSet1Lbl1;
void ui_event_BtnSet2(lv_event_t * e);
extern lv_obj_t * ui_BtnSet2;
extern lv_obj_t * ui_BtnSet2Lbl;
void ui_event_BtnSet3(lv_event_t * e);
extern lv_obj_t * ui_BtnSet3;
extern lv_obj_t * ui_BtnSet3Lbl3;
void ui_event_BtnSet4(lv_event_t * e);
extern lv_obj_t * ui_BtnSet4;
extern lv_obj_t * ui_BtnSet3Lbl1;
void ui_event_BtnSet5(lv_event_t * e);
extern lv_obj_t * ui_BtnSet5;
extern lv_obj_t * ui_BtnSet5Lbl;
void ui_event_BtnSet6(lv_event_t * e);
extern lv_obj_t * ui_BtnSet6;
extern lv_obj_t * ui_BtnSet3Lbl2;
void ui_event_BtnSet7(lv_event_t * e);
extern lv_obj_t * ui_BtnSet7;
extern lv_obj_t * ui_BtnSet7Lbl;
void ui_event_BtnSet8(lv_event_t * e);
extern lv_obj_t * ui_BtnSet8;
extern lv_obj_t * ui_BtnSet7Lbl1;
// SCREEN: ui_ScrPeers
void ui_ScrPeers_screen_init(void);
void ui_event_ScrPeers(lv_event_t * e);
extern lv_obj_t * ui_ScrPeers;
void ui_event_RollerPeers1(lv_event_t * e);
extern lv_obj_t * ui_RollerPeers1;
extern lv_obj_t * ui_LabelPeersName;
// SCREEN: ui_ScrJSON
void ui_ScrJSON_screen_init(void);
void ui_event_ScrJSON(lv_event_t * e);
extern lv_obj_t * ui_ScrJSON;
extern lv_obj_t * ui_TxtJSON1;
extern lv_obj_t * ui_LblJSON2;
// SCREEN: ui_ScrSingle
void ui_ScrSingle_screen_init(void);
void ui_event_ScrSingle(lv_event_t * e);
extern lv_obj_t * ui_ScrSingle;
extern lv_obj_t * ui_LblSinglePeer;
extern lv_obj_t * ui_LblSingleValue;
extern lv_obj_t * ui_LblSinglePeriph;
// SCREEN: ui_ScrMulti
void ui_ScrMulti_screen_init(void);
void ui_event_ScrMulti(lv_event_t * e);
extern lv_obj_t * ui_ScrMulti;
extern lv_obj_t * ui_LblMultiName;
void ui_event_ ? (lv_event_t * e);
extern lv_obj_t * ui_PnlMulti0;
extern lv_obj_t * lv.obj();
extern lv_obj_t * lv.obj();
extern lv_obj_t * lv.obj();
void ui_event_ ? (lv_event_t * e);
extern lv_obj_t * ui_PnlMulti1;
extern lv_obj_t * lv.obj();
extern lv_obj_t * lv.obj();
extern lv_obj_t * lv.obj();
void ui_event_ ? (lv_event_t * e);
extern lv_obj_t * ui_PnlMulti2;
extern lv_obj_t * lv.obj();
extern lv_obj_t * lv.obj();
extern lv_obj_t * lv.obj();
void ui_event_ ? (lv_event_t * e);
extern lv_obj_t * ui_PnlMulti3;
extern lv_obj_t * lv.obj();
extern lv_obj_t * lv.obj();
extern lv_obj_t * lv.obj();
extern lv_obj_t * ui_ImgButtonSwitchSmall1;
extern lv_obj_t * ui_ImgButtonSwitchSmall3;
extern lv_obj_t * ui_ImgButtonSwitchSmall2;
extern lv_obj_t * ui_ImgButtonSwitchSmall4;
// SCREEN: ui_ScrPeer
void ui_ScrPeer_screen_init(void);
void ui_event_ScrPeer(lv_event_t * e);
extern lv_obj_t * ui_ScrPeer;
extern lv_obj_t * ui_LblPeerName;
extern lv_obj_t * ui_LblPeerTypeLbl;
void ui_event_BtnPeer1(lv_event_t * e);
extern lv_obj_t * ui_BtnPeer1;
void ui_event_BtnPeer1Lbl(lv_event_t * e);
extern lv_obj_t * ui_BtnPeer1Lbl;
void ui_event_BtnPeer2(lv_event_t * e);
extern lv_obj_t * ui_BtnPeer2;
extern lv_obj_t * ui_BtnPeer2Lbl1;
void ui_event_BtnPeer3(lv_event_t * e);
extern lv_obj_t * ui_BtnPeer3;
extern lv_obj_t * ui_BtnPeer3Lbl1;
void ui_event_BtnPeer4(lv_event_t * e);
extern lv_obj_t * ui_BtnPeer4;
extern lv_obj_t * ui_BtnPeer4Lbl1;
void ui_event_BtnPeer5(lv_event_t * e);
extern lv_obj_t * ui_BtnPeer5;
extern lv_obj_t * ui_BtnPeer5Lbl1;
void ui_event_BtnPeer6(lv_event_t * e);
extern lv_obj_t * ui_BtnPeer6;
extern lv_obj_t * ui_BtnPeer6Lbl1;
// SCREEN: ui_ScrEichen
void ui_ScrEichen_screen_init(void);
extern lv_obj_t * ui_ScrEichen;
extern lv_obj_t * ui_Label1;
void ui_event_BtnEichenStart(lv_event_t * e);
extern lv_obj_t * ui_BtnEichenStart;
extern lv_obj_t * ui_BtnPeer4Lbl3;
extern lv_obj_t * ui_LblEichenPeer;
// SCREEN: ui_ScrVolt
void ui_ScrVolt_screen_init(void);
void ui_event_ScrVolt(lv_event_t * e);
extern lv_obj_t * ui_ScrVolt;
extern lv_obj_t * ui_Keyboard;
extern lv_obj_t * ui_TxtVolt;
void ui_event_LblVoltPeer(lv_event_t * e);
extern lv_obj_t * ui_LblVoltPeer;
// SCREEN: ui_ScrPeriph
void ui_ScrPeriph_screen_init(void);
void ui_event_ScrPeriph(lv_event_t * e);
extern lv_obj_t * ui_ScrPeriph;
extern lv_obj_t * ui_ImgPeerChoice;
extern lv_obj_t * ui_LblPeriphChoicePeer;
extern lv_obj_t * ui_LblPeriphChoiceType;
extern lv_obj_t * ui_LblPeriphChoiceOnline;
extern lv_obj_t * ui_LblPeriphChoicePeriph;
void ui_event____initial_actions0(lv_event_t * e);
extern lv_obj_t * ui____initial_actions0;

LV_IMG_DECLARE(ui_img_jeepifybackground_png);    // assets\JeepifyBackground.png
LV_IMG_DECLARE(ui_img_menubtn1_png);    // assets\MenuBtn1.png
LV_IMG_DECLARE(ui_img_menubtn2_png);    // assets\MenuBtn2.png
LV_IMG_DECLARE(ui_img_menubtn3_png);    // assets\MenuBtn3.png
LV_IMG_DECLARE(ui_img_menubtn4_png);    // assets\MenuBtn4.png
LV_IMG_DECLARE(ui_img_rubicon_png);    // assets\Rubicon.png
LV_IMG_DECLARE(ui_img_1527887470);    // assets\Btn-small-off-100.png
LV_IMG_DECLARE(ui_img_551600910);    // assets\Btn-small-on-100.png
LV_IMG_DECLARE(ui_img_pdc_png);    // assets\pdc.png



void ui_init(void);

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif