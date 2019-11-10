#include "TcpConnect.h"
#include "EventLoop.h"
#include <sstream>
#include <iostream>

namespace wd
{

TcpConnect::TcpConnect(int fd, EventLoop *eventLoop)
: _socket(fd)
, _socketIO(fd) 
, _localAddr(getLocalAddr())
, _peerAddr(getPeerAddr())
, _isShutDownWrite(false)
, _eventLoop(eventLoop)
{}

TcpConnect::~TcpConnect()
{
    if(!_isShutDownWrite)
        shutdown();
}

void TcpConnect::shutdown()
{
    if(!_isShutDownWrite)
    {
        _isShutDownWrite = true;
        _socket.shutdownWrite();
    }
}

string TcpConnect::toString()
{
    ostringstream os;
    os << _localAddr.ip() << ": " << _localAddr.port()
       << "---->" << _peerAddr.ip() << ": " << _peerAddr.port();
    return os.str();
}

void TcpConnect::send(const string &meg)
{
    _socketIO.writen(meg.c_str(), meg.size());
}

void TcpConnect::sendInLoop(string &msg)
{
    _eventLoop->runInLoop(std::bind(&TcpConnect::send, this, msg));
    /* std::cout << "sendInloop: " << msg << std::endl; */
}

string TcpConnect::receive()
{
    char buf[1024] = { 0 };
    _socketIO.getMessage(buf, sizeof(buf));
    return string(buf);
}

InetAddress TcpConnect::getLocalAddr()
{
    struct sockaddr_in addr;
    socklen_t len = sizeof(struct sockaddr);
    if(getsockname(_socket.fd(), (struct sockaddr*)&addr, &len) == -1)
    {
        perror("getsockname");
    }
    return InetAddress(addr);
}
 
InetAddress TcpConnect::getPeerAddr()
{
    struct sockaddr_in addr;
    socklen_t len = sizeof(struct sockaddr);
    if(getpeername(_socket.fd(), (struct sockaddr*)&addr, &len) < 0)
    {
        perror("getsockname");
    }
    return InetAddress(addr);
}
 
void TcpConnect::setConnectCallBack(const TcpConnectCallBack &cb)
{
    _onConnect = std::move(cb);
}

void TcpConnect::setMessageCallBack(const TcpConnectCallBack &cb)
{
    _onMessage = std::move(cb);
}

void TcpConnect::setCloseCallBack(const TcpConnectCallBack &cb)
{
    _onClose = std::move(cb);
}

    void TcpConnect::handleConnectCallBack()
{
    if(_onConnect)
    _onConnect(shared_from_this());
}

    void TcpConnect::handleMessageCallBack()
{
    if(_onMessage)
        _onMessage(shared_from_this());
}

    void TcpConnect::handleCloseCallBack()
{
    if(_onClose)
        _onClose(shared_from_this());
}

}//end of namespace wd
