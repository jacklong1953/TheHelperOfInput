#include "EventLoop.h"
#include "Acceptor.h"
#include "TcpConnect.h"
#include <sys/eventfd.h>
#include <unistd.h>
#include <iostream>
#include <algorithm>

namespace wd
{

EventLoop::EventLoop(Acceptor &acceptor)
    : _efd(createEpollfd())
      , _acceptor(acceptor)
      , _epollEventList(1024)
      , _isLooping(false)
      , _eventfd(createEventfd())
    {
        addEpollfdRead(_acceptor.fd());
        addEpollfdRead(_eventfd);
    }

int EventLoop::createEpollfd()
{
    int ret = epoll_create1(0);
    if(ret < 0)
    {
        perror("epoll_create");
    }
    return ret;
}

int EventLoop::createEventfd()
{
    int ret = eventfd(0, 0);
    if(ret < 0)
    {
        perror("eventfd");
    }
    return ret;
}

void EventLoop::handleRead()
{
    uint64_t num;
    int ret = read(_eventfd, &num, sizeof(num));
    if(ret != sizeof(num))
    {
        perror(">>read");
    }
}

void EventLoop::wakeup()
{
    uint64_t one = 1;
    int ret = write(_eventfd, &one , sizeof(one));
    if(ret != sizeof(one))
    {
        perror(">>write");
    }
}

void EventLoop::runInLoop(functor &&cb)
{
    {
        MutexLockGuard autolock(_mutex);
        _pendingMessage.push_back(std::move(cb));
    }
    wakeup();
}

void EventLoop::doPendingMesssage()
{
    vector<functor> tmp;
    {
        MutexLockGuard autolock(_mutex);
        swap(tmp, _pendingMessage);
    }
    for(auto functor : tmp)
        functor();
}

void EventLoop::addEpollfdRead(int fd)
{
    struct epoll_event eve;
    eve.data.fd = fd;
    eve.events = EPOLLIN;
    if(epoll_ctl(_efd, EPOLL_CTL_ADD, fd, &eve) < 0)
    {
        perror("epoll_ctl");
    }

}

void EventLoop::delEpollfdRead(int fd)
{
    struct epoll_event eve;
    eve.data.fd = fd;
    eve.events = EPOLLIN;
    if(epoll_ctl(_efd, EPOLL_CTL_DEL, fd, &eve) < 0)
    {
        perror("epoll_ctl");
    }
}

bool EventLoop::isConnectClose(int fd)
{
    int ret;
    do{
        char buf[1024] = { 0 };
        ret = recv(fd, buf, 1024, MSG_PEEK);
    }while(-1 == ret && EINTR == ret);
    return (ret == 0);
}

void EventLoop::loop()
{
    _isLooping = true;
    while(_isLooping)
        waitEpollfd(); 
}

void EventLoop::unLoop()
{
    if(_isLooping)
        _isLooping = false;
}

void EventLoop::waitEpollfd()
{
    int readynum;
    do{
        readynum = epoll_wait(_efd, &*_epollEventList.begin(), _epollEventList.size(), 5000);
    }while(-1 == readynum && EINTR == readynum);
    if(readynum == -1)
    {
        perror("epoll_wait");
        return;
    } else if(readynum == 0) {
        std::cout << ">> epoll_wait timeout !" << std::endl;
    } else {
        if(readynum == (int)_epollEventList.size())
            _epollEventList.resize(2 * readynum);
        for(int i = 0; i < readynum; ++i)
        {
            int fd = _epollEventList[i].data.fd;
            if(fd == _acceptor.fd())
            {
                if(_epollEventList[i].events & EPOLLIN)
                    handleNewConnect();
            } else if(_eventfd == fd){
                handleRead();
                doPendingMesssage();
            } else {
                if(_epollEventList[i].events & EPOLLIN)
                {
                    handleMessage(fd);
                }
            }
        }
    }
}

void EventLoop::handleNewConnect()
{
    int peerfd = _acceptor.accept();
    addEpollfdRead(peerfd);
    TcpConnectPtr conn(new TcpConnect(peerfd, this));
    conn->setConnectCallBack(_onConnect);
    conn->setMessageCallBack(_onMessage);
    conn->setCloseCallBack(_onClose);

    _conns.insert(std::make_pair(peerfd, conn));

    conn->handleConnectCallBack();
}

void EventLoop::handleMessage(int fd)
{
    auto it = _conns.find(fd);
    if(it == _conns.end())
    {
        std::cout << "No found the fd of peer on connections." << endl;
        return ;
    }
    bool isClose = isConnectClose(fd);
    if(isClose)
    {
        delEpollfdRead(fd);
        it->second->handleCloseCallBack();
        _conns.erase(it);
    } else {
        it->second->handleMessageCallBack();
    }
}

void EventLoop::setConnectCallBack(TcpConnectCallBack &&cb)
{
    _onConnect = std::move(cb);
}

void EventLoop::setMessageCallBack(TcpConnectCallBack &&cb)
{
    _onMessage = std::move(cb);
}

void EventLoop::setCloseCallBack(TcpConnectCallBack &&cb)
{
    _onClose = std::move(cb);
}


}//end of namespace wd
