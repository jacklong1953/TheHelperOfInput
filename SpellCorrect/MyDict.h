#pragma once

#include <vector>
#include <set>
#include <unordered_map>

using namespace std;

namespace  wd
{

class Configuration;
class MyDict
{
public:
    static MyDict *getInstance();

    static void init();
    vector<pair<string, int>> &getDict();
    unordered_map<string, set<int>> &getIndex();
    void ready(const string &);

private:
    static void destroy();
    void createIndex(const string&);
    void createMyDict(const string &);
    MyDict() {}
    ~MyDict() {}

private:
    static MyDict *_pinstance;
    static pthread_once_t _pthreadOnce;
    vector<pair<string, int>> _dict;
    unordered_map<string, set<int>> _index;

};

}//end of namespace wd
