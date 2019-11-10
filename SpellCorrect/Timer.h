#pragma once
#include <functional>

namespace  wd
{

class Timer
{
    using FuncCallBack = std::function<void ()>;
public:
    Timer(int initialtime, int interTime, FuncCallBack &&cb)
        : _timerFd(createFd())
          , _initialTime(initialtime)
          , _interTime(interTime)
          , _isStart(false)
          , _cb(std::move(cb))
    {}

    ~Timer() {}

    void start();
    void stop();

private:
    int createFd();
    int setTimer(int, int);
    void handleRead();

private:
    int _timerFd;
    int _initialTime;
    int _interTime;
    bool _isStart;
    FuncCallBack _cb;

};

}//end of namespace wd
