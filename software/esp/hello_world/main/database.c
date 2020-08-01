#include "database.h"
#include "esp_http_client.h"
#include "singleton.h"

#define FIREBASE_REST_BASE "https://door-reminder-89e84.firebaseio.com"
#define FIREBASE_REST_DEVICES "/devices"
#define FIREBASE_REST_USERS "/users"
#define FIREBASE_REST_REMINDERS "/reminders"

static esp_http_client_handle_t firebase_rest_handle;

//creates handle to https client with Firebase REST API url
void connect_to_firebase()
{
    esp_http_client_config_t firebase_configuration = 
    {
        .url = FIREBASE_REST_BASE
        
    };
    firebase_rest_handle = esp_http_client_init(&firebase_configuration);
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

//checks for reminders for each user in singleton
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
}