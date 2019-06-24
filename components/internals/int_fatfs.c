#include <esp_log.h>
#include <esp_vfs_fat.h>
#include "int_fatfs.h"


static const char *TAG = "fatfs";


esp_err_t fatfs_init(const char *base, const char *partition) {
  static wl_handle_t handle;
  const esp_vfs_fat_mount_config_t config = {
    .max_files = 4,
    .format_if_mount_failed = true
  };
  return esp_vfs_fat_spiflash_mount(base, partition, &config, &handle);
}
