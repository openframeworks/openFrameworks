#pragma once
#include "ofMain.h"

class Parser{
public:
    static string grepStringInRegex(string str, string reg);
    static int countOccurencesInRegex(string, string reg);
    static bool isKeyInRegex(int key, string reg);
    static vector<string> matchesInRegex(string str, string reg);
};