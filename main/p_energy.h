#pragma once
#include <esp_err.h>


esp_err_t energy_begin();
esp_err_t energy_end();
esp_err_t energy_inow();
esp_err_t energy_vnow();
esp_err_t energy_pnow();
esp_err_t energy_irms();
esp_err_t energy_vrms();
esp_err_t energy_preal();
esp_err_t energy_papparent();
esp_err_t energy_preactive();
esp_err_t energy_pfundamental();
esp_err_t energy_pharmonic();
esp_err_t energy_pf();
esp_err_t energy_freq();
