#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <unistd.h>

#define PORT 8081
#define IP "127.0.0.1"

int connect_to_server(){
	int sock = 0;
	struct sockaddr_in serv_addr;
	
	
	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		printf("\n Socket creation error \n");
	}

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(PORT);

	if (inet_pton(AF_INET, IP, &serv_addr.sin_addr) <= 0) {
		printf("\nInvalid address/ Address not supported \n");
		return -1;
	}
	
	if (connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
		printf("\nConnection Failed \n");
		return -1;
	}else{
		return sock;
	}
}

void list_users_by_course(){
    char cmd_code = '1';
	char buffer[1024];

	// Initializing buffer array with NULL
    memset(buffer, '\0', sizeof(buffer));

	int sock = 0;
	sock = connect_to_server();
	if(sock == -1) return;
	send(sock, &cmd_code, 1, 0);

	// recv() receives the message from server and stores in buffer
    if(recv(sock, buffer, 1024, 0) < 0) {
        printf("Error in receiving data.\n");
    }else {
        printf("Server: %s\n", buffer);
        bzero(buffer, sizeof(buffer));
    }
}

void list_users_by_skill(){
    char cmd_code = '2';
	char buffer[1024];

	// Initializing buffer array with NULL
    memset(buffer, '\0', sizeof(buffer));

	int sock = 0;
	sock = connect_to_server();
	if(sock == -1) return;
	send(sock, &cmd_code, 1, 0);

	// recv() receives the message from server and stores in buffer
    if(recv(sock, buffer, 1024, 0) < 0) {
        printf("Error in receiving data.\n");
    }else {
        printf("Server: %s\n", buffer);
        bzero(buffer, sizeof(buffer));
    }
}

void list_users_by_year(){
    char cmd_code = '3';
	char buffer[1024];

	// Initializing buffer array with NULL
    memset(buffer, '\0', sizeof(buffer));

	int sock = 0;
	sock = connect_to_server();
	if(sock == -1) return;
	send(sock, &cmd_code, 1, 0);

	// recv() receives the message from server and stores in buffer
    if(recv(sock, buffer, 1024, 0) < 0) {
        printf("Error in receiving data.\n");
    }else {
        printf("Server: %s\n", buffer);
        bzero(buffer, sizeof(buffer));
    }
}

void list_all_users(){
    char cmd_code = '4';
	char buffer[1024];

	// Initializing buffer array with NULL
    memset(buffer, '\0', sizeof(buffer));

	int sock = 0;
	sock = connect_to_server();
	if(sock == -1) return;
	send(sock, &cmd_code, 1, 0);

	// recv() receives the message from server and stores in buffer
    if(recv(sock, buffer, 1024, 0) < 0) {
        printf("Error in receiving data.\n");
    }else {
        printf("Server: %s\n", buffer);
        bzero(buffer, sizeof(buffer));
    }
}

void list_user_by_email(){
    char cmd_code = '5';
	char buffer[1024];

	// Initializing buffer array with NULL
    memset(buffer, '\0', sizeof(buffer));

	int sock = 0;
	sock = connect_to_server();
	if(sock == -1) return;
	send(sock, &cmd_code, 1, 0);

	// recv() receives the message from server and stores in buffer
    if(recv(sock, buffer, 1024, 0) < 0) {
        printf("Error in receiving data.\n");
    }else {
        printf("Server: %s\n", buffer);
        bzero(buffer, sizeof(buffer));
    }
}

void add_user(){
    char cmd_code = '6';
	char buffer[1024];

	// Initializing buffer array with NULL
    memset(buffer, '\0', sizeof(buffer));

	int sock = 0;
	sock = connect_to_server();
	if(sock == -1) return;
	send(sock, &cmd_code, 1, 0);

	// recv() receives the message from server and stores in buffer
    if(recv(sock, buffer, 1024, 0) < 0) {
        printf("Error in receiving data.\n");
    }else {
        printf("Server: %s\n", buffer);
        bzero(buffer, sizeof(buffer));
    }
}

void remove_user_by_email(){
    char cmd_code = '7';
	char buffer[1024];

	// Initializing buffer array with NULL
    memset(buffer, '\0', sizeof(buffer));

	int sock = 0;
	sock = connect_to_server();
	if(sock == -1) return;
	send(sock, &cmd_code, 1, 0);

	// recv() receives the message from server and stores in buffer
    if(recv(sock, buffer, 1024, 0) < 0) {
        printf("Error in receiving data.\n");
    }else {
        printf("Server: %s\n", buffer);
        bzero(buffer, sizeof(buffer));
    }
}