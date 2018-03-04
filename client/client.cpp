#include "client.h"
#include "charts/session.h"

Client::Client(std::string host, int port, Session *session) :
    session_(session),
    host_(host),
    port_(port),
    buflen_(1024),
    buf_(new char[buflen_+1]),
    running_(true)
{

}

Client::~Client() {
}

void Client::run(int instances, int packetSize) {
    qInfo() << "Client::run";
    std::thread(&Client::instance_thread, this, instances, packetSize).detach();
}

void Client::stop() {
    running_ = false;
}

void Client::instance_thread(int instances, int packetSize) {
    qInfo() << "Client::instance_thread";
    for(int i=0; i<instances; i++) {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        std::thread(&Client::client_thread, this, packetSize).detach();
    }
}

void Client::client_thread(int packetSize) {
    qInfo() << "Client::client_thread";
    // connect to the server and run echo program
    session_->addClient();
    int server_;
    create(server_);
    echo(server_, packetSize);
    session_->removeClient();
}

void Client::create(int &server_) {
    qInfo() << "Client::create";
    struct sockaddr_in server_addr;

    // use DNS to get IP address
    struct hostent *hostEntry;
    hostEntry = gethostbyname(host_.c_str());
    if (!hostEntry) {
        qCritical() << "Client::create" << "No such host name:" << host_.c_str();
        exit(-1);
    }

    // setup socket address structure
    memset(&server_addr,0,sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port_);
    memcpy(&server_addr.sin_addr, hostEntry->h_addr_list[0], hostEntry->h_length);

    // create socket
    server_ = socket(AF_INET, SOCK_STREAM, 0);
    if (!server_) {
        qCritical() << "Client::create" << "Socket Failure";
        exit(-1);
    }

    // connect to server
    if (connect(server_,(const struct sockaddr *)&server_addr,sizeof(server_addr)) < 0) {
        qCritical() << "Client::create" << "Connection Failure";
        exit(-1);
    }
    qCritical() << "Client::create" << "Connected";
}

void Client::close_socket(int &server_) {
    qInfo() << "Client::close_socket";
    close(server_);
}


void Client::echo(int &server_, int packetSize) {
    qInfo() << "Client::echo";
    std::string line;

    int i = 0;
    while(running_){
        line = std::string(packetSize-1, 'x');
        line += "\n";
        //line = "Packet#" + std::to_string(i) +"\n";
        // send request
        bool success = send_request(server_, line);
        // break if an error occurred
        if (!success)
            break;
        session_->addDataSent(line.length());
        session_->addRequest();
        // get a response
        success = get_response(server_);
        // break if an error occurred
        if (!success)
            break;

        session_->addDataRecv(line.length());
        session_->addResponse();
        i++;
    }
    close_socket(server_);
}

bool Client::send_request(int &server_, std::string request) {
    // prepare to send request
    const char* ptr = request.c_str();
    int nleft = request.length();
    int nwritten;
    // loop to be sure it is all sent
    while (nleft) {
        if ((nwritten = send(server_, ptr, nleft, 0)) < 0) {
            if (errno == EINTR) {
                // the socket call was interrupted -- try again
                continue;
            } else {
                // an error occurred, so break out
                qCritical() << "Client::send_request" << "Write error";
                return false;
            }
        } else if (nwritten == 0) {
            // the socket is closed
            return false;
        }
        nleft -= nwritten;
        ptr += nwritten;
    }
    //qInfo() << "Client::send_request" << request.substr(0, request.size()-1).c_str();
    return true;
}

bool Client::get_response(int &server_) {
    std::string response = "";
    // read until we get a newline
    while (response.find("\n") == std::string::npos) {
        int nread = recv(server_, buf_, 1024, 0);
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
        response.append(buf_, nread);
    }
    // a better client would cut off anything after the newline and
    // save it in a cache
    //qInfo() << "Client::get_response" << response.substr(0, response.size()-1).c_str();
    return true;
}

