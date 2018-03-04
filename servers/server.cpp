#include "server.h"

Server::Server(int port, Session *session) :
    session_(session),
    port_(port),
    buflen_(1024),
    buf_(new char[buflen_+1])
{

}

Server::~Server() {
    delete buf_;
}

void Server::run() {
    qInfo() << "Server::run";
    std::thread(&Server::start_thread, this).detach();
}

void Server::start_thread(){
    qInfo() << "Server::start_thread";
    // create and run the server
    create();
    serve();
}

void Server::create() {
    qInfo() << "Server::create";
    struct sockaddr_in server_addr;

    // setup socket address structure
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port_);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    // create socket
    server_ = socket(AF_INET, SOCK_STREAM, 0);
    if (!server_) {
        qCritical() << "Server::create" << "socket error";
        exit(-1);
    }

    // set socket to immediately reuse port when the application closes
    int reuse = 1;
    if (setsockopt(server_, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)) < 0) {
        qCritical() << "Server::create" << "setsockopt error";
        exit(-1);
    }

    // call bind to associate the socket with our local address and
    // port
    if (bind(server_, (const struct sockaddr *)&server_addr,sizeof(server_addr)) < 0) {
        qCritical() << "Server::create" << "bind error";
        exit(-1);
    }

    // convert the socket to listen for incoming connections
    if (listen(server_, SOMAXCONN) < 0) {
        qCritical() << "Server::create" << "listen error";
        exit(-1);
    }
    qInfo() << "Server::create" << "Connected";
}

void Server::close_socket() {
    qInfo() << "Server::close_socket";
    close(server_);
}


void Server::serve() {
    qInfo() << "Server::serve";
    // setup client
    int client;
    struct sockaddr_in client_addr;
    socklen_t clientlen = sizeof(client_addr);

      // accept clients
    while ((client = accept(server_, (struct sockaddr *)&client_addr,&clientlen)) > 0) {
        qInfo() << "Server::serve" << "accept";
        handle(client);
    }
    close_socket();
}

void Server::handle(int client) {
    session_->addClient();
    qInfo() << "Server::handle";
    // loop to handle all requests
    while (1) {
        // get a request
        std::string request = get_request(client);
        // break if client is done or an error occurred
        if (request.empty())
            break;
        session_->addDataRecv(request.length());
        session_->addRequest();
        // send response
        bool success = send_response(client,request);
        // break if an error occurred
        if (!success)
            break;
        session_->addDataSent(request.length());
        session_->addResponse();
    }
    session_->removeClient();
    close(client);
}

std::string Server::get_request(int client) {
    //qInfo() << "Server::get_request";
    std::string request = "";
    // read until we get a newline
    while (request.find("\n") == std::string::npos) {
        int nread = recv(client, buf_, 1024, 0);
        if (nread < 0) {
            if (errno == EINTR)
                // the socket call was interrupted -- try again
                continue;
            else
                // an error occurred, so break out
                return "";
        } else if (nread == 0) {
            // the socket is closed
            return "";
        }
        // be sure to use append in case we have binary data
        request.append(buf_,nread);
    }
    // a better server would cut off anything after the newline and
    // save it in a cache
    return request;
}

bool Server::send_response(int client, std::string response) {
    //qInfo() << "Server::send_response";
    // prepare to send response
    const char* ptr = response.c_str();
    int nleft = response.length();
    int nwritten;
    // loop to be sure it is all sent
    while (nleft) {
        if ((nwritten = send(client, ptr, nleft, 0)) < 0) {
            if (errno == EINTR) {
                // the socket call was interrupted -- try again
                continue;
            } else {
                // an error occurred, so break out
                perror("write");
                return false;
            }
        } else if (nwritten == 0) {
            // the socket is closed
            return false;
        }
        nleft -= nwritten;
        ptr += nwritten;
    }
    return true;
}
