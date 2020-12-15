#ifndef DATA_CACHE_H
#define DATA_CACHE_H

#include "esp_err.h"
#include "esp_log.h"
#include <stdbool.h>
#include <stdint.h>

#define MAX_USERS 16
#define MAX_UID_LENGTH 20

typedef struct user {
  char id[MAX_UID_LENGTH];
  bool has_going_reminders;
  bool has_coming_reminders;
} user_t;

typedef struct user_array {
  user_t users[MAX_USERS + 1]; // extra for device user
  uint8_t length;
} user_array_t;

typedef struct bt_strength {
  char uid[MAX_UID_LENGTH];
  int32_t value;
} bt_strength;

typedef struct bt_connection_strengths {
  bt_strength values[MAX_USERS];
  uint8_t length;
} bt_strengths_t;

// initializes data_cache
// *** users array at 0 will be general device reminders (no user)
esp_err_t data_cache_init();

// returns device id
char *data_cache_get_device_id();

// adds user to users array
void data_cache_add_user(char *new_user_id);

// adds user to users array
void data_cache_remove_user(char *new_user_id);

// returns pointer to users array
user_array_t *data_cache_get_users();

user_t *data_cache_get_user(int index);

#endif