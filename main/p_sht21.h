#pragma once
#include <driver/gpio.h>
#include <driver/i2c.h>


// I2C slave address
#define SHT21_ADDR 0x40
// Register map
#define SHT21_CMD_TEMP_NO_HOLD        0xF3
#define SHT21_CMD_RH_NO_HOLD          0xF5
#define SHT21_CMD_WRITE_USER_REGISTER 0xE6
#define SHT21_CMD_READ_USER_REGISTER  0xE7


esp_err_t sht21_register(i2c_port_t port, uint8_t *ans);
esp_err_t sht21_cmd_bytes(i2c_port_t port, uint8_t cmd, uint8_t *buff);
esp_err_t sht21_rh(i2c_port_t port, float *ans);
esp_err_t sht21_temp(i2c_port_t port, float *ans);
esp_err_t sht21_json(i2c_port_t port, char *buff);
