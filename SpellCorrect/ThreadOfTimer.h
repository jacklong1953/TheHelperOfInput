#pragma once

#include "NonCopyable.h"
#include <pthread.h>
#include <functional>

namespace wd
{

class ThreadOfTimer
:NonCopyable
{
public:
    using ThreadCallBack = std::function<void()>;

    ThreadOfTimer(ThreadCallBack &&cd)
    :NonCopyable()
    ,_pid(0) 
    ,_isRunning(false)
    ,_cd(std::move(cd))
    {}

    void start();

    void join();

    virtual ~ThreadOfTimer() 
    {
        if(_isRunning)
        {
            pthread_detach(_pid);
            _isRunning = false;
        }
    }

private:
    static void * pthreadFunc( void *);

private:
    pthread_t _pid;
    bool _isRunning;
    ThreadCallBack _cd;

};

}//the end of namespace wd
