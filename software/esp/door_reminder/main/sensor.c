#include "sensor.h"

#include "database_proxy.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "si115x.h"
#include <limits.h>

static const char *TAG = "dr_sensor";

#define GOING_INPUT GPIO_NUM_26
#define COMING_INPUT GPIO_NUM_27
#define INPUT_PIN_SELECT ((1ULL << GOING_INPUT) | (1ULL << COMING_INPUT))
#define ESP_INTR_FLAG_DEFAULT 0

#define I2C_PORT_NUM 0
#define I2C_MASTER_FREQ_HZ 100000
#define LEFT_I2C_ADDRESS 0x52
#define RIGHT_I2C_ADDRESS 0x53
#define I2C_SDA_PIN 12
#define I2C_SCL_PIN 14

#define DEFAULT_SENSOR_CONFIG                                                                      \
  0, ADCCONFIG(DECIM_0, LRG_IR), ADCSENS(NORMAL_GAIN, 3, 7),                                       \
      ADCPOST(U_16, 0, ENTER_WINDOW, THRESH_WINDOW), MEASCONFIG(MC_0, NOMINAL, BANK_A, LED_1)

static xTaskHandle button_task;

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
      // acknowledge interrupt on sensor. read irq, pause?, resume
      //
      printf("direction: %s\n", (dir == ALERT_DIR_COMING)  ? "coming"
                                : (dir == ALERT_DIR_GOING) ? "going"
                                                           : "error");
      database_alert_users(dir);
      vTaskDelay(pdMS_TO_TICKS(5000));
      printf("finished task sleep");
      ESP_LOGI(TAG, "Finished task sleeping");
    }
  }
}

#define DEFAULT_CHANNEL                                                                            \
  .adcconfig = ADCCONFIG(DECIM_0, LRG_IR), .adcsens = ADCSENS(NORMAL_GAIN, 3, 7),                  \
  .adcpost = ADCPOST(U_16, 0, ENTER_WINDOW, THRESH_WINDOW),                                        \
  .measconfig = MEASCONFIG(MC_0, NOMINAL, BANK_A, LED_1)

#define EMPTY_CHANNEL .adcconfig = 0, .adcsens = 0, .adcpost = 0, .measconfig = 0

// Program IR mcu, sets up pin for input, registers isr
esp_err_t sensor_init() {
  esp_err_t err;
  ESP_LOGI(TAG, "initializing sensor");
  ESP_LOGI(TAG, "initializing interrupts\n");
  // setup button handling task
  xTaskCreate(interrupt_handler, "interrupt_handler", 10240, NULL, 10, &button_task);
  gpio_config_t cnfg = {
      .intr_type = GPIO_INTR_POSEDGE,
      .mode = GPIO_MODE_INPUT,
      .pin_bit_mask = INPUT_PIN_SELECT,
      .pull_down_en = GPIO_PULLDOWN_ENABLE,
      .pull_up_en = GPIO_PULLUP_DISABLE,
  };

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

  ESP_LOGI(TAG, "interrupts initialized\n");

  ESP_LOGI(TAG, "initializing si115x\n");
  // init sensors (setup i2c)
  si115x_init(I2C_PORT_NUM, I2C_SDA_PIN, I2C_SCL_PIN, I2C_MASTER_FREQ_HZ);

  // get setup values
  param_table_t left_sensor_pt = {
      .i2c_addr = LEFT_I2C_ADDRESS,
      .chan_list = 0x01 << 0,
      .channel =
          {
              {DEFAULT_CHANNEL},
              {EMPTY_CHANNEL},
              {EMPTY_CHANNEL},
              {EMPTY_CHANNEL},
              {EMPTY_CHANNEL},
              {EMPTY_CHANNEL},
          },
      .measrate = {.val = 1},
      .meascount = {1, 0, 0},
      .led =
          {
              {.a = L15_111, .b = 0},
              {.a = 0, .b = 0},
              {.a = 0, .b = 0},
          },
      .threshold =
          {
              {.val = 0},
              {.val = 0},
          },
      .upper_threshold = {.val = 400},
      .burst = (0x01 << 7) | 0x01,
      .lower_threshold = {.val = 200},
  };
  param_table_t right_sensor_pt = {
      .i2c_addr = RIGHT_I2C_ADDRESS,
      .chan_list = 0x01 << 0,
      .channel =
          {
              {DEFAULT_CHANNEL},
              {EMPTY_CHANNEL},
              {EMPTY_CHANNEL},
              {EMPTY_CHANNEL},
              {EMPTY_CHANNEL},
              {EMPTY_CHANNEL},
          },
      .measrate = {.val = 1},
      .meascount = {1, 0, 0},
      .led =
          {
              {.a = L15_111, .b = 0},
              {.a = 0, .b = 0},
              {.a = 0, .b = 0},
          },
      .threshold =
          {
              {.val = 0},
              {.val = 0},
          },
      .upper_threshold = {.val = 400},
      .burst = (0x01 << 7) | 0x01,
      .lower_threshold = {.val = 200},
  };
  // pull from nvs, if not there, store the default ^

  // configure left sensor
  // set lower threshold
  si115x_write_param(LEFT_I2C_ADDRESS, LOWER_THRESHOLD_H, left_sensor_pt.lower_threshold.parts.hi);
  si115x_write_param(LEFT_I2C_ADDRESS, LOWER_THRESHOLD_L, left_sensor_pt.lower_threshold.parts.lo);
  // set upper threshold
  si115x_write_param(LEFT_I2C_ADDRESS, UPPER_THRESHOLD_H, left_sensor_pt.upper_threshold.parts.hi);
  si115x_write_param(LEFT_I2C_ADDRESS, UPPER_THRESHOLD_L, left_sensor_pt.upper_threshold.parts.lo);
  // set meas rate
  si115x_write_param(LEFT_I2C_ADDRESS, MEASRATE_H, left_sensor_pt.measrate.parts.hi);
  si115x_write_param(LEFT_I2C_ADDRESS, MEASRATE_L, left_sensor_pt.measrate.parts.lo);
  // set meas count
  si115x_write_param(LEFT_I2C_ADDRESS, MEASCOUNT0, left_sensor_pt.meascount[0]);
  // set burst count
  si115x_write_param(LEFT_I2C_ADDRESS, BURST, left_sensor_pt.burst);
  // set LED current
  si115x_write_param(LEFT_I2C_ADDRESS, LED1_A, left_sensor_pt.led[0].a);
  // configure channel
  si115x_channel_config(LEFT_I2C_ADDRESS, // i2c address
                        0, left_sensor_pt.channel[0].adcconfig, left_sensor_pt.channel[0].adcsens,
                        left_sensor_pt.channel[0].adcpost, left_sensor_pt.channel[0].measconfig);
  // enable channel
  si115x_write_param(LEFT_I2C_ADDRESS, CHAN_LIST, left_sensor_pt.chan_list);
  // enable interrupts
  si115x_write_reg(LEFT_I2C_ADDRESS, IRQ_ENABLE, 0x01 << 0);

  // configure right sensor
  // set lower threshold
  si115x_write_param(RIGHT_I2C_ADDRESS, LOWER_THRESHOLD_H,
                     right_sensor_pt.lower_threshold.parts.hi);
  si115x_write_param(RIGHT_I2C_ADDRESS, LOWER_THRESHOLD_L,
                     right_sensor_pt.lower_threshold.parts.lo);
  // set upper threshold
  si115x_write_param(RIGHT_I2C_ADDRESS, UPPER_THRESHOLD_H,
                     right_sensor_pt.upper_threshold.parts.hi);
  si115x_write_param(RIGHT_I2C_ADDRESS, UPPER_THRESHOLD_L,
                     right_sensor_pt.upper_threshold.parts.lo);
  // set meas rate
  si115x_write_param(RIGHT_I2C_ADDRESS, MEASRATE_H, right_sensor_pt.measrate.parts.hi);
  si115x_write_param(RIGHT_I2C_ADDRESS, MEASRATE_L, right_sensor_pt.measrate.parts.lo);
  // set meas count
  si115x_write_param(RIGHT_I2C_ADDRESS, MEASCOUNT0, right_sensor_pt.meascount[0]);
  // set burst count
  si115x_write_param(RIGHT_I2C_ADDRESS, BURST, right_sensor_pt.burst);
  // set LED current
  si115x_write_param(RIGHT_I2C_ADDRESS, LED1_A, right_sensor_pt.led[0].a);
  // configure channel
  si115x_channel_config(RIGHT_I2C_ADDRESS, // i2c address
                        0, right_sensor_pt.channel[0].adcconfig, right_sensor_pt.channel[0].adcsens,
                        right_sensor_pt.channel[0].adcpost, right_sensor_pt.channel[0].measconfig);
  // enable channel
  si115x_write_param(RIGHT_I2C_ADDRESS, CHAN_LIST, right_sensor_pt.chan_list);
  // enable interrupts
  si115x_write_reg(RIGHT_I2C_ADDRESS, IRQ_ENABLE, 0x01 << 0);

  // start autonomous mode
  si115x_issue_command(LEFT_I2C_ADDRESS, START);
  si115x_issue_command(RIGHT_I2C_ADDRESS, START);
  ESP_LOGI(TAG, "si115x initialized\n");
  ESP_LOGI(TAG, "sensor initialized");
  return err;
}
