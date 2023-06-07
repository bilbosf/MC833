#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h> 
#include <stdlib.h>
#include <unistd.h>
#include "netservice.h"

void download_img(int sockfd, char* name);
void upload_img(int sockfd, struct sockaddr_in servaddr, socklen_t servlen, char* path);

/*Organizes the command and arguments in the buffer and sends the request to the server.
Prints the response received from server.*/
int send_request(char cmd, char* arg, char* ip, int port, char* image_path){

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

	
	sendto(sockfd, (const char *)msg, MAXLINE, 0, (const struct sockaddr *) &servaddr, sizeof(servaddr));

	if(cmd == '8'){ // Image upload
		upload_img(sockfd, servaddr, sizeof(servaddr), image_path);
		return 0;
	}
	if(cmd == '9'){ // Image download
		download_img(sockfd, arg);
		return 0;
	}

	printf("\nResposta do servidor:\n");

	int tries = 0;
	char buffer[MAXLINE] = {0};
	while(1){
		recvfrom(sockfd, (char *)buffer, MAXLINE, 0, NULL, NULL); 

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

void download_img(int sockfd, char* name){
	FILE *image_file;
	size_t total_bytes_received = 0;
	char buffer[MAXLINE];
	char filename[MAXLINE] = {0};

	sprintf(filename, "perfil_%s.jpg", name);

	image_file = fopen(filename, "w"); 
    if (image_file == NULL) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

	while (1) {
        ssize_t bytesRead = recvfrom(sockfd, buffer, MAXLINE, 0, NULL, NULL);
        if (bytesRead <= 0) {
			// End of transmission
            break;
        }

        // Write received data to file
        size_t bytesWritten = fwrite(buffer, sizeof(char), bytesRead, image_file);
        if (bytesWritten != bytesRead) {
            perror("Error writing to file");
            exit(EXIT_FAILURE);
        }

        total_bytes_received += bytesWritten;
    }

    fclose(image_file);

	if(total_bytes_received == 0){
		printf("Erro ao receber imagem, ou imagem não existe no banco de dados.\n");
		remove(filename); //exclui o arquivo de foto vazio
	}else{
		printf("Imagem recebida com sucesso. Total de bytes recebidos: %ld\n", total_bytes_received);
	}
}

void upload_img(int sockfd, struct sockaddr_in servaddr, socklen_t servlen, char* path) {
    FILE* fp = fopen(path, "rb");
    if (fp == NULL) {
        perror("Erro ao ler o arquivo");
        exit(1);
    }

    char buffer[MAXLINE];
    int bytes_read;
    while ((bytes_read = fread(buffer, 1, sizeof(buffer), fp)) > 0) {
        if ((sendto(sockfd, buffer, bytes_read, 0, (const struct sockaddr *) &servaddr, servlen) < 0)) {
            printf("Erro ao enviar os dados para o usuario");
            return;
        }
        bzero(buffer, MAXLINE);
    }

    printf("Imagem enviada.\n");
    fclose(fp);
}