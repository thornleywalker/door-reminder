
#ifndef BLUETOOTH_H
#define BLUETOOTH_H

#include "esp_err.h"
#include "esp_task.h"
#include "sdkconfig.h"
#include <stdbool.h>
#include <stdint.h>

esp_err_t bluetooth_init();
// checks strength of bluetooth connections
void bluetooth_strength_check();

#endif