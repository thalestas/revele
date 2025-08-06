#include "esp_err.h"
#include "esp_log.h"
#include "esp_wifi.h"
#include "nvs_flash.h"
#include "wifi_provisioning/scheme_softap.h"

void connect_wifi(esp_event_handler_t event_handler) {
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
      WIFI_EVENT, ESP_EVENT_ANY_ID, event_handler, NULL, &instance_any_id));

  wifi_prov_mgr_config_t prov_cfg = {
      .scheme = wifi_prov_scheme_softap,
      .scheme_event_handler = WIFI_PROV_EVENT_HANDLER_NONE};

  wifi_prov_mgr_init(prov_cfg);

  bool provisioned = false;
  wifi_prov_mgr_is_provisioned(&provisioned);

  if (!provisioned) {
    ESP_LOGI("WIFI", "Starting provisioning portal...");
    esp_netif_create_default_wifi_ap();
    wifi_prov_mgr_start_provisioning(WIFI_PROV_SECURITY_1, NULL, "ESP32_PROV",
                                     NULL);
  } else {
    ESP_LOGI("WIFI", "Already provisioned, connecting to WIFI...");
    esp_wifi_start();
    esp_wifi_connect();
  }
}
