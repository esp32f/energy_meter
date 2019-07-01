#include <string.h>
#include <esp_wifi.h>
#include <esp_event.h>
#include "p_json.h"
#include "p_wifi.h"
#include "macros.h"


#define WIFI_AP_SSID      "charmender"
#define WIFI_AP_PASSWORD  "charmender"


esp_err_t wifi_config_sta_json(char *buff) {
  wifi_config_t c;
  ERET( esp_wifi_get_config(WIFI_IF_STA, &c) );
  sprintf(buff,
    "{\"ssid\": \"%s\", \"password\": \"%s\"}",
    c.sta.ssid, c.sta.password);
  return ESP_OK;
}


esp_err_t wifi_set_config_sta_json(const char *json) {
  wifi_config_t c;
  ERET( esp_wifi_get_config(WIFI_IF_STA, &c) );
  json_string(json, "\"ssid\":", (char*)c.sta.ssid);
  json_string(json, "\"password\":", (char*)c.sta.password);
  return esp_wifi_set_config(WIFI_IF_STA, &c);
}


void wifi_on_sta(void *arg, esp_event_base_t base, int32_t id, void *data) {
  if (id == WIFI_EVENT_AP_STACONNECTED) {
    wifi_event_ap_staconnected_t *d = (wifi_event_ap_staconnected_t*) data;
    printf("- Station " MACSTR " joined, AID = %d (event)\n", MAC2STR(d->mac), d->aid);
  } else if (id == WIFI_EVENT_AP_STADISCONNECTED) {
    wifi_event_ap_stadisconnected_t *d = (wifi_event_ap_stadisconnected_t*) data;
    printf("- Station " MACSTR " left, AID = %d (event)\n", MAC2STR(d->mac), d->aid);
  }
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
