#pragma once
#include <driver/gpio.h>
#include <driver/i2c.h>


#define NVS_NAMESPACE "storage"


#define NVS_OPEN(h) \
  nvs_handle_t (h); \
  ERET( nvs_open(NVS_NAMESPACE, NVS_READONLY, &(h)) )

#define NVS_OPENW(h)  \
  nvs_handle_t (h); \
  ERET( nvs_open(NVS_NAMESPACE, NVS_READWRITE, &(h)) )


#define NVS_CLOSE(h)  \
  nvs_close((h))

#define NVS_CLOSEW(h) \
  ERET( nvs_commit((h)) );  \
  nvs_close((h))


esp_err_t efuse_get_mac_string(char *buf);
esp_err_t i2c_init(i2c_port_t port, gpio_num_t sda, gpio_num_t scl, uint32_t clk_speed);
esp_err_t nvs_init();
esp_err_t spiffs_init();
