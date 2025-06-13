#pragma once

#include "lvgl.h"
#include <stdint.h>

/* BOX BACKGROUND COLORS */
#define BOX_BG_COLOR LV_COLOR_MAKE(0x15, 0x2f, 0x50)
#define BOX_GRAD_COLOR LV_COLOR_MAKE(0x16, 0x1d, 0x37)

/* SPEND BARS COLORS */
#define BAR_OPACITY LV_OPA_COVER
#define BAR1_BG_COLOR lv_palette_lighten(LV_PALETTE_BLUE, 1)
#define BAR1_GRAD_COLOR lv_palette_darken(LV_PALETTE_BLUE, 4)
#define BAR2_BG_COLOR lv_palette_lighten(LV_PALETTE_DEEP_ORANGE, 1)
#define BAR2_GRAD_COLOR lv_palette_darken(LV_PALETTE_DEEP_ORANGE, 4)

/* LABEL COLORS */
#define LABEL_COLOR lv_palette_lighten(LV_PALETTE_BLUE_GREY, 3)

/* SAVING CHART COLORS */
#define CHART_LINE1_COLOR lv_palette_darken(LV_PALETTE_BLUE, 1)
#define CHART_LINE2_COLOR lv_palette_darken(LV_PALETTE_DEEP_ORANGE, 1)

LV_FONT_DECLARE(fontawesome);
LV_FONT_DECLARE(nerdfont);
#define SYMBOL_HOUSE		"\xEF\x91\xAD"
#define SYMBOL_CAR	 		"\xEE\xBA\xA0"
#define SYMBOL_LIGHT 		"\xEF\x90\x80"
#define SYMBOL_WIFI  		"\xF3\xB0\xBA\x90"
#define SYMBOL_WATER		"\xEE\x80\x86"
#define SYMBOL_KETTLEBELL		"\xF3\xB1\x8C\x80"
#define SYMBOL_INSURANCE	"\xF3\xB0\xBE\x83"

typedef struct  {
	const char *saving_update_date;
	bool connection_status;
	bool rent_status;
	bool car_status;
	bool light_status;
	bool wifi_status;
	bool water_status;
	bool gym_status;
	bool insurance_status;
	uint32_t total_spend;
	uint32_t general_spend;
	uint32_t aforro_value[12];
	uint32_t trade_value[12];
} saving_data;

void savings_screen(const saving_data *data);
