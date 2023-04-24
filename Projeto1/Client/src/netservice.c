#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <unistd.h>
#include "netservice.h"

/*Function that connects to the server via TCP connection, it needs the ip and port number*/
int connect_to_server(char *ip, int port){
	int sock_fd = 0; //socket file descriptor
	struct sockaddr_in serv_addr; //socket address structure
	
	//PF_INET: socket uses internet IPv4
    //SOCK_STREAM: TCP socket
    //third parameter 0: chooses the proper protocol for the given type
	if ((sock_fd = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
		printf("\n Socket creation error \n");
	}

	/*Fills server socket address structure */
    bzero(&serv_addr, sizeof(serv_addr)); // fill with zeros first
    serv_addr.sin_family = AF_INET;      //address family AF_INET, for IPv4
    serv_addr.sin_port = htons (port);   //fill port number and convert to network byte order

	//Converts the string ip received to numeric binary form and fills the server socket address structure
	if (inet_pton(AF_INET, ip, &serv_addr.sin_addr) <= 0) {
		printf("\nInvalid address/ Address not supported \n");
		return -1;
	}
	
	//If it succeeded in connecting to the server it returns the socket descriptor
	if (connect(sock_fd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
		printf("\nConnection Failed \n");
		return -1;
	}else{
		return sock_fd;
	}
}

/*Organizes the command and arguments in the buffer and sends the request to the server.
Prints the response received from server.*/
int send_request(char cmd, char* arg, char* ip, int port){
	char msg[BUFFER_LEN] = {0};
	bzero(msg, sizeof(msg));

	msg[0] = cmd;
	memcpy(&msg[1], arg, BUFFER_LEN - 1);

	int sock = 0;
	sock = connect_to_server(ip, port);
	if(sock == -1) return 1;
	send(sock, msg, BUFFER_LEN, 0);

	char buffer[BUFFER_LEN] = {0};

	/* Keeps receiving from recv() until detects end of message character: EOT */
	printf("\nResposta do servidor:\n");
	int end_of_message = 0;
	while(end_of_message == 0){
		if(recv(sock, buffer, BUFFER_LEN, 0) < 0) {
			printf("Error in receiving data.\n");
			return 1;
		}

		for(int i = 0; i < BUFFER_LEN; i++) {
			if(buffer[i] != 0x04){
				printf("%c", buffer[i]);
			} else {
				end_of_message = 1; //0x04 EOT -End of Transmission special character
			}
		}

		bzero(buffer, sizeof(buffer)); //clear buffer with zeros
	}
	return 0;
}