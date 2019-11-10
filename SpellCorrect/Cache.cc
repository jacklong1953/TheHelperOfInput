#include "Cache.h"
#include <iostream>
#include <fstream>

namespace wd
{

string Cache::getCacheWord(const string &key)
{
    string ret;
    auto it = _search.find(key);
    if(it != _search.end())
    {
        ret = *(it->second);
        _cachTable.erase(it->second);
        _cachTable.push_front(key);
        _search[key] = _cachTable.begin();
    }
    return ret;
}

void Cache::putsCache(string key)
{
    auto it = _search.find(key);
    if(it != _search.end())
    {
        _cachTable.erase(it->second);
        _cachTable.push_front(key);
        _search[key] = _cachTable.begin();
    } 
    else if(_search.size() < _sz) 
    {
        _cachTable.push_front(key);
        _search.insert(make_pair(key, _cachTable.begin()));
    } else {
        auto end = --_cachTable.end();
        _cachTable.erase(end);
        _search.erase(*end);
        _cachTable.push_front(key);
        _search.insert(make_pair(key, _cachTable.begin()));
    }
}

void Cache::updata(const Cache &cache)
{
    for(auto &it : cache._cachTable)
        putsCache(it);
}

void Cache::writeToFile(const string &filepath)
{
    ofstream ofs(filepath);
    for(auto &it : _cachTable)
        ofs << it << endl;
}

}//end of namespace wd
