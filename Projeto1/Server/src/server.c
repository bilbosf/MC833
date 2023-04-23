#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <sys/time.h>
#include "userdb.h"
#include <sqlite3.h>

#define SERV_PORT 8080
#define LISTENQ 5
#define BUFFER_LEN 1024

void process_request(int new_fd, sqlite3* db);
void send_all_info(int new_fd, sqlite3* db);
void send_users_by_course(int new_fd, sqlite3* db, char* course);
 
int main(){

    sqlite3 *db = load_db("test.db");

    int sock_fd, new_fd;
    pid_t childpid;
    socklen_t clilen; 

    struct sockaddr_in cliaddr, servaddr;
    sock_fd= socket (PF_INET, SOCK_STREAM, 0);
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl (INADDR_ANY);
    servaddr.sin_port = htons (SERV_PORT);
    bind(sock_fd, (struct sockaddr*) &servaddr, sizeof(servaddr));
    listen(sock_fd, LISTENQ);
    printf("server listening\n");

    while(1){
        clilen = sizeof(cliaddr);
        new_fd = accept(sock_fd, (struct sockaddr*) &cliaddr, &clilen);
        if ( (childpid = fork()) == 0) { /* child process */
            close(sock_fd); /* close listening socket */
            process_request(new_fd, db); /* process the request */
            exit (0);
        }
        close(new_fd); /* parent closes connected socket */
    }

    return (0);
}

void process_request(int new_fd, sqlite3* db){
    char buffer[BUFFER_LEN] = {0};
    char response[BUFFER_LEN] = {0};
    // Initializing buffer array with NULL
    memset(buffer, '\0', sizeof(buffer));

    if (recv(new_fd, buffer, BUFFER_LEN, 0) < 0) {
        printf("Error in receiving data.\n");
    }else {
        switch(buffer[0]) {
            case '1': //Listar todas as pessoas (email e nome) formadas em um determinado curso
                send_users_by_course(new_fd, db, buffer+1);
				break;
			case '2': //Listar todas as pessoas (email e nome) que possuam uma determinada habilidade
				strcpy(response, "resultado op 2");
                send(new_fd, response, strlen(response), 0);
				break;
			case '3': //Listar todas as pessoas (email, nome e curso) formadas em um determinado ano
				strcpy(response, "resultado op 3");
                send(new_fd, response, strlen(response), 0);
				break;
			case '4': //Listar todas as informações de todos os perfis
				send_all_info(new_fd, db);
				break;
			case '5': //Retornar informações de um perfil
				strcpy(response, "resultado op 5");
                send(new_fd, response, strlen(response), 0);
				break;
			case '6':  //Cadastrar um novo perfil
				strcpy(response, "resultado op 6");
                send(new_fd, response, strlen(response), 0);
				break;
			case '7':  //Remover um perfil
				strcpy(response, "resultado op 7");
                send(new_fd, response, strlen(response), 0);
				break;
        }
    }
}

void send_all_info(int new_fd, sqlite3* db){
    char response[BUFFER_LEN] = {0};
    user_list_t *list = get_all_users(db);

    if(list == NULL || list->length == 0){
        char msg_null[35] = {0};
        sprintf(msg_null, "Nao há usuários cadastrados.\n %c", 0x04);
        send(new_fd, msg_null, strlen(msg_null), 0);
        return;
    }

    for(int i = 0 ; i < list->length ; i++) {
        char tmp[215] = {0};
        sprintf(tmp, "\nEmail: %s\n", list->list[i].email);
        strcat(response, tmp);
        sprintf(tmp, "Nome: %s Sobrenome: %s\n", list->list[i].first_name, list->list[i].last_name);
        strcat(response, tmp);
        sprintf(tmp, "Residência: %s\n", list->list[i].city);
        strcat(response, tmp);
        sprintf(tmp, "Formação Acadêmica: %s\n", list->list[i].graduation_course);
        strcat(response, tmp);
        sprintf(tmp, "Ano de formatura: %s\n", list->list[i].graduation_year);
        strcat(response, tmp);
        sprintf(tmp, "Habilidades: %s\n\n", list->list[i].skills);
        strcat(response, tmp);

        if(i == list->length - 1){
            sprintf(tmp, "%c", 0x04); //EOT - End of Transmission character
            strcat(response, tmp);
        }

        send(new_fd, response, strlen(response), 0);
        memset(response, 0, sizeof(response));
    }

    free(list->list);
    free(list);
}

void send_users_by_course(int new_fd, sqlite3* db, char* course){
    char response[BUFFER_LEN] = {0};
    user_list_t *list = get_users_by_course(db, course);

    printf("course: %s\n", course);

    printf("list->length: %d\n", list->length);

    if(list == NULL || list->length == 0){
        char msg_null[24] = {0};
        sprintf(msg_null, "Nao encontrado.\n %c", 0x04);
        send(new_fd, msg_null, strlen(msg_null), 0);
        return;
    }

    for(int i = 0 ; i < list->length ; i++) {
        char tmp[215] = {0};
        sprintf(tmp, "\nEmail: %s\n", list->list[i].email);
        strcat(response, tmp);
        sprintf(tmp, "Nome: %s Sobrenome: %s\n", list->list[i].first_name, list->list[i].last_name);
        strcat(response, tmp);

        if(i == list->length - 1){
            sprintf(tmp, "%c", 0x04); //EOT - End of Transmission character
            strcat(response, tmp);
        }

        printf("%s", response);
        send(new_fd, response, strlen(response), 0);
        memset(response, 0, sizeof(response));
    }

    free(list->list);
    free(list);
}