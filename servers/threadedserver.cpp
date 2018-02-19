#include "threadedserver.h"

ThreadedServer::ThreadedServer(int port) : Server(port) {
}

void ThreadedServer::serve() {
    qInfo() << "ThreadedServer::serve";
    // setup client
    int client;
    struct sockaddr_in client_addr;
    socklen_t clientlen = sizeof(client_addr);

    // accept clients
    while ((client = accept(server_,(struct sockaddr *)&client_addr,&clientlen)) > 0) {
        qInfo() << "ThreadedServer::serve" << "accept";
        std::thread(&ThreadedServer::handle, this, client).detach();
    }
    close_socket();
}

