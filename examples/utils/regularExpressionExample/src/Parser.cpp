#include "Parser.h"
#include <regex>

string Parser::grepStringInRegex(string _str, string _reg){
    smatch match;
    regex regEx(_reg, regex_constants::icase);
    
    stringstream buffer;
    while (regex_search(_str,match,regEx)) {
        for (auto x : match){
            buffer << x;
        }
        _str = match.suffix().str();
    }
    return buffer.str();
}

int Parser::countOccurencesInRegex(string _str, string _reg){
    regex regEx(_reg, regex_constants::icase);
    auto wordsBegin = sregex_iterator(_str.begin(), _str.end(), regEx);
    auto wordsEnd = sregex_iterator();
    return distance(wordsBegin, wordsEnd);
};

bool Parser::isKeyInRegex(int keyPressed, string _reg){
    string typedKey(1, keyPressed);
    regex regEx(_reg, regex_constants::icase);
    if (regex_match(typedKey, regEx)) {
        return true;
    }else{
        return false;
    }
}

vector<string> Parser::matchesInRegex(string _str, string _reg){
    smatch match;
    regex regEx(_reg, regex_constants::icase);
    vector<string> results;
    auto wordsBegin = sregex_iterator(_str.begin(), _str.end(), regEx);
    auto wordsEnd = sregex_iterator();
    
    for(std::sregex_iterator i = wordsBegin; i != wordsEnd; ++i){
        smatch m = *i;
        results.push_back(m.str());
    }
    return results;
}

