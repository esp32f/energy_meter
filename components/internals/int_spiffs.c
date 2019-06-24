#include <stdint.h>
#include <sdkconfig.h>
#include <esp_log.h>
#include <esp_spiffs.h>
#include "macros.h"
#include "int_spiffs.h"


static const char *TAG = "spiffs";


esp_err_t spiffs_init() {
  esp_vfs_spiffs_conf_t config = {
    .base_path = "/spiffs",
    .max_files = 5,
    .partition_label = NULL,
    .format_if_mount_failed = true
  };
  ERET( esp_vfs_spiffs_register(&config) );
  size_t total, used;
  ERET( esp_spiffs_info(NULL, &total, &used) );
  ESP_LOGI(TAG, "Total: %d, Used: %d", total, used);
  return ESP_OK;
}
