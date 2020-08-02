
#ifndef BLUETOOTH_H
#define BLUETOOTH_H

#include <stdint.h>
#include <stdbool.h>
#include "esp_err.h"
#include "sdkconfig.h"
#include "esp_task.h"

#include "singleton.h"



//checks strength of bluetooth connections
void bluetooth_strength_check();

#endif