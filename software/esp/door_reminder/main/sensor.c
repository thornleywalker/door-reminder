#include "sensor.h"

#include "database_proxy.h"
#include "driver/gpio.h"
#include "driver/i2c.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <limits.h>

static const char *TAG = "dr_sensor";

#define GOING_INPUT GPIO_NUM_26
#define COMING_INPUT GPIO_NUM_27
#define INPUT_PIN_SELECT ((1ULL << GOING_INPUT) | (1ULL << COMING_INPUT))
#define ESP_INTR_FLAG_DEFAULT 0

#define LEFT_I2C_ADDRESS 0x52
#define RIGHT_I2C_ADDRESS 0x53
#define I2C_SDA_PIN 12
#define I2C_SCL_PIN 14

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
      vTaskDelay(pdMS_TO_TICKS(5000));
      printf("finished task sleep");
      ESP_LOGI(TAG, "Finished task sleeping");
    }
  }
}

#define i2c_wrt_help(addr, data)                                                                   \
  {                                                                                                \
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();                                                  \
    i2c_master_start(cmd);                                                                         \
    i2c_master_write_byte(cmd, addr, ACK);                                                         \
    i2c_master_write(cmd, data, 1, ACK);                                                           \
    i2c_master_stop(cmd);                                                                          \
    i2c_master_cmd_begin(i2c_master_port, cmd, pdMS_TO_TICKS(50));                                 \
    i2c_cmd_link_delete(cmd);                                                                      \
  }

#define ADCCONFIG0 0x02
#define ADCSENS0 0x03
#define ADCPOST0 0x04
#define MEASCONFIG0 0x05

#define LED1_A 0x1F
#define LED1_B 0x20

#define MEASCOUNT1 0x1C

// Writes the given param (6-bits) with the given input (NULL if no input)
void write_param(uint8_t param, uint8_t input) {
  if (input != NULL)
    i2c_wrt_help(0x0A, input);

  uint8_t param_addr = 0b10000000 | param;
  i2c_wrt_help(0x0B, param_addr);
}

void mcu_program() {
  int i2c_master_port = 0;
  i2c_config_t conf = {.mode = I2C_MODE_MASTER,
                       .sda_io_num = I2C_SDA_PIN, // select GPIO specific to your project
                       .sda_pullup_en = GPIO_PULLUP_ENABLE,
                       .scl_io_num = I2C_SCL_PIN, // select GPIO specific to your project
                       .scl_pullup_en = GPIO_PULLUP_ENABLE,
                       .master.clk_speed =
                           I2C_MASTER_FREQ_HZ, // select frequency specific to your project
                       .slave_addr = RIGHT_I2C_ADDRESS};
  i2c_param_config(i2c_master_port, conf);

  i2c_driver_install(i2c_master_port, I2C_MODE_MASTER, 0, 0, 0);

  // ADCCONFIG
  uint8_t adc_config;
  //  Decim Rate
  //  ADC mux
  write_param(ADCCONFIG0, adc_config);
  // ADCSENS
  //  HSIG
  //  sw gain
  //  hw gain
  // ADCPOST
  //  24bit out
  //  post shift
  //  thresh pol
  //  thresh sel
  // MEASCONFIG
  //  counter index
  //  led trim
  //  bank sel
  //  led enables

  return void;
}

// Program IR mcu, sets up pin for input, registers isr
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
