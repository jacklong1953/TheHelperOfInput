#pragma once

#include "Mutex.h"
#include "Cond.h"
#include "NonCopyable.h"
#include <pthread.h>
#include <memory>
#include <queue>
#include <functional>

using std::queue;
using std::unique_ptr;

namespace wd
{

class Cache;
class Task;
using ElementType = std::function<void (Cache &)>;

class TaskQueue
: NonCopyable
{
public:
    TaskQueue(size_t sz = 10)
    :_capacity(sz)
    ,_taskQueue()
    ,_mutex()
    ,_isFull(_mutex)
    ,_isEmpty(_mutex)
    ,_flage(true)
    {}

    void push(ElementType randomNum);

    ElementType pop();

    void wakeup();

    MutexLock &getMutex() { return _mutex; }

    bool isFull() { return _capacity == _taskQueue.size(); }
    Cond &getCondFull() { return _isFull; }

    bool isEmpty() { return _taskQueue.empty(); }
    Cond &getCondEmpty() { return _isEmpty; }

    ~TaskQueue() {}

private:
    size_t _capacity;
    queue<ElementType> _taskQueue;
    MutexLock _mutex;
    Cond  _isFull;
    Cond  _isEmpty;
    bool _flage;

};

}//end of namespace wd
