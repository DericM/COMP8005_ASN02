#ifndef CLIENT_H
#define CLIENT_H

#include <errno.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>

#include <fstream>
#include <iostream>
#include <string>
#include <thread>

#include <QDebug>

#include "charts/session.h"

class Client {
public:
    Client(std::string host, int port, Session *session);
    ~Client();

    void run(int, int);
    void stop();

private:
    virtual void create(int&);
    virtual void close_socket(int&);
    void instance_thread(int, int);
    void client_thread(int);
    void echo(int&, int);
    bool send_request(int&, std::string);
    bool get_response(int&);


    Session *session_;
    std::string host_;
    int port_;
    int buflen_;
    char* buf_;
    bool running_;
};

#endif // CLIENT_H
