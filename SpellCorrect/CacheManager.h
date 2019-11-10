#pragma once

#include <pthread.h>
#include <vector>
#include <string>

using namespace std;

namespace wd
{

class Cache;
class CacheManager
{
public:
    static CacheManager *getPinstance();
    static void initCache(const string &);
    static void updataCache();
    static Cache &getCache();

private:
    static void destroy();
    static void init();

    CacheManager() {}
    ~CacheManager() {}

private:
    static string _filepath;
    static Cache _readCache;
    static CacheManager *_pInstance;
    static pthread_once_t _pthreadOnce;

};

}//end of namespace wd
