#pragma once
#include <stdlib.h>
#include <pthread.h>
#include <string>
#include <map>
#include <set>
#include <iostream>
#include <fstream>
#include <sstream>

using std::cout;
using std::endl;
using std::string;
using std::ifstream;
using std::map;
using std::set;
using std::istringstream;

namespace  wd
{

class Configuration
{
public:
    static Configuration *getpInstance()
    {
        pthread_once(&_pthreadOnce, init);
        return _pInstance;
    }

     static void init()
    {
        _pInstance = new Configuration();
        ::atexit(Configuration::destroy);
    }

    static void destroy()
    {
        if(_pInstance)
            delete _pInstance;
    }

    void ready(const string &pathname)
    {
        setFilePath(pathname);
        createConfigMap();
    }

    void setFilePath(const string &filepath) { _filepath = filepath; }

    void createConfigMap();

    map<string, set<string>> getConfigMap() const
    {
        return _configMap;
    }

    void insertConfMap(string dict, set<string> pathname)
    {
        _configMap.insert(make_pair(dict, pathname));
    }

private:
    Configuration() {}
    ~Configuration() {}

private:
    string _filepath;
    map<string, set<string>> _configMap;
    static Configuration *_pInstance;
    static pthread_once_t _pthreadOnce;

};

}//end of namespace wd
