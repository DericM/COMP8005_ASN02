#include "selectserver.h"

SelectServer::SelectServer(int port) : Server(port) {
}

void SelectServer::serve() {

    //***select stuff
//    int maxfd, maxi;
//    int client[FD_SETSIZE];

//    maxfd	= listen_sd;	// initialize
//    maxi	= -1;		// index into client[] array

//    for (i = 0; i < FD_SETSIZE; i++)
//            client[i] = -1;             // -1 indicates available entry
//    FD_ZERO(&allset);
//    FD_SET(listen_sd, &allset);

    //***********

    // setup client
    int client;
    struct sockaddr_in client_addr;
    socklen_t clientlen = sizeof(client_addr);

      // accept clients
    while ((client = accept(server_,(struct sockaddr *)&client_addr,&clientlen)) > 0) {
        std::thread(&SelectServer::handle, this, client).detach();
    }
    close_socket();
}


void SelectServer::bleh(){
    int i, maxi, nready;
    int new_sd, sockfd, client_len, maxfd, client[FD_SETSIZE];
    struct sockaddr_in client_addr;
    fd_set rset, allset;

    maxfd = server_;	// initialize
    maxi = -1;		    // index into client[] array

    for (i = 0; i < FD_SETSIZE; i++)
        client[i] = -1;             // -1 indicates available entry

    FD_ZERO(&allset);
    FD_SET(server_, &allset);

    while (TRUE) {
        rset = allset;               // structure assignment
        nready = select(maxfd + 1, &rset, NULL, NULL, NULL);

        if (FD_ISSET(server_, &rset)) {
            client_len = sizeof(client_addr);
            if ((new_sd = accept(server_, (struct sockaddr *) &client_addr, &client_len)) == -1){
                qFatal() << "accept error";
                exit(1);
            }

            printf(" Remote Address:  %s\n", inet_ntoa(client_addr.sin_addr));

            for (i = 0; i < FD_SETSIZE; i++)
            if (client[i] < 0) {
                client[i] = new_sd;	// save descriptor
                break;
            }
            if (i == FD_SETSIZE) {
                qCritical() << "Too many clients";
                exit(1);
            }

            FD_SET (new_sd, &allset);     // add new descriptor to set
            if (new_sd > maxfd)
                maxfd = new_sd;	// for select

            if (i > maxi)
                maxi = i;	// new max index in client[] array

            if (--nready <= 0)
                continue;	// no more readable descriptors
        }

        for (i = 0; i <= maxi; i++)	{ //check all clients for data
            if ((sockfd = client[i]) < 0)
                continue;

            if (FD_ISSET(sockfd, &rset)) {
                handle(client);

                FD_CLR(sockfd, &allset);
                    client[i] = -1;
            }

            if (--nready <= 0)
                break;        // no more readable descriptors
        }
    }
}

