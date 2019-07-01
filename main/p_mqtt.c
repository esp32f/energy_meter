#include <nvs_flash.h>
#include <mqtt_client.h>
#include "p_device.h"
#include "p_json.h"
#include "p_mqtt.h"
#include "macros.h"


#define MQTT_URI_KEY      "mqtt_uri"
#define MQTT_INTERVAL_KEY "mqtt_int"


static char uri[128];
static uint32_t interval = 0;


esp_err_t mqtt_get_config_json(char *buf) {
  size_t length;
  NVS_READ(nvs, nvs_get_str(nvs, MQTT_URI_KEY, uri, &length));
  NVS_READ(nvs, nvs_get_u32(nvs, MQTT_INTERVAL_KEY, &interval));
  sprintf(buf, "{\"uri\": \"%s\", \"interval\": \"%d\"}", uri, interval);
  return ESP_OK;
}


esp_err_t mqtt_set_config_json(esp_mqtt_client_handle_t handle, const char *json) {
  json_string(json, "\"interval\":", uri);
  sscanf(uri, "%d", &interval);
  json_string(json, "\"uri\":", uri);
  printf("@ MQTT set config: uri=%s, interval=%d\n", uri, interval);
  ERET( esp_mqtt_client_set_uri(handle, uri) );
  NVS_WRITE(nvs, nvs_set_u32(nvs, MQTT_INTERVAL_KEY, interval));
  NVS_WRITE(nvs, nvs_set_str(nvs, MQTT_URI_KEY, uri));
  return ESP_OK;
}


esp_err_t mqtt_init(esp_mqtt_client_handle_t *handle) {
  printf("# Init MQTT client\n");
  size_t length = sizeof(uri);
  NVS_READ(nvs, nvs_get_str(nvs, MQTT_URI_KEY, uri, &length));
  if (strlen(uri) == 0) {
    strcpy(uri, CONFIG_MQTT_BROKER_URL);
    NVS_WRITE(nvs, nvs_set_str(nvs, MQTT_URI_KEY, uri));
  }
  NVS_READ(nvs, nvs_get_u32(nvs, MQTT_INTERVAL_KEY, &interval));
  if (interval == 0) {
    interval = CONFIG_MQTT_PUBLISH_INTERVAL;
    NVS_WRITE(nvs, nvs_set_u32(nvs, MQTT_INTERVAL_KEY, interval));
  }
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
