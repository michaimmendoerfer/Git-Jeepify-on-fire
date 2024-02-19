// This file was generated by SquareLine Studio
// SquareLine Studio version: SquareLine Studio 1.3.4
// LVGL version: 8.3.6
// Project name: Jeepify

#include "../ui.h"

void ui_ScrMenu_screen_init(void)
{
    ui_ScrMenu = lv_obj_create(NULL);
    lv_obj_clear_flag(ui_ScrMenu, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_style_bg_img_src(ui_ScrMenu, &ui_img_jeepifybackground_png, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_BtnMenu1 = lv_btn_create(ui_ScrMenu);
    lv_obj_set_width(ui_BtnMenu1, 60);
    lv_obj_set_height(ui_BtnMenu1, 60);
    lv_obj_set_x(ui_BtnMenu1, -45);
    lv_obj_set_y(ui_BtnMenu1, -55);
    lv_obj_set_align(ui_BtnMenu1, LV_ALIGN_CENTER);
    lv_obj_add_flag(ui_BtnMenu1, LV_OBJ_FLAG_SCROLL_ON_FOCUS);     /// Flags
    lv_obj_clear_flag(ui_BtnMenu1, LV_OBJ_FLAG_PRESS_LOCK | LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_style_bg_color(ui_BtnMenu1, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_BtnMenu1, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_img_src(ui_BtnMenu1, &ui_img_menubtn1_png, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui_BtnMenu1, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui_BtnMenu1, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui_BtnMenu1, LV_BORDER_SIDE_FULL, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui_BtnMenu1, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_spread(ui_BtnMenu1, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui_BtnMenu1, lv_color_hex(0xAD0808), LV_PART_MAIN | LV_STATE_PRESSED);
    lv_obj_set_style_bg_opa(ui_BtnMenu1, 255, LV_PART_MAIN | LV_STATE_PRESSED);
    lv_obj_set_style_border_color(ui_BtnMenu1, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_PRESSED);
    lv_obj_set_style_border_opa(ui_BtnMenu1, 0, LV_PART_MAIN | LV_STATE_PRESSED);

    ui_BtnMenu2 = lv_btn_create(ui_ScrMenu);
    lv_obj_set_width(ui_BtnMenu2, 60);
    lv_obj_set_height(ui_BtnMenu2, 60);
    lv_obj_set_x(ui_BtnMenu2, 45);
    lv_obj_set_y(ui_BtnMenu2, -55);
    lv_obj_set_align(ui_BtnMenu2, LV_ALIGN_CENTER);
    lv_obj_add_flag(ui_BtnMenu2, LV_OBJ_FLAG_SCROLL_ON_FOCUS);     /// Flags
    lv_obj_clear_flag(ui_BtnMenu2, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_style_bg_color(ui_BtnMenu2, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_BtnMenu2, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_img_src(ui_BtnMenu2, &ui_img_menubtn2_png, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_color(ui_BtnMenu2, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_opa(ui_BtnMenu2, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui_BtnMenu2, lv_color_hex(0xAD0808), LV_PART_MAIN | LV_STATE_PRESSED);
    lv_obj_set_style_bg_opa(ui_BtnMenu2, 255, LV_PART_MAIN | LV_STATE_PRESSED);
    lv_obj_set_style_border_color(ui_BtnMenu2, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_PRESSED);
    lv_obj_set_style_border_opa(ui_BtnMenu2, 0, LV_PART_MAIN | LV_STATE_PRESSED);

    ui_BtnMenu3 = lv_btn_create(ui_ScrMenu);
    lv_obj_set_width(ui_BtnMenu3, 60);
    lv_obj_set_height(ui_BtnMenu3, 60);
    lv_obj_set_x(ui_BtnMenu3, -45);
    lv_obj_set_y(ui_BtnMenu3, 55);
    lv_obj_set_align(ui_BtnMenu3, LV_ALIGN_CENTER);
    lv_obj_add_flag(ui_BtnMenu3, LV_OBJ_FLAG_SCROLL_ON_FOCUS);     /// Flags
    lv_obj_clear_flag(ui_BtnMenu3, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_style_bg_color(ui_BtnMenu3, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_BtnMenu3, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_img_src(ui_BtnMenu3, &ui_img_menubtn3_png, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_color(ui_BtnMenu3, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_opa(ui_BtnMenu3, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui_BtnMenu3, lv_color_hex(0xAD0808), LV_PART_MAIN | LV_STATE_PRESSED);
    lv_obj_set_style_bg_opa(ui_BtnMenu3, 255, LV_PART_MAIN | LV_STATE_PRESSED);
    lv_obj_set_style_border_color(ui_BtnMenu3, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_PRESSED);
    lv_obj_set_style_border_opa(ui_BtnMenu3, 0, LV_PART_MAIN | LV_STATE_PRESSED);

    ui_BtnMenu4 = lv_btn_create(ui_ScrMenu);
    lv_obj_set_width(ui_BtnMenu4, 60);
    lv_obj_set_height(ui_BtnMenu4, 60);
    lv_obj_set_x(ui_BtnMenu4, 45);
    lv_obj_set_y(ui_BtnMenu4, 55);
    lv_obj_set_align(ui_BtnMenu4, LV_ALIGN_CENTER);
    lv_obj_add_flag(ui_BtnMenu4, LV_OBJ_FLAG_SCROLL_ON_FOCUS);     /// Flags
    lv_obj_clear_flag(ui_BtnMenu4, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_style_bg_color(ui_BtnMenu4, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_BtnMenu4, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_img_src(ui_BtnMenu4, &ui_img_menubtn4_png, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_color(ui_BtnMenu4, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_opa(ui_BtnMenu4, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui_BtnMenu4, lv_color_hex(0xAD0808), LV_PART_MAIN | LV_STATE_PRESSED);
    lv_obj_set_style_bg_opa(ui_BtnMenu4, 255, LV_PART_MAIN | LV_STATE_PRESSED);
    lv_obj_set_style_border_color(ui_BtnMenu4, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_PRESSED);
    lv_obj_set_style_border_opa(ui_BtnMenu4, 0, LV_PART_MAIN | LV_STATE_PRESSED);

    ui_ImgRubicon = lv_img_create(ui_ScrMenu);
    lv_img_set_src(ui_ImgRubicon, &ui_img_rubicon_png);
    lv_obj_set_width(ui_ImgRubicon, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_ImgRubicon, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_align(ui_ImgRubicon, LV_ALIGN_CENTER);
    lv_obj_add_flag(ui_ImgRubicon, LV_OBJ_FLAG_ADV_HITTEST);     /// Flags
    lv_obj_clear_flag(ui_ImgRubicon, LV_OBJ_FLAG_SCROLLABLE);      /// Flags

    lv_obj_add_event_cb(ui_BtnMenu1, ui_event_BtnMenu1, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(ui_BtnMenu4, ui_event_BtnMenu4, LV_EVENT_ALL, NULL);

}
