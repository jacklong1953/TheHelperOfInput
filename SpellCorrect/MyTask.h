#pragma once
#include "TcpConnect.h"
#include "MyDict.h"
#include <string>
#include <queue>

using namespace std;

namespace  wd
{

class Cache;
struct MyResult;
class Task
{
public:
    Task(const string &msg, const TcpConnectPtr &conn)
        : _msg(msg)
          , _conn(conn)
          , _pmyDict(MyDict::getInstance())
    {}

    void proccess(Cache &);

    ~Task(){}

private:
    //计算最小编辑距离
    int editDistance(const string &, const string &);
    //计算每个字符utf-8编码长度
    int nByteCode(const char &);
    //计算一个字符串中含有的字符个数
    int length(const string &);
    string searchIndex();

private:
    string _msg;
    TcpConnectPtr _conn;
    MyDict *_pmyDict;
    priority_queue<MyResult> _candWord;

};

struct MyResult
{
    /* MyResult() {} */
    MyResult(const string candidatorWord, int frequency, int distance)
        : _candidate(candidatorWord)
          , _frequency(frequency)
          , _distance(distance)
    {}

    friend bool operator < (const MyResult &lhs, const MyResult &rhs);

    string _candidate;
    int _frequency;
    int _distance;
};

}//end of namespace wd
