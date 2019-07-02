#include <driver/gpio.h>
#include <driver/i2c.h>
#include <nvs_flash.h>
#include <esp_spiffs.h>
#include "p_device.h"
#include "macros.h"


esp_err_t efuse_get_mac_string(char *buf) {
  uint8_t mac[6];
  ERET( esp_efuse_mac_get_default(mac) );
  sprintf(buf, MACSTR, MAC2STR(mac));
  return ESP_OK;
}


esp_err_t i2c_init(i2c_port_t port, gpio_num_t sda, gpio_num_t scl, uint32_t clk_speed) {
  printf("# Init I2C %d\n", port);
  printf(": sda=%d, scl=%d, clk_speed=%d\n", sda, scl, clk_speed);
  i2c_config_t c = {
    .mode = I2C_MODE_MASTER,
    .sda_io_num = sda,
    .sda_pullup_en = GPIO_PULLUP_ENABLE,
    .scl_io_num = scl,
    .scl_pullup_en = GPIO_PULLUP_ENABLE,
    .master.clk_speed = clk_speed,
  };
  ERET( i2c_param_config(port, &c) );
  ERET( i2c_driver_install(port, c.mode, 0, 0, 0) );
  return ESP_OK;
}


esp_err_t nvs_init() {
  printf("# Init NVS flash\n");
  esp_err_t ret = nvs_flash_init();
  if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
    ERET( nvs_flash_erase() );
    ERET( nvs_flash_init() );
  }
  return ret;
}


esp_err_t spiffs_init() {
  printf("# Mount SPIFFS as VFS\n");
  esp_vfs_spiffs_conf_t c = {
    .base_path = "/spiffs",
    .partition_label = NULL,
    .max_files = 16,
    .format_if_mount_failed = false,
  };
  ERET( esp_vfs_spiffs_register(&c) );
  size_t total, used;
  ERET( esp_spiffs_info(NULL, &total, &used) );
  printf(": total=%d, used=%d\n", total, used);
  return ESP_OK;
}
