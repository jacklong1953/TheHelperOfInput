#include "Configuration.h"

namespace wd
{

Configuration *Configuration::_pInstance = Configuration::getpInstance();
pthread_once_t Configuration::_pthreadOnce = PTHREAD_ONCE_INIT;

void Configuration::createConfigMap()
{
    ifstream ifs(_filepath.c_str());
    string text;
    string attribute;
    string relativeRout;
    string word;
    while(getline(ifs, text))
    {
        set<string>  pathname;
        istringstream line(text);
        line >> attribute;
        line >> relativeRout;
        while(line >> word)
        {
            string tmp = relativeRout;
            tmp.append(word);
            pathname.insert(tmp);
        }
        _configMap.insert(make_pair(attribute, pathname));
    }
    ifs.close();
}

/* const string &getFilePath() const { return _filepath; } */

/* const string & setFilePath(const string &filepath) */
/* { */
/*     _filepath = filepath; */ 
/*     return _filepath; */
/* } */

/* void createConfigMap() */
/* { */
/*     ifstream ifs(_filepath.c_str()); */
/*     string data1; */
/*     while(ifs >> data1) */
/*     { */
/*         string data2; */
/*         ifs >> data2; */
/*         _configMap.insert(make_pair(data1, data2)); */
/*     } */
/*     ifs.close(); */
/* } */

}//end of namespace wd
