#include "MyDict.h"
#include "Configuration.h"

namespace  wd
{

MyDict *MyDict::_pinstance = nullptr;
pthread_once_t MyDict::_pthreadOnce = PTHREAD_ONCE_INIT;

MyDict *MyDict::getInstance()
{
    pthread_once(&_pthreadOnce, init);
    return _pinstance;
}

void MyDict::init()
{
    _pinstance = new MyDict();
    ::atexit(MyDict::destroy);
}

void MyDict::destroy()
{
    if(_pinstance)
        delete _pinstance;
}

void MyDict::ready(const string &route)
{
    Configuration *pconfig = Configuration::getpInstance();
    pconfig->ready(route);
    string pathname = pconfig->getConfigMap()["mydict"];
    createMyDict(pathname);
    pathname = pconfig->getConfigMap()["myindex"];
    createIndex(pathname);
}

void MyDict::createMyDict(const string &dictRoute)
{
    ifstream ifs(dictRoute.c_str());
    string text;
    string word;
    while(getline(ifs, text))
    {
        istringstream line(text);
        while(line >> word)
        {
            string tmp;
            line >> tmp;
            _dict.push_back(make_pair(word, std::stoi(tmp)));
        }
    }
    ifs.close();
}

void MyDict::createIndex(const string &indexRoute)
{
    ifstream ifs(indexRoute.c_str());
    string text;
    while(getline(ifs, text))
    {
        string word;
        string idx;
        set<int>  index;
        istringstream line(text);
        line >> word;
        while(line >> idx)
        {
            index.insert(stoi(idx));
        }
        _index.insert(make_pair(word, index));
    }
    ifs.close();
}

vector<pair<string, int>> & MyDict::getDict() { return _dict; }

unordered_map<string, set<int>> &MyDict::getIndex() { return _index; }

}//end of namespace wd
