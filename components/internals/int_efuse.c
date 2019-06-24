#include <stdio.h>
#include <sdkconfig.h>
#include "int_efuse.h"


esp_err_t efuse_mac(char *buff) {
  uint8_t mac[6];
  ERET( esp_efuse_mac_get_default(mac) );
  sprintf(buff, "%02X:%02X:%02X:%02X:%02X:%02X\n", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
  return ESP_OK;
}
