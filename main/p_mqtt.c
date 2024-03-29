#include <nvs_flash.h>
#include <mqtt_client.h>
#include "p_device.h"
#include "p_json.h"
#include "p_mqtt.h"
#include "macros.h"


#define MQTT_BROKER_URL_KEY       "mqtt_uri"
#define MQTT_PUBLISH_INTERVAL_KEY "mqtt_interval"


static char uri[128];
static uint32_t interval = 0;


esp_err_t mqtt_get_config_json(char *buf) {
  size_t length = sizeof(uri);
  NVS_OPEN(nvs);
  ERET( nvs_get_str(nvs, MQTT_BROKER_URL_KEY, uri, &length) );
  ERET( nvs_get_u32(nvs, MQTT_PUBLISH_INTERVAL_KEY, &interval) );
  NVS_CLOSE(nvs);
  sprintf(buf, "{\"uri\": \"%s\", \"interval\": \"%d\"}", uri, interval);
  return ESP_OK;
}


esp_err_t mqtt_set_config_json(esp_mqtt_client_handle_t handle, const char *json) {
  json_string(json, "\"interval\":", uri);
  sscanf(uri, "%d", &interval);
  json_string(json, "\"uri\":", uri);
  printf("- MQTT set config: uri=%s, interval=%d\n", uri, interval);
  ERET( esp_mqtt_client_set_uri(handle, uri) );
  NVS_OPENW(nvs);
  ERET( nvs_set_u32(nvs, MQTT_PUBLISH_INTERVAL_KEY, interval) );
  ERET( nvs_set_str(nvs, MQTT_BROKER_URL_KEY, uri) );
  NVS_CLOSEW(nvs);
  return ESP_OK;
}


esp_err_t mqtt_init(esp_mqtt_client_handle_t *handle) {
  printf("# Init MQTT client\n");
  size_t length = sizeof(uri);
  esp_err_t ret;
  NVS_OPENW(nvs);
  ret = nvs_get_str(nvs, MQTT_BROKER_URL_KEY, uri, &length);
  if (ret != ESP_OK) {
    strcpy(uri, CONFIG_MQTT_BROKER_URL);
    ERET( nvs_set_str(nvs, MQTT_BROKER_URL_KEY, uri) );
  }
  ret = nvs_get_u32(nvs, MQTT_PUBLISH_INTERVAL_KEY, &interval);
  if (ret != ESP_OK) {
    interval = CONFIG_MQTT_PUBLISH_INTERVAL;
    ERET( nvs_set_u32(nvs, MQTT_PUBLISH_INTERVAL_KEY, interval) );
  }
  NVS_CLOSEW(nvs);
  printf(": uri=%s, interval=%d\n", uri, interval);
  esp_mqtt_client_config_t c = {
    .uri = uri
  };
  *handle = esp_mqtt_client_init(&c);
  if (*handle == NULL) {
    printf("* MQTT init failed!\n");
    return ESP_FAIL;
  }
  return ESP_OK;
}


uint32_t mqtt_interval() {
  return interval;
}
