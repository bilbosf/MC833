#ifndef NETSERVICE
#define NETSERVICE

int connect_to_server();

int send_request(char cmd, char* arg, char* ip, int port);

void add_user();

#endif