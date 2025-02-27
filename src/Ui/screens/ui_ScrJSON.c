// This file was generated by SquareLine Studio
// SquareLine Studio version: SquareLine Studio 1.5.0
// LVGL version: 8.3.6
// Project name: SQ-Jeepify-Monitor240

#include "../ui.h"

void ui_ScrJSON_screen_init(void)
{
    ui_ScrJSON = lv_obj_create(NULL);
    lv_obj_clear_flag(ui_ScrJSON, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_style_bg_color(ui_ScrJSON, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_ScrJSON, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_img_src(ui_ScrJSON, &ui_img_jeepifybackground_png, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_ScrJSON, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_TxtJSON1 = lv_textarea_create(ui_ScrJSON);
    lv_obj_set_width(ui_TxtJSON1, 193);
    lv_obj_set_height(ui_TxtJSON1, 142);
    lv_obj_set_x(ui_TxtJSON1, 5);
    lv_obj_set_y(ui_TxtJSON1, -6);
    lv_obj_set_align(ui_TxtJSON1, LV_ALIGN_CENTER);
    lv_textarea_set_placeholder_text(ui_TxtJSON1, "JSON-Message");
    lv_obj_set_style_text_color(ui_TxtJSON1, lv_color_hex(0xDBDBDB), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_TxtJSON1, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_TxtJSON1, &lv_font_montserrat_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui_TxtJSON1, lv_color_hex(0xAD0808), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_TxtJSON1, 180, LV_PART_MAIN | LV_STATE_DEFAULT);



    ui_LblJSON2 = lv_label_create(ui_ScrJSON);
    lv_obj_set_width(ui_LblJSON2, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_LblJSON2, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_LblJSON2, 0);
    lv_obj_set_y(ui_LblJSON2, 85);
    lv_obj_set_align(ui_LblJSON2, LV_ALIGN_CENTER);
    lv_label_set_text(ui_LblJSON2, "JSON");
    lv_obj_set_style_text_color(ui_LblJSON2, lv_color_hex(0xAD0808), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_LblJSON2, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_LblJSON2, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_obj_add_event_cb(ui_ScrJSON, ui_event_ScrJSON, LV_EVENT_ALL, NULL);

}
