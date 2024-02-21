// This file was generated by SquareLine Studio
// SquareLine Studio version: SquareLine Studio 1.3.4
// LVGL version: 8.3.6
// Project name: Jeepify

#include "../ui.h"

void ui_ScrSingle_screen_init(void)
{
    ui_ScrSingle = lv_obj_create(NULL);
    lv_obj_clear_flag(ui_ScrSingle, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_style_bg_color(ui_ScrSingle, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_ScrSingle, 255, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_LblSinglePeer = lv_label_create(ui_ScrSingle);
    lv_obj_set_width(ui_LblSinglePeer, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_LblSinglePeer, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_LblSinglePeer, 0);
    lv_obj_set_y(ui_LblSinglePeer, 80);
    lv_obj_set_align(ui_LblSinglePeer, LV_ALIGN_CENTER);
    lv_label_set_text(ui_LblSinglePeer, "Peer");
    lv_obj_set_style_text_color(ui_LblSinglePeer, lv_color_hex(0xAD0808), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_LblSinglePeer, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_LblSinglePeer, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_LblSingleValue = lv_label_create(ui_ScrSingle);
    lv_obj_set_width(ui_LblSingleValue, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_LblSingleValue, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_LblSingleValue, 0);
    lv_obj_set_y(ui_LblSingleValue, 46);
    lv_obj_set_align(ui_LblSingleValue, LV_ALIGN_CENTER);
    lv_label_set_text(ui_LblSingleValue, "Value");
    lv_obj_set_style_text_color(ui_LblSingleValue, lv_color_hex(0xDBDBDB), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_LblSingleValue, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_LblSingleValue, &lv_font_montserrat_24, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_LblSinglePeriph = lv_label_create(ui_ScrSingle);
    lv_obj_set_width(ui_LblSinglePeriph, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_LblSinglePeriph, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_LblSinglePeriph, 0);
    lv_obj_set_y(ui_LblSinglePeriph, -50);
    lv_obj_set_align(ui_LblSinglePeriph, LV_ALIGN_CENTER);
    lv_label_set_text(ui_LblSinglePeriph, "Periph");
    lv_obj_set_style_text_color(ui_LblSinglePeriph, lv_color_hex(0xDBDBDB), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_LblSinglePeriph, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_LblSinglePeriph, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_obj_add_event_cb(ui_ScrSingle, ui_event_ScrSingle, LV_EVENT_ALL, NULL);

}
