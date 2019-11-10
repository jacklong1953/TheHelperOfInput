#pragma once

#include "NonCopyable.h"
#include "Cache.h"
#include <pthread.h>
#include <functional>
#include <iostream>

namespace wd
{

class Thread
:NonCopyable
{
public:
    using threadCallBack = std::function< void (Cache &) > ;

    Thread(threadCallBack &&cb, Cache &cache)
    :NonCopyable()
    ,_pid(0) 
    ,_isRunning(false)
    ,_cb(std::move(cb))
    ,_cache(cache)
    { std::cout << "Thread()" << std::endl; }

    void start();

    void join();

    Cache &getCache() { return _cache; }

    virtual ~Thread() 
    {
        if(_isRunning)
        {
            pthread_detach(_pid);
            _isRunning = false;
        }
        std::cout << "~Thread()" << std::endl;
    }

private:
    static void * pthreadFunc( void *);

private:
    pthread_t _pid;
    bool _isRunning;
    threadCallBack _cb;
    Cache _cache;

};

}//the end of namespace wd
