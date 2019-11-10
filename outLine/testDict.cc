#include "DictProducer.h"
#include "SplitTool.h"
#include <iostream>

using namespace wd;

int main()
{
    DictProducer config(new MyJieba());
    config.initialConfig("./config");
    config.buildEnglishDict();
    config.buildChinaDict();
    config.storeDictionary("./Dictionary.txt");
    config.storeIndexTable("./IndexTable.txt");
    return 0;
}

