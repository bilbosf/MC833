#ifndef NETSERVICE
#define NETSERVICE

#define BUFFER_LEN 1024

int connect_to_server();

int send_request(char cmd, char* arg, char* ip, int port);

void add_user();

#endif
