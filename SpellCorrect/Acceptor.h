#pragma once

#include "InetAddress.h"
#include "Socket.h"

namespace wd
{

class Acceptor
{
public:
    Acceptor(const string &ip, unsigned short port);

    void ready();
    int accept();
    int fd();

    /* ~Acceptor(); */

private:
    void setReuseAddr(bool on);
    void setReusePort(bool on);
    void bind();
    void listen();

private:
    InetAddress _addr;
    Socket _socketFd;

};

}//end of namespace wd
