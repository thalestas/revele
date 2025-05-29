#include "core/lv_obj.h"
#include "core/lv_obj_style_gen.h"
#include "esp_err.h"
#include "display.h"
#include "esp_lcd_types.h"
#include "lvgl_port.h"
#include "esp_lvgl_port.h"
#include "misc/lv_palette.h"
#include "savings_screen.h"

static char* TAG = "app_main";


void app_main(void)
{
	static esp_lcd_panel_io_handle_t io_handle;
	static esp_lcd_panel_handle_t panel_handle;

	ESP_ERROR_CHECK(app_lcd_init(&io_handle, &panel_handle));
	lv_display_t *lvgl_disp = lvgl_init(&io_handle, &panel_handle);
	
	//lvgl_port_lock(0);
	////Set background color
	//lv_obj_clear_flag(lv_scr_act(), LV_OBJ_FLAG_SCROLLABLE);
	//lv_obj_set_style_bg_color(lv_scr_act(), lv_palette_darken(LV_PALETTE_BLUE_GREY,4), 0);

	//lvgl_port_unlock();
	
	savings_screen(83, 98);
	vTaskDelay(2000/ portTICK_PERIOD_MS);
	savings_screen(94, 87);
}

