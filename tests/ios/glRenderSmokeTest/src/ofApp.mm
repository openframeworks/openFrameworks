#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	ofSetLogLevel(OF_LOG_NOTICE);
	ofSetVerticalSync(false);
	ofBackground(0);
	ofLogNotice("glRenderSmokeTest") << "renderer=" << glGetString(GL_RENDERER)
		<< " version=" << glGetString(GL_VERSION);
	core.allocate();
}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){
	if(core.finished()) {
		if(!resultLogged) {
			resultLogged = true;
			// simctl/logcat-style scraping: the runner script greps device log
			// output for this exact marker rather than relying on process exit
			// codes, since a foreground iOS app can't cleanly exit(0)/exit(1).
			ofLogNotice("glRenderSmokeTest") << core.resultLine();
		}
		return;
	}

	core.drawFrame();
}

//--------------------------------------------------------------
void ofApp::exit(){

}

//--------------------------------------------------------------
void ofApp::touchDown(ofTouchEventArgs & touch){

}

//--------------------------------------------------------------
void ofApp::touchMoved(ofTouchEventArgs & touch){

}

//--------------------------------------------------------------
void ofApp::touchUp(ofTouchEventArgs & touch){

}

//--------------------------------------------------------------
void ofApp::touchDoubleTap(ofTouchEventArgs & touch){

}

//--------------------------------------------------------------
void ofApp::touchCancelled(ofTouchEventArgs & touch){

}

//--------------------------------------------------------------
void ofApp::lostFocus(){

}

//--------------------------------------------------------------
void ofApp::gotFocus(){

}

//--------------------------------------------------------------
void ofApp::gotMemoryWarning(){

}

//--------------------------------------------------------------
void ofApp::deviceOrientationChanged(int newOrientation){

}
