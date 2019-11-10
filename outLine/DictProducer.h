#pragma once
#include "Configuration.h"
#include <vector>
#include <utility>

using std::vector;
using std::string;
using std::pair;

namespace  wd
{

class SplitTool;
class DictProducer
{
public:
    DictProducer(SplitTool *splitTool)
    : _splitTool(splitTool) 
    {}

    ~DictProducer() {}

    void initialConfig(const string &confName)
    {
        Configuration::getpInstance()->ready(confName);
    }

    void buildEnglishDict();
    void buildChinaDict();

    void storeDictionary(const string &);
    void storeIndexTable(const string &);

private:
    string handText(const string &);
    void createIndexTable();

private:
    SplitTool *_splitTool;    
    map<string, int> _dict;
    vector<pair<string, int>> _readMap;
    map<string, set<int>> _indexTable;

};

}//end of namespace wd
