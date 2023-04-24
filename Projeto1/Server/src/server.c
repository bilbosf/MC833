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
#define LISTENQ 5

void process_request(int new_fd, sqlite3* db);

int main(){
    sqlite3 *db = load_db("users.db"); //Database File

    int sock_fd, new_fd; //sockets file descriptors
    pid_t childpid;      //pid of the process that handles the request
    socklen_t clilen;    //platform independent type for socket address structure length

    struct sockaddr_in cliaddr, servaddr; //sockets address structure

    //PF_INET: socket uses internet IPv4
    //SOCK_STREAM: TCP socket
    //third parameter 0: chooses the proper protocol for the given type
    sock_fd = socket (PF_INET, SOCK_STREAM, 0);


    /*Fills server socket address structure */
    bzero(&servaddr, sizeof(servaddr));            // fill with zeros first
    servaddr.sin_family = AF_INET;                 //address family AF_INET, for IPv4
    servaddr.sin_addr.s_addr = htonl (INADDR_ANY); //fill internet address and convert to network byte order
    servaddr.sin_port = htons (SERV_PORT);         //fill port number and convert to network byte order

    /*Assign a type of socket address to a socket*/
    bind(sock_fd, (struct sockaddr*) &servaddr, sizeof(servaddr));

    /*Announces that it is ready to receive a maximum of LISTENQ connections
    on the incoming queue*/
    listen(sock_fd, LISTENQ);
    printf("Server listening on port %d\n", SERV_PORT);

    while(1){
        clilen = sizeof(cliaddr); //fills length by size of client sockaddr_in

        /*new_fd receives new socket descriptor created by OS. 
        cliaddr stores information about protocol address from client */
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

/*Directs the command received to the proper function*/
void process_request(int new_fd, sqlite3* db){
    char buffer[BUFFER_LEN] = {0};

    /*
    Receive a message from client socket.
    Because flags is set to zero, if a message is too long to fit in the supplied buffer
     the excess bytes shall be discarded.
    We designed BUFFER_LEN to be sufficient to receive any message from client
    */
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
