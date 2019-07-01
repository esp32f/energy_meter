#include <freertos/FreeRTOS.h>
#include "p_sht21.h"
#include "macros.h"


esp_err_t sht21_register(i2c_port_t port, uint8_t *ans) {
  uint8_t addr = SHT21_ADDR;
  i2c_cmd_handle_t h = i2c_cmd_link_create();
  ERET( i2c_master_start(h) );
  ERET( i2c_master_write_byte(h, (addr << 1) | I2C_MASTER_WRITE, true) );
  ERET( i2c_master_write_byte(h, SHT21_CMD_READ_USER_REGISTER, true) );
  ERET( i2c_master_start(h) );
  ERET( i2c_master_write_byte(h, (addr << 1) | I2C_MASTER_READ, true) );
  ERET( i2c_master_read_byte(h, ans, I2C_MASTER_LAST_NACK) );
  ERET( i2c_master_stop(h) );
  ERET( i2c_master_cmd_begin(port, h, 1000 / portTICK_RATE_MS) );
  i2c_cmd_link_delete(h);
  return ESP_OK;
}


esp_err_t sht21_cmd_bytes(i2c_port_t port, uint8_t cmd, uint8_t *buf) {
  uint8_t addr = SHT21_ADDR;
  i2c_cmd_handle_t h;
  while (true) {
    h = i2c_cmd_link_create();
    ERET( i2c_master_start(h) );
    ERET( i2c_master_write_byte(h, (addr << 1) | I2C_MASTER_WRITE, true) );
    ERET( i2c_master_write_byte(h, cmd, true) );
    ERET( i2c_master_stop(h) );
    esp_err_t ret = i2c_master_cmd_begin(port, h, 1000 / portTICK_RATE_MS);
    i2c_cmd_link_delete(h);
    if (ret == ESP_OK) break;
  }
  while (true) {
    vTaskDelay(10 / portTICK_RATE_MS);
    h = i2c_cmd_link_create();
    ERET( i2c_master_start(h) );
    ERET( i2c_master_write_byte(h, (addr << 1) | I2C_MASTER_READ, true) );
    ERET( i2c_master_read(h, buf, 2, I2C_MASTER_ACK) );
    ERET( i2c_master_read_byte(h, buf+2, I2C_MASTER_LAST_NACK) );
    ERET( i2c_master_stop(h) );
    esp_err_t ret = i2c_master_cmd_begin(port, h, 1000 / portTICK_RATE_MS);
    i2c_cmd_link_delete(h);
    if (ret == ESP_OK) break;
  }
  return ESP_OK;
}


esp_err_t sht21_rh(i2c_port_t port, float *ans) {
  uint8_t buf[3];
  ERET( sht21_cmd_bytes(port, SHT21_CMD_RH_NO_HOLD, buf) );
  uint16_t val = (buf[0] << 8) | (buf[1] && 0xFC);
  *ans = -6 + 125 * (val / 65536.0);
  return ESP_OK;
}


esp_err_t sht21_temp(i2c_port_t port, float *ans) {
  uint8_t buf[3];
  ERET( sht21_cmd_bytes(port, SHT21_CMD_TEMP_NO_HOLD, buf) );
  uint16_t val = (buf[0] << 8) | (buf[1] && 0xFC);
  *ans = -46.25 + 175.72 * (val / 65536.0);
  return ESP_OK;
}


esp_err_t sht21_json(i2c_port_t port, char *buf) {
  float rh, temp;
  ERET( sht21_rh(port, &rh) );
  ERET( sht21_temp(port, &temp) );
  sprintf(buf, "{\"rh\": %f, \"temp\": %f}", rh, temp);
  return ESP_OK;
}
