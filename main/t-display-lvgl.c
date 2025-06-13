#include "esp_err.h"
#include "display.h"
#include "esp_lcd_types.h"
#include "lvgl_port.h"
#include "savings_screen.h"

static char* TAG = "app_main";


void app_main(void)
{
	static esp_lcd_panel_io_handle_t io_handle;
	static esp_lcd_panel_handle_t panel_handle;

	ESP_ERROR_CHECK(app_lcd_init(&io_handle, &panel_handle));
	lv_display_t *lvgl_disp = lvgl_init(&io_handle, &panel_handle);

	saving_data data = {
		.saving_update_date = "16/06",
		.connection_status = true,
		.rent_status = true,
		.car_status = true,
		.light_status = false,
		.wifi_status = true,
		.water_status = false,
		.gym_status = true,
		.insurance_status = false,
		.total_spend = 61,
		.general_spend = 84,
		.aforro_value = {50, 53, 64, 65, 79},
		.trade_value = {1, 5, 14, 19, 23}
	};
	
	savings_screen(&data);
	//vTaskDelay(2000/ portTICK_PERIOD_MS);
	//savings_screen(94, 87);
	
}

