SET(SOURCES screens/ui_ScrMenu.c
    screens/ui_ScrSettings.c
    screens/ui_ScrPeers.c
    screens/ui_ScrJSON.c
    screens/ui_ScrSingle.c
    screens/ui_ScrMulti.c
    screens/ui_ScrPeer.c
    screens/ui_ScreenEichen.c
    screens/ui_ScreenVolt.c
    ui.c
    components/ui_comp_hook.c
    ui_helpers.c
    ui_events.cpp
    images/ui_img_jeepifybackground_png.c
    images/ui_img_menubtn1_png.c
    images/ui_img_menubtn2_png.c
    images/ui_img_menubtn3_png.c
    images/ui_img_menubtn4_png.c
    images/ui_img_rubicon_png.c)

add_library(ui ${SOURCES})
