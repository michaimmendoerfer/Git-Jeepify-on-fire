// This file was generated by SquareLine Studio
// SquareLine Studio version: SquareLine Studio 1.3.4
// LVGL version: 8.3.6
// Project name: Jeepify

#include "../ui.h"

void ui_ScrPeers_screen_init(void)
{
    ui_ScrPeers = lv_obj_create(NULL);
    lv_obj_clear_flag(ui_ScrPeers, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_style_bg_img_src(ui_ScrPeers, &ui_img_jeepifybackground_png, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_RollerPeers1 = lv_roller_create(ui_ScrPeers);
    lv_roller_set_options(ui_RollerPeers1, "Option 1\nOption 2\nOption 3", LV_ROLLER_MODE_NORMAL);
    lv_obj_set_height(ui_RollerPeers1, 165);
    lv_obj_set_width(ui_RollerPeers1, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_x(ui_RollerPeers1, 0);
    lv_obj_set_y(ui_RollerPeers1, -15);
    lv_obj_set_align(ui_RollerPeers1, LV_ALIGN_CENTER);
    lv_obj_set_style_bg_color(ui_RollerPeers1, lv_color_hex(0xAD0808), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_RollerPeers1, 180, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_Label5 = lv_label_create(ui_ScrPeers);
    lv_obj_set_width(ui_Label5, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_Label5, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_Label5, 0);
    lv_obj_set_y(ui_Label5, 90);
    lv_obj_set_align(ui_Label5, LV_ALIGN_CENTER);
    lv_label_set_text(ui_Label5, "Peers");
    lv_obj_set_style_text_color(ui_Label5, lv_color_hex(0xAD0808), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_Label5, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_Label5, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_obj_add_event_cb(ui_RollerPeers1, ui_event_RollerPeers1, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(ui_ScrPeers, ui_event_ScrPeers, LV_EVENT_ALL, NULL);

}
