#pragma once
#include "esp_err.h"
#include "esp_lcd_types.h"

#define LCD_PIXEL_CLOCK_HZ (40 * 1000 * 1000) //40MHz
#define LCD_H_RES 240
#define LCD_V_RES 135

#define LCD_HOST SPI2_HOST
#define PIN_NUM_SCKL 18
#define PIN_NUM_MOSI 19
#define PIN_NUM_MISO -1
#define PIN_NUM_LCD_DC 16
#define PIN_NUM_LCD_CS 5
#define PIN_NUM_LCD_RST 23
#define PIN_NUM_BACKLIGHT 4

esp_err_t app_lcd_init(esp_lcd_panel_io_handle_t *const io_handle, esp_lcd_panel_handle_t *const panel_handle);
