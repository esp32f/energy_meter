#pragma once


// Functions
#define ERET(cmd) \
  do { \
    esp_err_t ret = cmd; \
    if (ret != ESP_OK) return ret; \
  } while(0)
