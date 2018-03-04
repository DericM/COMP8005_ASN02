#ifndef EPOLLSERVER_H
#define EPOLLSERVER_H

#include "server.h"

#include <sys/epoll.h>

#define EPOLL_QUEUE_LEN 256

class EpollServer : public Server {
public:
    EpollServer(int port, Session *session);
protected:
    void create();
    void serve();

private:

    int make_bound();
    int make_non_blocking(int sfd);

    int sfd, s;
    int efd;
    struct epoll_event event;
    struct epoll_event *events;
};

#endif // EPOLLSERVER_H
