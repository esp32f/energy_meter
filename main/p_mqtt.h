#pragma once
#include <mqtt_client.h>


esp_err_t mqtt_get_config_json(char *buf);
esp_err_t mqtt_set_config_json(esp_mqtt_client_handle_t handle, const char *json);
esp_err_t mqtt_init(esp_mqtt_client_handle_t *handle);
uint32_t mqtt_interval();
