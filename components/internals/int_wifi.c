#include <freertos/FreeRTOS.h>
#include <freertos/event_groups.h>
#include <tcpip_adapter.h>
#include <esp_log.h>
#include <esp_event.h>
#include <esp_wifi.h>
#include "int_wifi.h"
#include "macros.h"


#define TAG_OLD TAG
#define TAG "wifi"


static void on_wifi(void *arg, esp_event_base_t base, int32_t id, void *data) {
  if (id == WIFI_EVENT_STA_START || id == WIFI_EVENT_STA_DISCONNECTED) {
    esp_err_t ret = esp_wifi_connect();
    if (ret != ESP_OK) ESP_LOGE(TAG, "on_wifi: %s", esp_err_to_name(ret));
    else ESP_LOGI(TAG, "on_wifi: connect successful");
  }
}


static void on_ip(void *arg, esp_event_base_t base, int32_t id, void* data) {
  if (id == IP_EVENT_STA_GOT_IP) {
    ip_event_got_ip_t *gotip = (ip_event_got_ip_t*) data;
    ESP_LOGI(TAG, "on_ip: IP addr = %s", ip4addr_ntoa(&gotip->ip_info.ip));
    ESP_LOGI(TAG, "on_ip: Netmask = %s", ip4addr_ntoa(&gotip->ip_info.netmask));
    ESP_LOGI(TAG, "on_ip: Gateway = %s", ip4addr_ntoa(&gotip->ip_info.gw));
  }
}


esp_err_t wifi_init(uint8_t *ssid, uint8_t *pass) {
  tcpip_adapter_init();
  ERET( esp_event_loop_create_default() );
  wifi_init_config_t iconfig = WIFI_INIT_CONFIG_DEFAULT();
  ERET( esp_wifi_init(&iconfig) );
  ERET( esp_event_handler_register(WIFI_EVENT, ESP_EVENT_ANY_ID, &on_wifi, NULL) );
  ERET( esp_event_handler_register(IP_EVENT, IP_EVENT_STA_GOT_IP, &on_ip, NULL) );
  wifi_config_t config = {.sta = {
    .ssid = ssid,
    .password = pass,
    .scan_method = WIFI_ALL_CHANNEL_SCAN,
    .sort_method = WIFI_CONNECT_AP_BY_SIGNAL,
    .threshold.rssi = -127,
    .threshold.authmode = WIFI_AUTH_OPEN
  }};
  ERET( esp_wifi_set_mode(WIFI_MODE_STA) );
  ERET( esp_wifi_set_config(ESP_IF_WIFI_STA, &config) );
  return esp_wifi_start();
}


#define TAG TAG_OLD
