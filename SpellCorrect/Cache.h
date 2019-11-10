#pragma once

#include <list>
#include <string>
#include <unordered_map>

using namespace std;

namespace  wd
{

class Cache
{
public:
    Cache(size_t sz = 10)
    : _sz(sz)
    {}

    Cache(const Cache &cache)
        : _sz(cache._sz)
          , _cachTable(cache._cachTable)
          , _search(cache._search)
    {}

    string getCacheWord(const string &);
    void putsCache(string);
    void updata(const Cache &);
    void writeToFile(const string &);

private:
    size_t _sz;
    list<string> _cachTable;
    unordered_map<string, list<string>::iterator> _search;

};

}//end of namespace wd
