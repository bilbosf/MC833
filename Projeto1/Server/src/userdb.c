#include <sqlite3.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "userdb.h"

sqlite3 *load_db(char *db_name){
    sqlite3 *db;
    int rc = sqlite3_open(db_name, &db);

    if (rc != SQLITE_OK) {
        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return NULL;
    }

    char *sql = "CREATE TABLE IF NOT EXISTS Users ("
            "email TEXT PRIMARY KEY,"
            "first_name TEXT,"
            "last_name TEXT,"
            "city TEXT,"
            "graduation_course TEXT,"
            "graduation_year TEXT,"
            "skills TEXT);";

    char *err_msg = 0;
    rc = sqlite3_exec(db, sql, 0, 0, &err_msg);

    if (rc != SQLITE_OK ) {
        fprintf(stderr, "Error: %s\n", err_msg);
        sqlite3_free(err_msg);        
        sqlite3_close(db);
        return NULL;
    } 

    printf("Database %s loaded successfully.\n", db_name);
    return db;
}

int get_single_result(sqlite3 *db, char *sql){
    sqlite3_stmt *stmt;

    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Error: %s\n", sqlite3_errmsg(db));
        return -1;
    }

    rc = sqlite3_step(stmt);
    if (rc != SQLITE_ROW) {
        fprintf(stderr, "Error: %s\n", sqlite3_errmsg(db));
        return -1;
    }

    int result = sqlite3_column_int(stmt, 0);
    sqlite3_finalize(stmt);
    return result;
}

int build_list(sqlite3 *db, char *sql, user_list_t *user_list){
    sqlite3_stmt *stmt;

    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        return 1;
    }

    int i = 0;
    while (((rc = sqlite3_step(stmt)) == SQLITE_ROW) && i < user_list->length) {
        strncpy(user_list->list[i].last_name,        (char*) sqlite3_column_text(stmt, 2), sqlite3_column_bytes(stmt, 2) + 1);
        strncpy(user_list->list[i].city,             (char*) sqlite3_column_text(stmt, 3), sqlite3_column_bytes(stmt, 3) + 1);
        strncpy(user_list->list[i].graduation_course,(char*) sqlite3_column_text(stmt, 4), sqlite3_column_bytes(stmt, 4) + 1);
        strncpy(user_list->list[i].first_name,       (char*) sqlite3_column_text(stmt, 1), sqlite3_column_bytes(stmt, 1) + 1);
        strncpy(user_list->list[i].email,            (char*) sqlite3_column_text(stmt, 0), sqlite3_column_bytes(stmt, 0) + 1);
        strncpy(user_list->list[i].graduation_year,  (char*) sqlite3_column_text(stmt, 5), sqlite3_column_bytes(stmt, 5) + 1);
        strncpy(user_list->list[i].skills,           (char*) sqlite3_column_text(stmt, 6), sqlite3_column_bytes(stmt, 6) + 1);
        i++;
    }

    sqlite3_finalize(stmt);

    if (rc != SQLITE_DONE) {
        return 1;
    }

    return 0;
}

int add_user(sqlite3 *db, user_t user){
    char sql[600];
    snprintf(sql, 600, "INSERT INTO Users VALUES ('%s', '%s', '%s', '%s', '%s', '%s', '%s');",
        user.email,
        user.first_name,
        user.last_name,
        user.city,
        user.graduation_course,
        user.graduation_year,
        user.skills);
    
    char *err_msg = 0;
    int rc = sqlite3_exec(db, sql, 0, 0, &err_msg);
    if (rc != SQLITE_OK ) {
        fprintf(stderr, "Error adding user: %s\n", err_msg);
        sqlite3_free(err_msg);        
        return 1;
    }

    return 0;
}

user_list_t *get_all_users(sqlite3 *db){
    user_list_t *user_list = malloc(sizeof(user_list));
    int count = get_single_result(db, "SELECT COUNT(*) FROM Users;");
    user_list->list = malloc(count * sizeof(user_t));
    user_list->length = count;

    char *sql = "SELECT * FROM USERS;";

    if (build_list(db, sql, user_list) == 1) {
        free(user_list->list);
        free(user_list);
        printf("Error fetching all users: %s\n", sqlite3_errmsg(db));
        return NULL;
    }

    return user_list;
}

user_list_t *get_users_by_column(sqlite3 *db, char *column, char *value) {
    char count_sql[600];
    snprintf(count_sql, 600, "SELECT COUNT(*) FROM Users WHERE %s = '%s';", column, value);
    int count = get_single_result(db, count_sql);

    user_list_t *user_list = malloc(sizeof(user_list));
    user_list->list = malloc(count * sizeof(user_t));
    user_list->length = count;

    char sql[600];
    snprintf(sql, 600, "SELECT * FROM Users WHERE %s = '%s';", column, value);

    if (build_list(db, sql, user_list) == 1) {
        free(user_list->list);
        free(user_list);
        printf("Error fetching users by %s: %s\n", column, sqlite3_errmsg(db));
        return NULL;
    }

    return user_list;
}

user_list_t *get_users_by_course(sqlite3 *db, char *course) {
    return get_users_by_column(db, "graduation_course", course);
}

user_list_t *get_users_by_year(sqlite3 *db, char *year) {
    return get_users_by_column(db, "graduation_year", year);
}

user_list_t *get_users_by_email(sqlite3 *db, char *email) {
    return get_users_by_column(db, "email", email);
}

user_list_t *get_users_by_skill(sqlite3 *db, char *skill) {
    char count_sql[600];
    snprintf(count_sql, 600, "SELECT COUNT(*) FROM Users WHERE skills LIKE '%%%s%%';", skill);
    int count = get_single_result(db, count_sql);

    user_list_t *user_list = malloc(sizeof(user_list));
    user_list->list = malloc(count * sizeof(user_t));
    user_list->length = count;

    char sql[600];
    snprintf(sql, 600, "SELECT * FROM Users WHERE skills LIKE '%%%s%%';", skill);

    if (build_list(db, sql, user_list) == 1) {
        free(user_list->list);
        free(user_list);
        printf("Error fetching users by skill: %s\n", sqlite3_errmsg(db));
        return NULL;
    }

    return user_list;
}

int remove_user(sqlite3 *db, char *email) {
    char sql[600];
    snprintf(sql, 600, "DELETE FROM Users WHERE email = '%s';", email);
    
    char *err_msg = 0;
    int rc = sqlite3_exec(db, sql, 0, 0, &err_msg);
    if (rc != SQLITE_OK ) {
        fprintf(stderr, "Error removing user: %s\n", err_msg);
        sqlite3_free(err_msg);        
        return 1;
    }

    return 0;
}
