#pragma once
#include <mqtt_client.h>


esp_err_t mqtt_config_json(char *buff);
esp_err_t mqtt_set_config_json(esp_mqtt_client_handle_t handle, const char *json);
esp_err_t mqtt_init(esp_mqtt_client_handle_t *handle);
esp_err_t mqtt_deinit(esp_mqtt_client_handle_t handle);
void mqtt_on_default(void *arg, esp_event_base_t base, int32_t id, void *data);
uint32_t mqtt_interval();
