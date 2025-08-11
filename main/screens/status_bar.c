#include "esp_lvgl_port.h"
#include "esp_wifi.h"
#include "status_bar.h"
#include "styles/styles.h"

void status_bar_init() {
  status_bar = lv_obj_create(lv_layer_top());

  static lv_style_t box_style;
  lv_style_init(&box_style);
  box_style_init(&box_style);

  static lv_style_t subtitle_style;
  lv_style_init(&subtitle_style);
  subtitle_style_init(&subtitle_style);

  // bar style
  lv_obj_add_style(status_bar, &box_style, 0);
  lv_obj_set_style_size(status_bar, 230, 20, 0);
  lv_obj_align(status_bar, LV_ALIGN_TOP_MID, 0, 5);

  // refresh symbol
  lv_obj_t *refresh_data_symbol = lv_label_create(status_bar);
  lv_label_set_text(refresh_data_symbol, LV_SYMBOL_REFRESH " ");
  lv_obj_add_style(refresh_data_symbol, &subtitle_style, 0);
  lv_obj_align(refresh_data_symbol, LV_ALIGN_LEFT_MID, 5, 0);

  // refresh date
  lv_obj_t *refresh_data_label = lv_label_create(status_bar);
  lv_label_set_text(refresh_data_label, "25/12");
  lv_obj_add_style(refresh_data_label, &subtitle_style, 0);
  lv_obj_set_style_text_font(refresh_data_label, &lv_font_montserrat_10, 0);
  lv_obj_align_to(refresh_data_label, refresh_data_symbol,
                  LV_ALIGN_OUT_RIGHT_MID, 0, 0);

  // Title
  status_bar_title = lv_label_create(status_bar);
  lv_obj_set_style_text_font(status_bar_title, &lv_font_montserrat_16, 0);
  lv_label_set_text(status_bar_title, "Financas");
  lv_obj_add_style(status_bar_title, &subtitle_style, 0);
  lv_obj_align(status_bar_title, LV_ALIGN_CENTER, 0, 0);

  // wifi connection status
  wifi_status = lv_label_create(status_bar);
  lv_label_set_text(wifi_status, LV_SYMBOL_WIFI);
  lv_obj_add_style(wifi_status, &subtitle_style, 0);
  lv_obj_align(wifi_status, LV_ALIGN_RIGHT_MID, -5, 0);
  lv_obj_set_style_text_opa(wifi_status, LV_OPA_20, 0);
}

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

void wifi_event_handler(void *arg, esp_event_base_t event_base,
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

void set_status_bar_opa(lv_opa_t value) {
  lv_obj_set_style_opa(status_bar, value, 0);
}
void set_status_bar_text(const char *text) {
  lv_label_set_text(status_bar_title, text);
}
