#include <stdio.h>
#include <string.h>
#include "esp_system.h"
#include "esp_log.h"
#include "esp_console.h"
#include "esp_vfs_dev.h"
#include "driver/uart.h"
#include "linenoise/linenoise.h"
#include "argtable3/argtable3.h"
#include "cmd_main.h"
#include "esp_vfs_fat.h"
#include "nvs.h"
#include "nvs_flash.h"
#include "int_nvs.h"
#include "macros.h"


static const char *TAG = "esp32";


void app_main() {
  tcpip_adapter_init();
  ERET( esp_event_loop_create_default() );
  ERET( nvs_init() );
  setup_cmd();

  // Prompt to be printed before each line.
  // This can be customized, made dynamic, etc.
  const char* prompt = LOG_COLOR_I "esp32> " LOG_RESET_COLOR;

  printf("\n"
          "This is an example of ESP-IDF console component.\n"
          "Type 'help' to get the list of commands.\n"
          "Use UP/DOWN arrows to navigate through command history.\n"
          "Press TAB when typing command name to auto-complete.\n");
  // Figure out if the terminal supports escape sequences
  if (0) {
#if CONFIG_LOG_COLORS
    // Since the terminal doesn't support escape sequences,
    // don't use color codes in the prompt.
    prompt = "esp32> ";
#endif //CONFIG_LOG_COLORS
  }

  // Main loop
  while(true) {
    // Get a line using linenoise.
    // The line is returned when ENTER is pressed.
    char* line = linenoise(prompt);
    if (line == NULL) { // Ignore empty lines
        continue;
    }
    // Add the command to the history
    linenoiseHistoryAdd(line);
#if CONFIG_STORE_HISTORY
    // Save command history to filesystem
    linenoiseHistorySave(HISTORY_PATH);
#endif

    // Try to run the command
    int ret;
    esp_err_t err = esp_console_run(line, &ret);
    if (err == ESP_ERR_NOT_FOUND) {
        printf("Unrecognized command\n");
    } else if (err == ESP_ERR_INVALID_ARG) {
        // command was empty
    } else if (err == ESP_OK && ret != ESP_OK) {
        printf("Command returned non-zero error code: 0x%x (%s)\n", ret, esp_err_to_name(err));
    } else if (err != ESP_OK) {
        printf("Internal error: %s\n", esp_err_to_name(err));
    }
    // linenoise allocates line buffer on the heap, so need to free it
    linenoiseFree(line);
  }
}
