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
#include "ui_events.h"
// SCREEN: ui_ScrMenu
void ui_ScrMenu_screen_init(void);
extern lv_obj_t * ui_ScrMenu;
void ui_event_BtnMenu1(lv_event_t * e);
extern lv_obj_t * ui_BtnMenu1;
extern lv_obj_t * ui_BtnMenu2;
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
extern lv_obj_t * ui_Label5;
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
extern lv_obj_t * ui_LblSingle1;
// SCREEN: ui_ScrMulti
void ui_ScrMulti_screen_init(void);
void ui_event_ScrMulti(lv_event_t * e);
extern lv_obj_t * ui_ScrMulti;
extern lv_obj_t * ui_LblMulti1;
// SCREEN: ui_ScrPeer
void ui_ScrPeer_screen_init(void);
void ui_event_ScrPeer(lv_event_t * e);
extern lv_obj_t * ui_ScrPeer;
extern lv_obj_t * ui_LblPeer1;
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
// SCREEN: ui_ScreenEichen
void ui_ScreenEichen_screen_init(void);
extern lv_obj_t * ui_ScreenEichen;
// SCREEN: ui_ScreenVolt
void ui_ScreenVolt_screen_init(void);
extern lv_obj_t * ui_ScreenVolt;
extern lv_obj_t * ui_Keyboard2;
extern lv_obj_t * ui____initial_actions0;

LV_IMG_DECLARE(ui_img_jeepifybackground_png);    // assets\JeepifyBackground.png
LV_IMG_DECLARE(ui_img_menubtn1_png);    // assets\MenuBtn1.png
LV_IMG_DECLARE(ui_img_menubtn2_png);    // assets\MenuBtn2.png
LV_IMG_DECLARE(ui_img_menubtn3_png);    // assets\MenuBtn3.png
LV_IMG_DECLARE(ui_img_menubtn4_png);    // assets\MenuBtn4.png
LV_IMG_DECLARE(ui_img_rubicon_png);    // assets\Rubicon.png


void ui_init(void);

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif
