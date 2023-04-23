#ifndef SERVER_SEND
#define SERVER_SEND

#define BUFFER_LEN 1024

void send_users_by_course(int new_fd, sqlite3* db, char* course);

void send_users_by_skill(int new_fd, sqlite3* db, char* skill);

void send_users_by_year(int new_fd, sqlite3* db, char* year);

void send_all_info(int new_fd, sqlite3* db);

void send_users_by_email(int new_fd, sqlite3* db, char* email);

void send_add_user(int new_fd, sqlite3* db, char* arg);

void send_remove_user(int new_fd, sqlite3* db, char* email);

#endif
