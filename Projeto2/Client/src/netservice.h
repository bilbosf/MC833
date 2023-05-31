#ifndef NETSERVICE
#define NETSERVICE

#define BUFFER_LEN 1024
#define MAXLINE 1024
#define MAX_TRIES 2
#define TIMEOUT_UCSECONDS 250000

int connect_to_server();

int send_request(char cmd, char* arg, char* ip, int port, char* image_path);

void add_user();

#endif
