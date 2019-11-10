#pragma once

#include <cerrno>
#include <cstdio>
#include <pthread.h>

namespace  wd
{

class MutexLock
{
public:
    MutexLock() 
    :_isLocking(false)
    { 
        if(pthread_mutex_init(&_mutex, nullptr))
        {
            perror("pthread_MutexLock_init");
        }
    }

    void lock()
    { 
        if(pthread_mutex_lock(&_mutex))
        {
            perror("pthread_mutex_lock");
        }
        _isLocking = true;
    }

    void unlock()
    {
        if(pthread_mutex_unlock(&_mutex))
        {
            perror("pthread_mutex_unlock");
        }
        _isLocking = false;
    }

    MutexLock(const MutexLock &rhs) = delete;
    MutexLock &operator = (const MutexLock &rhs) = delete;

    pthread_mutex_t * getMutexLock() { return &_mutex; }
    bool isLocking() const { return _isLocking; }

    ~MutexLock()
    {
        if(pthread_mutex_destroy(&_mutex))
        {
            perror("pthread_mutex_destroy");
        }
    }

private:
    pthread_mutex_t _mutex;
    bool _isLocking;
    
};

//RAII技术(resource acquisition is initialization)
class MutexLockGuard
{
public:
    MutexLockGuard(MutexLock &mutexLock)
    :_mutexLock(mutexLock)
    { _mutexLock.lock(); }

    ~MutexLockGuard() { _mutexLock.unlock(); }

private:
    MutexLock &_mutexLock;

};

}//end of namespace wd
