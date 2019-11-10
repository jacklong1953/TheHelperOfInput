#include "InetAddress.h"

namespace wd
{

InetAddress::InetAddress(unsigned short port)
{
    _addr.sin_family = AF_INET;
    _addr.sin_port = htons(port);
    _addr.sin_addr.s_addr = INADDR_ANY;
}

InetAddress::InetAddress(unsigned short port, const string &ip)
{
    _addr.sin_family = AF_INET;
    _addr.sin_port = htons(port);
    _addr.sin_addr.s_addr = inet_addr(ip.c_str());
}

InetAddress::InetAddress(const struct sockaddr_in &addr)
    :_addr(addr) {}

string InetAddress::ip() const { return inet_ntoa(_addr.sin_addr); }
unsigned short InetAddress::port() const { return ntohs(_addr.sin_port); }
struct sockaddr_in *InetAddress::getInetAddrPtr() { return &_addr; }

}//end of namespace wd
