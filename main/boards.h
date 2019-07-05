#pragma once
#include <driver/gpio.h>
#include <driver/i2c.h>


// ESP32 DevKit v4
#ifndef CONFIG_I2C_MASTER_PORT
#define CONFIG_I2C_MASTER_PORT I2C_NUM_0
#define CONFIG_I2C_MASTER_SDA  18
#define CONFIG_I2C_MASTER_SCL  19
#define CONFIG_I2C_MASTER_CLK_SPEED 10000
#endif
