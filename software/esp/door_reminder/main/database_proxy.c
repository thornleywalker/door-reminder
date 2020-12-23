#include "database_proxy.h"

#include "data_cache.h"
#include "esp_http_client.h"
#include "esp_log.h"
#include <string.h>

#define FIREBASE_REST_BASE "https://door-reminder-89e84.firebaseio.com"
#define FIREBASE_REST_DEVICES "/devices"
#define FIREBASE_REST_USERS "/users"
#define FIREBASE_REST_REMINDERS "/reminders"
#define FIREBASE_REST_SUFFIX ".json"

#define REMINDERS_BASE_LENGTH 16
#define MAX_HTTP_OUTPUT_BUFFER 2048

static const char *TAG = "database_proxy";

typedef enum direction_e { COMING, GOING } direction_t;

typedef struct reminder {
  char *user_id;
  direction_t direction;
} reminder_t;

esp_err_t _http_event_handler(esp_http_client_event_t *evt) {
  static char *output_buffer; // Buffer to store response of http request from event handler
  static int output_len;      // Stores number of bytes read
  switch (evt->event_id) {
  case HTTP_EVENT_ERROR:
    ESP_LOGD(TAG, "HTTP_EVENT_ERROR");
    break;
  case HTTP_EVENT_ON_CONNECTED:
    ESP_LOGD(TAG, "HTTP_EVENT_ON_CONNECTED");
    break;
  case HTTP_EVENT_HEADER_SENT:
    ESP_LOGD(TAG, "HTTP_EVENT_HEADER_SENT");
    break;
  case HTTP_EVENT_ON_HEADER:
    ESP_LOGD(TAG, "HTTP_EVENT_ON_HEADER, key=%s, value=%s", evt->header_key, evt->header_value);
    break;
  case HTTP_EVENT_ON_DATA:
    ESP_LOGD(TAG, "HTTP_EVENT_ON_DATA, len=%d", evt->data_len);
    /*
     *  Check for chunked encoding is added as the URL for chunked encoding used in this example
     * returns binary data. However, event handler can also be used in case chunked encoding is
     * used.
     */
    if (!esp_http_client_is_chunked_response(evt->client)) {
      // If user_data buffer is configured, copy the response into the buffer
      if (evt->user_data) {
        memcpy(evt->user_data + output_len, evt->data, evt->data_len);
      } else {
        if (output_buffer == NULL) {
          output_buffer = (char *)malloc(esp_http_client_get_content_length(evt->client));
          output_len = 0;
          if (output_buffer == NULL) {
            ESP_LOGE(TAG, "Failed to allocate memory for output buffer");
            return ESP_FAIL;
          }
        }
        memcpy(output_buffer + output_len, evt->data, evt->data_len);
      }
      output_len += evt->data_len;
    }

    break;
  case HTTP_EVENT_ON_FINISH:
    ESP_LOGD(TAG, "HTTP_EVENT_ON_FINISH");
    if (output_buffer != NULL) {
      // Response is accumulated in output_buffer. Uncomment the below line to print the accumulated
      // response ESP_LOG_BUFFER_HEX(TAG, output_buffer, output_len);
      free(output_buffer);
      output_buffer = NULL;
    }
    output_len = 0;
    break;
  case HTTP_EVENT_DISCONNECTED:
    ESP_LOGI(TAG, "HTTP_EVENT_DISCONNECTED");
    int mbedtls_err = 0;
    esp_err_t err = esp_tls_get_and_clear_last_error(evt->data, &mbedtls_err, NULL);
    if (err != 0) {
      if (output_buffer != NULL) {
        free(output_buffer);
        output_buffer = NULL;
      }
      output_len = 0;
      ESP_LOGI(TAG, "Last esp error code: 0x%x", err);
      ESP_LOGI(TAG, "Last mbedtls failure: 0x%x", mbedtls_err);
    }
    break;
  }
  return ESP_OK;
}

static esp_http_client_handle_t firebase_rest_handle;

static char response_buffer[MAX_HTTP_OUTPUT_BUFFER] = {0};

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
  const char dlim[] = "{}:\"\n\r, ";
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

esp_err_t database_init() {
  update_all_reminders();
  ESP_LOGI(TAG, "database proxy initialized\n");

  return ESP_OK;
}

// gets users from Firebase, adds them to data_cache user array
// returns true if successful
bool get_device_users() {
  char *device_users_url;
  sprintf(device_users_url, "%s%s%s%s%s" FIREBASE_REST_BASE, FIREBASE_REST_DEVICES, "/",
          data_cache_get_device_id(), FIREBASE_REST_SUFFIX);
  esp_err_t err = esp_http_client_set_url(firebase_rest_handle, device_users_url);

  if (err != ESP_OK) {
    ESP_LOGW(TAG, "could not get webpage\n");
    return false;
  } else {
    printf("%s", );
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

void update_all_reminders() {
  char device_users_url[512];

  sprintf(device_users_url, FIREBASE_REST_BASE, FIREBASE_REST_DEVICES, '/',
          data_cache_get_device_id(), FIREBASE_REST_SUFFIX);

  esp_http_client_config_t config = {
      .url = device_users_url,
      .method = HTTP_METHOD_GET,
      .event_handler = _http_event_handler,
      .user_data = response_buffer, // Pass address of local buffer to get response
      .disable_auto_redirect = true,
  };

  // setup client
  firebase_rest_handle = esp_http_client_init(&firebase_configuration);
  // perform get
  esp_http_client_perform(firebase_rest_handle);
  // check response
  printf("\n\n\n%s\n\n\n", response_buffer);
  // cleanup
  esp_http_client_cleanup(firebase_rest_handle);
}