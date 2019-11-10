#include "Thread.h"
#include <cstdio>
#include <cerrno>

using namespace::std;

namespace wd
{
void Thread::start()
{
    if(pthread_create(&_pid, nullptr, pthreadFunc, this))
    {
        perror("pthread_create");
        return ;
    }
    _isRunning = true;
}

void Thread::join()
{
    if(_isRunning)
    {
        pthread_join(_pid, nullptr);
        _isRunning = false;
    }
}

void *Thread::pthreadFunc(void *argv)
{
    Thread *runFunc = static_cast<Thread*> (argv);
    if(runFunc)
        runFunc->_cb(runFunc->_cache);
    return nullptr;
}

}//end of namespace wd
