#include "ofApp.h"
#include <regex>
// This example shows what you can do with regular espressions, it does not
// cover the grammar of the regular expression but how they can be used.
//
// http://www.informit.com/articles/article.aspx?p=2079020
// https://solarianprogrammer.com/2011/10/20/cpp-11-regex-tutorial-part-2/
// http://www.cplusplus.com/reference/regex/
//--------------------------------------------------------------
void ofApp::setup(){
    text = "openFrameworks is developed in a collaborative way. We use git, a distributed versioning system, which means also that people can branch, experiment, and make suggestions. If you look at the network diagram on GitHub, it's looks like some alien diagram, full of weaving branches, code pulling apart and coming together. There's a huge community, all over the world, working on the core code: fixing bugs, submitting pull requests, and shaping the tool the way they want to see it. It's a world wide project, and it's common to wake up in the USA to an inbox full of pull requests and issues emails from coders in Asia and Europe. Over 70 people have contributed to the openFrameworks core directly, and hundreds of people have forked the code or contributed in other ways.";
    // To grep substrings and return them in a string
    // [a-z]*ing\\s means all verbs ending with 'ing' followed by a whitespace
    greppedText = grepStringInRegex(text, "[a-z]*ing\\s");

    // To Count occurrences of a string in another string
    countedOccurrences = countOccurencesInRegex(text, "[^\\s]+");

    // To parse a text file
    file.open(ofToDataPath("HeadShouldersKneesAndToes.lrc"), ofFile::ReadWrite, false);
    string line;
    while(getline(file,line)){
        if(line.empty() == false) {
            string minutes = grepStringInRegex(line, "[0-9]+:[0-9]+");
            cout << "Time:" << minutes << endl;
            string sentence = grepStringInRegex(line, "[^0-9_:\\[\\]])");
            cout << "sentence:" << sentence << endl;
        }
    }

    //To collect all the items that match certains criteria defined by a regular expression
    matches = matchesInRegex(text, "[a-z]+r[a-z]+");
}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){
    ofDrawBitmapString(greppedText, 20, 20);
    ofDrawBitmapString(countedOccurrences, 20, 40);
    ofDrawBitmapString("Words containing 'r'", 20, 60);
    ofDrawBitmapString(matches.size(), 20, 80);
    int y = 100;
    for(auto s:matches){
        ofDrawBitmapString(s, 20, y);
        y+=20;
    }
}

//--------------------------------------------------------------
void ofApp::keyPressed(int _key){
    if(isKeyInRegex(_key, "[0-9]")){
        cout << "You have entered a number " << endl;
    };
    if(isKeyInRegex(_key, "[a-zA-Z]")){
        cout << "You have entered a letter" << endl;
    };
}

string ofApp::grepStringInRegex(string _str, string _reg){
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

int ofApp::countOccurencesInRegex(string _str, string _reg){
    regex regEx(_reg, regex_constants::icase);
    auto wordsBegin = sregex_iterator(_str.begin(), _str.end(), regEx);
    auto wordsEnd = sregex_iterator();
    return distance(wordsBegin, wordsEnd);
};

bool ofApp::isKeyInRegex(int keyPressed, string _reg){
    string typedKey(1, keyPressed);
    regex regEx(_reg, regex_constants::icase);
    if (regex_match(typedKey, regEx)) {
        return true;
    }else{
        return false;
    }
}

vector<string> ofApp::matchesInRegex(string _str, string _reg){
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
