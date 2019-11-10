#pragma once

#include "Socket.h"
#include "SocketIO.h"
#include "InetAddress.h"
#include "NonCopyable.h"
#include <functional>
#include <memory>
#include <string>

using namespace std;

namespace wd
{

class EventLoop;
class TcpConnect;
using TcpConnectPtr = std::shared_ptr<TcpConnect>;
using TcpConnectCallBack = std::function<void (const TcpConnectPtr &)>;

class TcpConnect
:NonCopyable
, public std::enable_shared_from_this<TcpConnect>
{
public:
    TcpConnect(int fd, EventLoop *eventLoop);
    ~TcpConnect();

    void shutdown();
    string toString();
    void send(const string &meg);
    string receive();

    void sendInLoop(string &);

    void setConnectCallBack(const TcpConnectCallBack &);
    void setMessageCallBack(const TcpConnectCallBack &);
    void setCloseCallBack(const TcpConnectCallBack &);

    void handleConnectCallBack();
    void handleMessageCallBack();
    void handleCloseCallBack();

private:
    InetAddress getLocalAddr();
    InetAddress getPeerAddr();

private:
    Socket _socket;
    SocketIO _socketIO;
    InetAddress _localAddr;
    InetAddress _peerAddr;
    bool _isShutDownWrite;
    EventLoop *_eventLoop;

    TcpConnectCallBack _onConnect;
    TcpConnectCallBack _onMessage;
    TcpConnectCallBack _onClose;

};

}//end of namespace wd
