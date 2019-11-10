#include "ThreadOfTimer.h"
#include <cstdio>
#include <cerrno>

using namespace::std;

namespace wd
{
void ThreadOfTimer::start()
{
    if(pthread_create(&_pid, nullptr, pthreadFunc, this))
    {
        perror("pthread_create");
        return ;
    }
    _isRunning = true;
}

void ThreadOfTimer::join()
{
    if(_isRunning)
    {
        pthread_join(_pid, nullptr);
        _isRunning = false;
    }
}

void *ThreadOfTimer::pthreadFunc(void *argv)
{
    ThreadOfTimer *runFunc = static_cast<ThreadOfTimer*> (argv);
    runFunc->_cd();
    return nullptr;
}

}//end of namespace wd
