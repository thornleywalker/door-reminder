#ifndef DATABASE_PROXY_H
#define DATABASE_PROXY_H

#include "esp_err.h"
#include <stdio.h>
#include <stdlib.h>

typedef enum { ALERT_DIR_COMING = (1 << 0), ALERT_DIR_GOING = (1 << 1) } alert_dir_t;

esp_err_t database_init();
esp_err_t database_alert_users(alert_dir_t direction);
#endif