#pragma once
#include <esp_wifi.h>


// Kconfig constants:
// CONFIG_WIFI_AP_SSID
// CONFIG_WIFI_AP_PASSWORD
// CONFIG_WIFI_STA_SSID
// CONFIG_WIFI_STA_PASSWORD


esp_err_t wifi_get_config_json(wifi_interface_t interface, char *buf);
esp_err_t wifi_set_config_json(wifi_interface_t interface, const char *json);
esp_err_t wifi_init(const char *mac);
