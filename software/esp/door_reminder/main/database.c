#include "database.h"
// #include <stdarg.h>
#include <string.h>
#include "esp_http_client.h"
#include "singleton.h"

#define FIREBASE_REST_BASE "https://door-reminder-89e84.firebaseio.com"
#define FIREBASE_REST_DEVICES "/devices"
#define FIREBASE_REST_USERS "/users"
#define FIREBASE_REST_REMINDERS "/reminders"

#define REMINDERS_BASE_LENGTH 16
#define ADDRESS_MAX_LENGTH 128

typedef enum direction_e{
    COMING,
    GOING
}direction_t;

typedef struct reminder{
    char* user_id;
    direction_t direction;
}reminder_t;

static esp_http_client_handle_t firebase_rest_handle;

static const char *TAG = "HTTP_CLIENT";

static esp_err_t _http_event_handler(esp_http_client_event_t *evt)
{
    esp_http_client_config_t firebase_configuration = 
    {
        .url = FIREBASE_REST_BASE
        
    };
    firebase_rest_handle = esp_http_client_init(&firebase_configuration);
    switch(evt->event_id) {
        case HTTP_EVENT_ERROR:
            // ESP_LOGD(TAG, "HTTP_EVENT_ERROR");
            break;
        case HTTP_EVENT_ON_CONNECTED:
            // ESP_LOGD(TAG, "HTTP_EVENT_ON_CONNECTED");
            break;
        case HTTP_EVENT_HEADER_SENT:
            // ESP_LOGD(TAG, "HTTP_EVENT_HEADER_SENT");
            break;
        case HTTP_EVENT_ON_HEADER:
            // ESP_LOGD(TAG, "HTTP_EVENT_ON_HEADER, key=%s, value=%s", evt->header_key, evt->header_value);
            break;
        case HTTP_EVENT_ON_DATA:
            ESP_LOGD(TAG, "HTTP_EVENT_ON_DATA, len=%d", evt->data_len);
            if (!esp_http_client_is_chunked_response(evt->client)) {
                // Write out data
                printf("%.*s\n\r", evt->data_len, (char*)evt->data);
            }

            break;
        case HTTP_EVENT_ON_FINISH:
            ESP_LOGD(TAG, "HTTP_EVENT_ON_FINISH");
            break;
        case HTTP_EVENT_DISCONNECTED:
            ESP_LOGI(TAG, "HTTP_EVENT_DISCONNECTED");
            int mbedtls_err = 0;
            // esp_err_t err = esp_tls_get_and_clear_last_error(evt->data, &mbedtls_err, NULL);
            // if (err != 0) {
            //     ESP_LOGI(TAG, "Last esp error code: 0x%x");
            //     ESP_LOGI(TAG, "Last mbedtls failure: 0x%x");
            // }
            break;
    }
    return ESP_OK;
}

//gets users from Firebase, adds them to singleton user array
void get_device_users()
{
    char* device_users_url;
    sprintf(device_users_url, "%s, %s"
            FIREBASE_REST_BASE,
            FIREBASE_REST_DEVICES,
            '/', singleton_get_device_id(),
            FIREBASE_REST_USERS
    );
    esp_http_client_set_url(firebase_rest_handle, device_users_url);
    //GET values
    //parse values
    //add to singleton 
}

// checks for reminders for each user in singleton
void get_user_reminders(char* user_id)
{
    char* device_users_url;
        sprintf(device_users_url, 
                FIREBASE_REST_BASE,
                FIREBASE_REST_DEVICES,
                '/', singleton_get_device_id(),
                FIREBASE_REST_REMINDERS
        );
    esp_http_client_set_url(firebase_rest_handle, device_users_url);
    reminder_t reminders[REMINDERS_BASE_LENGTH];
    size_t reminders_size = 0;
    //get/parse reminders from firebase

    user_array_t *users = singleton_get_users();
    for (size_t i = 0; i < reminders_size; i++)
    {
        for (size_t i = 0; i < users->length; i++)
        {
            strcmp();   //compare strings
            //check each reminder to see who's it is.
            //if user ids match, set corresponding flag
            //for user (coming, going) in singleton
        }
    }
}

void testing_function(){
    esp_http_client_config_t config = {
        .url = curr_address_string,                /*!< HTTP URL, the information on the URL is most important, it overrides the other fields below, if any */
        .method = HTTP_METHOD_GET,                   /*!< HTTP Method */
        .event_handler = _http_event_handler,             /*!< HTTP Event Handle */
    };

    esp_http_client_handle_t handle = esp_http_client_init(&config);

    esp_err_t err;

    err = esp_http_client_perform(handle);
    if(err == ESP_OK)
    {
        int length = 512;
        char buffer[512];
        printf("it worked\n\r");
        esp_http_client_read(handle, buffer, length);
        printf("the data: %s\n\r", buffer);
    }
    else printf("didn't work\n\r");

    esp_http_client_cleanup(handle);

}

void database_init(){
    esp_http_client_config_t config = {
        .url = FIREBASE_REST_BASE,                /*!< HTTP URL, the information on the URL is most important, it overrides the other fields below, if any */
        .method = HTTP_METHOD_GET,                   /*!< HTTP Method */
        .event_handler = _http_event_handler,             /*!< HTTP Event Handle */
    };

    firebase_rest_handle = esp_http_client_init(&config);
}

void database_login(){
    printf("attempting to connect to firebase\n\r");
    // connect_to_firebase();

    testing_function();

    // printf("attempting to get device users\n\r");
    // get_device_users();

}