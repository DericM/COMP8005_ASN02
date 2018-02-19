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

class Server {
public:
    Server(int port);
    ~Server();
    void run();

protected:
    void handle(int);
    void close_socket();
    virtual void serve()=0;

    int server_;

private:
    void start_thread();
    void create();
    std::string get_request(int);
    bool send_response(int, std::string);

    int port_;
    int buflen_;
    char* buf_;
};

#endif // SERVER_H
