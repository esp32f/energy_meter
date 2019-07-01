#include <string.h>
#include <stdio.h>
#include <esp_wifi.h>
#include "p_json.h"
#include "p_wifi.h"
#include "macros.h"


esp_err_t wifi_get_config_json(wifi_interface_t interface, char *buf) {
  wifi_config_t c;
  ERET( esp_wifi_get_config(interface, &c) );
  sprintf(buf, "{\"ssid\": \"%s\", \"password\": \"%s\"}",
    interface == WIFI_IF_AP? c.ap.ssid : c.sta.ssid,
    interface == WIFI_IF_AP? c.ap.password : c.ap.password);
  return ESP_OK;
}


esp_err_t wifi_set_config_json(wifi_interface_t interface, const char *json) {
  wifi_config_t c;
  ERET( esp_wifi_get_config(interface, &c) );
  json_string(json, "\"ssid\":",
    (char*) (interface == WIFI_IF_AP? c.ap.ssid : c.sta.ssid) );
  json_string(json, "\"password\":",
    (char*) (interface == WIFI_IF_AP? c.ap.password : c.sta.password));
  return esp_wifi_set_config(interface, &c);
}


esp_err_t wifi_init(const char *mac) {
  printf("# Init WiFi\n");
  wifi_config_t c;
  ERET( esp_wifi_get_config(WIFI_IF_AP, &c) );
  if (strlen(c.ap.ssid) == 0) {
    sprintf(c.ap.ssid, CONFIG_WIFI_AP_SSID, mac);
    sprintf(c.ap.password, CONFIG_WIFI_AP_PASSWORD, mac);
    c.ap.authmode = WIFI_AUTH_WPA_PSK;
    ERET( esp_wifi_set_config(WIFI_IF_AP, &c) );
  }
  printf(": WiFi AP: ssid=%s, password=%s\n", c.ap.ssid, c.ap.password);
  ERET( esp_wifi_get_config(WIFI_IF_STA, &c) );
  if (strlen(c.sta.ssid) == 0) {
    sprintf(c.sta.ssid, CONFIG_WIFI_STA_SSID, mac);
    sprintf(c.sta.password, CONFIG_WIFI_STA_PASSWORD, mac);
    ERET( esp_wifi_set_config(WIFI_IF_STA, &c) );
  }
  printf(": WiFi STA: ssid=%s, password=%s\n", c.sta.ssid, c.sta.password);
  wifi_init_config_t ic = WIFI_INIT_CONFIG_DEFAULT();
  return esp_wifi_init(&ic);
}
