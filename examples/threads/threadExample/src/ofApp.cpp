#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	threadedObject.setup();
	doLock = false;
	ofLogNotice("main thread Id") << ofGetMainThreadId();
}

//--------------------------------------------------------------
void ofApp::update(){
	if(doLock){
		threadedObject.update();
	}else{
		// this updates the texture without locking the pixels
		// which will make the thread update the pixels simultaneously so
		// we will see tearing.
		// it still won't crash since we are still reading from a "legal"
		// memory area.
		threadedObject.updateNoLock();
	}
}

//--------------------------------------------------------------
void ofApp::draw(){
	ofSetColor(255);
	threadedObject.draw();
	auto threadFrame = threadedObject.getThreadFrameNum();

	ofSetColor(255,0,0);
	ofDrawBitmapString("app frame: " + ofToString(ofGetFrameNum()), 20,20);
	ofDrawBitmapString("thread frame: " + ofToString(threadFrame), 20,35);
	ofDrawBitmapString("diff: " + ofToString(int64_t(ofGetFrameNum()) - threadFrame), 20,50);
	ofDrawBitmapString("a starts the thread", 20,65);
	ofDrawBitmapString("s stops the thread", 20,80);
	ofDrawBitmapString("l turns lock on", 20,95);
	ofDrawBitmapString("n turns lock off", 20,110);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	if (key == 'a'){
		if (ofIsCurrentThreadTheMainThread()) {
			ofLogNotice("ofApp::keyPressed") << "processed in main thread";
		} else {
			// will never happen but to document the branch:
			ofLogNotice("ofApp::keyPressed") << "processed in other thread";
		}
		threadedObject.start();
	}else if (key == 's'){
		threadedObject.stop();
	}else if (key == 'n'){
		doLock = false;
	}else if (key == 'l'){
		doLock = true;
	}
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//------------- -------------------------------------------------
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

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){

}
