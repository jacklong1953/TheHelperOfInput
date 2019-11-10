#pragma once

#include "TcpConnect.h"
#include "Mutex.h"
#include <functional>
#include <vector>
#include <map>
#include <memory>
#include <sys/epoll.h>
#include <vector>

using namespace::std;

namespace wd
{

using functor = function<void ()>;
class Acceptor;
class EventLoop
{
public:
    using TcpConnectPtr = shared_ptr<TcpConnect>;
    using TcpConnectCallBack = function<void (const TcpConnectPtr &)>;

    EventLoop(Acceptor &);
    /* ~EventLoop(); */

    void loop();
    void unLoop();
    void setConnectCallBack(TcpConnectCallBack &&);
    void setMessageCallBack(TcpConnectCallBack &&);
    void setCloseCallBack(TcpConnectCallBack &&);

    void handleRead();
    void wakeup();
    void runInLoop(functor &&);
    void doPendingMesssage();

private:
    int createEpollfd();
    int createEventfd();
    void waitEpollfd();
    void handleNewConnect();
    void handleMessage(int fd);

    void addEpollfdRead(int fd);
    void delEpollfdRead(int fd);
    bool isConnectClose(int fd);

private:
    int _efd;
    Acceptor &_acceptor;
    vector<struct epoll_event> _epollEventList;
    map<int, TcpConnectPtr> _conns;
    bool _isLooping;
    int _eventfd;
    MutexLock _mutex;
    vector<functor> _pendingMessage;

    TcpConnectCallBack _onConnect;
    TcpConnectCallBack _onMessage;
    TcpConnectCallBack _onClose;

};

}//end of namespace wd
