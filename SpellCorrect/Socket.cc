#include "Socket.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>

namespace wd
{

Socket::Socket()
{
    if((_sfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
       {
            perror("socket");
            exit(-1);
       }
}

Socket::Socket(int &fd)
    : _sfd(fd)
{}

void Socket::shutdownWrite()
{
    shutdown(_sfd, SHUT_WR);
}

int Socket::fd() const { return _sfd; }

Socket::~Socket() { close(_sfd); }

}//end of namespace wd
