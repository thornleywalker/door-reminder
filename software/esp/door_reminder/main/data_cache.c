#include "data_cache.h"

#include "esp_log.h"
#include "nvs.h"
#include "nvs_flash.h"
#include <stdlib.h>
#include <string.h>

static const char *TAG = "dr_data_cache";

// NVS variables/values
#define DEVICE_ID_KEY "deviceid"
#define USER_ARRAY_KEY "user-array"

static nvs_handle_t users_handle;

static const char *USER_KEYS[MAX_USERS] = {"u0", "u1", "u2",  "u3",  "u4",  "u5",  "u6",  "u7",
                                           "u8", "u9", "u10", "u11", "u12", "u13", "u14", "u15"};

// static values
static char device_id[MAX_UID_LENGTH];
static user_array_t users_array;
static bt_strengths_t bt_strengths;

#define UPDATE_MASK_NO_UPDATES 0x0000
#define UPDATE_MASK_USER_ARRAY (1ULL << 1)
#define UPDATE_MASK_USER_NAMES (1ULL << 2)
#define UPDATE_MASK_DEVICE_ID (1ULL << 3)

#define add_to_update(mask) (update_mask |= mask)

static uint16_t update_mask = UPDATE_MASK_NO_UPDATES;
// store current state to nvs
void _nvs_update() {
  // user array
  if (update_mask & UPDATE_MASK_USER_ARRAY) {
    size_t array_size = sizeof(user_array_t);
    nvs_set_blob(users_handle, USER_ARRAY_KEY, &users_array, array_size);
  }
  // user ids
  if (update_mask & UPDATE_MASK_USER_NAMES) {
    for (int i = 0; i < users_array.length; i++) {
      nvs_set_str(users_handle, USER_KEYS[i], users_array.users[i].id);
    }
  }
  // device id
  if (update_mask & UPDATE_MASK_DEVICE_ID) {
    nvs_set_str(users_handle, DEVICE_ID_KEY, device_id);
  }
  // clear update mask
  update_mask = UPDATE_MASK_NO_UPDATES;
}

// initializes users
// opens "users" namespace of NVS
// gets device id
esp_err_t users_init() {
  ESP_LOGI(TAG, "initializing users array");

  // open nvs namespace "users"
  esp_err_t err = nvs_open("users", NVS_READWRITE, &users_handle);
  if (err != ESP_OK)
    ESP_LOGI(TAG, "error opening users nvs: %s", esp_err_to_name(err));

  // get device id from memory
  size_t str_size = MAX_UID_LENGTH;
  err = nvs_get_str(users_handle, DEVICE_ID_KEY, device_id, &str_size);
  switch (err) {
  case ESP_OK:
    ESP_LOGI(TAG, "Done");
    ESP_LOGI(TAG, "Device id: %s", device_id);
    break;
  case ESP_ERR_NVS_NOT_FOUND:
    ESP_LOGI(TAG, "No device id found!");
    ESP_LOGI(TAG, "Setting default device id: %s", DEVICE_ID_KEY);
    strcpy(device_id, DEVICE_ID_KEY);
    add_to_update(UPDATE_MASK_DEVICE_ID);
    break;
  default:
    ESP_LOGI(TAG, "Error (%s) reading!", esp_err_to_name(err));
  }

  // get users array from NVS
  size_t array_size = sizeof(user_array_t);
  err = nvs_get_blob(users_handle, USER_ARRAY_KEY, &users_array, &array_size);

  switch (err) {
  case ESP_OK:
    ESP_LOGI(TAG, "Done");
    ESP_LOGI(TAG, "Users array retreived");
    break;
  case ESP_ERR_NVS_NOT_FOUND:
    ESP_LOGI(TAG, "No user array found!");
    ESP_LOGI(TAG, "Using default user array");
    strcpy(users_array.users[0].id, device_id);
    users_array.length++;
    add_to_update(UPDATE_MASK_USER_ARRAY);
    break;
  default:
    ESP_LOGI(TAG, "Error (%s) reading!", esp_err_to_name(err));
  }

  // get user id's
  char user_string[] = "us0";
  for (int i = 1; i < users_array.length; i++) {
    itoa(i, user_string + 2, 10);
    size_t str_size = strlen(user_string) + 1;
    nvs_get_str(users_handle, user_string, users_array.users[i].id, &str_size);
  }

  _nvs_update();

  ESP_LOGI(TAG, "users array initialized");
  return err;
}

// adds user to users array
void data_cache_add_user(char *new_user_id) {
  esp_err_t err;
  int index = users_array.length;
  strcpy(users_array.users[index].id, new_user_id);

  err = nvs_set_str(users_handle, USER_KEYS[index], users_array.users[index].id);
  if (err != ESP_OK) {
    ESP_LOGI(TAG, "Error adding user id to nvs: %s", esp_err_to_name(err));
  }

  err = nvs_commit(users_handle);
  if (err != ESP_OK) {
    ESP_LOGI(TAG, "Error commiting user id to nvs: %s", esp_err_to_name(err));
  }

  ESP_LOGI(TAG, "initializing data cache");
}

// adds user to users array
void data_cache_remove_user(char *new_user_id) {}

esp_err_t bt_strengths_init() {
  esp_err_t err = ESP_OK;

  for (int i = 0; i < MAX_USERS; i++) {
    bt_strengths.values[i].value = 0;
  }

  bt_strengths.length = 0;

  return err;
}

esp_err_t data_cache_init() {
  ESP_LOGI(TAG, "initializing data cache");
  // Initialize NVS
  esp_err_t err = nvs_flash_init();

  if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
    ESP_ERROR_CHECK(nvs_flash_erase());
    err = nvs_flash_init();
  }
  ESP_ERROR_CHECK(err);

  // initialize users
  users_init(&users_array);
  bt_strengths_init(&bt_strengths);

  ESP_LOGI(TAG, "data cache initialized");
  return err;
}

char *data_cache_get_device_id() { return device_id; }

user_t *data_cache_get_user(int index) { return &users_array.users[index]; }
user_array_t *data_cache_get_users() { return &users_array; }