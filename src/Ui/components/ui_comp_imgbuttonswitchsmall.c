// This file was generated by SquareLine Studio
// SquareLine Studio version: SquareLine Studio 1.3.4
// LVGL version: 8.3.6
// Project name: Jeepify

#include "../ui.h"


// COMPONENT ImgButtonSwitchSmall

lv_obj_t * ui_ImgButtonSwitchSmall_create(lv_obj_t * comp_parent)
{

    lv_obj_t * cui_ImgButtonSwitchSmall;
    cui_ImgButtonSwitchSmall = lv_imgbtn_create(comp_parent);
    lv_imgbtn_set_src(cui_ImgButtonSwitchSmall, LV_IMGBTN_STATE_RELEASED, NULL, &ui_img_1527887470, NULL);
    lv_imgbtn_set_src(cui_ImgButtonSwitchSmall, LV_IMGBTN_STATE_PRESSED, NULL, &ui_img_1527887470, NULL);
    lv_imgbtn_set_src(cui_ImgButtonSwitchSmall, LV_IMGBTN_STATE_CHECKED_PRESSED, NULL, &ui_img_551600910, NULL);
    lv_imgbtn_set_src(cui_ImgButtonSwitchSmall, LV_IMGBTN_STATE_CHECKED_RELEASED, NULL, &ui_img_551600910, NULL);
    lv_obj_set_height(cui_ImgButtonSwitchSmall, 100);
    lv_obj_set_width(cui_ImgButtonSwitchSmall, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_x(cui_ImgButtonSwitchSmall, -40);
    lv_obj_set_y(cui_ImgButtonSwitchSmall, -36);
    lv_obj_set_align(cui_ImgButtonSwitchSmall, LV_ALIGN_CENTER);
    lv_obj_add_flag(cui_ImgButtonSwitchSmall, LV_OBJ_FLAG_CHECKABLE);     /// Flags

    lv_obj_t * cui_LblPeer;
    cui_LblPeer = lv_label_create(cui_ImgButtonSwitchSmall);
    lv_obj_set_width(cui_LblPeer, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(cui_LblPeer, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(cui_LblPeer, 0);
    lv_obj_set_y(cui_LblPeer, 10);
    lv_obj_set_align(cui_LblPeer, LV_ALIGN_CENTER);
    lv_label_set_text(cui_LblPeer, "Peer");
    lv_obj_set_style_text_color(cui_LblPeer, lv_color_hex(0xDBDBDB), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(cui_LblPeer, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(cui_LblPeer, &lv_font_montserrat_10, LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_obj_t * cui_LblPeriph;
    cui_LblPeriph = lv_label_create(cui_ImgButtonSwitchSmall);
    lv_obj_set_width(cui_LblPeriph, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(cui_LblPeriph, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(cui_LblPeriph, 0);
    lv_obj_set_y(cui_LblPeriph, -13);
    lv_obj_set_align(cui_LblPeriph, LV_ALIGN_CENTER);
    lv_obj_set_style_text_color(cui_LblPeriph, lv_color_hex(0xDBDBDB), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(cui_LblPeriph, 255, LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_obj_t ** children = lv_mem_alloc(sizeof(lv_obj_t *) * _UI_COMP_IMGBUTTONSWITCHSMALL_NUM);
    children[UI_COMP_IMGBUTTONSWITCHSMALL_IMGBUTTONSWITCHSMALL] = cui_ImgButtonSwitchSmall;
    children[UI_COMP_IMGBUTTONSWITCHSMALL_LBLPEER] = cui_LblPeer;
    children[UI_COMP_IMGBUTTONSWITCHSMALL_LBLPERIPH] = cui_LblPeriph;
    lv_obj_add_event_cb(cui_ImgButtonSwitchSmall, get_component_child_event_cb, LV_EVENT_GET_COMP_CHILD, children);
    lv_obj_add_event_cb(cui_ImgButtonSwitchSmall, del_component_child_event_cb, LV_EVENT_DELETE, children);
    ui_comp_ImgButtonSwitchSmall_create_hook(cui_ImgButtonSwitchSmall);
    return cui_ImgButtonSwitchSmall;
}

