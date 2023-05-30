#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <sqlite3.h>
#include <stdbool.h>
#include "userdb.h"
#include "server_send.h"

/*Format the response and send it to the client via socket*/
void send_response(int new_fd, struct sockaddr_in cliaddr, socklen_t clilen, user_list_t *list, bool city, bool course, bool year, bool skills) {
    char response[MAXLINE] = "\n";

    if(list == NULL || list->length == 0){
        char msg_null[50] = {0};
        sprintf(msg_null, "Nenhum resultado encontrado.\n\n%c", 0x04); //EOT - End of Transmission character
        sendto(new_fd, msg_null, strlen(msg_null), 0, (const struct sockaddr *) &cliaddr, clilen);
        return;
    }

    char tmp[USER_SKILLS_LEN + 15] = {0};
    sprintf(tmp, "Perfis encontrados: %d\n\n", list->length);
    strcat(response, tmp);

    for(int i = 0 ; i < list->length ; i++) {
        sprintf(tmp, "Email: %s\n", list->list[i].email);
        strcat(response, tmp);
        sprintf(tmp, "Nome: %s\tSobrenome: %s\n", list->list[i].first_name, list->list[i].last_name);
        strcat(response, tmp);
        if (city) {
            sprintf(tmp, "Residência: %s\n", list->list[i].city);
            strcat(response, tmp);
        }
        if (course) {
            sprintf(tmp, "Formação Acadêmica: %s\n", list->list[i].graduation_course);
            strcat(response, tmp);
        }
        if (year) {
            sprintf(tmp, "Ano de formatura: %s\n", list->list[i].graduation_year);
            strcat(response, tmp);
        }
        if (skills) {
            sprintf(tmp, "Habilidades: %s\n", list->list[i].skills);
            strcat(response, tmp);
        }
        strcat(response, "\n");

        if(i == list->length - 1){
            sprintf(tmp, "%c", 0x04); //EOT - End of Transmission character
            strcat(response, tmp);
        }

        /*Sends the data inside the buffer "response" to new_fd socket*/
        sendto(new_fd, response, strlen(response), 0, (const struct sockaddr *) &cliaddr, clilen);

        /*fills with zero the buffer "response"*/
        memset(response, 0, sizeof(response));
    }
}

void send_users_by_course(int new_fd, struct sockaddr_in cliaddr, socklen_t clilen, sqlite3* db, char* course){
    user_list_t *list = get_users_by_course(db, course);

    send_response(new_fd, cliaddr, clilen, list, false, false, false, false);

    free(list->list);
    free(list);
}

void send_users_by_skill(int new_fd, struct sockaddr_in cliaddr, socklen_t clilen, sqlite3* db, char* skill){
    user_list_t *list = get_users_by_skill(db, skill);

    send_response(new_fd, cliaddr, clilen, list, false, false, false, false);

    free(list->list);
    free(list);
}

void send_users_by_year(int new_fd,struct sockaddr_in cliaddr, socklen_t clilen, sqlite3* db, char* year){
    user_list_t *list = get_users_by_year(db, year);

    send_response(new_fd, cliaddr, clilen, list, false, true, false, false);

    free(list->list);
    free(list);
}

void send_all_info(int new_fd, struct sockaddr_in cliaddr, socklen_t clilen, sqlite3* db){
    user_list_t *list = get_all_users(db);

    send_response(new_fd, cliaddr, clilen, list, true, true, true, true);

    free(list->list);
    free(list);
}

void send_users_by_email(int new_fd, struct sockaddr_in cliaddr, socklen_t clilen, sqlite3* db, char* email){
    user_list_t *list = get_users_by_email(db, email);

    send_response(new_fd, cliaddr, clilen, list, true, true, true, true);

    free(list->list);
    free(list);
}

user_t parse_user(char *arg) {
    user_t user;
    strncpy(user.email,             arg + 0 * USER_FIELD_LEN, USER_FIELD_LEN);
    strncpy(user.first_name,        arg + 1 * USER_FIELD_LEN, USER_FIELD_LEN);
    strncpy(user.last_name,         arg + 2 * USER_FIELD_LEN, USER_FIELD_LEN);
    strncpy(user.city,              arg + 3 * USER_FIELD_LEN, USER_FIELD_LEN);
    strncpy(user.graduation_course, arg + 4 * USER_FIELD_LEN, USER_FIELD_LEN);
    strncpy(user.graduation_year,   arg + 5 * USER_FIELD_LEN, USER_FIELD_LEN);
    strncpy(user.skills,            arg + 6 * USER_FIELD_LEN, USER_SKILLS_LEN);
    return user;
}

void send_add_user(int new_fd, struct sockaddr_in cliaddr, socklen_t clilen, sqlite3* db, char* arg) {
    char response[MAXLINE] = {0};
    user_t new_user = parse_user(arg);

    if (add_user(db, new_user) != 0) {
        sprintf(response, "Falha ao adicionar usuário: %s\n\n%c", new_user.email, 0x04); //EOT - End of Transmission character
    } else {
        sprintf(response, "Usuário adicionado com sucesso: %s\n\n%c", new_user.email, 0x04); //EOT - End of Transmission character
    }

    sendto(new_fd, response, strlen(response), 0, (const struct sockaddr *) &cliaddr, clilen);
}

void send_remove_user(int new_fd, struct sockaddr_in cliaddr, socklen_t clilen, sqlite3* db, char* email) {
    char response[MAXLINE] = {0};

    if (remove_user(db, email) != 0) {
        sprintf(response, "Falha ao remover usuário: %s\n\n%c", email, 0x04); //EOT - End of Transmission character
    } else {
        sprintf(response, "Usuário removido com sucesso: %s\n\n%c", email, 0x04); //EOT - End of Transmission character
    }

    sendto(new_fd, response, strlen(response), 0, (const struct sockaddr *) &cliaddr, clilen);
}
