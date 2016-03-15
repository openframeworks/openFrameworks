#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){

	ofSetFrameRate(60);

	ofSetVerticalSync(true);

	gui_doc =  std::make_unique<ofx::DOM::Document>();
	gui = gui_doc->add<ofxPanel>("panel","",10,10);

	filled = gui->add<ofxToggle>("fill");
	radius = gui->add<ofxFloatSlider>("radius", 140, 10, 300);
	center = gui->add<ofxVec2Slider>("center", ofVec2f(ofGetWidth()*.5, ofGetHeight()*.5), ofVec2f(0, 0), ofVec2f(ofGetWidth(), ofGetHeight()));
	color = gui->add<ofxColorSlider>("color", ofColor(100, 100, 140), ofColor(0, 0), ofColor(255, 255));
	circleResolution = gui->add<ofxIntSlider>("circle res", 5, 3, 90);
	twoCircles = gui->add<ofxButton>("two circles");
	ringButton = gui->add<ofxButton>("ring");
	screenSize = gui->add<ofxLabel>("screen size", ofToString(ofGetWidth())+"x"+ofToString(ofGetHeight()));

	ringButton->addListener(this, &ofApp::ringButtonPressed);
	//ugly fix to trigger initial value
	ringButton->getParameter().cast<bool>().set(*ringButton);

	bHide = false;

	ring.load("ring.wav");
}

//--------------------------------------------------------------
void ofApp::exit(){
	ringButton->removeListener(this, &ofApp::ringButtonPressed);
}

//--------------------------------------------------------------
void ofApp::ringButtonPressed(){
	ring.play();
}

//--------------------------------------------------------------
void ofApp::update(){
	ofSetCircleResolution(*circleResolution);
}

//--------------------------------------------------------------
void ofApp::draw(){
	ofBackgroundGradient(ofColor::white, ofColor::gray);

	if(*filled){
		ofFill();
	}else{
		ofNoFill();
	}

	ofSetColor(*color);
	if(*twoCircles){
		ofDrawCircle((*center)->x-*radius*.5, (*center)->y, *radius );
		ofDrawCircle((*center)->x+*radius*.5, (*center)->y, *radius );
	}else{
		ofDrawCircle((ofVec2f)*center, *radius );
	}

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	if(key == 'h'){
		bHide = !bHide;
	}
	else if(key == 's'){
		gui->saveToFile("settings.xml");
	}
	else if(key == 'l'){
		gui->loadFromFile("settings.xml");
	}
	else if(key == ' '){
		*color = ofColor(255);
	}
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

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
	*screenSize = ofToString(w) + "x" + ofToString(h);
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){

}
