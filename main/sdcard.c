#include <string.h>

#include "driver/sdspi_host.h"
#include "driver/spi_common.h"
#include "esp_err.h"
#include "esp_log.h"
#include "esp_vfs_fat.h"
#include "hal/spi_types.h"
#include "misc/lv_fs.h"
#include "sd_protocol_types.h"
#include "sdcard.h"
#include "sdmmc_cmd.h"

#define MOUNT_POINT "/sdcard"
static const char *TAG = "sdcard";

void init_sdcard() {
  esp_err_t err;
  ESP_LOGI(TAG, SDCARD_PATH);

  spi_bus_config_t bus_cfg = {
      .mosi_io_num = 26,
      .miso_io_num = 27,
      .sclk_io_num = 25,
      .quadwp_io_num = -1,
      .quadhd_io_num = -1,
      .max_transfer_sz = 4000,
  };

  err = spi_bus_initialize(SPI3_HOST, &bus_cfg, SPI_DMA_CH_AUTO);
  if (err != ESP_OK) {
    ESP_LOGE(TAG, "Fail to init spi: %s", esp_err_to_name(err));
    return;
  }

  sdspi_device_config_t slot_config = SDSPI_DEVICE_CONFIG_DEFAULT();
  slot_config.host_id = SPI3_HOST;
  slot_config.gpio_cs = 33;

  esp_vfs_fat_sdmmc_mount_config_t mount_config = {
      .format_if_mount_failed = false,
      .max_files = 5,
      .allocation_unit_size = 16 * 1024};

  sdmmc_card_t *sdcard;
  const sdmmc_host_t host = SDSPI_HOST_DEFAULT();

  err = esp_vfs_fat_sdspi_mount(MOUNT_POINT, &host, &slot_config, &mount_config,
                                &sdcard);
  if (err != ESP_OK) {
    ESP_LOGE(TAG, "Fail to mount sdcard: %s", esp_err_to_name(err));
    return;
  }

  sdmmc_card_print_info(stdout, sdcard);
  ESP_LOGI(TAG, "sdcard mounted in %s", MOUNT_POINT);
}

void list_dir(const char *path) {
  lv_fs_dir_t dir;
  lv_fs_res_t res;
  res = lv_fs_dir_open(&dir, path);
  if (res != LV_FS_RES_OK) {
    ESP_LOGE(TAG, "Error to open %s", path);
    return;
  }

  char fn[256];
  while (1) {
    res = lv_fs_dir_read(&dir, fn, sizeof(fn));
    if (res != LV_FS_RES_OK) {
      ESP_LOGE(TAG, "Error to read dir");
      break;
    }

    if (strlen(fn) == 0) {
      break;
    }

    ESP_LOGI(TAG, "File %s", fn);
  }
  lv_fs_dir_close(&dir);
}
