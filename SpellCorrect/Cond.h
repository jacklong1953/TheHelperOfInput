#pragma once

#include "Mutex.h"
#include <pthread.h>
#include <iostream>

namespace wd
{

class Cond
{
public:
    Cond(MutexLock &mutex)
    :_mutex(mutex)
    { 
        if(pthread_cond_init(&_cond, nullptr))
        {
            perror("pthread_cond_init");
        }
    }

    Cond(const Cond &rhs) = delete;
    Cond &operator = (const Cond &rhs) = delete;

    void wait()
    { 
        if(pthread_cond_wait(&_cond, _mutex.getMutexLock()))
        {
            perror("pthread_cond_wait");
        }
    }

    void notify() 
    {
        if(pthread_cond_signal(&_cond))
        {
            perror("pthread_cond_signal");
        }
    }

    void notifyAll()
    {
        if(pthread_cond_broadcast(&_cond))
        {
            perror("pthread_cond_broadcast");
        }
    }

    ~Cond() 
    { 
        if(pthread_cond_destroy(&_cond))
        {
            perror("pthread_cond_destroy");
        }
    }

private:
    pthread_cond_t _cond;
    MutexLock &_mutex;

};

}//end of namespace wd
