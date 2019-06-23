#pragma once
#include "main_i2c.h"


// I2C slave address
#define SHT21_ADDR 0x40
// Register map
#define SHT21_CMD_TEMP_WAIT     0xE3
#define SHT21_CMD_TEMP_NO_WAIT  0xF3
#define SHT21_CMD_RH_WAIT       0xE5
#define SHT21_CMD_RH_NO_WAIT    0xF5


// Functions
esp_err_t sht21_rh(i2c_port_t port, float *ans);
esp_err_t sht21_temp(i2c_port_t port, float *ans);
