#pragma once

#include <arpa/inet.h>
#include <string>

using std::string;

namespace wd
{

class InetAddress
{
public:

    InetAddress(unsigned short port);
    InetAddress(unsigned short port, const string &ip);
    InetAddress(const struct sockaddr_in &);

    string ip() const;
    unsigned short port() const;
    struct sockaddr_in *getInetAddrPtr();

private:
    struct sockaddr_in _addr;

};

}//end of namespace wd
