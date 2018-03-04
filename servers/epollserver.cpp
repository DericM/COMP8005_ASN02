#include "epollserver.h"

#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <sys/epoll.h>

#define MAXEVENTS 128

EpollServer::EpollServer(int port, Session *session) : Server(port, session) {
}

void EpollServer::create(){
    qInfo() << "EpollServer::create()";

    sfd = make_bound();
    if (sfd == -1)
        abort();

    s = make_non_blocking(sfd);
    if (s == -1)
        abort();

    s = listen (sfd, SOMAXCONN);
    if (s == -1) {
        qWarning() << "EpollServer:: listen error";
        abort();
    }

    efd = epoll_create1 (0);
    if (efd == -1) {
        qWarning() << "EpollServer:: epoll_create error";
        abort();
    }

    event.data.fd = sfd;
    event.events = EPOLLIN | EPOLLET;
    s = epoll_ctl (efd, EPOLL_CTL_ADD, sfd, &event);
    if (s == -1) {
        qWarning() << "EpollServer:: epoll_ctl error";
        abort();
    }

    /* Buffer where events are returned */
    events = (epoll_event*)calloc (MAXEVENTS, sizeof event);
}


void EpollServer::serve(){
    qInfo() << "EpollServer::serve()";

    /* The event loop */
    while (1) {
        int n, i;

        n = epoll_wait (efd, events, MAXEVENTS, -1);
        for (i = 0; i < n; i++) {
            if ((events[i].events & EPOLLERR) ||
                    (events[i].events & EPOLLHUP) ||
                    (!(events[i].events & EPOLLIN)))
            {
                /* An error has occured on this fd, or the socket is not
                   ready for reading (why were we notified then?) */
                qWarning() << "EpollServer:: epoll error";
                close(events[i].data.fd);
                continue;
            }
            else if (sfd == events[i].data.fd) {
            /* We have a notification on the listening socket, which
               means one or more incoming connections. */
                while(1) {
                    struct sockaddr in_addr;
                    socklen_t in_len;
                    int infd;
                    char hbuf[NI_MAXHOST], sbuf[NI_MAXSERV];

                    in_len = sizeof in_addr;
                    infd = accept (sfd, &in_addr, &in_len);
                    qInfo() << "EpollServer::accept()";
                    if (infd == -1) {
                        if ((errno == EAGAIN) ||
                            (errno == EWOULDBLOCK))
                        {
                            /* We have processed all incoming connections. */
                            break;
                        }
                        else {
                            qWarning() << "EpollServer:: accept error";
                            break;
                        }
                    }

                    s = getnameinfo (&in_addr, in_len,
                                   hbuf, sizeof hbuf,
                                   sbuf, sizeof sbuf,
                                   NI_NUMERICHOST | NI_NUMERICSERV);
                    if (s == 0) {
                        qInfo() << "EpollServer::AcceptedConnection()";
                    }

                    /* Make the incoming socket non-blocking and add it to the
                     list of fds to monitor. */
                    s = make_non_blocking (infd);
                    if (s == -1)
                        abort ();

                    event.data.fd = infd;
                    event.events = EPOLLIN | EPOLLET;
                    s = epoll_ctl (efd, EPOLL_CTL_ADD, infd, &event);
                    if (s == -1) {
                        qWarning() << "EpollServer:: epoll_ctl error";
                        abort();
                    }
                    session_->addClient();
                }
                continue;
            }
            else {
                /* We have data on the fd waiting to be read. Read and
                 display it. We must read whatever data is available
                 completely, as we are running in edge-triggered mode
                 and won't get a notification again for the same
                 data. */

                int done = 0;

                while(1) {
                    ssize_t count;
                    char buf[1024];

                    count = read (events[i].data.fd, buf, 1024);
                    if (count == -1) {
                        // If errno == EAGAIN, that means we have read all data. So go back to the main loop.
                        if (errno != EAGAIN) {
                            qWarning() << "EpollServer:: read error";
                            done = 1;
                        }
                        break;
                    }
                    else if (count == 0)
                    {
                        // End of file. The remote has closed the connection.
                        done = 1;
                        break;
                    }
                    session_->addDataRecv(count);
                    session_->addRequest();
                    // Write the buffer to standard output
                    s = write (events[i].data.fd, buf, count);
                    if (s == -1) {
                        qWarning() << "EpollServer:: write error";
                        abort ();
                    }
                    session_->addDataSent(count);
                    session_->addResponse();
                }

                if (done) {
                    //printf ("Closed connection on descriptor %d\n", events[i].data.fd);
                    qInfo() << "EpollServer::ClosedConnection()";

                    // Closing the descriptor will make epoll remove it
                    //from the set of descriptors which are monitored.
                    close (events[i].data.fd);
                    session_->removeClient();
                }


            }
        }
    }

    free (events);
    close (sfd);
}

int EpollServer::make_bound() {
    qInfo() << "EpollServer::make_bound()";

    std::string port = std::to_string(port_);

    struct addrinfo hints;
    struct addrinfo *result, *rp;
    int s, sfd;

    memset (&hints, 0, sizeof (struct addrinfo));
    hints.ai_family = AF_UNSPEC;     /* Return IPv4 and IPv6 choices */
    hints.ai_socktype = SOCK_STREAM; /* We want a TCP socket */
    hints.ai_flags = AI_PASSIVE;     /* All interfaces */

    s = getaddrinfo (NULL, port.c_str(), &hints, &result);
    if (s != 0) {
        //fprintf (stderr, "getaddrinfo: %s\n", gai_strerror (s));
        qInfo() << "EpollServer:: getaddrinfo error";
        return -1;
    }

    for (rp = result; rp != NULL; rp = rp->ai_next) {
    sfd = socket (rp->ai_family, rp->ai_socktype, rp->ai_protocol);
    if (sfd == -1)
        continue;

    s = bind (sfd, rp->ai_addr, rp->ai_addrlen);
    if (s == 0) {
        // We managed to bind successfully!
        break;
    }

    close (sfd);
    }

    if (rp == NULL) {
        //fprintf (stderr, "Could not bind\n");
        qInfo() << "EpollServer:: Could not bind";
        return -1;
    }

    freeaddrinfo (result);

    return sfd;
}

int EpollServer::make_non_blocking(int sfd) {
    qInfo() << "EpollServer::make_non_blocking()";

    int flags, s;

    flags = fcntl (sfd, F_GETFL, 0);
    if (flags == -1) {
        //perror ("fcntl");
        qInfo() << "EpollServer:: fcntl error";
        return -1;
    }

    flags |= O_NONBLOCK;
    s = fcntl (sfd, F_SETFL, flags);
    if (s == -1) {
        //perror ("fcntl");
        qInfo() << "EpollServer:: fcntl error";
        return -1;
    }

    return s;
}

