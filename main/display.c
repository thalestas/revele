#include "display.h"
#include "driver/spi_common.h"
#include "esp_err.h"
#include "esp_check.h"
#include "esp_lcd_panel_io.h"
#include "esp_log.h"
#include "esp_lcd_io_spi.h"
#include "esp_lcd_panel_dev.h"
#include "esp_lcd_panel_st7789.h"
#include "esp_lcd_panel_ops.h"
#include "driver/gpio.h"

const char* TAG = "display";

esp_err_t app_lcd_init(esp_lcd_panel_io_handle_t *const io_handle, esp_lcd_panel_handle_t *const panel_handle) {
	esp_err_t ret = ESP_OK;

	ESP_LOGI(TAG, "Display Initialization");

	//SPI Bus Configuration
	spi_bus_config_t bus_cfg = {
		.sclk_io_num = PIN_NUM_SCKL,
		.mosi_io_num = PIN_NUM_MOSI,
		.miso_io_num = PIN_NUM_MISO,
		.quadwp_io_num = -1,
		.quadhd_io_num = -1,
		.max_transfer_sz = LCD_H_RES * LCD_V_RES * sizeof(uint16_t),
	};
	ESP_GOTO_ON_ERROR(spi_bus_initialize(LCD_HOST, &bus_cfg, SPI_DMA_CH_AUTO), err, TAG, "SPI bus init failed!");

	//IO Configuration
	esp_lcd_panel_io_spi_config_t io_cfg = {
		.dc_gpio_num = PIN_NUM_LCD_DC,
		.cs_gpio_num = PIN_NUM_LCD_CS,
		.pclk_hz = LCD_PIXEL_CLOCK_HZ,
		.lcd_cmd_bits = 8,
		.lcd_param_bits = 8,
		.spi_mode = 0,
		.trans_queue_depth = 10,
	};
	ESP_GOTO_ON_ERROR(esp_lcd_new_panel_io_spi((esp_lcd_spi_bus_handle_t)LCD_HOST, &io_cfg, io_handle), err, TAG, "Panel IO failed!");

	//ST7789 Configuration
	esp_lcd_panel_dev_config_t panel_cfg = {
		.reset_gpio_num = PIN_NUM_LCD_RST,
		.color_space = ESP_LCD_COLOR_SPACE_RGB,
		.bits_per_pixel = 16,
	};
	ESP_GOTO_ON_ERROR(esp_lcd_new_panel_st7789(*io_handle, &panel_cfg, panel_handle), err, TAG, "Panel ST7789 failed!");

	//Reset and init
	esp_lcd_panel_reset(*panel_handle);
	esp_lcd_panel_init(*panel_handle);

	//Colors and Orientation
	esp_lcd_panel_set_gap(*panel_handle, 40, 53);
	esp_lcd_panel_invert_color(*panel_handle, true);
	ESP_GOTO_ON_ERROR(esp_lcd_panel_swap_xy(*panel_handle, true), err, TAG, "Panel swap failed!");
	ESP_GOTO_ON_ERROR(esp_lcd_panel_mirror(*panel_handle, true, true), err, TAG, "Panel mirror failed!");

	//Turn Display ON
	ESP_GOTO_ON_ERROR(esp_lcd_panel_disp_on_off(*panel_handle, true), err, TAG, "Turn panel on failed!");

	//Backlight
	gpio_config_t bk_cfg = {
		.mode = GPIO_MODE_OUTPUT,
		.pin_bit_mask = 1ULL << PIN_NUM_BACKLIGHT,
	};
	ESP_GOTO_ON_ERROR(gpio_config(&bk_cfg), err, TAG, "Backlight gpio config failed!");
	ESP_GOTO_ON_ERROR(gpio_set_level(PIN_NUM_BACKLIGHT, 1), err, TAG, "Set Backlight gpio failed!");

	ESP_LOGI(TAG, "Display Initialized");

	return ret;

err:
	if(panel_handle) 
		esp_lcd_panel_del(*panel_handle);
	if(io_handle)
		ESP_ERROR_CHECK(esp_lcd_panel_io_del(*io_handle));
	ESP_ERROR_CHECK(spi_bus_free(LCD_HOST));
	return ret;

}
