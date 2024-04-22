// This file was generated by SquareLine Studio
// SquareLine Studio version: SquareLine Studio 1.3.4
// LVGL version: 8.3.6
// Project name: Jeepify

#include "../ui.h"

void ui_ScrSettings_screen_init(void)
{
    ui_ScrSettings = lv_obj_create(NULL);
    lv_obj_clear_flag(ui_ScrSettings, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_style_bg_img_src(ui_ScrSettings, &ui_img_jeepifybackground_png, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_BtnSet1 = lv_btn_create(ui_ScrSettings);
    lv_obj_set_width(ui_BtnSet1, 70);
    lv_obj_set_height(ui_BtnSet1, 30);
    lv_obj_set_x(ui_BtnSet1, -40);
    lv_obj_set_y(ui_BtnSet1, 60);
    lv_obj_set_align(ui_BtnSet1, LV_ALIGN_CENTER);
    lv_obj_add_flag(ui_BtnSet1, LV_OBJ_FLAG_HIDDEN | LV_OBJ_FLAG_SCROLL_ON_FOCUS);     /// Flags
    lv_obj_clear_flag(ui_BtnSet1, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_style_bg_color(ui_BtnSet1, lv_color_hex(0xAD0808), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_BtnSet1, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui_BtnSet1, lv_color_hex(0xDBDBDB), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui_BtnSet1, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui_BtnSet1, 2, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_BtnSet1Lbl1 = lv_label_create(ui_BtnSet1);
    lv_obj_set_width(ui_BtnSet1Lbl1, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_BtnSet1Lbl1, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_align(ui_BtnSet1Lbl1, LV_ALIGN_CENTER);
    lv_label_set_text(ui_BtnSet1Lbl1, "Hidden");
    lv_obj_set_style_text_color(ui_BtnSet1Lbl1, lv_color_hex(0xDDDDDD), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_BtnSet1Lbl1, 255, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_BtnSet2 = lv_btn_create(ui_ScrSettings);
    lv_obj_set_width(ui_BtnSet2, 70);
    lv_obj_set_height(ui_BtnSet2, 30);
    lv_obj_set_x(ui_BtnSet2, 40);
    lv_obj_set_y(ui_BtnSet2, -60);
    lv_obj_set_align(ui_BtnSet2, LV_ALIGN_CENTER);
    lv_obj_add_flag(ui_BtnSet2, LV_OBJ_FLAG_CHECKABLE | LV_OBJ_FLAG_SCROLL_ON_FOCUS);     /// Flags
    lv_obj_clear_flag(ui_BtnSet2, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_style_bg_color(ui_BtnSet2, lv_color_hex(0xAD0808), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_BtnSet2, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui_BtnSet2, lv_color_hex(0xDBDBDB), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui_BtnSet2, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui_BtnSet2, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui_BtnSet2, lv_color_hex(0xFF0000), LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_bg_opa(ui_BtnSet2, 255, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_bg_color(ui_BtnSet2, lv_color_hex(0xAD0808), LV_PART_MAIN | LV_STATE_PRESSED);
    lv_obj_set_style_bg_opa(ui_BtnSet2, 255, LV_PART_MAIN | LV_STATE_PRESSED);
    lv_obj_set_style_bg_color(ui_BtnSet2, lv_color_hex(0xAD0808), LV_PART_MAIN | LV_STATE_DISABLED);
    lv_obj_set_style_bg_opa(ui_BtnSet2, 50, LV_PART_MAIN | LV_STATE_DISABLED);

    ui_BtnSet2Lbl = lv_label_create(ui_BtnSet2);
    lv_obj_set_width(ui_BtnSet2Lbl, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_BtnSet2Lbl, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_BtnSet2Lbl, 0);
    lv_obj_set_y(ui_BtnSet2Lbl, 1);
    lv_obj_set_align(ui_BtnSet2Lbl, LV_ALIGN_CENTER);
    lv_label_set_text(ui_BtnSet2Lbl, "Pair");
    lv_obj_set_style_text_color(ui_BtnSet2Lbl, lv_color_hex(0xDDDDDD), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_BtnSet2Lbl, 255, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_BtnSet3 = lv_btn_create(ui_ScrSettings);
    lv_obj_set_width(ui_BtnSet3, 70);
    lv_obj_set_height(ui_BtnSet3, 30);
    lv_obj_set_x(ui_BtnSet3, -40);
    lv_obj_set_y(ui_BtnSet3, -20);
    lv_obj_set_align(ui_BtnSet3, LV_ALIGN_CENTER);
    lv_obj_add_flag(ui_BtnSet3, LV_OBJ_FLAG_SCROLL_ON_FOCUS);     /// Flags
    lv_obj_clear_flag(ui_BtnSet3, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_style_bg_color(ui_BtnSet3, lv_color_hex(0xAD0808), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_BtnSet3, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui_BtnSet3, lv_color_hex(0xDBDBDB), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui_BtnSet3, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui_BtnSet3, 2, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_BtnSet3Lbl3 = lv_label_create(ui_BtnSet3);
    lv_obj_set_width(ui_BtnSet3Lbl3, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_BtnSet3Lbl3, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_align(ui_BtnSet3Lbl3, LV_ALIGN_CENTER);
    lv_label_set_text(ui_BtnSet3Lbl3, "Restart");
    lv_obj_set_style_text_color(ui_BtnSet3Lbl3, lv_color_hex(0xDDDDDD), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_BtnSet3Lbl3, 255, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_BtnSet4 = lv_btn_create(ui_ScrSettings);
    lv_obj_set_width(ui_BtnSet4, 70);
    lv_obj_set_height(ui_BtnSet4, 30);
    lv_obj_set_x(ui_BtnSet4, 40);
    lv_obj_set_y(ui_BtnSet4, -20);
    lv_obj_set_align(ui_BtnSet4, LV_ALIGN_CENTER);
    lv_obj_add_flag(ui_BtnSet4, LV_OBJ_FLAG_SCROLL_ON_FOCUS);     /// Flags
    lv_obj_clear_flag(ui_BtnSet4, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_style_bg_color(ui_BtnSet4, lv_color_hex(0xAD0808), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_BtnSet4, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui_BtnSet4, lv_color_hex(0xDBDBDB), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui_BtnSet4, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui_BtnSet4, 2, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_BtnSet3Lbl1 = lv_label_create(ui_BtnSet4);
    lv_obj_set_width(ui_BtnSet3Lbl1, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_BtnSet3Lbl1, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_align(ui_BtnSet3Lbl1, LV_ALIGN_CENTER);
    lv_label_set_text(ui_BtnSet3Lbl1, "Reset");
    lv_obj_set_style_text_color(ui_BtnSet3Lbl1, lv_color_hex(0xDDDDDD), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_BtnSet3Lbl1, 255, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_BtnSet5 = lv_btn_create(ui_ScrSettings);
    lv_obj_set_width(ui_BtnSet5, 70);
    lv_obj_set_height(ui_BtnSet5, 30);
    lv_obj_set_x(ui_BtnSet5, -40);
    lv_obj_set_y(ui_BtnSet5, 20);
    lv_obj_set_align(ui_BtnSet5, LV_ALIGN_CENTER);
    lv_obj_add_flag(ui_BtnSet5, LV_OBJ_FLAG_SCROLL_ON_FOCUS);     /// Flags
    lv_obj_clear_flag(ui_BtnSet5, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_style_bg_color(ui_BtnSet5, lv_color_hex(0xAD0808), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_BtnSet5, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui_BtnSet5, lv_color_hex(0xDBDBDB), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui_BtnSet5, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui_BtnSet5, 2, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_BtnSet5Lbl = lv_label_create(ui_BtnSet5);
    lv_obj_set_width(ui_BtnSet5Lbl, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_BtnSet5Lbl, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_align(ui_BtnSet5Lbl, LV_ALIGN_CENTER);
    lv_label_set_text(ui_BtnSet5Lbl, "Peers");
    lv_obj_set_style_text_color(ui_BtnSet5Lbl, lv_color_hex(0xDDDDDD), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_BtnSet5Lbl, 255, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_BtnSet6 = lv_btn_create(ui_ScrSettings);
    lv_obj_set_width(ui_BtnSet6, 70);
    lv_obj_set_height(ui_BtnSet6, 30);
    lv_obj_set_x(ui_BtnSet6, 40);
    lv_obj_set_y(ui_BtnSet6, 20);
    lv_obj_set_align(ui_BtnSet6, LV_ALIGN_CENTER);
    lv_obj_add_flag(ui_BtnSet6, LV_OBJ_FLAG_SCROLL_ON_FOCUS);     /// Flags
    lv_obj_clear_flag(ui_BtnSet6, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_style_bg_color(ui_BtnSet6, lv_color_hex(0xAD0808), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_BtnSet6, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui_BtnSet6, lv_color_hex(0xDBDBDB), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui_BtnSet6, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui_BtnSet6, 2, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_BtnSet3Lbl2 = lv_label_create(ui_BtnSet6);
    lv_obj_set_width(ui_BtnSet3Lbl2, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_BtnSet3Lbl2, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_align(ui_BtnSet3Lbl2, LV_ALIGN_CENTER);
    lv_label_set_text(ui_BtnSet3Lbl2, "JSON");
    lv_obj_set_style_text_color(ui_BtnSet3Lbl2, lv_color_hex(0xDDDDDD), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_BtnSet3Lbl2, 255, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_BtnSet7 = lv_btn_create(ui_ScrSettings);
    lv_obj_set_width(ui_BtnSet7, 70);
    lv_obj_set_height(ui_BtnSet7, 30);
    lv_obj_set_x(ui_BtnSet7, -40);
    lv_obj_set_y(ui_BtnSet7, -60);
    lv_obj_set_align(ui_BtnSet7, LV_ALIGN_CENTER);
    lv_obj_add_flag(ui_BtnSet7, LV_OBJ_FLAG_CHECKABLE | LV_OBJ_FLAG_SCROLL_ON_FOCUS);     /// Flags
    lv_obj_clear_flag(ui_BtnSet7, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_style_bg_color(ui_BtnSet7, lv_color_hex(0xAD0808), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_BtnSet7, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui_BtnSet7, lv_color_hex(0xDBDBDB), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui_BtnSet7, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui_BtnSet7, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui_BtnSet7, lv_color_hex(0xFF0000), LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_bg_opa(ui_BtnSet7, 255, LV_PART_MAIN | LV_STATE_CHECKED);

    ui_BtnSet7Lbl = lv_label_create(ui_BtnSet7);
    lv_obj_set_width(ui_BtnSet7Lbl, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_BtnSet7Lbl, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_align(ui_BtnSet7Lbl, LV_ALIGN_CENTER);
    lv_label_set_text(ui_BtnSet7Lbl, "Debug");
    lv_obj_set_style_text_color(ui_BtnSet7Lbl, lv_color_hex(0xDDDDDD), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_BtnSet7Lbl, 255, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_BtnSet8 = lv_btn_create(ui_ScrSettings);
    lv_obj_set_width(ui_BtnSet8, 70);
    lv_obj_set_height(ui_BtnSet8, 30);
    lv_obj_set_x(ui_BtnSet8, 40);
    lv_obj_set_y(ui_BtnSet8, 60);
    lv_obj_set_align(ui_BtnSet8, LV_ALIGN_CENTER);
    lv_obj_add_flag(ui_BtnSet8, LV_OBJ_FLAG_CHECKABLE | LV_OBJ_FLAG_SCROLL_ON_FOCUS);     /// Flags
    lv_obj_clear_flag(ui_BtnSet8, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_style_bg_color(ui_BtnSet8, lv_color_hex(0xAD0808), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_BtnSet8, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui_BtnSet8, lv_color_hex(0xDBDBDB), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui_BtnSet8, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui_BtnSet8, 2, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_BtnSet7Lbl1 = lv_label_create(ui_BtnSet8);
    lv_obj_set_width(ui_BtnSet7Lbl1, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_BtnSet7Lbl1, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_align(ui_BtnSet7Lbl1, LV_ALIGN_CENTER);
    lv_label_set_text(ui_BtnSet7Lbl1, "Save");
    lv_obj_set_style_text_color(ui_BtnSet7Lbl1, lv_color_hex(0xDDDDDD), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_BtnSet7Lbl1, 255, LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_obj_add_event_cb(ui_BtnSet1, ui_event_BtnSet1, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(ui_BtnSet2, ui_event_BtnSet2, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(ui_BtnSet3, ui_event_BtnSet3, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(ui_BtnSet4, ui_event_BtnSet4, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(ui_BtnSet5, ui_event_BtnSet5, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(ui_BtnSet6, ui_event_BtnSet6, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(ui_BtnSet7, ui_event_BtnSet7, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(ui_BtnSet8, ui_event_BtnSet8, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(ui_ScrSettings, ui_event_ScrSettings, LV_EVENT_ALL, NULL);

}
