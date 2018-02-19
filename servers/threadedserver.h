#ifndef THREADEDSERVER_H
#define THREADEDSERVER_H

#include "server.h"

class ThreadedServer : public Server{
public:
    ThreadedServer(int port);

protected:
    void serve();

};

#endif // THREADEDSERVER_H
