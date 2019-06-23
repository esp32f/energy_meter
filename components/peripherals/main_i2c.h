#pragma once
#include <driver/i2c.h>


// Functions
esp_err_t i2c_init(i2c_port_t port, gpio_num_t sda, gpio_num_t scl, uint32_t clk_speed);
esp_err_t i2c_read(i2c_port_t port, uint8_t addr, uint8_t *buff, size_t size);
esp_err_t i2c_write(i2c_port_t port, uint8_t addr, uint8_t *data, size_t size);

