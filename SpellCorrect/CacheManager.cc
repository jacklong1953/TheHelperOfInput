#include "CacheManager.h"
#include "Cache.h"
#include "Mutex.h"
#include <stdlib.h>
#include <fstream>
#include <sstream>

namespace wd
{

CacheManager *CacheManager::_pInstance = nullptr;
pthread_once_t CacheManager::_pthreadOnce = PTHREAD_ONCE_INIT;
string CacheManager::_filepath;
Cache CacheManager::_readCache;

CacheManager *CacheManager::getPinstance()
{
    pthread_once(&_pthreadOnce, init);
    ::atexit(CacheManager::destroy);
    return _pInstance;
}

void CacheManager::init() { _pInstance = new CacheManager(); }

void CacheManager::destroy()
{
    if(_pInstance)
        delete _pInstance;
}

void CacheManager::initCache(const string &fileName)
{
    _filepath = fileName;
    Cache _readCache;
    ifstream ifs(fileName.c_str());
    string text;
    string word;
    while(getline(ifs, text))
    {
        istringstream line(text);
        while(line >> word)
        {
            _readCache.putsCache(word);
        }
    }
    ifs.close();
}

void CacheManager::updataCache()
{
    _readCache.writeToFile(_filepath);
}

Cache &CacheManager::getCache()
{
    return _readCache;
}

}//end of namespace wd
