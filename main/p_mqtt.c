#include <nvs_flash.h>
#include <mqtt_client.h>
#include "p_device.h"
#include "p_json.h"
#include "p_mqtt.h"
#include "macros.h"


#define MQTT_URI_KEY          "mqtt_uri"
#define MQTT_URI_DEFAULT      "mqtt://10.2.28.74:1883"
#define MQTT_INTERVAL_KEY     "mqtt_int"
#define MQTT_INTERVAL_DEFAULT 10000


static char uri[128];
static uint32_t interval = MQTT_INTERVAL_DEFAULT;


esp_err_t mqtt_config_json(char *buff) {
  sprintf(buff, "{\"uri\": \"%s\", \"interval\": \"%d\"}", uri, interval);
  return ESP_OK;
}


esp_err_t mqtt_set_config_json(esp_mqtt_client_handle_t handle, const char *json) {
  json_string(json, "\"interval\":", uri);
  sscanf(uri, "%d", &interval);
  json_string(json, "\"uri\":", uri);
  printf("- MQTT set config: uri=%s, interval=%d\n", uri, interval);
  ERET( esp_mqtt_client_set_uri(handle, uri) );
  NVS_WRITE(nvs, nvs_set_u32(nvs, MQTT_INTERVAL_KEY, interval));
  NVS_WRITE(nvs, nvs_set_str(nvs, MQTT_URI_KEY, uri));
  return ESP_OK;
}


esp_err_t mqtt_init(esp_mqtt_client_handle_t *handle) {
  size_t length = sizeof(uri);
  NVS_READ(nvs, nvs_get_str(nvs, MQTT_URI_KEY, uri, &length));
  if (strstr(uri, "mqtt://") != uri) strcpy(uri, MQTT_URI_DEFAULT);
  NVS_READ(nvs, nvs_get_u32(nvs, MQTT_INTERVAL_KEY, &interval));
  printf("# Init MQTT client\n");
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


esp_err_t mqtt_deinit(esp_mqtt_client_handle_t handle) {
  ERET( esp_mqtt_client_stop(handle) );
  ERET( esp_mqtt_client_destroy(handle) );
  return ESP_OK;
}
