#include "display.h"
#include "esp_event.h"
#include "esp_log.h"
#include "esp_lvgl_port.h"
#include "esp_wifi.h"
#include "lvgl_port.h"
#include "nvs_flash.h"
#include "screens/savings_screen.h"
#include "styles/styles.h"
#include "wifi_provisioning/scheme_softap.h"

static char *TAG = "app_main";
static lv_obj_t *status_bar;
static lv_obj_t *wifi_status;

static void anim_opacity_cb(void *obj, int32_t v) {
  lv_obj_set_style_text_opa(obj, v, 0);
}

static void wifi_icon_anim_start() {
  lv_anim_t a;
  lv_anim_init(&a);
  lv_anim_set_var(&a, wifi_status);
  lv_anim_set_values(&a, LV_OPA_10, LV_OPA_COVER);
  lv_anim_set_time(&a, 800);
  lv_anim_set_playback_time(&a, 800);
  lv_anim_set_repeat_delay(&a, 300);
  lv_anim_set_playback_delay(&a, 300);
  lv_anim_set_repeat_count(&a, LV_ANIM_REPEAT_INFINITE);
  lv_anim_set_exec_cb(&a, anim_opacity_cb);
  lv_anim_start(&a);
}

static void wifi_icon_anim_stop() { lv_anim_del(wifi_status, anim_opacity_cb); }

void status_bar_init(lv_obj_t *const status_bar,
                     const lv_style_t *const bar_style,
                     const lv_style_t *const label_style,
                     const saving_data *const data) {
  // bar style
  lv_obj_add_style(status_bar, bar_style, 0);
  lv_obj_set_style_size(status_bar, 230, 20, 0);
  lv_obj_align(status_bar, LV_ALIGN_TOP_MID, 0, 5);

  // refresh symbol
  lv_obj_t *refresh_data_symbol = lv_label_create(status_bar);
  lv_label_set_text(refresh_data_symbol, LV_SYMBOL_REFRESH " ");
  lv_obj_add_style(refresh_data_symbol, label_style, 0);
  lv_obj_align(refresh_data_symbol, LV_ALIGN_LEFT_MID, 5, 0);

  // refresh date
  lv_obj_t *refresh_data_label = lv_label_create(status_bar);
  lv_label_set_text(refresh_data_label, "25/12");
  lv_obj_add_style(refresh_data_label, label_style, 0);
  lv_obj_set_style_text_font(refresh_data_label, &lv_font_montserrat_10, 0);
  lv_obj_align_to(refresh_data_label, refresh_data_symbol,
                  LV_ALIGN_OUT_RIGHT_MID, 0, 0);

  // Title
  lv_obj_t *dashboard_title = lv_label_create(status_bar);
  lv_obj_set_style_text_font(dashboard_title, &lv_font_montserrat_16, 0);
  lv_label_set_text(dashboard_title, "Financas");
  lv_obj_add_style(dashboard_title, label_style, 0);
  lv_obj_align(dashboard_title, LV_ALIGN_CENTER, 0, 0);

  // wifi connection status
  wifi_status = lv_label_create(status_bar);
  lv_label_set_text(wifi_status, LV_SYMBOL_WIFI);
  lv_obj_add_style(wifi_status, label_style, 0);
  lv_obj_align(wifi_status, LV_ALIGN_RIGHT_MID, -5, 0);
  lv_obj_set_style_text_opa(wifi_status, LV_OPA_20, 0);
}

static void wifi_event_handler(void *arg, esp_event_base_t event_base,
                               int32_t event_id, void *event_data) {
  lvgl_port_lock(0);
  if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_START) {
    // connecting
    wifi_icon_anim_start();
  } else if (event_base == WIFI_EVENT &&
             event_id == WIFI_EVENT_STA_DISCONNECTED) {
    // disconnected
    wifi_icon_anim_stop();
    lv_obj_set_style_text_opa(wifi_status, LV_OPA_20, 0);
  } else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_CONNECTED) {
    // connected
    wifi_icon_anim_stop();
    lv_obj_set_style_text_opa(wifi_status, LV_OPA_COVER, 0);
  }
  lvgl_port_unlock();
}

void connect_wifi() {
  esp_err_t ret = nvs_flash_init();
  if (ret == ESP_ERR_NVS_NO_FREE_PAGES ||
      ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
    ESP_ERROR_CHECK(nvs_flash_erase());
    ret = nvs_flash_init();
  }
  ESP_ERROR_CHECK(ret);

  ESP_ERROR_CHECK(esp_netif_init());
  ESP_ERROR_CHECK(esp_event_loop_create_default());

  wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
  ESP_ERROR_CHECK(esp_wifi_init(&cfg));
  ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));

  esp_event_handler_instance_t instance_any_id;
  ESP_ERROR_CHECK(esp_event_handler_instance_register(
      WIFI_EVENT, ESP_EVENT_ANY_ID, &wifi_event_handler, NULL,
      &instance_any_id));

  wifi_prov_mgr_config_t prov_cfg = {
      .scheme = wifi_prov_scheme_softap,
      .scheme_event_handler = WIFI_PROV_EVENT_HANDLER_NONE};

  wifi_prov_mgr_init(prov_cfg);

  bool provisioned = false;
  wifi_prov_mgr_is_provisioned(&provisioned);

  if (!provisioned) {
    ESP_LOGI("WIFI", "Iniciando portal de provisionamento...");
    esp_netif_create_default_wifi_ap();
    wifi_prov_mgr_start_provisioning(WIFI_PROV_SECURITY_1, NULL, "ESP32_PROV",
                                     NULL);
  } else {
    ESP_LOGI("WIFI", "Ja provisionado, conectando WIFI...");
    esp_wifi_start();
    esp_wifi_connect();
  }
}

void app_main(void) {
  // Init LVGL
  static esp_lcd_panel_io_handle_t io_handle;
  static esp_lcd_panel_handle_t panel_handle;

  ESP_ERROR_CHECK(app_lcd_init(&io_handle, &panel_handle));
  lv_display_t *lvgl_disp = lvgl_init(&io_handle, &panel_handle);

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

  // Start saving screen

  status_bar = lv_obj_create(lv_layer_top());

  static lv_style_t box_style;
  lv_style_init(&box_style);
  box_style_init(&box_style);

  // subtitle style
  static lv_style_t subtitle_style;
  lv_style_init(&subtitle_style);
  subtitle_style_init(&subtitle_style);

  status_bar_init(status_bar, &box_style, &subtitle_style, &data);

  lv_obj_t *sav_scr = savings_screen(&data);

  lv_obj_t *wallpaper = lv_obj_create(NULL);
  LV_IMAGE_DECLARE(arc_dither);
  lv_obj_set_style_bg_img_src(wallpaper, &arc_dither, 0);

  /*		Wifi connection		*/
  connect_wifi();

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
