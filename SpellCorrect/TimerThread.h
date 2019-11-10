#pragma once
#include "ThreadOfTimer.h"
#include "Timer.h"
#include <functional>

namespace  wd
{

class TimerThread
{
    using TimerCallBack = std::function<void ()>;
public:
    TimerThread(int initialTime, int interTime, TimerCallBack &&cb)
        : _timer(initialTime, interTime, std::move(cb))
          , _thread(std::bind(&Timer::start, &_timer))
    {}

    void start();

    void stop();

    ~TimerThread() {}

private:
    Timer _timer;
    ThreadOfTimer _thread;

};

}//end of namespace wd
