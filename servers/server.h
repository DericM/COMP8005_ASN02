#ifndef SERVER_H
#define SERVER_H

#include <errno.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <thread>
#include <string>
#include <QDebug>
#include "charts/session.h"

class Server {
public:
    Server(int port, Session *session);
    ~Server();
    void run();

protected:
    void handle(int);
    void close_socket();
    virtual void create();
    virtual void serve();

    Session *session_;
    int server_;
    int port_;

private:
    void start_thread();
    std::string get_request(int);
    bool send_response(int, std::string);

    int buflen_;
    char* buf_;
};

#endif // SERVER_H
