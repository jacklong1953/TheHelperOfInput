#pragma once

#include "CacheManager.h"
#include"TimerThread.h"
#include "TaskQueue.h"
#include "Thread.h"
#include <iostream>
#include <memory>

using namespace std;
using std::unique_ptr;

namespace wd
{

class Threadpool
{
    friend class WorkThread;
public:
    Threadpool(size_t threadNum, size_t queSize)
    : _threadNum(threadNum)
    , _queSize(queSize)
    , _taskQue(queSize)
    , _isExit(false)
    , _cache(CacheManager::getPinstance()->getCache())
    , _timer(5, 10, std::bind(&Threadpool::updataCache, this))
    {
        _threads.reserve(_threadNum);
    }

    void start();
    void stop();
    void addTask(ElementType &&);
    void updataCache();

    ~Threadpool() {}

private:
    void threadFunc(Cache &);
    ElementType getTask();

private:
    size_t _threadNum;
    size_t _queSize;
    vector<unique_ptr<Thread>> _threads;
    TaskQueue _taskQue;
    bool _isExit;
    Cache _cache;
    /* Timer _timer; */
    TimerThread _timer;

};
}//end of namespace wd
