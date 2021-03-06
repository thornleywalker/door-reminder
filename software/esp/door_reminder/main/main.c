/* Hello World Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/

#include "bluetooth.h"
#include "data_cache.h"
#include "database_proxy.h"
#include "sensor.h"
#include "wifi.h"

#include "esp_spi_flash.h"
#include "esp_system.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <stdio.h>

static const char *TAG = "dr_main";
#define TEST_WIFI "WeeFee"
#define TEST_PASSWORD "P@ssw0rd"

void app_main() {
  esp_err_t err = nvs_flash_init();

  // initialize flash memory
  if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
    ESP_ERROR_CHECK(nvs_flash_erase());
    err = nvs_flash_init();
  }
  ESP_ERROR_CHECK(nvs_flash_erase());
  ESP_ERROR_CHECK(err);

  // initializations
  err = data_cache_init(); // before wifi
  err = wifi_init();       // after data cache, to check for existing ssid/pass
  if (!wifi_attempt_connect_to(TEST_WIFI, TEST_PASSWORD))
    ESP_LOGW(TAG, "Could not connect to wifi\n");

  // await wifi connection before database connection
  while (true) {
    vTaskDelay(500 / portTICK_PERIOD_MS);
    if (wifi_connected()) {
      err = database_init();
      break;
    }
  }
  err = bluetooth_init(); // who knows

  err = sensor_init(); // last, sensors hot when complete

  // main function, idle until interrupt
  printf("\n\r");
  int count = 0;
  while (count <= 10) {
    vTaskDelay(1000 / portTICK_PERIOD_MS);
    ESP_LOGI(TAG, "count: %d", count++);
  }
  while (true) {
    vTaskDelay(1000 / portTICK_PERIOD_MS);
    if (++count % 5 == 0)
      ESP_LOGI(TAG, "count: %d", count++);
  }
}

#if false
    printf("Hello world!\n");

    /* Print chip information */
    esp_chip_info_t chip_info;
    esp_chip_info(&chip_info);
    printf("This is ESP32 chip with %d CPU cores, WiFi%s%s, ",
            chip_info.cores,
            (chip_info.features & CHIP_FEATURE_BT) ? "/BT" : "",
            (chip_info.features & CHIP_FEATURE_BLE) ? "/BLE" : "");

    printf("silicon revision %d, ", chip_info.revision);

    printf("%dMB %s flash\n", spi_flash_get_chip_size() / (1024 * 1024),
            (chip_info.features & CHIP_FEATURE_EMB_FLASH) ? "embedded" : "external");

    for (int i = 10; i >= 0; i--) {
        printf("Restarting in %d seconds...\n", i);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
    printf("Restarting now.\n");
    fflush(stdout);
    esp_restart();
#endif
