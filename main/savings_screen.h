#pragma once

#include "lvgl.h"
#include <stdint.h>

#define BOX_BG_COLOR LV_COLOR_MAKE(0x15, 0x2f, 0x50)
#define BOX_GRAD_COLOR LV_COLOR_MAKE(0x16, 0x1d, 0x37)

#define BAR_OPACITY LV_OPA_COVER
#define BAR_BG_COLOR lv_palette_lighten(LV_PALETTE_BLUE, 1)
#define BAR_GRAD_COLOR lv_palette_darken(LV_PALETTE_BLUE, 2)

LV_FONT_DECLARE(fontawesome);
#define SYMBOL_HOUSE		"\xEF\x80\x95"
#define SYMBOL_CAR	 		"\xEF\x86\xB9"
#define SYMBOL_LIGHT 		"\xEF\x83\xAB"
#define SYMBOL_WIFI  		"\xEF\x87\xAB"
#define SYMBOL_WATER		"\xEE\x80\x86"
#define SYMBOL_DUMBBELL		"\xEF\x91\x8B"
#define SYMBOL_INSURANCE	"\xEE\x93\xB0"

void savings_screen(uint32_t start_value1, uint32_t start_value2);
