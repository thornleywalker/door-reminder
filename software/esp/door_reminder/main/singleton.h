#ifndef SINGLETON_H
#define SINGLETON_H

#include <stdint.h>
#include <stdbool.h>

#define MAX_USERS 16
#define MAX_UID_LENGTH 20

typedef struct user{
    char* id;
    bool has_going_reminders;
    bool has_coming_reminders;
}user_t;

typedef struct user_array{
    user_t users[MAX_USERS+1];
    uint8_t length;
}user_array_t;

typedef struct bt_strength{
    char* uid;
    int32_t value;
}bt_strength;

typedef struct bt_connection_strengths{
    bt_strength values[MAX_USERS];
    uint8_t length;
}bt_strengths_t;

//initializes singleton
// *** users array at 0 will be general device reminders (no user)
esp_err_t singleton_init();

//returns device id
char* singleton_get_device_id();

//adds user to users array
void singleton_add_user(char* new_user_id);

//adds user to users array
void singleton_remove_user(char* new_user_id);

//returns pointer to users array
user_array_t* singleton_get_users();

#endif