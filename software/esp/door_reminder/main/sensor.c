#include "sensor.h"

#include "driver/gpio.h"
#include "esp_log.h"

static const char *TAG = "sensor";

#define IR_INPUT 2
#define IR_INPUT_PIN_SELECT (1ULL << IR_INPUT)
#define ESP_INTR_FLAG_DEFAULT 0

void sensor_isr() {
  ESP_LOGI(TAG, "interrupt detected\n");
  ESP_LOGI(TAG, "interrupt executed\n");
}

// sets up pin for input, registers isr
esp_err_t sensor_init() {
  gpio_config_t cnfg;

  // ir sensor config
  cnfg.intr_type = GPIO_INTR_POSEDGE;
  cnfg.mode = GPIO_MODE_INPUT;
  cnfg.pin_bit_mask = IR_INPUT_PIN_SELECT;
  cnfg.pull_up_en = 1;

  esp_err_t err;

  // config pin for input
  err = gpio_config(&cnfg);
  if (err != ESP_OK)
    ESP_LOGI(TAG, "error in config: %s\n", esp_err_to_name(err));

  // install isr
  err = gpio_install_isr_service(ESP_INTR_FLAG_DEFAULT);
  if (err != ESP_OK)
    ESP_LOGI(TAG, "error in config: %s\n", esp_err_to_name(err));

  err = gpio_isr_handler_add(IR_INPUT, sensor_isr, NULL);

  if (err != ESP_OK)
    ESP_LOGI(TAG, "error in config: %s\n", esp_err_to_name(err));

  return err;
}