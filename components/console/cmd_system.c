#include <stdio.h>
#include <sdkconfig.h>
#include <esp_log.h>
#include <esp_system.h>
#include <esp_console.h>
#include <esp_spi_flash.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include "cmd_system.h"


#ifdef CONFIG_FREERTOS_USE_STATS_FORMATTING_FUNCTIONS
#define WITH_TASKS_INFO 1
#endif


static const char *TAG = "console";


esp_err_t cmd_system_register() {
  ERET( register_heap() );
  ERET( register_version() );
  ERET( register_restart() );
#if WITH_TASKS_INFO
  ERET( register_tasks() );
#endif
  return ESP_OK;
}


static int cmd_version(int argc, char **argv) {
  esp_chip_info_t info;
  esp_chip_info(&info);
  uint8_t mac[6];
  esp_efuse_mac_get_default(mac);
  printf("%s%s%s%s, %d cores, %d MB %s flash\r\n",
    info.model == CHIP_ESP32? "ESP32":"?",
    info.features & CHIP_FEATURE_WIFI_BGN? "/802.11bgn":"",
    info.features & CHIP_FEATURE_BT? "/BT":"",
    info.features & CHIP_FEATURE_BLE? "/BLE":"",
    info.cores, spi_flash_get_chip_size() / (1024 * 1024),
    info.features & CHIP_FEATURE_EMB_FLASH ? "embedded":"external"
  );
  printf("MAC: %02X:%02X:%02X:%02X:%02X:%02X\n",
    mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]
  );
  printf("Revision: %d, IDF: %s\r\n", info.revision, esp_get_idf_version());
  return 0;
}


static esp_err_t register_version() {
  const esp_console_cmd_t cmd = {
    .command = "version",
    .help = "Get version of Chip and SDK",
    .hint = NULL,
    .func = &cmd_version,
  };
  return esp_console_cmd_register(&cmd);
}


static int cmd_restart(int argc, char **argv) {
  ESP_LOGI(TAG, "Restarting ...");
  esp_restart();
}


static esp_err_t register_restart() {
  const esp_console_cmd_t cmd = {
    .command = "restart",
    .help = "Software reset of the chip",
    .hint = NULL,
    .func = &cmd_restart,
  };
  return esp_console_cmd_register(&cmd);
}


static int cmd_heap(int argc, char **argv) {
  printf("Heap: %dK free of %dK\r\n",
    esp_get_free_heap_size() / 1024,
    heap_caps_get_minimum_free_size(MALLOC_CAP_DEFAULT) / 1024
  );
  return 0;
}


static esp_err_t register_heap() {
  const esp_console_cmd_t cmd = {
    .command = "heap",
    .help = "Get heap memory status",
    .hint = NULL,
    .func = &cmd_heap,
  };
  return esp_console_cmd_register(&cmd_heap);
}


#if WITH_TASKS_INFO
static int cmd_tasks(int argc, char **argv) {
  const size_t bytes_per_task = 40; /* see vTaskList description */
  char *buff = malloc(uxTaskGetNumberOfTasks() * bytes_per_task);
  if (buff == NULL) {
    ESP_LOGE(TAG, "Failed to allocate buffer for vTaskList()");
    return -1;
  }
  printf("Task Name\tStatus\tPrio\tHWM\tTask#");
#ifdef CONFIG_FREERTOS_VTASKLIST_INCLUDE_COREID
  printf("\tAffinity");
#endif
  printf("\n");
  vTaskList(buff);
  printf(buff);
  free(buff);
  return 0;
}

static esp_err_t register_tasks() {
  const esp_console_cmd_t cmd = {
    .command = "tasks",
    .help = "Get information about running tasks",
    .hint = NULL,
    .func = &cmd_tasks,
  };
  return esp_console_cmd_register(&cmd);
}
#endif // WITH_TASKS_INFO
