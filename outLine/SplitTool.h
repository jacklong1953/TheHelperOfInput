#pragma once
#include "../cppjieba/include/cppjieba/Jieba.hpp"
#include <vector>
#include <string>

using namespace std;

namespace  wd
{

class SplitTool
{
public:
    SplitTool() {}
    virtual ~SplitTool() {}

    virtual vector<string> cut(const string &sentence) = 0;

};

class MyJieba
:public SplitTool
{
public:
    MyJieba() 
    : _extractor(init())
    {}

    ~MyJieba() {}

    vector<string> cut(const string &sentence);
    
private:
    cppjieba::KeywordExtractor init();

private:
    cppjieba::KeywordExtractor _extractor;

};

}//end of namespace wd
