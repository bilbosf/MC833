#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <sqlite3.h>
#include "userdb.h"
#include "server_send.h"

#define SERV_PORT 8080

void process_request(int new_fd, sqlite3* db);

int main(){
    sqlite3 *db = load_db("users.db"); //Database File

    int sock_fd; //socket file descriptor
    struct sockaddr_in servaddr; //sockets address structure

    //SOCK_DGRAM: UDP socket
    //third parameter 0: chooses the proper protocol for the given type
    sock_fd = socket (AF_INET, SOCK_DGRAM, 0);


    /*Fills server socket address structure */
    bzero(&servaddr, sizeof(servaddr));            // fill with zeros first
    servaddr.sin_family = AF_INET;                 //address family AF_INET, for IPv4
    servaddr.sin_addr.s_addr = htonl (INADDR_ANY); //fill internet address and convert to network byte order
    servaddr.sin_port = htons (SERV_PORT);         //fill port number and convert to network byte order

    /*Assign a type of socket address to a socket*/
    bind(sock_fd, (struct sockaddr*) &servaddr, sizeof(servaddr));

    while(1){
        process_request(sock_fd, db); /* process the request */
    }

    return (0);
}

/*Directs the command received to the proper function*/
void process_request(int new_fd, sqlite3* db){
    struct sockaddr_in cliaddr; //socket address structure
    socklen_t clilen;
    char buffer[MAXLINE] = {0};

    /*
    Receive a message from client socket.
    We designed MAXLINE to be sufficient to receive any message from client
    */
    if (recvfrom(new_fd, (char *)buffer, MAXLINE, 0, (struct sockaddr *) &cliaddr, &clilen) < 0) {
        printf("Error in receiving data.\n");
        return;
    }

    printf("Received request with command %c\n", buffer[0]);

    switch(buffer[0]) {
        case '1': //Listar todas as pessoas (email e nome) formadas em um determinado curso
            send_users_by_course(new_fd, cliaddr, clilen, db, buffer + 1);
            break;
        case '2': //Listar todas as pessoas (email e nome) que possuam uma determinada habilidade
            send_users_by_skill(new_fd, cliaddr, clilen, db, buffer + 1);
            break;
        case '3': //Listar todas as pessoas (email, nome e curso) formadas em um determinado ano
            send_users_by_year(new_fd, cliaddr, clilen, db, buffer + 1);
            break;
        case '4': //Listar todas as informações de todos os perfis
            send_all_info(new_fd, cliaddr, clilen, db);
            break;
        case '5': //Retornar informações de um perfil
            send_users_by_email(new_fd, cliaddr, clilen, db, buffer + 1);
            break;
        case '6':  //Cadastrar um novo perfil
            send_add_user(new_fd, cliaddr, clilen, db, buffer + 1);
            break;
        case '7':  //Remover um perfil
            send_remove_user(new_fd, cliaddr, clilen, db, buffer + 1);
            break;
    }
}
