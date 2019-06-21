// Declarations of command registration functions.
#pragma once
#ifdef __cplusplus
extern "C" {
#endif

#include "esp_system.h"
#include "esp_log.h"
#include "esp_console.h"
#include "esp_vfs_dev.h"
#include "esp_vfs_fat.h"
#include "driver/uart.h"
#include "linenoise/linenoise.h"
#include "argtable3/argtable3.h"

#include "cmd_system.h"
#include "cmd_wifi.h"
#include "cmd_nvs.h"


#ifndef TAG
#define TAG "energy"
#endif
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

static void setup_fs() {
  static wl_handle_t handle;
  const esp_vfs_fat_mount_config_t config = {
    .max_files = 4,
    .format_if_mount_failed = true
  };
  esp_err_t err = esp_vfs_fat_spiflash_mount(MOUNT_PATH, "storage", &config, &handle);
  if (err != ESP_OK) {
    ESP_LOGE(TAG, "Failed to mount FATFS (%s)", esp_err_to_name(err));
    return;
  }
}
#endif // CONFIG_STORE_HISTORY


static void setup_uart() {
  // Configure UART. Note that REF_TICK is used so that the baud rate remains
  // correct while APB frequency is changing in light sleep mode.
  const uart_port_t port = (uart_port_t) CONFIG_ESP_CONSOLE_UART_NUM;
  const uart_config_t config = {
    .baud_rate = CONFIG_ESP_CONSOLE_UART_BAUDRATE,
    .data_bits = UART_DATA_8_BITS,
    .parity = UART_PARITY_DISABLE,
    .stop_bits = UART_STOP_BITS_1,
    .use_ref_tick = true
  };
  ESP_ERROR_CHECK( uart_param_config(port, &config) );
  // Install UART driver for interrupt-driven reads and writes
  ESP_ERROR_CHECK( uart_driver_install(port, 256, 0, 0, NULL, 0) );
}

static void setup_vfs() {
  // Minicom, screen, idf_monitor send CR when ENTER key is pressed
  esp_vfs_dev_uart_set_rx_line_endings(ESP_LINE_ENDINGS_CR);
  // Move the caret to the beginning of the next line on '\n'
  esp_vfs_dev_uart_set_tx_line_endings(ESP_LINE_ENDINGS_CRLF);
  // Tell VFS to use UART driver
  setup_uart();
  esp_vfs_dev_uart_use_driver(CONFIG_ESP_CONSOLE_UART_NUM);
}

static void setup_console() {
  // Initialize the console
  esp_console_config_t config = {
    .max_cmdline_args = 8,
    .max_cmdline_length = 256,
#if CONFIG_LOG_COLORS
    .hint_color = atoi(LOG_COLOR_CYAN)
#endif
  };
  ESP_ERROR_CHECK( esp_console_init(&config) );
}

static void setup_linenoise() {
  // Configure linenoise line completion library
  // Enable multiline editing. If not set, long commands will scroll within single line.
  linenoiseSetMultiLine(1);
  // Tell linenoise where to get command completions and hints
  linenoiseSetCompletionCallback(&esp_console_get_completion);
  linenoiseSetHintsCallback((linenoiseHintsCallback*) &esp_console_get_hint);
  // Set command history size
  linenoiseHistorySetMaxLen(100);
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

static void setup_cmd() {
  // Disable buffering on stdin
  setvbuf(stdin, NULL, _IONBF, 0);
  // Setup tools
#if CONFIG_STORE_HISTORY
  setup_fs();
#endif
  setup_vfs();
  setup_console();
  setup_linenoise();
  // Register commands
  esp_console_register_help_command();
  register_system();
  register_wifi();
  register_nvs();
}


#ifdef __cplusplus
}
#endif
