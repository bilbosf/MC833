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

#define SERV_PORT 8081
#define LISTENQ 5

void process_request(int new_fd);
 
int main(){

    sqlite3 *db = load_db("users.db");

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
            process_request(new_fd); /* process the request */
            exit (0);
        }
        close(new_fd); /* parent closes connected socket */
    }

    return (0);
}

void process_request(int new_fd){
    char buffer[1024] = {0};
    char response[1024] = {0};
    // Initializing buffer array with NULL
    memset(buffer, '\0', sizeof(buffer));

    if (recv(new_fd, buffer, 1024, 0) < 0) {
        printf("Error in receiving data.\n");
    }else {
        switch(buffer[0]) {
            case '1':
				strcpy(response, "resultado op 1");
				break;
			case '2':
				strcpy(response, "resultado op 2");
				break;
			case '3':
				strcpy(response, "resultado op 3");
				break;
			case '4':
				strcpy(response, "resultado op 4");
				break;
			case '5':
				strcpy(response, "resultado op 5");
				break;
			case '6':
				strcpy(response, "resultado op 6");
				break;
			case '7':
				strcpy(response, "resultado op 7");
				break;
        }

        send(new_fd, response, strlen(response), 0);
    }
}