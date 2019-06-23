#include "main_sht21.h"



esp_err_t sht21_rh_int(i2c_port_t port, uint16_t *ans) {
  uint8_t addr = SHT21_ADDR;
  // write RH wait command
  uint8_t buff[3] = {SHT21_CMD_RH_WAIT};
  esp_err_t ret = i2c_write(port, addr, buff, 1);
  if (ret != ESP_OK) return ret;
  // read RH data + checksum byte
  ret = i2c_read(port, addr, buff, sizeof(buff));
  if (ret != ESP_OK) return ret;
  *ans = (((uint16_t) buff[0]) << 8) | buff[1];
  return ESP_OK;
}


esp_err_t sht21_temp_int(i2c_port_t port, uint16_t *ans) {
  uint8_t addr = SHT21_ADDR;
  // write temp wait command
  uint8_t buff[3] = {SHT21_CMD_TEMP_WAIT};
  esp_err_t ret = i2c_write(port, addr, buff, 1);
  // read temp data + checksum byte
  ret = i2c_read(port, addr, buff, 3);
  if (ret != ESP_OK) return ret;
  *ans = (((uint16_t) buff[0])<<8) | buff[1];
  return ESP_OK;
}


esp_err_t sht21_rh(i2c_port_t port, float *ans) {
    uint16_t intv;
    esp_err_t ret = sht21_rh_int(port, &intv);
    *ans = (0.0019073486328125 * (intv & 0xFFFC) ) - 6; 
    return ret;
}


esp_err_t sht21_temp(i2c_port_t port, float *ans) {
  uint16_t intv;
  esp_err_t ret = sht21_temp_int(port, &intv);
  *ans = (0.0026812744140625* (intv & 0xFFFC)) - 46.85;
  return ret;
}
