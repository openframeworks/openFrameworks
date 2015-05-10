#include "ofApp.h"

static string voices[24] = {"Agnes", "Albert", "Alex", "Bad News", "Bahh", "Bells", "Boing", "Bruce", "Bubbles", "Cellos", "Deranged", "Fred", "Good News", "Hysterical", "Junior", "Kathy", "Pipe Organ", "Princess", "Ralph", "Trinoids", "Vicki", "Victoria", "Whisper", "Zarvox"};

//--------------------------------------------------------------
void ofApp::setup() {

	font.load("verdana.ttf", 34);
    voice = "Cellos";
    bRandomVoice = false;

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
            string cmd = "say -v "+voice+" "+words[step]+" ";   // create the command
            system(cmd.c_str());
        #endif
        #ifdef TARGET_WIN32
            string cmd = "data\\SayStatic.exe "+words[step];   // create the command
            cout << cmd << endl;
            system(cmd.c_str());
        #endif



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
    if(bRandomVoice) {
        voice = voices[(int)ofRandom(24)];
    }
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
    if(key == 'r') {
        bRandomVoice = !bRandomVoice;
    }

    if(key == ' ') {
        voice = voices[(int)ofRandom(24)];
    }
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
