#include "display.h"
#include "esp_lvgl_port.h"
#include "lvgl_port.h"
#include "screens/savings_screen.h"
#include "screens/status_bar.h"
#include "wifi_prov.h"

static char *TAG = "app_main";

void app_main(void) {
  // Init LVGL
  static esp_lcd_panel_io_handle_t io_handle;
  static esp_lcd_panel_handle_t panel_handle;

  ESP_ERROR_CHECK(app_lcd_init(&io_handle, &panel_handle));
  lv_display_t *lvgl_disp = lvgl_init(&io_handle, &panel_handle);

  // Init
  status_bar_init();

  // Init saving screen data
  saving_data data = {.rent_status = true,
                      .car_status = true,
                      .light_status = false,
                      .wifi_status = true,
                      .water_status = false,
                      .gym_status = true,
                      .insurance_status = false,
                      .total_spend = 61,
                      .general_spend = 84,
                      .aforro_value = {50, 53, 64, 65, 79},
                      .trade_value = {1, 5, 14, 19, 23}};

  // Create saving screen
  lv_obj_t *sav_scr = savings_screen(&data);

  // Create wallpaper screen
  lv_obj_t *wallpaper = lv_obj_create(NULL);
  LV_IMAGE_DECLARE(arc_dither);
  lv_obj_set_style_bg_img_src(wallpaper, &arc_dither, 0);

  // Wifi connection
  connect_wifi(&wifi_event_handler);

  // Load screens
  while (1) {
    lvgl_port_lock(0);
    lv_scr_load_anim(sav_scr, LV_SCR_LOAD_ANIM_FADE_IN, 2000, 0, false);
    lvgl_port_unlock();
    vTaskDelay(6000 / portTICK_PERIOD_MS);

    lvgl_port_lock(0);
    lv_scr_load_anim(wallpaper, LV_SCR_LOAD_ANIM_FADE_OUT, 2000, 0, false);
    lvgl_port_unlock();
    vTaskDelay(6000 / portTICK_PERIOD_MS);
  }
}
