#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <unistd.h>

#define BUFFER_LEN 1024

int connect_to_server(char *ip, int port){
	int sock = 0;
	struct sockaddr_in serv_addr;
	
	
	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		printf("\n Socket creation error \n");
	}

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(port);

	if (inet_pton(AF_INET, ip, &serv_addr.sin_addr) <= 0) {
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

int send_request(char cmd, char* arg, char* ip, int port){
	char msg[BUFFER_LEN] = {0};
	bzero(msg, sizeof(msg));

	msg[0] = cmd;
	strncpy(&msg[1], arg, BUFFER_LEN - 1);

	int sock = 0;
	sock = connect_to_server(ip, port);
	if(sock == -1) return 1;
	send(sock, msg, BUFFER_LEN, 0);

	char buffer[BUFFER_LEN] = {0};

	/* Keeps receiving from recv() until detects end of message character: EOT */
	printf("Server response:\n");
	int end_of_message = 0;
	while(end_of_message == 0){
		if(recv(sock, buffer, BUFFER_LEN, 0) < 0) {
			printf("Error in receiving data.\n");
			return 1;
		}
		printf("%s", buffer);
		for(int i = 0; i < BUFFER_LEN; i++)
			if(buffer[i] == 0x04) end_of_message = 1; //0x04 EOT -End of Transmission special character
		bzero(buffer, sizeof(buffer)); //clear buffer with zeros
	}
	return 0;
}

void add_user(char* ip, int port){
    char cmd_code = '6';
	char buffer[1024];

	// Initializing buffer array with NULL
    memset(buffer, '\0', sizeof(buffer));

	int sock = 0;
	sock = connect_to_server(ip, port);
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
