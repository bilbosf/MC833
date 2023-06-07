#ifndef USERDB
#define USERDB

#include <sqlite3.h>
#include "images.h"
#include <stdbool.h>

#define USER_FIELD_LEN 50
#define USER_SKILLS_LEN 200
#define MAX_IMAGE_SIZE 1000000

typedef struct user_t {
    char email              [USER_FIELD_LEN];
    char first_name         [USER_FIELD_LEN];
    char last_name          [USER_FIELD_LEN];
    char city               [USER_FIELD_LEN];
    char graduation_course  [USER_FIELD_LEN];
    char graduation_year    [USER_FIELD_LEN];
    char skills             [USER_SKILLS_LEN];
    bool has_image;
} user_t;

typedef struct user_list_t {
    user_t *list;
    int length;
} user_list_t;

sqlite3 *load_db(char *db_name);

int add_user(sqlite3 *db, user_t user);

int add_photo(sqlite3 *db, char *email, image_t *img);

image_t *get_photo(sqlite3 *db, char *email);

user_list_t *get_all_users(sqlite3 *db);

user_list_t *get_users_by_course(sqlite3 *db, char *course);

user_list_t *get_users_by_year(sqlite3 *db, char *year);

user_list_t *get_users_by_email(sqlite3 *db, char *email);

user_list_t *get_users_by_skill(sqlite3 *db, char *skill);

int remove_user(sqlite3 *db, char *email);

#endif