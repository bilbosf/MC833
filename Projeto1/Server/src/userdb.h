#ifndef USERDB
#define USERDB

#include <sqlite3.h>

typedef struct user_t {
    char email[50];
    char first_name[50];
    char last_name[50];
    char city[50];
    char graduation_course[50];
    char graduation_year[10];
    char skills[200];
} user_t;

typedef struct user_list_t {
    user_t *list;
    int length;
} user_list_t;

sqlite3 *load_db(char *db_name);

int add_user(sqlite3 *db, user_t user);

user_list_t *get_all_users(sqlite3 *db);

user_list_t *get_users_by_course(sqlite3 *db, char *course);

user_list_t *get_users_by_year(sqlite3 *db, char *year);

user_list_t *get_users_by_email(sqlite3 *db, char *email);

user_list_t *get_users_by_skill(sqlite3 *db, char *skill);

int remove_user(sqlite3 *db, char *email);

#endif