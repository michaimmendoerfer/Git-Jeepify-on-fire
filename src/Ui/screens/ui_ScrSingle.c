// This file was generated by SquareLine Studio
// SquareLine Studio version: SquareLine Studio 1.5.0
// LVGL version: 8.3.6
// Project name: SQ-Jeepify-Monitor240

#include "../ui.h"

void ui_ScrSingle_screen_init(void)
{
    ui_ScrSingle = lv_obj_create(NULL);
    lv_obj_clear_flag(ui_ScrSingle, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_style_bg_color(ui_ScrSingle, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_ScrSingle, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_img_src(ui_ScrSingle, &ui_img_jeepifybackground_png, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_img_opa(ui_ScrSingle, 255, LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_obj_add_event_cb(ui_ScrSingle, ui_event_ScrSingle, LV_EVENT_ALL, NULL);

}
