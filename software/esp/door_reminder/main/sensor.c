#include "sensor.h"

#include "database_proxy.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <limits.h>

static const char *TAG = "dr_sensor";

#define GOING_INPUT GPIO_NUM_22
#define COMING_INPUT GPIO_NUM_2
#define INPUT_PIN_SELECT ((1ULL << GOING_INPUT) | (1ULL << COMING_INPUT))
#define ESP_INTR_FLAG_DEFAULT 0

xTaskHandle button_task;

void going_isr() {
  // send alert direction to handler task
  xTaskGenericNotifyFromISR(button_task, ALERT_DIR_GOING, eSetValueWithOverwrite, NULL, NULL);
}

void coming_isr() {
  // send alert direction to handler task
  xTaskGenericNotifyFromISR(button_task, ALERT_DIR_COMING, eSetValueWithOverwrite, NULL, NULL);
}

static void interrupt_handler(void *arg) {
  alert_dir_t dir;

  for (;;) {
    if (xTaskNotifyWait(ULONG_MAX, ULONG_MAX, &dir, pdMS_TO_TICKS(1000)) == pdPASS) {
      printf("direction: %s\n",
             (dir == ALERT_DIR_COMING) ? "coming" : (dir == ALERT_DIR_GOING) ? "going" : "error");
      database_alert_users(dir);
      vTaskDelay(pdMS_TO_TICKS(1000));
      printf("finished task sleep");
      ESP_LOGI(TAG, "Finished task sleeping")
    }
  }
}

// sets up pin for input, registers isr
esp_err_t sensor_init() {
  ESP_LOGI(TAG, "initializing sensor");

  // setup button handling task
  xTaskCreate(interrupt_handler, "interrupt_handler", 10240, NULL, 10, &button_task);

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