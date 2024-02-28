// This file was generated by SquareLine Studio
// SquareLine Studio version: SquareLine Studio 1.3.4
// LVGL version: 8.3.6
// Project name: Jeepify

#include "../ui.h"

void ui_ScrMulti_screen_init(void)
{
    ui_ScrMulti = lv_obj_create(NULL);
    lv_obj_clear_flag(ui_ScrMulti, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_style_bg_img_src(ui_ScrMulti, &ui_img_jeepifybackground_png, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_ButtonSwitchSmall1 = ui_ButtonSwitchSmall_create(ui_ScrMulti);
    lv_obj_set_x(ui_ButtonSwitchSmall1, -45);
    lv_obj_set_y(ui_ButtonSwitchSmall1, -45);
    lv_obj_clear_flag(ui_ButtonSwitchSmall1, LV_OBJ_FLAG_HIDDEN);      /// Flags


    ui_ButtonSwitchSmall2 = ui_ButtonSwitchSmall_create(ui_ScrMulti);
    lv_obj_set_x(ui_ButtonSwitchSmall2, 45);
    lv_obj_set_y(ui_ButtonSwitchSmall2, -45);
    lv_obj_clear_flag(ui_ButtonSwitchSmall2, LV_OBJ_FLAG_HIDDEN);      /// Flags


    ui_ButtonSwitchSmall3 = ui_ButtonSwitchSmall_create(ui_ScrMulti);
    lv_obj_set_x(ui_ButtonSwitchSmall3, -45);
    lv_obj_set_y(ui_ButtonSwitchSmall3, 45);
    lv_obj_clear_flag(ui_ButtonSwitchSmall3, LV_OBJ_FLAG_HIDDEN);      /// Flags


    ui_ButtonSwitchSmall4 = ui_ButtonSwitchSmall_create(ui_ScrMulti);
    lv_obj_set_x(ui_ButtonSwitchSmall4, 45);
    lv_obj_set_y(ui_ButtonSwitchSmall4, 45);
    lv_obj_clear_flag(ui_ButtonSwitchSmall4, LV_OBJ_FLAG_HIDDEN);      /// Flags


    ui_BtnSensSmall1 = ui_BtnSensSmall_create(ui_ScrMulti);
    lv_obj_set_x(ui_BtnSensSmall1, -45);
    lv_obj_set_y(ui_BtnSensSmall1, -45);
    lv_obj_clear_flag(ui_BtnSensSmall1, LV_OBJ_FLAG_HIDDEN);      /// Flags





    ui_BtnSensSmall2 = ui_BtnSensSmall_create(ui_ScrMulti);
    lv_obj_set_x(ui_BtnSensSmall2, 45);
    lv_obj_set_y(ui_BtnSensSmall2, -45);
    lv_obj_clear_flag(ui_BtnSensSmall2, LV_OBJ_FLAG_HIDDEN);      /// Flags





    ui_BtnSensSmall3 = ui_BtnSensSmall_create(ui_ScrMulti);
    lv_obj_set_x(ui_BtnSensSmall3, -45);
    lv_obj_set_y(ui_BtnSensSmall3, 45);
    lv_obj_clear_flag(ui_BtnSensSmall3, LV_OBJ_FLAG_HIDDEN);      /// Flags





    ui_BtnSensSmall4 = ui_BtnSensSmall_create(ui_ScrMulti);
    lv_obj_set_x(ui_BtnSensSmall4, 45);
    lv_obj_set_y(ui_BtnSensSmall4, 45);
    lv_obj_clear_flag(ui_BtnSensSmall4, LV_OBJ_FLAG_HIDDEN);      /// Flags





    ui_LblMultiName = lv_label_create(ui_BtnSensSmall4);
    lv_obj_set_width(ui_LblMultiName, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_LblMultiName, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_LblMultiName, -52);
    lv_obj_set_y(ui_LblMultiName, 75);
    lv_obj_set_align(ui_LblMultiName, LV_ALIGN_CENTER);
    lv_label_set_text(ui_LblMultiName, "Peer");
    lv_obj_set_style_text_color(ui_LblMultiName, lv_color_hex(0xAD0808), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_LblMultiName, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_LblMultiName, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_LblMultiName1 = lv_label_create(ui_ScrMulti);
    lv_obj_set_width(ui_LblMultiName1, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_LblMultiName1, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_LblMultiName1, 4);
    lv_obj_set_y(ui_LblMultiName1, 99);
    lv_obj_set_align(ui_LblMultiName1, LV_ALIGN_CENTER);
    lv_label_set_text(ui_LblMultiName1, "Screen");
    lv_obj_set_style_text_color(ui_LblMultiName1, lv_color_hex(0xDBDBDB), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_LblMultiName1, 255, LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_obj_add_event_cb(ui_ScrMulti, ui_event_ScrMulti, LV_EVENT_ALL, NULL);

}
