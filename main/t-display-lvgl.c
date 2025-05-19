#include "esp_err.h"
#include "display.h"
#include "esp_lcd_types.h"
#include "lvgl_port.h"
#include "demos/benchmark/lv_demo_benchmark.h"

static char* TAG = "app_main";

void app_main(void)
{
	static esp_lcd_panel_io_handle_t io_handle;
	static esp_lcd_panel_handle_t panel_handle;

	ESP_ERROR_CHECK(app_lcd_init(&io_handle, &panel_handle));
	lv_display_t *lvgl_disp = lvgl_init(&io_handle, &panel_handle);
	lv_demo_benchmark();
}
