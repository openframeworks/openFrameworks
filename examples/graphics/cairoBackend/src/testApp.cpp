#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
	path.setStrokeWidth(6);
	path.setFillColor(ofColor(255,0,0));
	path.setStrokeColor(ofColor(255));
	path.setUseShapeColor(true);
	ofBackground(0);
	ofSetColor(255);
	method = BOTH;
	firstPoint = true;
}

//--------------------------------------------------------------
void testApp::update(){
}

//--------------------------------------------------------------
void testApp::draw(){
	if(method==CAIRO || method==BOTH) ofEnableCairoGLBackend();
	path.draw();
	if(method==CAIRO || method==BOTH) ofDisableCairoGLBackend();

	if(method == BOTH){
		ofPushMatrix();
		ofTranslate(ofGetWidth()*.5,0);
		path.draw();
		ofPopMatrix();
	}

	if(method==CAIRO)
		ofDrawBitmapString("cairo rendering",20,20);
	if(method==GL)
		ofDrawBitmapString("gl rendering",20,20);
	if(method==BOTH)
		ofDrawBitmapString("both",20,20);
	ofDrawBitmapString("press mouse button to create a vertex, space to close the path",20,40);
	ofDrawBitmapString(ofToString(ofGetFrameRate()),20,60);
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
	if(key==OF_KEY_UP){
		method=(Method)(method+1);
		method=(Method)(method%3);
	}
	if(key==OF_KEY_DOWN){
		method=(Method)(method-1);
		method=(Method)(method%3);
	}
	if(key==' '){
		path.close();
		firstPoint = true;
	}
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
	if(button==0){
		if(firstPoint){
			path.moveTo(x,y);
			firstPoint = false;
		}else{
			path.lineTo(x,y);
		}
	}
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
