#include "database_proxy.h"

#include "data_cache.h"
#include "esp_http_client.h"
#include "esp_log.h"
#include <ctype.h>
#include <string.h>

#define FIREBASE_REST_BASE "https://door-reminder-89e84.firebaseio.com"
#define FIREBASE_REST_DEVICES "/devices"
#define FIREBASE_REST_USERS "/users"
#define FIREBASE_REST_REMINDERS "/reminders"
#define FIREBASE_REST_SUFFIX ".json"

#define REMINDERS_BASE_LENGTH 16
#define MAX_HTTP_OUTPUT_BUFFER 2048

static const char *TAG = "dr_database_proxy";

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
    // if (output_buffer != NULL) {
    //   // Response is accumulated in output_buffer. Uncomment the below line to print the
    //   accumulated
    //   // response ESP_LOG_BUFFER_HEX(TAG, output_buffer, output_len);
    //   free(output_buffer);
    //   output_buffer = NULL;
    // }
    // output_len = 0;
    break;
  case HTTP_EVENT_DISCONNECTED:
    ESP_LOGI(TAG, "HTTP_EVENT_DISCONNECTED");
    // int mbedtls_err = 0;
    // esp_err_t err = esp_tls_get_and_clear_last_error(evt->data, &mbedtls_err, NULL);
    // if (err != 0) {
    //   if (output_buffer != NULL) {
    //     free(output_buffer);
    //     output_buffer = NULL;
    //   }
    //   output_len = 0;
    //   ESP_LOGI(TAG, "Last esp error code: 0x%x", err);
    //   ESP_LOGI(TAG, "Last mbedtls failure: 0x%x", mbedtls_err);
    // }
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

  user->has_going_reminders = false;
  user->has_coming_reminders = false;

  // parse user id
  curr_token = strtok(NULL, dlim);
  if (curr_token == NULL)
    return NULL;
  strcpy(user->id, curr_token);
  if (curr_token == NULL) {
    printf("got null unexpectedly\n");
    return NULL;
  }
  // printf("id token: %s\n", curr_token);

  // consume "coming"
  curr_token = strtok(NULL, dlim);
  // printf("coming/going token: %s\n", curr_token);
  if (strcmp(curr_token, "coming") == 0) {
    // consume coming "count"
    curr_token = strtok(NULL, dlim);
    // printf("count token: %s\n", curr_token);

    // parse coming count value
    curr_token = strtok(NULL, dlim);
    // printf("count token: %s\n", curr_token);

    if (atoi(curr_token) != 0)
      user->has_coming_reminders = true;
    else
      user->has_coming_reminders = false;
    // consume coming "flag"
    curr_token = strtok(NULL, dlim);
    // printf("flag token: %s\n", curr_token);

    // consume coming flag value
    curr_token = strtok(NULL, dlim);
    // printf("flag value token: %s\n", curr_token);

    // consume "going"
    curr_token = strtok(NULL, dlim);
    // printf("going token: %s\n", curr_token);
  }
  if (!strcmp(curr_token, "going")) {
    // consume going "count"
    curr_token = strtok(NULL, dlim);
    // printf("count token: %s\n", curr_token);

    // parse going count value
    curr_token = strtok(NULL, dlim);
    // printf("count token: %s\n", curr_token);
    if (atoi(curr_token) != 0)
      user->has_going_reminders = true;
    else
      user->has_going_reminders = false;
    // consume going "flag"
    curr_token = strtok(NULL, dlim);
    // printf("flag token: %s\n", curr_token);
    // consume going flag value
    curr_token = strtok(NULL, dlim);
    // printf("flag value token: %s\n", curr_token);
  }

  return strtok(NULL, "\"");
}

void update_all_reminders() {
  ESP_LOGI(TAG, "updating reminders");
  char device_users_url[512];
  sprintf(device_users_url, "%s%s%s%s%s", FIREBASE_REST_BASE, FIREBASE_REST_DEVICES, "/",
          data_cache_get_device_id(), FIREBASE_REST_SUFFIX);
  esp_http_client_config_t config = {
      .url = device_users_url,
      .method = HTTP_METHOD_GET,
      .event_handler = _http_event_handler,
      .user_data = response_buffer, // Pass address of local buffer to get response
      .disable_auto_redirect = true,
  };

  // setup client
  firebase_rest_handle = esp_http_client_init(&config);
  // perform get
  esp_http_client_perform(firebase_rest_handle);
  // check response
  printf("\n\n%s\n\n", response_buffer);

  char *buf_loc = response_buffer;
  user_t *temp_user;
  int index = 0;
  data_cache_get_users()->length = 0;
  temp_user = data_cache_get_user(index);
  // load response into strtok internal buffer
  buf_loc = strtok(response_buffer, "\"");
  while ((buf_loc = json_parse_user(buf_loc, temp_user)) != NULL) {
    printf("id: %s\ngoing?: %i\ncoming?: %i\n", temp_user->id, temp_user->has_going_reminders,
           temp_user->has_coming_reminders);
    temp_user = data_cache_get_user(++index);
    data_cache_get_users()->length++;
  }

  ESP_LOGI(TAG, "all device info parsed");

  // cleanup
  esp_http_client_cleanup(firebase_rest_handle);
  ESP_LOGI(TAG, "reminders updated");
}

static bool is_initialized = false;
esp_err_t database_init() {
  // avoid reinitialization
  if (is_initialized)
    return ESP_OK;

  ESP_LOGI(TAG, "initializing database proxy");
  update_all_reminders();
  ESP_LOGI(TAG, "database proxy initialized");
  is_initialized = true;
  return ESP_OK;
}

esp_http_client_handle_t flag_setter_client;

// sets corresponding flag in database
void set_reminder_flag(char *user_id, alert_dir_t direction) {
  char *dir_string;
  if (direction == ALERT_DIR_COMING)
    dir_string = "coming";
  else if (direction == ALERT_DIR_GOING)
    dir_string = "going";
  else
    return;

  ESP_LOGI(TAG, "setting reminder flag. user: %s, direction: %s", user_id, dir_string);

  char device_flag_url[512];
  sprintf(device_flag_url, "%s%s%s%s%s%s%s%s%s", FIREBASE_REST_BASE, FIREBASE_REST_DEVICES, "/",
          data_cache_get_device_id(), "/", user_id, "/", dir_string, FIREBASE_REST_SUFFIX);

  // create a second http client for setting flags as from getting updates
  esp_http_client_set_url(flag_setter_client, device_flag_url);
  const char *post_data = "{\"flag\":true}";
  esp_http_client_set_header(flag_setter_client, "Content-Type", "application/json");
  esp_http_client_set_post_field(flag_setter_client, post_data, strlen(post_data));
  // perform get
  esp_http_client_perform(flag_setter_client);

  ESP_LOGI(TAG, "reminder flag set");
}

// checks every user in the array for reminders, triggers them
esp_err_t database_alert_users(alert_dir_t direction) {
  ESP_LOGI(TAG, "alerting users");
  user_array_t *users_array_h = data_cache_get_users();

  esp_http_client_config_t config = {.url = FIREBASE_REST_BASE,
                                     .method = HTTP_METHOD_PATCH,
                                     .event_handler = _http_event_handler,
                                     .user_data = response_buffer,
                                     .disable_auto_redirect = true};
  // setup client
  flag_setter_client = esp_http_client_init(&config);

  for (int i = 0; i < users_array_h->length; i++) {
    if ((direction & ALERT_DIR_COMING) && users_array_h->users[i].has_coming_reminders)
      set_reminder_flag(users_array_h->users[i].id, direction);

    if ((direction & ALERT_DIR_GOING) && users_array_h->users[i].has_going_reminders)
      set_reminder_flag(users_array_h->users[i].id, direction);
  }

  esp_http_client_cleanup(flag_setter_client);

  ESP_LOGI(TAG, "users alerted");

  return ESP_OK;
}