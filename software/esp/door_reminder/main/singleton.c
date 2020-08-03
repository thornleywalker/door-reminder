#include "singleton.h"

#define MAX_DEVICE_ID_LENGTH 20

static char device_id[MAX_DEVICE_ID_LENGTH];
static user_array_t users;
static bt_strengths_t bt_strengths;

void users_init(user_array_t *array)
{
    user_t device_user = {
        .id="device-user",
        .has_going_reminders=false,
        .has_coming_reminders=false
    };
    array->users[0] = device_user;

}

void bt_strengths_init(bt_strengths_t *strengths)
{
    for (int i = 0; i < MAX_USERS; i++){
        char uid_buffer[MAX_UID_LENGTH];
        strengths->values[i].value = 0;
        strengths->values[i].uid = uid_buffer;
    }

    strengths->length=0;
}
void singleton_init()
{
    users_init(&users);
    bt_strengths_init(&bt_strengths);
}

char* singleton_get_device_id() { return device_id; }