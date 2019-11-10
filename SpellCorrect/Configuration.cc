#include "Configuration.h"

namespace wd
{

Configuration *Configuration::_pInstance = Configuration::getpInstance();
pthread_once_t Configuration::_pthreadOnce = PTHREAD_ONCE_INIT;

void Configuration::createConfigMap()
{
    ifstream ifs(_filepath.c_str());
    string text;
    string word;
    while(getline(ifs, text))
    {
        istringstream line(text);
        while(line >> word)
        {
            string tmp;
            line >> tmp;
            _configMap.insert(make_pair(word, tmp));
        }
    }
    ifs.close();
}

}//end of namespace wd
