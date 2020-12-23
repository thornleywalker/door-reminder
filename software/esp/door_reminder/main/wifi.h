#ifndef WIFI_H
#define WIFI_H

#include "esp_err.h"
#include "esp_event.h"
#include "esp_log.h"
#include "esp_system.h"
#include "esp_wifi.h"
#include "freertos/FreeRTOS.h"
#include "freertos/event_groups.h"
#include "freertos/task.h"
#include "nvs_flash.h"
#include <string.h>

esp_err_t wifi_init();
bool wifi_attempt_connect_to(char *ssid, char *pass);

#endif