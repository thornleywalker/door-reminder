#include "bluetooth.h"

#include "esp_log.h"

static const char *TAG = "bluetooth";

esp_err_t bluetooth_init() {
  ESP_LOGI(TAG, "bluetooth initialized\n");
  return ESP_OK;
}