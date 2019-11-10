#include "DictProducer.h"
#include "SplitTool.h"
#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <algorithm>
#include <iostream>
#include <string>

using std::ofstream;

namespace wd
{

void DictProducer::buildEnglishDict()
{
    //以引用获取时，解引用迭代器得不到正确的string
    auto pathname = Configuration::getpInstance()->getConfigMap()["ENG"];
    string tmpFilePath;
    for(auto it = pathname.begin(); it != pathname.end(); ++it)
    {
        tmpFilePath = handText(*it);
        ifstream ifs(tmpFilePath);
        string text;
        while(getline(ifs, text))
        {
            istringstream line(text);
            string word;
            while(line >> word)
            {
                //将大写字母转换成小写字母
                std::transform(word.begin(), word.end(), word.begin(), ::tolower);
                if(_dict.find(word) != _dict.end())
                    ++_dict[word];
                else 
                    _dict[word] = 1;
            }
        }
        ifs.close();
    }
    remove(tmpFilePath.c_str());
}

void DictProducer::buildChinaDict()
{
    std::vector<string> words;
    auto pathname = Configuration::getpInstance()->getConfigMap()["CN"];
    for(auto it = pathname.begin(); it != pathname.end(); ++it)
    {
        ifstream ifs(*it);
        string sentence;
        while(getline(ifs, sentence))
        {
            /* cout << sentence << endl; */
            words = _splitTool->cut(sentence);
            for(auto &word : words)
            {
                char ch = word.c_str()[0];
                if(ch >= 48 && ch <= 90) 
                    continue;
                if(_dict.find(word) != _dict.end())
                    ++_dict[word];
                else 
                    _dict[word] = 1;
            }
        }
        ifs.close();
    }
}

string DictProducer::handText(const string &route)
{
    FILE *fin, *fout;
    char buf;
    string tmpFile = "./.tmp";
    fin = fopen(route.c_str(), "r");
    if(!fin) { perror("fopen"); return tmpFile; }
    fout = fopen(tmpFile.c_str(), "w");
    if(!fout) { perror("fopen"); return tmpFile; }
    while(buf = fgetc(fin), EOF != buf)
    {
        if((buf >= 'a' && buf <= 'z') || (buf >= 'A' && buf <= 'Z')
           || buf == ' ' || buf == '\n')
        {
            fputc(buf, fout);
        } else {
            fputc(' ', fout);
        }
    }
    fclose(fin);
    fclose(fout);
    return tmpFile;
}

void DictProducer::storeDictionary(const string &route)
{
    //词典路径存放入配置文件
    string storePath = route;
    set<string> str = { storePath };
    Configuration::getpInstance()->insertConfMap("DICT_ENG", str);    
    //若放在循环中打开文件，输出全部会引发段错误,最多可打开1024个文件 
    FILE *fp = fopen("./Dictionary.txt", "w");
    if(!fp) { perror("fopen"); }
    for(auto it = _dict.begin(); it != _dict.end(); ++it)
    {
        fprintf(fp, "%-20s  %-5d\n", (*it).first.c_str(), (*it).second);
    }
}

void DictProducer::createIndexTable()
{
    for(auto it = _dict.begin(); it != _dict.end(); ++it)
    {
        string tmp = (*it).first;
        int ch = tmp.c_str()[0];
        //判断是ASCII表中的小写英文字母，还是UTF-8中文字符
        if(ch >= 97 && ch <= 122) {
            tmp = (*it).first.substr(0, 1);
        } else if(ch <= 90 && ch >= 48){
                continue;
        } else {
            tmp = (*it).first.substr(0, 3);
        }
        _readMap.push_back(*it);
        _indexTable[tmp].insert((int)_readMap.size() - 1);
    }
}

void DictProducer::storeIndexTable(const string &route)
{
    createIndexTable();
    std::ofstream ofs(route);
    for(auto it = _indexTable.begin(); it != _indexTable.end(); ++it)
    {
        ofs << (*it).first << "    ";
        for(auto &index : (*it).second)
            ofs << index << " ";
        ofs << endl;
    }
    ofs.close();
}

}//end of namespace wd
