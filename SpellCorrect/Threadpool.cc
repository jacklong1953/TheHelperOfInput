#include "Threadpool.h"
#include <unistd.h>

namespace wd
{
void Threadpool::start()
{
    _timer.start();
    /* printf("Threadpool::start()\n"); */
    for(size_t i = 0; i != _threadNum; ++i)
    {
        unique_ptr<Thread> pthread
            (new Thread(std::bind(&Threadpool::threadFunc, this, std::placeholders::_1),
                        _cache));
        _threads.push_back(std::move(pthread));
    }
    
    for(size_t i = 0; i != _threadNum; ++i)
    {
        _threads[i]->start();
    }
}

void Threadpool::addTask(ElementType &&task) { _taskQue.push(std::move(task)); }

ElementType Threadpool::getTask() { return _taskQue.pop(); }

void Threadpool::stop()
{
    _timer.stop();
    if(!_isExit)
    {
        while(!_taskQue.isEmpty())
        {
           ::sleep(2);
        }
        _isExit = true;
        _taskQue.wakeup();
        for(auto &pthread : _threads)
        {
            pthread->join();
        }
    }
}

void Threadpool::threadFunc(Cache &cache)
{
    while(!_isExit)
    {
        ElementType task = getTask();
        if(task)
            task(cache);
    }
}

void Threadpool::updataCache()
{
    for(size_t idx = 0; idx != _threads.size(); ++idx)
    {
        _cache.updata(_threads[idx]->getCache());
    }

    for(size_t idx = 0; idx != _threads.size(); ++idx)
    {
       _threads[idx]->getCache().updata(_cache);
    }

    CacheManager::getCache() = _cache;
    CacheManager::updataCache();
}

}//end of namespace wd
