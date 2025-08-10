#pragma once

#include "esp_lcd_types.h"
#include "misc/lv_types.h"

#define LCD_H_RES 240
#define LCD_V_RES 135
#define LVGL_TASK_STACK_SIZE 4096 * 2

void lvgl_init();
