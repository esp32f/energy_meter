#include "macros.h"
#include "int_uart.h"


esp_err_t uart_init(uart_port_t port, int baud_rate) {
  // Configure UART. Note that REF_TICK is used so that the baud rate remains
  // correct while APB frequency is changing in light sleep mode.
  const uart_config_t config = {
    .baud_rate = baud_rate,
    .data_bits = UART_DATA_8_BITS,
    .parity = UART_PARITY_DISABLE,
    .stop_bits = UART_STOP_BITS_1,
    .use_ref_tick = true
  };
  ERET( uart_param_config(port, &config) );
  // Install UART driver for interrupt-driven reads and writes
  return uart_driver_install(port, 256, 0, 0, NULL, 0);
}
