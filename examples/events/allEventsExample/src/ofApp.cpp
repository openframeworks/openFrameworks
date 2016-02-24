#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){	 
	counter = 0;
	vagRounded.load("vag.ttf", 32);
	ofBackground(50,50,50);	
}

//--------------------------------------------------------------
void ofApp::update(){
	counter = counter + 0.033f;
}

//--------------------------------------------------------------
void ofApp::draw(){
	
	timeString = "time: " + ofGetTimestampString("%H:%M:%S") + "\nelapsed time: " + ofToString(ofGetElapsedTimeMillis());
	
	ofSetHexColor(0xffffff);
	vagRounded.drawString(eventString, 98,198);
	
	ofSetColor(255,122,220);
	vagRounded.drawString(eventString, 100,200);
	
	
	ofSetHexColor(0xffffff);
	vagRounded.drawString(timeString, 98,98);
	
	ofSetColor(255,122,220);
	vagRounded.drawString(timeString, 100,100);
	
}


//--------------------------------------------------------------
void ofApp::keyPressed  (int key){

	eventString = "keyPressed = (" + ofToString(key) + ") ";

	if(key & OF_KEY_MODIFIER){
		if(key >= OF_KEY_F1 && key <= OF_KEY_F12){
			eventString += "F" +  ofToString(key+1-OF_KEY_F1);
		}else{
			switch(key){
			case OF_KEY_LEFT:
				eventString += "LEFT";
				break;
			case OF_KEY_UP:
				eventString += "UP";
				break;
			case OF_KEY_RIGHT:
				eventString += "RIGHT";
				break;
			case OF_KEY_DOWN:
				eventString += "DOWN";
				break;
			case OF_KEY_PAGE_UP:
				eventString += "PAGE UP";
				break;
			case OF_KEY_PAGE_DOWN:
				eventString += "PAGE DOWN";
				break;
			case OF_KEY_HOME:
				eventString += "HOME";
				break;
			case OF_KEY_END:
				eventString += "END";
				break;
			case OF_KEY_INSERT:
				eventString += "INSERT";
				break;
			case OF_KEY_LEFT_SHIFT:
				eventString += "LEFT SHIFT";
				break;
			case OF_KEY_LEFT_CONTROL:
				eventString += "LEFT CONTROL";
				break;
			case OF_KEY_LEFT_SUPER:
				eventString += "LEFT SUPER";
				break;
			case OF_KEY_RIGHT_SHIFT:
				eventString += "RIGHT SHIFT";
				break;
			case OF_KEY_RIGHT_CONTROL:
				eventString += "RIGHT CONTROL";
				break;
			case OF_KEY_LEFT_ALT:
				eventString += "LEFT ALT";
				break;
            case OF_KEY_RIGHT_ALT:
				eventString += "RIGHT ALT";
				break;
			case OF_KEY_RIGHT_SUPER:
				eventString += "RIGHT SUPER";
				break;
			}
		}
	}else{
		eventString += (char)key;
	}
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){ 
	eventString = "keyReleased = (" + ofToString(key) + ") ";

	if(key & OF_KEY_MODIFIER){
		if(key >= OF_KEY_F1 && key <= OF_KEY_F12){
			eventString += "F" +  ofToString(key+1-OF_KEY_F1);
		}else{
			switch(key){
			case OF_KEY_LEFT:
				eventString += "LEFT";
				break;
			case OF_KEY_UP:
				eventString += "UP";
				break;
			case OF_KEY_RIGHT:
				eventString += "RIGHT";
				break;
			case OF_KEY_DOWN:
				eventString += "DOWN";
				break;
			case OF_KEY_PAGE_UP:
				eventString += "PAGE UP";
				break;
			case OF_KEY_PAGE_DOWN:
				eventString += "PAGE DOWN";
				break;
			case OF_KEY_HOME:
				eventString += "HOME";
				break;
			case OF_KEY_END:
				eventString += "END";
				break;
			case OF_KEY_INSERT:
				eventString += "INSERT";
				break;
			case OF_KEY_LEFT_SHIFT:
				eventString += "LEFT SHIFT";
				break;
			case OF_KEY_LEFT_CONTROL:
				eventString += "LEFT CONTROL";
				break;
			case OF_KEY_LEFT_SUPER:
				eventString += "LEFT SUPER";
				break;
			case OF_KEY_RIGHT_SHIFT:
				eventString += "RIGHT SHIFT";
				break;
			case OF_KEY_RIGHT_CONTROL:
				eventString += "RIGHT CONTROL";
				break;
			case OF_KEY_LEFT_ALT:
				eventString += "LEFT ALT";
				break;
            case OF_KEY_RIGHT_ALT:
				eventString += "RIGHT ALT";
				break;
			case OF_KEY_RIGHT_SUPER:
				eventString += "RIGHT SUPER";
				break;
			}
		}
	}else{
		eventString += (char)key;
	}
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
	eventString = "mouseMoved = (" + ofToString(x) + ", " + ofToString(y) + ")";
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
	eventString = "mouseDragged = (" + ofToString(x) + ", " + ofToString(y) + " - button " + ofToString(button) + ")";
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
	eventString = "mousePressed = (" + ofToString(x) + ", " + ofToString(y) + " - button " + ofToString(button) + ")";
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
	eventString = "mouseReleased = (" + ofToString(x) + ", " + ofToString(y) + " - button " + ofToString(button) + ")";
}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){
	eventString = "resized = (" + ofToString(w) + ", " + ofToString(h) + ")";
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){
	eventString = "gotMessage " + msg.message;
}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 
	eventString = ofToString((int)dragInfo.files.size()) + " files dragged into the window at (" + ofToString((int)dragInfo.position.x) + ", " + ofToString((int)dragInfo.position.y) + ")";
	
}
