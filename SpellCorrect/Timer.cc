#include "Timer.h"
#include <sys/timerfd.h>
#include <poll.h>
#include <unistd.h>
#include <iostream>

namespace wd
{

int Timer::createFd()
{
    int clockid = timerfd_create(CLOCK_REALTIME, 0);
    return clockid;
}

void Timer::start()
{
    _isStart = true;
    setTimer(_initialTime, _interTime);
    struct pollfd eve;
    eve.fd = _timerFd;
    eve.events = POLLIN;
    while(_isStart)
    {
        int ret = poll(&eve, 1, 5000);
        if(ret < 0 && ret == EINTR)
            continue;
        else if(-1 == ret)
        {
            perror("poll");
            return ;
        } else if(ret == 0){
            std::cout << ">>poll timeout!" << std::endl;
            /* << " _isStart = " << _isStart << std::endl; */
        } else {
            if(eve.revents & POLLIN)
            {
                //poll/select采用水平触发，不去处理事件会一直触发
                handleRead();
                if(_cb)
                    _cb();
            }
        }
    }
}

void Timer::handleRead()
{
    uint64_t howmany;
    int ret = ::read(_timerFd, &howmany, sizeof(howmany));
    if(ret != sizeof(howmany))
        perror("read");
}

int Timer::setTimer(int initialTime, int interTime)
{
    struct itimerspec value;
    value.it_value.tv_sec = initialTime;
    value.it_value.tv_nsec = 0;
    value.it_interval.tv_sec = interTime;
    value.it_interval.tv_nsec = 0;
    int ret = timerfd_settime(_timerFd, 0, &value, nullptr);
    return ret;
}

void Timer::stop()
{
    if(_isStart)
        _isStart = false;
    setTimer(0, 0);
    std::cout << "stop..."
        << " _isStart = " << _isStart << std::endl;
}

}//end of namespace wd
