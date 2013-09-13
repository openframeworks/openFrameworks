#include "testApp.h"


//--------------------------------------------------------------
void testApp::setup(){

	delay.setFeedback(0.8);
	delay.setDelay(8192);
	
	// connect each object to the next in a chain, finishing with the global sound mixer.
	noise.connectTo(filter).connectTo(delay).connectTo(ofGetSystemSoundMixer());

	soundStream.setup(2, 0, 44100, 64, 1);
	soundStream.setOutput(&ofGetSystemSoundMixer());
	
	font.loadFont(OF_TTF_SANS, 12);
	ofBackground(0);
}


//--------------------------------------------------------------
void testApp::update(){

}


//--------------------------------------------------------------
void testApp::draw(){
	
	ofSetColor(255);
	font.drawString("Move your mouse to control the filter and delay\n", 20, 20);
	
	ofLine(mouseX, 0, mouseX, ofGetHeight());
	ofLine(0, mouseY, ofGetWidth(), mouseY);
	font.drawString("Filter Frequency: " + ofToString(filterCutoff) + " Hz", mouseX + 5, mouseY - 5);
	ofPushMatrix();
	ofTranslate(mouseX, mouseY);
	ofRotate(90, 0, 0, 1);
	font.drawString("Delay Feedback: " + ofToString(delayFeedback*100, 0) + "%", 5, -5);
	ofPopMatrix();
}


//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){
	float f = ofMap(x, 0, ofGetWidth(), 0, 1, true);
	// give the frequency a logarithmic scale.
	f = f * f;
	filterCutoff = ofMap(f, 0, 1, 50, 11025, true);
	
	filter.setParameters(filterCutoff, 0.9);
	
	
	delayFeedback = ofMap(y, 0, ofGetHeight(), 0.99, 0.1, true);
	delay.setFeedback(delayFeedback);
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
	
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 

}