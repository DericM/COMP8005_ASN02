#ifndef SELECTSERVER_H
#define SELECTSERVER_H

#include "server.h"
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <strings.h>
#include <arpa/inet.h>
#include <unistd.h>

class SelectServer : public Server {
public:
    SelectServer(int port, Session *session);
protected:
    void serve();
private:
    int maxi;
    int nready;
    int new_sd;
    int sockfd;
    int maxfd;
    int client[FD_SETSIZE];
    socklen_t client_len;
    struct sockaddr_in client_addr;
    fd_set rset;
    fd_set allset;
};

#endif // SELECTSERVER_H
