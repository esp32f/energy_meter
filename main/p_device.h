#pragma once
#include <driver/gpio.h>
#include <driver/i2c.h>


#define NVS_NAMESPACE "storage"


// e.g. NVS_READ(nvs, nvs_get_u32(nvs, "key", &value))
#define NVS_READ(handle, x) do { \
    nvs_handle_t (handle); \
    ERET( nvs_open(NVS_NAMESPACE, NVS_READONLY, &(handle)) ); \
    (x); \
    nvs_close((handle)); \
  } while (0)


// e.g. NVS_WRITE(nvs, nvs_set_u32(nvs, "key", value))
#define NVS_WRITE(handle, x) do { \
    nvs_handle_t (handle); \
    ERET( nvs_open(NVS_NAMESPACE, NVS_READWRITE, &(handle)) ); \
    (x); \
    ERET( nvs_commit((handle)) ); \
    nvs_close((handle)); \
  } while (0)


esp_err_t device_mac(const char *prefix, char *buff);
esp_err_t i2c_init(i2c_port_t port, gpio_num_t sda, gpio_num_t scl, uint32_t clk_speed);
esp_err_t nvs_init();
esp_err_t spiffs_init();
