#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){	 
	counter = 0;
	vagRounded.loadFont("vag.ttf", 32);
	ofBackground(50,50,50);	
}

//--------------------------------------------------------------
void testApp::update(){
	counter = counter + 0.033f;
}

//--------------------------------------------------------------
void testApp::draw(){
	
	sprintf (timeString, "time: %i:%i:%i \nelapsed time %lli", ofGetHours(), ofGetMinutes(), ofGetSeconds(), ofGetElapsedTimeMillis());
	
	float w = vagRounded.stringWidth(eventString);
	float h = vagRounded.stringHeight(eventString);
	
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
void testApp::keyPressed  (int key){ 
	if(key & OF_KEY_MODIFIER){
		if(key >= OF_KEY_F1 && key <= OF_KEY_F12){
			sprintf(eventString, "keyPressed = (%i) %s", key, ("F" + ofToString(key+1-OF_KEY_F1)).c_str());
		}else{
			switch(key){
			case OF_KEY_LEFT:
				sprintf(eventString, "keyPressed = (%i) %s", key, "LEFT");
				break;
			case OF_KEY_UP:
				sprintf(eventString, "keyPressed = (%i) %s", key, "UP");
				break;
			case OF_KEY_RIGHT:
				sprintf(eventString, "keyPressed = (%i) %s", key, "RIGHT");
				break;
			case OF_KEY_DOWN:
				sprintf(eventString, "keyPressed = (%i) %s", key, "DOWN");
				break;
			case OF_KEY_PAGE_UP:
				sprintf(eventString, "keyPressed = (%i) %s", key, "PAGE UP");
				break;
			case OF_KEY_PAGE_DOWN:
				sprintf(eventString, "keyPressed = (%i) %s", key, "PAGE DOWN");
				break;
			case OF_KEY_HOME:
				sprintf(eventString, "keyPressed = (%i) %s", key, "HOME");
				break;
			case OF_KEY_END:
				sprintf(eventString, "keyPressed = (%i) %s", key, "END");
				break;
			case OF_KEY_INSERT:
				sprintf(eventString, "keyPressed = (%i) %s", key, "INSERT");
				break;
			case OF_KEY_LEFT_SHIFT:
				sprintf(eventString, "keyPressed = (%i) %s", key, "LEFT SHIFT");
				break;
			case OF_KEY_LEFT_CONTROL:
				sprintf(eventString, "keyPressed = (%i) %s", key, "LEFT CONTROL");
				break;
			case OF_KEY_LEFT_SUPER:
				sprintf(eventString, "keyPressed = (%i) %s", key, "LEFT SUPER");
				break;
			case OF_KEY_RIGHT_SHIFT:
				sprintf(eventString, "keyPressed = (%i) %s", key, "RIGHT SHIFT");
				break;
			case OF_KEY_RIGHT_CONTROL:
				sprintf(eventString, "keyPressed = (%i) %s", key, "RIGHT CONTROL");
				break;
			case OF_KEY_RIGHT_ALT:
				sprintf(eventString, "keyPressed = (%i) %s", key, "RIGHT ALT");
				break;
			case OF_KEY_RIGHT_SUPER:
				sprintf(eventString, "keyPressed = (%i) %s", key, "RIGHT SUPER");
				break;
			}
		}
	}else{
		sprintf(eventString, "keyPressed = (%i) %c", key, (char)key);
	}
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){ 
	if(key & OF_KEY_MODIFIER){
		if(key >= OF_KEY_F1 && key <= OF_KEY_F12){
			sprintf(eventString, "keyReleased = (%i) %s", key, ("F" + ofToString(key+1-OF_KEY_F1)).c_str());
		}else{
			switch(key){
			case OF_KEY_LEFT:
				sprintf(eventString, "keyReleased = (%i) %s", key, "LEFT");
				break;
			case OF_KEY_UP:
				sprintf(eventString, "keyReleased = (%i) %s", key, "UP");
				break;
			case OF_KEY_RIGHT:
				sprintf(eventString, "keyReleased = (%i) %s", key, "RIGHT");
				break;
			case OF_KEY_DOWN:
				sprintf(eventString, "keyReleased = (%i) %s", key, "DOWN");
				break;
			case OF_KEY_PAGE_UP:
				sprintf(eventString, "keyReleased = (%i) %s", key, "PAGE UP");
				break;
			case OF_KEY_PAGE_DOWN:
				sprintf(eventString, "keyReleased = (%i) %s", key, "PAGE DOWN");
				break;
			case OF_KEY_HOME:
				sprintf(eventString, "keyReleased = (%i) %s", key, "HOME");
				break;
			case OF_KEY_END:
				sprintf(eventString, "keyReleased = (%i) %s", key, "END");
				break;
			case OF_KEY_INSERT:
				sprintf(eventString, "keyReleased = (%i) %s", key, "INSERT");
				break;
			case OF_KEY_LEFT_SHIFT:
				sprintf(eventString, "keyReleased = (%i) %s", key, "LEFT SHIFT");
				break;
			case OF_KEY_LEFT_CONTROL:
				sprintf(eventString, "keyReleased = (%i) %s", key, "LEFT CONTROL");
				break;
			case OF_KEY_LEFT_SUPER:
				sprintf(eventString, "keyReleased = (%i) %s", key, "LEFT SUPER");
				break;
			case OF_KEY_RIGHT_SHIFT:
				sprintf(eventString, "keyReleased = (%i) %s", key, "RIGHT SHIFT");
				break;
			case OF_KEY_RIGHT_CONTROL:
				sprintf(eventString, "keyReleased = (%i) %s", key, "RIGHT CONTROL");
				break;
			case OF_KEY_RIGHT_ALT:
				sprintf(eventString, "keyReleased = (%i) %s", key, "RIGHT ALT");
				break;
			case OF_KEY_RIGHT_SUPER:
				sprintf(eventString, "keyReleased = (%i) %s", key, "RIGHT SUPER");
				break;
			}
		}
	}else{
		sprintf(eventString, "keyReleased = (%i) %c", key, (char)key);
	}
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){
	sprintf(eventString, "mouseMoved = (%i,%i)", x, y);
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){
	sprintf(eventString, "mouseDragged = (%i,%i - button %i)", x, y, button);
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
	sprintf(eventString, "mousePressed = (%i,%i - button %i)", x, y, button);
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){
	sprintf(eventString, "mouseReleased = (%i,%i - button %i)", x, y, button);
}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){
	sprintf(eventString, "resized = (%i,%i)", w, h);
}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){
	sprintf(eventString, "gotMessage %s ", msg.message.c_str());
}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 
	sprintf(eventString, "%i files dragged into the window at (%i, %i)", (int)dragInfo.files.size(), (int)dragInfo.position.x, (int)dragInfo.position.y);
}
