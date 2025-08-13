#include "display.h"
#include "esp_lvgl_port.h"
#include "lvgl_port.h"
#include "screens/status_bar.h"
#include "sdcard.h"
#include "wifi_prov.h"

static char *TAG = "app_main";

void app_main(void) {
  init_sdcard();

  // Init LVGL
  lvgl_init();

  // Init
  status_bar_init();

  // Create wallpaper screen
  lv_obj_t *wallpaper = lv_obj_create(NULL);
  LV_IMAGE_DECLARE(arc_dither);
  lv_obj_set_style_bg_img_src(wallpaper, &arc_dither, 0);

  // Wifi connection
  connect_wifi(&wifi_event_handler);

  // Load screens
  while (1) {
    lvgl_port_lock(0);
    lv_scr_load_anim(wallpaper, LV_SCR_LOAD_ANIM_FADE_OUT, 2000, 0, false);
    lvgl_port_unlock();
    vTaskDelay(6000 / portTICK_PERIOD_MS);
  }
}
