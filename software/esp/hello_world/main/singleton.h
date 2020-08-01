#ifndef SINGLETON_H
#define SINGLETON_H

#define MAX_USERS 16

typedef struct user{
    char* id;
    bool has_going_reminders;
    bool has_coming_reminders;
}user_t;

typedef struct{
    user_t users[MAX_USERS];
    uint8_t len;
}user_array_t;

//initializes singleton
// *** users array at 0 will be general device reminders (no user)
void singleton_init();

//returns device id
char* singleton_get_device_id();

//adds user to users array
void singleton_add_user(char* new_user_id);

//returns pointer to users array
user_array_t* singleton_get_users();

#endif