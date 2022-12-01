#include "ofApp.h"


//--------------------------------------------------------------
void ofApp::setup() {

	font.load("verdana.ttf", 34);

	// load the lyrics from a text file and split them
	// up in to a vector of strings
	string lyrics = ofBufferFromFile("lyrics.txt").getText();
	step = 0;
	words = ofSplitString(lyrics, " ");

	// we are running the systems commands
	// in a sperate thread so that it does
	// not block the drawing
	startThread();
}

//--------------------------------------------------------------
void ofApp::threadedFunction() {

	while (isThreadRunning()) {

		// call the system command say
		#ifdef TARGET_OSX
			string cmd = "say " + words[step] + " "; // create the command
		#endif
		#ifdef TARGET_WIN32
			string cmd = "data\\SayStatic.exe " + words[step];        // create the command
		#endif
		#ifdef TARGET_LINUX
			string cmd = "echo " + words[step] + "|espeak";           // create the command
		#endif

				// print command and execute it
				cout << cmd << endl;
				ofSystem(cmd.c_str());

		// step to the next word
		step ++;
		step %= words.size();

		// slowdown boy
		ofSleepMillis(10);
	}
}

//--------------------------------------------------------------
void ofApp::update() {
	// get a random voice
}


//--------------------------------------------------------------
void ofApp::draw() {
	// center the word on the screen
	float x = (ofGetWidth() - font.stringWidth(words[step])) / 2;
	float y = ofGetHeight() / 2;

	// draw the word
	ofSetColor(0);
	font.drawString(words[step], x, y);
}

//--------------------------------------------------------------
void ofApp::exit() {
	// stop the thread on exit
	waitForThread(true);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){

}
