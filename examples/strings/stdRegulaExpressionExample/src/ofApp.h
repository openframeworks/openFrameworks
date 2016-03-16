#pragma once

#include "ofMain.h"

class ofApp : public ofBaseApp{

public:
    void setup();
    void update();
    void draw();
    void keyPressed(int key);
    string grepStringInRegex(string str, string reg);
    int countOccurencesInRegex(string, string reg);
    bool isKeyInRegex(int key, string reg);
    vector<string> matchesInRegex(string str, string reg);

    string text;
    string greppedText;
    vector<string> matches;
    int countedOccurrences;
    ofFile file;
};
