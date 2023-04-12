#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){	 
	eventString = "Alpha";

	vagRounded.load("vag.ttf", 32);
	ofBackground(50,50,50);	

	rainbow.allocate(256, 256, OF_IMAGE_COLOR_ALPHA);
	rainbow.load("rainbow.tiff");
	blendMode = OF_BLENDMODE_ALPHA;
}


//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){
	
	timeString = "Press 1 - 5 to switch blend modes";
	
	ofEnableBlendMode(OF_BLENDMODE_ALPHA);
	
	ofSetHexColor(0xffffff);
	vagRounded.drawString(eventString, 98,198);
	
	ofSetColor(255,122,220);
	vagRounded.drawString(eventString, 100,200);
	
	ofSetHexColor(0xffffff);
	vagRounded.drawString(timeString, 98,98);
	
	ofSetColor(255,122,220);
	vagRounded.drawString(timeString, 100,100);

	ofSetColor(255, 255, 255,255);
	
	
	ofEnableBlendMode(blendMode);
	
	rainbow.draw(mouseX, mouseY);
	
	ofDisableBlendMode();
}


//--------------------------------------------------------------
void ofApp::keyPressed  (int key){ 

	switch (key) {
		case 49:
			blendMode = OF_BLENDMODE_ALPHA;
			eventString = "Alpha";
			break;
		case 50:
			blendMode = OF_BLENDMODE_ADD;
			eventString = "Add";
			break;
		case 51:
			blendMode = OF_BLENDMODE_MULTIPLY;
			eventString = "Multiply";
			break;
		case 52:
			blendMode = OF_BLENDMODE_SUBTRACT;
			eventString = "Subtract";
			break;
		case 53:
			blendMode = OF_BLENDMODE_SCREEN;
			eventString = "Screen";
			break;
		default:
			break;
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

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
