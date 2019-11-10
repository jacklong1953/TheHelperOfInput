#include "TimerThread.h"
#include <unistd.h>

namespace wd
{

void TimerThread::start()
{
    _thread.start();
}

void TimerThread::stop()
{
    _timer.stop();
    _thread.join();
}


}//end of namespace wd
