// This file was generated by SquareLine Studio
// SquareLine Studio version: SquareLine Studio 1.3.4
// LVGL version: 8.3.6
// Project name: Jeepify

#include "../ui.h"

void ui_ScreenVolt_screen_init(void)
{
    ui_ScreenVolt = lv_obj_create(NULL);
    lv_obj_clear_flag(ui_ScreenVolt, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_style_bg_img_src(ui_ScreenVolt, &ui_img_jeepifybackground_png, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_Keyboard = lv_keyboard_create(ui_ScreenVolt);
    lv_keyboard_set_mode(ui_Keyboard, LV_KEYBOARD_MODE_USER_1);
    lv_obj_set_width(ui_Keyboard, 180);
    lv_obj_set_height(ui_Keyboard, 127);
    lv_obj_set_x(ui_Keyboard, 0);
    lv_obj_set_y(ui_Keyboard, -10);
    lv_obj_set_align(ui_Keyboard, LV_ALIGN_CENTER);
    lv_obj_set_style_bg_color(ui_Keyboard, lv_color_hex(0xABABAB), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_Keyboard, 255, LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_obj_set_style_bg_color(ui_Keyboard, lv_color_hex(0xDBDBDB), LV_PART_ITEMS | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_Keyboard, 255, LV_PART_ITEMS | LV_STATE_DEFAULT);

    ui_LblEichenPeer2 = lv_label_create(ui_ScreenVolt);
    lv_obj_set_width(ui_LblEichenPeer2, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_LblEichenPeer2, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_LblEichenPeer2, 0);
    lv_obj_set_y(ui_LblEichenPeer2, 80);
    lv_obj_set_align(ui_LblEichenPeer2, LV_ALIGN_CENTER);
    lv_label_set_text(ui_LblEichenPeer2, "Peer");
    lv_obj_set_style_text_color(ui_LblEichenPeer2, lv_color_hex(0xAD0808), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_LblEichenPeer2, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_LblEichenPeer2, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_TxtVolt = lv_textarea_create(ui_ScreenVolt);
    lv_obj_set_width(ui_TxtVolt, 74);
    lv_obj_set_height(ui_TxtVolt, LV_SIZE_CONTENT);    /// 10
    lv_obj_set_x(ui_TxtVolt, 0);
    lv_obj_set_y(ui_TxtVolt, -95);
    lv_obj_set_align(ui_TxtVolt, LV_ALIGN_CENTER);
    lv_textarea_set_max_length(ui_TxtVolt, 5);
    lv_textarea_set_placeholder_text(ui_TxtVolt, "Volt");
    lv_textarea_set_one_line(ui_TxtVolt, true);
    lv_obj_set_style_pad_left(ui_TxtVolt, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui_TxtVolt, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui_TxtVolt, 3, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui_TxtVolt, 3, LV_PART_MAIN | LV_STATE_DEFAULT);


    lv_obj_set_style_text_align(ui_TxtVolt, LV_TEXT_ALIGN_CENTER, LV_PART_TEXTAREA_PLACEHOLDER | LV_STATE_DEFAULT);

    lv_keyboard_set_textarea(ui_Keyboard, ui_TxtVolt);
    lv_obj_add_event_cb(ui_Keyboard, ui_event_Keyboard, LV_EVENT_ALL, NULL);

}
