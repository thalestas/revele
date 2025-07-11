#pragma once

#include "esp_lcd_types.h"
#include "misc/lv_types.h"

#define LCD_H_RES 240
#define LCD_V_RES 135
#define LVGL_TASK_STACK_SIZE 4096 * 2

lv_display_t *lvgl_init(esp_lcd_panel_io_handle_t *const io_handle,
                        esp_lcd_panel_handle_t *const panel_handle);
