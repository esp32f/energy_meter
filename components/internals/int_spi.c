#include <stdint.h>
#include <sdkconfig.h>
#include "int_spi.h"


esp_err_t spi_init(
  spi_host_device_t host, gpio_num_t miso, gpio_num_t mosi, gpio_num_t sclk,
  gpio_num_t cs, int clk_speed, spi_device_handle_t *handle) {
  spi_bus_config_t bconfig = {
    .miso_io_num = miso,
    .mosi_io_num = mosi,
    .sclk_io_num = sclk,
    .quadwp_io_num = -1,
    .quadhd_io_num = -1,
    .max_transfer_sz = 256
  };
  spi_device_interface_config_t dconfig = {
    .clock_speed_hz = clk_speed,
    .mode = 0,
    .spics_io_num = cs,
    .queue_size = 0,
    .pre_cb = NULL
  };
  esp_err_t ret = spi_bus_initialize(host, &bconfig, 0);
  if (ret != ESP_OK) return ret;
  return spi_bus_add_device(host, &dconfig, handle);
}


esp_err_t spi_write(spi_device_handle_t handle, uint8_t *data, size_t size) {
  spi_transaction_t trans;
  if (size == 0) return ESP_OK;
  memset(&trans, 0);
  trans.tx_buffer = data;
  trans.length = size;
  return spi_device_polling_transmit(handle, &trans);
}


esp_err_t spi_read(spi_device_handle_t handle, uint8_t *buff, size_t size) {
  spi_transaction_t trans;
  if (size == 0) return ESP_OK;
  memset(&trans, 0);
  trans.rx_buffer = buff;
  trans.length = size;
  return spi_device_polling_transmit(handle, &trans);
}
