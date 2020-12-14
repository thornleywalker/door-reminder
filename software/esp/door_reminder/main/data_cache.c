#include "data_cache.h"

#include "esp_log.h"
#include "nvs.h"
#include "nvs_flash.h"
#include <stdlib.h>
#include <string.h>

static const char *TAG = "data_cache";

#define MAX_DEVICE_ID_LENGTH 20

// NVS variables/values
#define DEVICE_ID_KEY "device-id"
#define USER_ARRAY_KEY "user-array"

static nvs_handle_t users_handle;

static const char *USER_KEYS[MAX_USERS] = {"u0", "u1", "u2",  "u3",  "u4",  "u5",  "u6",  "u7",
                                           "u8", "u9", "u10", "u11", "u12", "u13", "u14", "u15"};

// static values
static char device_id[MAX_DEVICE_ID_LENGTH];
static user_array_t users_array;
static bt_strengths_t bt_strengths;

// initializes users
// opens "users" namespace of NVS
// gets device id
esp_err_t users_init() {
  // open nvs namespace "users"
  esp_err_t err = nvs_open("users", NVS_READWRITE, &users_handle);
  if (err != ESP_OK)
    ESP_LOGI(TAG, "error opening users nvs: %s\n", esp_err_to_name(err));

  // get device id from memory
  err = nvs_get_str(users_handle, DEVICE_ID_KEY, device_id, MAX_DEVICE_ID_LENGTH);
  switch (err) {
  case ESP_OK:
    ESP_LOGI(TAG, "Done\n");
    ESP_LOGI(TAG, "Device id: %s\n", device_id);
    break;
  case ESP_ERR_NVS_NOT_FOUND:
    ESP_LOGI(TAG, "No device id found!\n");

    break;
  default:
    ESP_LOGI(TAG, "Error (%s) reading!\n", esp_err_to_name(err));
  }

  // get users array from NVS
  size_t array_size = sizeof(user_array_t);
  err = nvs_get_blob(users_handle, USER_ARRAY_KEY, &users_array, array_size);

  // get user id's
  char user_string[] = "us0";
  for (int i = 1; i < users_array.length; i++) {
    itoa(i, user_string + 2, 10);
    nvs_get_str(users_handle, user_string, users_array.users[i].id, strlen(user_string) + 1);
  }

  return err;
}

// adds user to users array
void data_cache_add_user(char *new_user_id) {
  esp_err_t err;
  int index = users_array.length;
  users_array.users[index].id = malloc(sizeof(char) * MAX_UID_LENGTH);
  strcpy(users_array.users[index].id, new_user_id);

  err = nvs_set_str(users_handle, USER_KEYS[index], users_array.users[index].id);
  if (err != ESP_OK) {
    ESP_LOGI(TAG, "Error adding user id to nvs: %s\n", esp_err_to_name(err));
  }

  err = nvs_commit(users_handle);
  if (err != ESP_OK) {
    ESP_LOGI(TAG, "Error commiting user id to nvs: %s\n", esp_err_to_name(err));
  }

  users_array.length++;
}

// adds user to users array
void data_cache_remove_user(char *new_user_id) {}

esp_err_t bt_strengths_init() {
  esp_err_t err = ESP_OK;

  for (int i = 0; i < MAX_USERS; i++) {
    char uid_buffer[MAX_UID_LENGTH];
    bt_strengths.values[i].value = 0;
    bt_strengths.values[i].uid = uid_buffer;
  }

  bt_strengths.length = 0;

  return err;
}

esp_err_t data_cache_init() {
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

  return err;
}

char *data_cache_get_device_id() { return device_id; }