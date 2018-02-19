#ifndef SELECTSERVER_H
#define SELECTSERVER_H

#include "server.h"

class SelectServer : public Server {
public:
    SelectServer(int port);

private:
    void serve();
};

#endif // SELECTSERVER_H
