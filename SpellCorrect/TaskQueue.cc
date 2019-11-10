#include "TaskQueue.h"

namespace  wd
{
void TaskQueue::push(ElementType task)
{
    {
        MutexLockGuard autoLock(_mutex); 
        while(isFull())
        {
            _isFull.wait(); 
        } 
        _taskQueue.push(task);
    }
    _isEmpty.notify();
}

ElementType TaskQueue::pop()
{
    ElementType num = nullptr;
    {
        MutexLockGuard autoLock(_mutex); 
        while(_flage && isEmpty())
        {
            /* std::cout << "empty >>" << _flage << std::endl; */
            _isEmpty.wait();
        }
        if(_flage)
        {
            num = _taskQueue.front();
            _taskQueue.pop();
        }
    }
    _isFull.notify();
    return num;
}

void TaskQueue::wakeup()
{
    if(_flage)
        _flage = false;
    _isEmpty.notifyAll();
}

}// end of namespace wd
