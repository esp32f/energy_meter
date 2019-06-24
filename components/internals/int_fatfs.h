#pragma once
#include <esp_err.h>


// Functions
esp_err_t fatfs_init(const char *base, const char *partition);
