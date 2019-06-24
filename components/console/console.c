#include "esp_log.h"
#include "esp_system.h"
#include "esp_console.h"
#include "esp_vfs_dev.h"
#include "linenoise/linenoise.h"
#include "cmd_system.h"
#include "cmd_wifi.h"
#include "int_uart.h"
#include "int_fatfs.h"
#include "macros.h"
#include "console.h"


#ifndef CONFIG_ESP_CONSOLE_UART_NUM
#define CONFIG_ESP_CONSOLE_UART_NUM 0
#endif
#ifndef CONFIG_ESP_CONSOLE_UART_BAUDRATE
#define CONFIG_ESP_CONSOLE_UART_BAUDRATE 115200
#endif


#if CONFIG_STORE_HISTORY
// Console command history can be stored to and loaded from a file.
// The easiest way to do this is to use FATFS filesystem on top of
// wear_levelling library.
#define MOUNT_PATH    "/data"
#define HISTORY_PATH  MOUNT_PATH "/history.txt"
#endif // CONFIG_STORE_HISTORY


static esp_err_t init_vfs(uart_port_t port, int baud_rate) {
  // Minicom, screen, idf_monitor send CR when ENTER key is pressed
  esp_vfs_dev_uart_set_rx_line_endings(ESP_LINE_ENDINGS_CR);
  // Move the caret to the beginning of the next line on '\n'
  esp_vfs_dev_uart_set_tx_line_endings(ESP_LINE_ENDINGS_CRLF);
  // Tell VFS to use UART driver
  ERET( uart_init(port, baud_rate) );
  esp_vfs_dev_uart_use_driver(port);
  return ESP_OK;
}


static esp_err_t init_console() {
  // Initialize the console
  esp_console_config_t config = {
    .max_cmdline_args = 8,
    .max_cmdline_length = 256,
#if CONFIG_LOG_COLORS
    .hint_color = atoi(LOG_COLOR_CYAN)
#endif
  };
  return esp_console_init(&config);
}


static void init_linenoise() {
  // Configure linenoise line completion library
  // Enable multiline editing. If not set, long commands will scroll within single line.
  linenoiseSetMultiLine(1);
  // Tell linenoise where to get command completions and hints
  linenoiseSetCompletionCallback(&esp_console_get_completion);
  linenoiseSetHintsCallback((linenoiseHintsCallback*) &esp_console_get_hint);
  // Set command history size
  linenoiseHistorySetMaxLen(64);
#if CONFIG_STORE_HISTORY
  // Load command history from filesystem
  linenoiseHistoryLoad(HISTORY_PATH);
#endif
  // Figure out if the terminal supports escape sequences
  int probe_status = linenoiseProbe();
  if (probe_status) { // zero indicates success
    printf("\n"
            "Your terminal application does not support escape sequences.\n"
            "Line editing and history features are disabled.\n"
            "On Windows, try using Putty instead.\n");
    linenoiseSetDumbMode(1);
  }
}

esp_err_t console_init() {
  // Disable buffering on stdin
  setvbuf(stdin, NULL, _IONBF, 0);
  // Initialize parts
#if CONFIG_STORE_HISTORY
  ERET( fatfs_init(MOUNT_PATH, "storage") );
#endif
  ERET( init_vfs(CONFIG_ESP_CONSOLE_UART_NUM, CONFIG_ESP_CONSOLE_UART_BAUDRATE) );
  ERET( init_console() );
  init_linenoise();
  // Register commands
  ERET( esp_console_register_help_command() );
  register_system();
  register_wifi();
  register_nvs();
}
