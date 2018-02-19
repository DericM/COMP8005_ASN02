#include "client.h"

Client::Client(string host, int port) {
    // setup variables
    host_ = host;
    port_ = port;
    buflen_ = 1024;
    buf_ = new char[buflen_+1];
}

Client::~Client() {
}

void Client::run() {
    qInfo() << "Client::run";
    std::thread(&Client::start_thread, this).detach();
}

void Client::start_thread(){
    qInfo() << "Client::start_thread";
    // connect to the server and run echo program
    create();
    echo();
}

void Client::create() {
    qInfo() << "Client::create";
    struct sockaddr_in server_addr;

    // use DNS to get IP address
    struct hostent *hostEntry;
    hostEntry = gethostbyname(host_.c_str());
    if (!hostEntry) {
        //cout << "No such host name: " << host_ << endl;
        qCritical() << "Client::create" << "No such host name:" << host_.c_str();
        exit(-1);
    }

    // setup socket address structure
    memset(&server_addr,0,sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port_);
    memcpy(&server_addr.sin_addr, hostEntry->h_addr_list[0], hostEntry->h_length);

    // create socket
    server_ = socket(PF_INET,SOCK_STREAM,0);
    if (!server_) {
        //perror("socket");
        qCritical() << "Client::create" << "Socket Failure";
        exit(-1);
    }

    // connect to server
    if (connect(server_,(const struct sockaddr *)&server_addr,sizeof(server_addr)) < 0) {
        //perror("connect");
        qCritical() << "Client::create" << "Connection Failure";
        exit(-1);
    }
    qCritical() << "Client::create" << "Connected";
}

void Client::close_socket() {
    qInfo() << "Client::close_socket";
    close(server_);
}

void Client::echo() {
    qInfo() << "Client::echo";
    string line;

//    // loop to handle user interface
//    while (getline(cin,line)) {
//        // append a newline
//        line += "\n";
//        // send request
//        bool success = send_request(line);
//        // break if an error occurred
//        if (not success)
//            break;
//        // get a response
//        success = get_response();
//        // break if an error occurred
//        if (not success)
//            break;
//    }

    int i = 0;
    while(i < 1000){
        line = "Packet#" + std::to_string(i) +"\n";
        // send request
        bool success = send_request(line);
        // break if an error occurred
        if (not success)
            break;
        // get a response
        success = get_response();
        // break if an error occurred
        if (not success)
            break;
        i++;
    }

    close_socket();
}

bool Client::send_request(string request) {
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
    qInfo() << "Client::send_request" << request.substr(0, request.size()-1).c_str();
    return true;
}

bool Client::get_response() {
    string response = "";
    // read until we get a newline
    while (response.find("\n") == string::npos) {
        int nread = recv(server_,buf_,1024,0);
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
        response.append(buf_,nread);
    }
    // a better client would cut off anything after the newline and
    // save it in a cache
    qInfo() << "Client::get_response" << response.substr(0, response.size()-1).c_str();
    return true;
}

