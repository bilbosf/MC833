#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <sys/time.h>
#include <sqlite3.h>
#include <stdbool.h>
#include "userdb.h"
#include "server_send.h"

#define SERV_PORT 8080
#define LISTENQ 5

void process_request(int new_fd, sqlite3* db);

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
    printf("Server listening on port %d\n", SERV_PORT);

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

    if (recv(new_fd, buffer, BUFFER_LEN, 0) < 0) {
        printf("Error in receiving data.\n");
        return;
    }

    printf("Received request with command %c\n", buffer[0]);

    switch(buffer[0]) {
        case '1': //Listar todas as pessoas (email e nome) formadas em um determinado curso
            send_users_by_course(new_fd, db, buffer + 1);
            break;
        case '2': //Listar todas as pessoas (email e nome) que possuam uma determinada habilidade
            send_users_by_skill(new_fd, db, buffer + 1);
            break;
        case '3': //Listar todas as pessoas (email, nome e curso) formadas em um determinado ano
            send_users_by_year(new_fd, db, buffer + 1);
            break;
        case '4': //Listar todas as informações de todos os perfis
            send_all_info(new_fd, db);
            break;
        case '5': //Retornar informações de um perfil
            send_users_by_email(new_fd, db, buffer + 1);
            break;
        case '6':  //Cadastrar um novo perfil
            send_add_user(new_fd, db, buffer + 1);
            break;
        case '7':  //Remover um perfil
            send_remove_user(new_fd, db, buffer + 1);
            break;
    }
}
