#ifndef DATABASE_PROXY_H
#define DATABASE_PROXY_H

#include "esp_err.h"
#include <stdio.h>
#include <stdlib.h>

esp_err_t database_init();
void database_login();
void database_sync();

#endif