#include "ofApp.h"
#include <regex>
// This example shows what you can do with regular espressions, it does not
// cover the grammar of the regular expression but how they can be used.
// Here some links to learn more about regular expression
// http://www.informit.com/articles/article.aspx?p=2079020
// https://solarianprogrammer.com/2011/10/20/cpp-11-regex-tutorial-part-2/
// http://www.cplusplus.com/reference/regex/
//--------------------------------------------------------------
void ofApp::setup(){
	text = "openFrameworks is developed in a collaborative way. We use git, a distributed versioning system, which means also that people can branch, experiment, and make suggestions. If you look at the network diagram on GitHub, it's looks like some alien diagram, full of weaving branches, code pulling apart and coming together. There's a huge community, all over the world, working on the core code: fixing bugs, submitting pull requests, and shaping the tool the way they want to see it. It's a world wide project, and it's common to wake up in the USA to an inbox full of pull requests and issues emails from coders in Asia and Europe. Over 70 people have contributed to the openFrameworks core directly, and hundreds of people have forked the code or contributed in other ways.";
	// Regular expression can be used:

	// To grep substrings and return them in a string
	// [a-z]*ing\\s means all verbs ending with 'ing' followed by a whitespace
	wordsWithS = grepStringInRegex(text, "[a-z]*ing\\s");

	// To Count occurrences of a string in another string
	countedOccurrences = countOccurencesInRegex(text, "[^\\s]+");

	//To collect all the items that match certains criteria
	// in this case, letter containing 'r'
	matchesWithR = matchesInRegex(text, "[a-z]+r[a-z]+");
}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){
	ofSetColor(ofColor::red);
	auto msg ="Number of words in the text";
	ofDrawBitmapString(msg, 20, 10);
	ofSetColor(ofColor::black);
	ofDrawBitmapString(countedOccurrences, 20, 30);

	ofSetColor(ofColor::red);
	auto msg1 ="Words containing 'ing'";
	ofDrawBitmapString(msg1, 20, 70);
	ofSetColor(ofColor::black);
	ofDrawBitmapString(wordsWithS, 20, 90);

	ofSetColor(ofColor::red);
	auto msg2 = "There are "+ofToString(matchesWithR.size())+" Words containing 'r'";
	ofDrawBitmapString(msg2, 20, 130);
	ofSetColor(ofColor::black);
	int y = 150;
	for(auto s:matchesWithR){
		ofDrawBitmapString(s, 20, y);
		y+=20;
	}

	// Regular expression can also be used parse a text file
	file.open(ofToDataPath("HeadShouldersKneesAndToes.lrc"), ofFile::ReadWrite, false);
	ofSetColor(ofColor::red);
	auto msg3 ="Let's parse a lyric file for a karaoke";
	ofDrawBitmapString(msg3, 400, 130);
	ofSetColor(ofColor::black);
	int posY = 150;
	string line;
	while(getline(file,line)){
		if(line.empty() == false) {
			string minutes = grepStringInRegex(line, "[0-9]+:[0-9]+");
			string sentence = grepStringInRegex(line, "[^0-9_:\\[\\]]");
			ofDrawBitmapString("Time:"+minutes, 400, posY);
			ofDrawBitmapString("Sentence:"+sentence, 500, posY);
			posY += 20;
		}
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
	} else {
		return false;
	}
}

vector<string> ofApp::matchesInRegex(string _str, string _reg){
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
