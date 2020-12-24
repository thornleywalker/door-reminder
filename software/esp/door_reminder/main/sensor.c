#include "sensor.h"

#include "database_proxy.h"
#include "driver/gpio.h"
#include "esp_log.h"

static const char *TAG = "dr_sensor";

#define GOING_INPUT GPIO_NUM_22
#define COMING_INPUT GPIO_NUM_2
#define INPUT_PIN_SELECT ((1ULL << GOING_INPUT) | (1ULL << COMING_INPUT))
#define ESP_INTR_FLAG_DEFAULT 0

void going_isr() {
  // ESP_LOGI(TAG, "interrupt detected on 22\n");
  database_alert_users(ALERT_DIR_GOING);
  // ESP_LOGI(TAG, "interrupt executed\n");
}

void coming_isr() {
  // ESP_LOGI(TAG, "interrupt detected on 2\n");
  database_alert_users(ALERT_DIR_COMING);
  // ESP_LOGI(TAG, "interrupt executed\n");
}

// sets up pin for input, registers isr
esp_err_t sensor_init() {
  ESP_LOGI(TAG, "initializing sensor");

  gpio_config_t cnfg = {
      .intr_type = GPIO_INTR_POSEDGE,
      .mode = GPIO_MODE_INPUT,
      .pin_bit_mask = INPUT_PIN_SELECT,
      .pull_down_en = GPIO_PULLDOWN_ENABLE,
      .pull_up_en = GPIO_PULLUP_DISABLE,
  };

  esp_err_t err;

  // config pin for input
  err = gpio_config(&cnfg);
  if (err != ESP_OK)
    ESP_LOGI(TAG, "error in config: %s\n", esp_err_to_name(err));

  // install isr
  err = gpio_install_isr_service(ESP_INTR_FLAG_DEFAULT);
  if (err != ESP_OK)
    ESP_LOGI(TAG, "error in config: %s\n", esp_err_to_name(err));

  err = gpio_isr_handler_add(GOING_INPUT, going_isr, NULL);
  err = gpio_isr_handler_add(COMING_INPUT, coming_isr, NULL);

  if (err != ESP_OK)
    ESP_LOGI(TAG, "error in config: %s\n", esp_err_to_name(err));

  ESP_LOGI(TAG, "sensor initialized");
  return err;
}