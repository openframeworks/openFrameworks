#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){

	ofSetLogLevel(OF_LOG_VERBOSE);

	setupPage1();
	setupPage2();
	setupPage3();

	page.setup("single page");
	page.setSize(300, 300);
    page.add(panel3);

	page1.setup("page 1");
    page1.add(panel1);

	page2.setup("page 2");
    page2.add(panel2);

	tabbed_pages.setup("tabbed pages", "", page.getShape().getRight() + 10);
	tabbed_pages.setSize(500, 300);
    tabbed_pages.add(page1);
    tabbed_pages.add(page2);

}

//--------------------------------------------------------------
void ofApp::exit(){
}

//--------------------------------------------------------------
void ofApp::update(){
}

//--------------------------------------------------------------
void ofApp::draw(){

	ofSetColor(255);

	page.draw();

	tabbed_pages.draw();

}

void ofApp::setupPage1(){
	panel1.setup("panel1");
    panel1.add(slider_param.set("slider", 0.5, 0, 1));
}

void ofApp::setupPage2(){
    panel2.setup("horizontal", "", 60, 10);
	panel2.setShowHeader(false);
    panel2.setExclusiveToggles(true);
    panel2.add(toggle2_param.set("toggle2", false));
    panel2.add(toggle3_param.set("toggle3", false));
    panel2.add(toggle4_param.set("toggle4", false));
}

void ofApp::setupPage3(){
    panel3.setup("just some toggles", "", 60, 66);
    panel3.setExclusiveToggles(true);
    panel3.add(toggle2_param);
    panel3.add(toggle3_param);
    panel3.add(toggle4_param);
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
