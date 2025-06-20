#pragma once

#include "lvgl.h"

/* BOX BACKGROUND COLORS */
#define BOX_BG_COLOR LV_COLOR_MAKE(0x15, 0x2f, 0x50)
#define BOX_GRAD_COLOR LV_COLOR_MAKE(0x16, 0x1d, 0x37)

/* LABEL COLORS */
#define LABEL_COLOR lv_palette_lighten(LV_PALETTE_BLUE_GREY, 3)

void box_style_init(lv_style_t *const style);
void subtitle_style_init(lv_style_t *const style);
