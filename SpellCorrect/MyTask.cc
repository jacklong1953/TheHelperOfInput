#include "MyTask.h"
#include "Cache.h"
#include <json/json.h>
#include <json/value.h>
#include <json/writer.h>

namespace  wd
{

void Task::proccess(Cache &cache)
{
    string respond;
    respond = cache.getCacheWord(_msg);
    /* cout << "respond = " << respond << endl; */
    //如果在缓存中没有找到
    if(respond.empty())
    {
        respond = searchIndex();
    } else {
        Json::Value word;
        word[_msg].append(respond.substr(0, respond.size() - 1));
        respond = word.toStyledString();
    }
    cache.putsCache(_msg);
    _conn->sendInLoop(respond);
}

string Task::searchIndex()
{
    string respond;
    int nByte = nByteCode(_msg[0]);
    string subStr = _msg.substr(0, nByte);
    set<int> matchcWordIdx = _pmyDict->getIndex()[subStr];
    if(matchcWordIdx.empty())
    {
        printf("%s------>>NonIndexWord\n", subStr.c_str());
        respond = _msg;
    } else {
        for(auto &it : matchcWordIdx)
        {
            string candiWord = _pmyDict->getDict()[it].first;
            int dis = editDistance(_msg, candiWord);
            int frequency = _pmyDict->getDict()[it].second;
            _candWord.push(MyResult(candiWord, frequency, dis));
        }
        //从设计上来说，消息的发送应交给IO线程
        //而并不是由计算线程发送
        /* _conn->send(respond); */
        /* cout << "_conn->sendInLoop(respond): " <<  _msg << endl; */
        Json::Value root;
        for(int idx = 0; idx < 3; ++idx)
        {
            if(_candWord.empty())
                break;
            MyResult tmp = _candWord.top();
            /* respond.append(tmp._candidate); */
            /* respond.append("  "); */
            root[_msg].append(tmp._candidate);
            _candWord.pop();
        }
        respond = root.toStyledString();
    }
    return respond;
}

int Task::editDistance(const string &lhs, const string &rhs)
{
    int lhsLen = length(lhs);
    int rhsLen = length(rhs);
    int lhsSize = static_cast<int>(lhs.size());
    int rhsSize = static_cast<int>(rhs.size());
    int editDistance[lhsLen + 1][rhsLen + 1];

    for(int idx = 0; idx < lhsLen + 1; ++idx)
    {
        editDistance[idx][0] = idx;
    }

    for(int idx = 0; idx < rhsLen + 1; ++idx)
    {
        editDistance[0][idx] = idx;
    }

    string lhsSubstr, rhsSubstr;
    for(int lIdx = 0, lLen = 1; lIdx != lhsSize && lLen != lhsLen + 1; ++lIdx, ++lLen)
    {
        int nByteL = nByteCode(lhs[lIdx]);
        lhsSubstr = lhs.substr(lIdx, nByteL);
        lIdx += nByteL -1;

        for(int rIdx = 0, rLen = 1; rIdx != rhsSize && rLen != rhsLen + 1; ++rIdx, ++rLen)
        {
            int nByteR = nByteCode(rhs[rIdx]);
            rhsSubstr = rhs.substr(rIdx, nByteR);
            rIdx += nByteR -1;
            if(lhsSubstr == rhsSubstr)
            {
                editDistance[lLen][rLen] = editDistance[lLen - 1][rLen - 1];
            } else {
                    int ret1 = editDistance[lLen -1][rLen] + 1;
                    int ret2 = editDistance[lLen][rLen - 1] + 1;
                    int ret3 = editDistance[lLen - 1][rLen - 1] + 1;
                auto fun = [&] {
                    return ret1 < ret2 ? ret1 < ret3 ? ret1 : ret3 : ret2 < ret3 ? ret2 : ret3;
                };
                editDistance[lLen][rLen] = fun();
            }

        }

    }
    return editDistance[lhsLen + 1][rhsLen + 1];
}

int Task::nByteCode(const char &ch)
{
    int nByte = 1;
    if(ch & (1 << 7))
    {
        for(int idx = 6; idx != 0; --idx)
        {
            if(ch & (1 << idx))
            {
                ++nByte;
            } else {
                break;
            }
        }
    }
    return nByte;
}

int Task::length(const string &str)
{
    int len = 0;
    for(int idx = 0; idx != static_cast<int>(str.size()); ++idx)
    {
        int nByte = nByteCode(str[idx]);
        ++len;
        idx += nByte -1;
    }
    return len;
}

bool operator < (const MyResult &lhs, const MyResult &rhs)
{
    bool ret;
    if(lhs._distance < rhs._distance)
    {
        ret = false;
    }
    else if(lhs._distance == rhs._distance)
    {
        if(lhs._frequency > rhs._frequency)
        {
            ret = false;
        }
        else if(lhs._frequency == rhs._frequency)
        {
            if(lhs._candidate < rhs._candidate)
                ret = false;
            else
                ret = true;
        } 
        else 
            ret = true;
    }
    else 
        ret = true;
    return ret;
}

}//end of namespace wd
