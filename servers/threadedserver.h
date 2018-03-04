#ifndef THREADEDSERVER_H
#define THREADEDSERVER_H

#include "server.h"

class ThreadedServer : public Server {
public:
    ThreadedServer(int port, Session *session);
protected:
    void serve();
};

#endif // THREADEDSERVER_H
