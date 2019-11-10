#pragma once
#include "Acceptor.h"
#include "EventLoop.h"
#include <string>

using std::string;

namespace  wd
{

using TcpConnectPtr = std::shared_ptr<TcpConnect>;
using TcpConnectCallBack = std::function<void (const TcpConnectPtr &)>;

class TcpServer
{
public:
    TcpServer(const string &);

    void start();

    void setConnectCallBack(TcpConnectCallBack &&);
    void setMessageCallBack(TcpConnectCallBack &&);
    void setCloseCallBack(TcpConnectCallBack &&);
    Acceptor initial();

private:
    string _confRoute;
    Acceptor _acceptor;
    EventLoop _evenLoop;

};

}//end of namespace wd
