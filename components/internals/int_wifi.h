#pragma once
#include <stdint.h>
#include <sdkconfig.h>
#include <esp_err.h>


// Functions
esp_err_t wifi_init(uint8_t *ssid, uint8_t *pass);
