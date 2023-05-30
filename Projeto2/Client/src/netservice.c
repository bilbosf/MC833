#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h> 
#include <stdlib.h>
#include <unistd.h>
#include "netservice.h"

/*Organizes the command and arguments in the buffer and sends the request to the server.
Prints the response received from server.*/
int send_request(char cmd, char* arg, char* ip, int port){

	int sockfd;
    struct sockaddr_in servaddr;
   
    // Creating socket file descriptor
    if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }

	// Set a receive time-out for the socket recvfrom()
	struct timeval read_timeout;
	read_timeout.tv_sec = 0;
	read_timeout.tv_usec = TIMEOUT_UCSECONDS;
	setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, &read_timeout, sizeof read_timeout);
   
    memset(&servaddr, 0, sizeof(servaddr));
       
    // Filling server information
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(port);

	//Converts the string ip received to numeric binary form and fills the server socket address structure
	if (inet_pton(AF_INET, ip, &servaddr.sin_addr) <= 0) {
		printf("\nInvalid address/ Address not supported \n");
		return -1;
	}

	char msg[MAXLINE] = {0};
	bzero(msg, sizeof(msg));

	msg[0] = cmd;
	memcpy(&msg[1], arg, MAXLINE - 1);

	
	sendto(sockfd, (const char *)msg, strlen(msg), 0, (const struct sockaddr *) &servaddr, sizeof(servaddr));

	char buffer[MAXLINE] = {0};

	printf("\nResposta do servidor:\n");

	int tries = 0;
	while(1){
		recvfrom(sockfd, (char *)buffer, MAXLINE, 0, NULL, NULL); //Blocking

		if(strlen(buffer) == 0) tries++;

		if(tries >= MAX_TRIES) break;

		for(int i = 0; i < MAXLINE; i++) {
			if(buffer[i] != 0x04){
				printf("%c", buffer[i]);
			} 
		}

		bzero(buffer, sizeof(buffer)); //clear buffer with zeros
	}

	return 0;
}