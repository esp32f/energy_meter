#pragma once
#include <esp_event.h>


esp_err_t wifi_config_sta_json(char *buff);
esp_err_t wifi_set_config_sta_json(const char *json);
void wifi_on_sta(void *arg, esp_event_base_t base, int32_t id, void *data);
esp_err_t wifi_start_apsta();
esp_err_t wifi_start_sta();
esp_err_t wifi_init();
