#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){

	ofSetFrameRate(120);

	ofSetLogLevel(OF_LOG_VERBOSE);

	gui_doc =  std::make_unique<ofx::DOM::Document>();

	tabbed_pages = gui_doc->add<ofxGuiTabs>("tabbed pages", "", 10,10);
	tabbed_pages->setSize(500, 300);

	page1 = tabbed_pages->add<ofxPanel>("page 1");

	panel1 = page1->add<ofxGuiGroup>("panel1");
	panel1->add(slider_param.set("slider", 0.5, 0, 1));

	page2= tabbed_pages->add<ofxPanel>("page 2");

	panel2 = page2->add<ofxGuiGroup>("horizontal", "", 60, 10);
	panel2->setShowHeader(false);
	panel2->setExclusiveToggles(true);
	panel2->add(toggle2_param.set("toggle2", false));
	panel2->add(toggle3_param.set("toggle3", false));
	panel2->add(toggle4_param.set("toggle4", false));

}

//--------------------------------------------------------------
void ofApp::exit(){
}

//--------------------------------------------------------------
void ofApp::update(){
}

//--------------------------------------------------------------
void ofApp::draw(){

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
	switch(key){
	 case 'f': {
		 ofToggleFullscreen();
		 break;
	 }

	 default:
		 break;
	}

}

//--------------------------------------------------------------
void ofApp::mouseMoved(ofMouseEventArgs & args){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(ofMouseEventArgs & args){
}

//--------------------------------------------------------------
void ofApp::mousePressed(ofMouseEventArgs & args){
}

//--------------------------------------------------------------
void ofApp::mouseReleased(ofMouseEventArgs & args){
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
