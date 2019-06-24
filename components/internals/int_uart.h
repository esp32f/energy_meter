#pragma once
#include <driver/uart.h>


// Functions
esp_err_t uart_init(uart_port_t port, int baud_rate);
