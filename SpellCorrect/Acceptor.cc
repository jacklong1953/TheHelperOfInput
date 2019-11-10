#include "Acceptor.h"
#include <sys/types.h>
#include <sys/socket.h>

namespace wd
{

Acceptor::Acceptor(const string &ip, unsigned short port)
    :_addr(port, ip)
    , _socketFd() {}

void Acceptor::ready()
{
    setReuseAddr(true);
    setReusePort(true);
    bind();
    listen();
}

void Acceptor::setReuseAddr(bool on)
{
    int one = on;
    if(setsockopt(_socketFd.fd(), SOL_SOCKET, SO_REUSEADDR
                  , &one, sizeof(one)) < 0)
    {
        perror("setsockopt");
    }
}

void Acceptor::setReusePort(bool on)
{
    int one = on;
    if(setsockopt(_socketFd.fd(), SOL_SOCKET, SO_REUSEPORT
                  , &one, sizeof(one)) < 0)
    {
        perror("setsockopt");
    }
}

void Acceptor::bind()
{
    if(::bind(_socketFd.fd(), (struct sockaddr *)_addr.getInetAddrPtr()
            , sizeof(struct sockaddr)) < 0)
    {
        perror("bind");
    }
}

void Acceptor::listen()
{
    if(::listen(_socketFd.fd(), 10) < 0)
    {
        perror("listen");
    }
}

int Acceptor::accept()
{
    int newfd = ::accept(_socketFd.fd(), nullptr, nullptr);
    if(newfd == -1)
    {
        perror("accept");
    }

    return newfd;
}

int Acceptor::fd() { return _socketFd.fd(); }

}//end of namespace wd
