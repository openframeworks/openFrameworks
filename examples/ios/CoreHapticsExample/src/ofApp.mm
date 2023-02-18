#include "ofApp.h"

void ofApp::update(){
    haptics_.sendParameters(fmod(ofGetElapsedTimef()*2.0f,1),ofMap(sin(ofGetElapsedTimef()*PI/3.0f), -1, 1, 0, 1));
}
