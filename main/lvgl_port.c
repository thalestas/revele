#include "lvgl_port.h"
#include "esp_err.h"
#include "esp_lvgl_port.h"
#include "esp_log.h"

static char* TAG = "lvgl_port";

lv_display_t* lvgl_init(esp_lcd_panel_io_handle_t *const io_handle, esp_lcd_panel_handle_t *const panel_handle){

	// Init LVGL
	ESP_LOGI(TAG, "LVGL initialization");
	const lvgl_port_cfg_t lvgl_config = {
		.task_priority = 4,
		.task_stack = LVGL_TASK_STACK_SIZE,
		.task_affinity = -1,
		.task_max_sleep_ms = 500,
		.timer_period_ms = 5
	};
	ESP_ERROR_CHECK(lvgl_port_init(&lvgl_config));

	//Add LCD
	ESP_LOGI(TAG, "Add LCD");
	const lvgl_port_display_cfg_t disp_cfg = {
		.io_handle = *io_handle,
		.panel_handle = *panel_handle,
		.buffer_size = LCD_H_RES * LCD_V_RES,
		.double_buffer = true,
		.hres = LCD_H_RES,
		.vres = LCD_V_RES,
		.monochrome = false,
		.color_format = LV_COLOR_FORMAT_RGB565,
		.rotation = {
			.swap_xy = true,
			.mirror_x = true,
			.mirror_y = false,
		},
		.flags = {
			.buff_dma = true,
			.swap_bytes = true,
		}
	};
	return lvgl_port_add_disp(&disp_cfg);
}


