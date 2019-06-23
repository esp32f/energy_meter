#pragma once
#include <driver/gpio.h>
#include <driver/spi_master.h>


// Functions
esp_err_t spi_init(
  spi_host_device_t host, gpio_num_t miso, gpio_num_t mosi, gpio_num_t sclk,
  gpio_num_t cs, int clk_speed, spi_device_handle_t *handle);
esp_err_t spi_write(spi_device_handle_t handle, uint8_t *data, size_t size);
esp_err_t spi_read(spi_device_handle_t handle, uint8_t *buff, size_t size);
