#include "SplitTool.h"

const char* const DICT_PATH = "../cppjieba/dict/jieba.dict.utf8";
const char* const HMM_PATH = "../cppjieba/dict/hmm_model.utf8";
const char* const IDF_PATH = "../cppjieba/dict/idf.utf8";
const char* const STOP_WORD_PATH = "../cppjieba/dict/stop_words.utf8";


namespace wd
{

vector<string> MyJieba::cut(const string &sentence)
{
    vector<string> words;
    size_t topN = 5;
    _extractor.Extract(sentence, words, topN);
    return words;
}

cppjieba::KeywordExtractor MyJieba::init()
{
    cppjieba::KeywordExtractor _extractor(DICT_PATH,
                                          HMM_PATH,
                                          IDF_PATH,
                                          STOP_WORD_PATH);
    return _extractor;
}

}//end of namespace wd
