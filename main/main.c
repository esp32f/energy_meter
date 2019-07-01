#include <esp_wifi.h>
#include <esp_event.h>
#include <nvs_flash.h>
#include <esp_spiffs.h>
#include <lwip/err.h>
#include <lwip/sys.h>
#include <freertos/FreeRTOS.h>
#include "p_device.h"
#include "p_sht21.h"
#include "p_wifi.h"
#include "p_httpd.h"
#include "p_mqtt.h"
#include "macros.h"


#define MAX_JSON_SIZE 256


static char[32] mac;
static i2c_port_t i2c = CONFIG_I2C_MASTER_PORT;
static httpd_handle_t httpd = NULL;
static esp_mqtt_client_handle_t mqtt = NULL;
static bool mqtt_connected = false;
static bool wifi_connected = false;


#if 0
static esp_err_t deinit() {
  printf("- Deinit all\n");
  ERET( esp_mqtt_client_stop(mqtt) );
  ERET( esp_mqtt_client_destroy(mqtt) );
  ERET( httpd_stop(httpd) );
  ERET( esp_wifi_deinit() );
  ERET( esp_vfs_spiffs_unregister(NULL) );
  ERET( nvs_flash_deinit() );
  ERET( i2c_driver_delete(i2c) );
  return ESP_OK;
}
#endif


static esp_err_t on_sht21(httpd_req_t *req) {
  char json[MAX_JSON_SIZE];
  ERET( sht21_json(i2c, json) );
  printf("@ SHT21: json=%s\n", json);
  ERET( httpd_resp_send_json(req, json) );
  return ESP_OK;
}


static esp_err_t on_wifi_get_config(httpd_req_t *req) {
  char json[MAX_JSON_SIZE];
  ERET( wifi_get_config_json(strstr(req->uri, "_ap")? WIFI_IF_AP : WIFI_IF_STA, json) );
  printf("@ WiFi get config %s: json=%s\n", req->uri, json);
  ERET( httpd_resp_send_json(req, json) );
  return ESP_OK;
}


static esp_err_t on_wifi_set_config(httpd_req_t *req) {
  char json[MAX_JSON_SIZE];
  httpd_req_recv(req, json, req->content_len);
  json[req->content_len] = '\0';
  ERET( wifi_set_config_json(strstr(req->uri, "_ap")? WIFI_IF_AP : WIFI_IF_STA, json) );
  printf("@ WiFi set config %s: json=%s\n", req->uri, json);
  ERET( httpd_resp_send_json(req, "1") );
  return ESP_OK;
}


static esp_err_t on_mqtt_get_config(httpd_req_t *req) {
  char json[MAX_JSON_SIZE];
  ERET( mqtt_get_config_json(json) );
  printf("@ MQTT get config: json=%s\n", json);
  ERET( httpd_resp_send_json(req, json) );
  return ESP_OK;
}

static esp_err_t on_mqtt_set_config(httpd_req_t *req) {
  char json[MAX_JSON_SIZE];
  httpd_req_recv(req, json, req->content_len);
  json[req->content_len] = '\0';
  printf("@ MQTT set config: json=%s\n", json);
  ERET( mqtt_set_config_json(mqtt, json) );
  ERET( httpd_resp_send_json(req, "1") );
  return ESP_OK;
}


static esp_err_t on_restart(httpd_req_t *req) {
  printf("@ Restart\n");
  esp_restart();
  return ESP_OK;
}


void on_mqtt(void *arg, esp_event_base_t base, int32_t id, void *data) {
  esp_mqtt_event_handle_t d = (esp_mqtt_event_handle_t) data;
  esp_mqtt_client_handle_t h = d->client;
  switch(d->event_id) {
    case MQTT_EVENT_CONNECTED:
    printf("@ Connected to MQTT broker\n");
    mqtt_connected = true;
    break;
    case MQTT_EVENT_DISCONNECTED:
    printf("@ Disconnected from MQTT broker\n");
    mqtt_connected = false;
    break;
    default:
    break;
  }
}


static void on_wifi(void *arg, esp_event_base_t base, int32_t id, void *data) {
  wifi_sta_config_t sta;
  wifi_ap_config_t ap;
  ERETV( esp_wifi_get_config(WIFI_IF_STA, (wifi_config_t*) &sta) );
  ERETV( esp_wifi_get_config(WIFI_IF_AP, (wifi_config_t*) &ap) );
  switch (id) {
    case WIFI_EVENT_STA_START:
    printf("@ WiFi station mode started\n");
    printf("- Connect to WiFi AP\n");
    printf(": ssid=%s, password=%s\n", sta.ssid, sta.password);
    ERETV( esp_wifi_connect() );
    break;
    case WIFI_EVENT_STA_CONNECTED:
    printf("@ Connected to WiFi AP '%s'\n", sta.ssid);
    wifi_connected = true;
    break;
    case WIFI_EVENT_STA_DISCONNECTED:
    printf("@ Disconnected from WiFi AP '%s'\n", sta.ssid);
    wifi_connected = false;
    break;
    case WIFI_EVENT_AP_START:
    printf("@ WiFi AP mode started\n");
    printf(": ssid=%s, password=%s\n", ap.ssid, ap.password);
    ERETV( httpd_init(&httpd) );
    ERETV( httpd_on(httpd, "/sht21", HTTP_GET, on_sht21) );
    ERETV( httpd_on(httpd, "/wifi_config_ap", HTTP_GET,  on_wifi_get_config) );
    ERETV( httpd_on(httpd, "/wifi_config_ap", HTTP_POST, on_wifi_set_config) );
    ERETV( httpd_on(httpd, "/wifi_config_sta", HTTP_GET,  on_wifi_get_config) );
    ERETV( httpd_on(httpd, "/wifi_config_sta", HTTP_POST, on_wifi_set_config) );
    ERETV( httpd_on(httpd, "/mqtt_config", HTTP_GET, on_mqtt_get_config) );
    ERETV( httpd_on(httpd, "/mqtt_config", HTTP_POST, on_mqtt_set_config) );
    ERETV( httpd_on(httpd, "/restart", HTTP_POST,  on_restart) );
    ERETV( httpd_on(httpd, "/*", HTTP_GET, httpd_on_static) );
    break;
    case WIFI_EVENT_AP_STACONNECTED: {
    wifi_event_ap_staconnected_t *d = (wifi_event_ap_staconnected_t*) data;
    printf("@ Station " MACSTR " joined, AID = %d (event)\n", MAC2STR(d->mac), d->aid);
    } break;
    case WIFI_EVENT_AP_STADISCONNECTED: {
    wifi_event_ap_stadisconnected_t *d = (wifi_event_ap_stadisconnected_t*) data;
    printf("@ Station " MACSTR " left, AID = %d (event)\n", MAC2STR(d->mac), d->aid);
    } break;
    default:
    printf("@ Other WiFi event: %d\n", id);
    break;
  }
}


static void on_ip(void *arg, esp_event_base_t base, int32_t id, void *data) {
  switch (id) {
    case IP_EVENT_STA_GOT_IP: {
    ip_event_got_ip_t *d = (ip_event_got_ip_t*) data;
    printf("@ WiFi station got IP: IP=%s\n", ip4addr_ntoa(&d->ip_info.ip));
    ( mqtt_init(&mqtt) );
    ( esp_mqtt_client_register_event(mqtt, ESP_EVENT_ANY_ID, on_mqtt, mqtt) );
    ( esp_mqtt_client_start(mqtt) );
    } break;
  }
}


void app_main() {
  chip_print_info();
  ERETV( nvs_init() );
  ERETV( spiffs_init() );
  ERETV( i2c_init(i2c, CONFIG_I2C_MASTER_SDA, CONFIG_I2C_MASTER_SCL, CONFIG_I2C_MASTER_CLK_SPEED) );
  tcpip_adapter_init();
  ERETV( esp_event_loop_create_default() );
  ERETV( efuse_get_mac_string(mac) );
  ERETV( wifi_init(mac) );
  esp_event_handler_register(WIFI_EVENT, ESP_EVENT_ANY_ID, on_wifi, NULL);
  esp_event_handler_register(IP_EVENT, IP_EVENT_STA_GOT_IP, on_ip, NULL);
  ERETV( esp_wifi_set_mode(WIFI_MODE_APSTA) );
  ERETV( esp_wifi_start() );
  TickType_t tpub = xTaskGetTickCount();
  while (true) {
    vTaskDelayUntil(& tpub, mqtt_interval() / portTICK_RATE_MS);
    if (!wifi_connected) {
      ERETV( esp_wifi_connect() );
      continue;
    } else if (!mqtt_connected) {
      ERETV( esp_mqtt_client_reconnect(mqtt) );
      continue;
    }
    char json[MAX_JSON_SIZE];
    ERETV( sht21_json(i2c, json) );
    printf("# Send to MQTT broker\n");
    printf(": topic=%s, json=%s\n", CONFIG_MQTT_PUBLISH_TOPIC, json);
    esp_mqtt_client_publish(mqtt, CONFIG_MQTT_PUBLISH_TOPIC, json, strlen(json)+1, 0, 0);
  }
}
