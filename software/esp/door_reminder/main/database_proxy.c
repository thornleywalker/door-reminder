#include "database_proxy.h"

#include "cJSON.h"
#include "data_cache.h"
#include "esp_http_client.h"
#include "esp_log.h"
#include <string.h>

#define FIREBASE_REST_BASE "https://door-reminder-89e84.firebaseio.com"
#define FIREBASE_REST_DEVICES "/devices"
#define FIREBASE_REST_USERS "/users"
#define FIREBASE_REST_REMINDERS "/reminders"

#define REMINDERS_BASE_LENGTH 16

static const char *TAG = "database_proxy";

typedef enum direction_e { COMING, GOING } direction_t;

typedef struct reminder {
  char *user_id;
  direction_t direction;
} reminder_t;

static esp_http_client_handle_t firebase_rest_handle;

// creates handle to https client with Firebase REST API url
void connect_to_firebase() {
  esp_http_client_config_t firebase_configuration = {.url = FIREBASE_REST_BASE};
  firebase_rest_handle = esp_http_client_init(&firebase_configuration);
}

void remove_whitespace(char *str) {
  int char_count = 0;
  for (int i = 0; i < strlen(str); i++)
    if (!isspace(str[i]))
      str[char_count++] = str[i];
}

// steps through the given json string str, puts parsed values into user pointer
// returns pointer where it stopped parsing, returns NULL when no more users
char *json_parse_user(char *str, user_t *user) {
  const char dlim[] = "{}:\"\n, ";
  char *curr_token;

  // parse user id
  curr_token = strtok(str, dlim);
  strcpy(curr_token, user->id);

  // consume "coming"
  strtok(NULL, dlim);
  // consume coming "count"
  strtok(NULL, dlim);
  // parse coming count value
  curr_token = strtok(NULL, dlim);
  if (atoi(curr_token) != 0)
    user->has_coming_reminders = true;
  else
    user->has_coming_reminders = false;
  // consume coming "flag"
  strtok(NULL, dlim);
  // consume coming flag value
  strtok(NULL, dlim);

  // consume "going"
  strtok(NULL, dlim);
  // consume going "count"
  strtok(NULL, dlim);
  // parse going count value
  curr_token = strtok(NULL, dlim);
  if (atoi(curr_token) != 0)
    user->has_going_reminders = true;
  else
    user->has_going_reminders = false;
  // consume going "flag"
  strtok(NULL, dlim);
  // consume going flag value
  strtok(NULL, dlim);

  return strtok(NULL, dlim);
}

// gets users from Firebase, adds them to data_cache user array
// returns true if successful
bool get_device_users() {
  char *device_users_url;
  sprintf(device_users_url, "%s%s%s%s" FIREBASE_REST_BASE, FIREBASE_REST_DEVICES, '/',
          data_cache_get_device_id());
  esp_err_t err = esp_http_client_set_url(firebase_rest_handle, device_users_url);
  if (err != ESP_OK) {
    ESP_LOGW(TAG, "could not get webpage\n");
    return false;
  }

  // GET json object
  char *json_string;

  // parse values
  char *curr_string = json_string;
  for (int i = 0; i < MAX_USERS; i++)
    if (json_parse_user(curr_string, data_cache_get_user(i)) == NULL)
      break;

  return true;
}

// checks for reminders for each user in data_cache
void get_user_reminders(char *user_id) {
  char *device_users_url;
  // turn this into a macro
  sprintf(device_users_url, FIREBASE_REST_BASE, FIREBASE_REST_DEVICES, '/',
          data_cache_get_device_id(), FIREBASE_REST_REMINDERS);
  esp_http_client_set_url(firebase_rest_handle, device_users_url);
  reminder_t reminders[REMINDERS_BASE_LENGTH];
  size_t reminders_size = 0;
  // get/parse reminders from firebase

  user_array_t *users = data_cache_get_users();
  for (size_t i = 0; i < reminders_size; i++) {
    for (size_t i = 0; i < users->length; i++) {
      // check each reminder to see who's it is.
      // if user ids match, set corresponding flag
      // for user (coming, going) in data_cache
    }
  }
}