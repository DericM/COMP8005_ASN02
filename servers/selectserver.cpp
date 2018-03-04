#include "selectserver.h"

SelectServer::SelectServer(int port, Session *session) : Server(port, session) {
}

void SelectServer::serve() {
    qInfo() << "SelectServer::serve";

    maxfd = server_;	// initialize
    maxi = -1;		    // index into client[] array

    for (int i = 0; i < FD_SETSIZE; i++)
        client[i] = -1;             // -1 indicates available entry

    FD_ZERO(&allset);
    FD_SET(server_, &allset);

    while (1) {
        rset = allset;               // structure assignment
        nready = select(maxfd + 1, &rset, NULL, NULL, NULL);

        if (FD_ISSET(server_, &rset)) {
            client_len = sizeof(client_addr);
            if ((new_sd = accept(server_, (struct sockaddr *) &client_addr, &client_len)) == -1){
                qCritical() << "SelectServer::serve" << "accept error";
                exit(1);
            }

            //printf(" Remote Address:  %s\n", inet_ntoa(client_addr.sin_addr));
            int i;
            for (i = 0; i < FD_SETSIZE; i++)
            if (client[i] < 0) {
                client[i] = new_sd;	// save descriptor
                break;
            }
            if (i == FD_SETSIZE) {
                qCritical() << "SelectServer::serve" << "Too many clients";
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

        for (int i = 0; i <= maxi; i++)	{ //check all clients for data
            if ((sockfd = client[i]) < 0)
                continue;

            if (FD_ISSET(sockfd, &rset)) {
                std::thread(&SelectServer::handle, this, sockfd).detach();
                //handle(sockfd);

                FD_CLR(sockfd, &allset);
                    client[i] = -1;
            }

            if (--nready <= 0)
                break; // no more readable descriptors
        }
    }

    close_socket();
}

