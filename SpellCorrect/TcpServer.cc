#include "TcpServer.h"
#include "Configuration.h"
#include "MyDict.h"
#include "CacheManager.h"
#include <algorithm>

namespace wd
{

TcpServer::TcpServer(const string &confRoute)
    : _confRoute(confRoute)
      , _acceptor(initial())
      , _evenLoop(_acceptor)
    {}

Acceptor TcpServer::initial()
{
    Configuration::getpInstance()->ready(_confRoute);
    unsigned short port = stoi(Configuration::getpInstance()->getConfigMap()["myport"]);
    string ip = Configuration::getpInstance()->getConfigMap()["myip"];
    return Acceptor(ip, port);
}

void TcpServer::start()
{
    wd::MyDict::getInstance()->ready("./myConfig");
    string filepath = Configuration::getpInstance()->getConfigMap()["mycache"];
    CacheManager::getPinstance()->initCache(filepath);
    _acceptor.ready();
    _evenLoop.loop();
}

void TcpServer::setConnectCallBack(TcpConnectCallBack &&cb)
{
    _evenLoop.setConnectCallBack(std::move(cb));
}
void TcpServer::setMessageCallBack(TcpConnectCallBack &&cb)
{
    _evenLoop.setMessageCallBack(std::move(cb));
}
void TcpServer::setCloseCallBack(TcpConnectCallBack &&cb)
{
    _evenLoop.setCloseCallBack(std::move(cb));
}

}//end of namespace wd
