#include <sqlite3.h>
#include <stdio.h>
#include <string.h>

typedef struct user_t {
    char email[50];
    char first_name[50];
    char last_name[50];
    char city[50];
    char graduation_course[50];
    char graduation_year[10];
    char skills[200];
} user_t;

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
        fprintf(stderr, "SQL error: %s\n", err_msg);
        sqlite3_free(err_msg);        
        sqlite3_close(db);
        return NULL;
    } 

    printf("Database %s loaded successfully.\n", db_name);
    return db;
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
        fprintf(stderr, "SQL error: %s\n", err_msg);
        sqlite3_free(err_msg);        
        return 1;
    }

    return 0;
}

int callback(void *NotUsed, int argc, char **argv, char **azColName) {
    
    NotUsed = 0;
    
    for (int i = 0; i < argc; i++) {

        printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
    }
    
    printf("\n");
    
    return 0;
}

void get_all_users(sqlite3 *db){
    // user_t *user_list;
    char *sql = "SELECT * FROM USERS";

    char *err_msg = 0;
    int rc = sqlite3_exec(db, sql, callback, 0, &err_msg);
    if (rc != SQLITE_OK ) {
        fprintf(stderr, "SQL error: %s\n", err_msg);
        sqlite3_free(err_msg);        
        // return 1;
    }
}

int main(){
    sqlite3 *db = load_db("test.db");

    user_t user;
    strcpy(user.email, "maria_souza@gmail.com");
    strcpy(user.first_name, "Maria");
    strcpy(user.last_name, "Souza");
    strcpy(user.city, "Campinas");
    strcpy(user.graduation_course, "Ciencia da Computacao");
    strcpy(user.graduation_year, "2018");
    strcpy(user.skills, "Ciencia dos Dados, Internet das Coisas, Computacao em Nuvem");

    add_user(db, user);
    get_all_users(db);

    sqlite3_close(db);
}

