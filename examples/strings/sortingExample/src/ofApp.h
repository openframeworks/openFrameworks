/* Refer to the README.md in the example's root folder for more information on usage */

#pragma once

#include "ofMain.h"

// ---------------------------------------------
class LyricWord {
public:
	string  word;
	int     occurrences;
	
};


// ---------------------------------------------
class ofApp : public ofBaseApp {
	
public:
	
	void setup();
	void update();
	void draw();
	
	void keyPressed(int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y);
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void mouseEntered(int x, int y);
	void mouseExited(int x, int y);
	void windowResized(int w, int h);
	void gotMessage(ofMessage msg);
	void dragEvent(ofDragInfo dragInfo);
	
	static bool sortOnABC(const LyricWord &a, const LyricWord &b);
	static bool sortOnOccurrences(const LyricWord &a, const LyricWord &b);
	static bool sortOnLength(const LyricWord &a, const LyricWord &b);
	static bool removeWordIf(LyricWord &wrd);
	
	
	ofTrueTypeFont      font;
	string              sortTypeInfo;
	vector <LyricWord>  words;
};
