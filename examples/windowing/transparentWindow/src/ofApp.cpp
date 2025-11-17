#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){	 
	if(bLogVerbose){
		ofSetLogLevel(OF_LOG_VERBOSE);
	}
	int screenW = ofGetScreenWidth();
	int screenH = ofGetScreenHeight();
	ofSetWindowPosition(screenW/2-1280/2, screenH/2-720/2);
	fontRenderer.load(OF_TTF_MONO, 14);
	lineHeight = fontRenderer.getLineHeight();
	
	bFullscreen	= 0;
	ofBackground(0, 0, 0, 0);
	ofSetWindowTitle("GLFW Transparency"); // should not be shown with floating
	ofSetFrameRate(60);
	uint64_t stepNanos = ofGetFixedStepForFps(60);
	ofSetTimeModeFixedRate(stepNanos);
	
	ballPositionX = 500;
	ballPositionY = 500;
	ballVelocityX = ofRandom(-5,5);
	ballVelocityY = ofRandom(-5,5);
	
	bgColor = ofColor(240, 248, 255); // Light Blue
	textColor = ofColor(0, 51, 102);  // Dark Blue
	primaryBtnColor = ofColor(51, 153, 255); // Bright Blue
	secondaryBtnColor = ofColor(204, 204, 255); // Light Blue
	highlightColor = ofColor(255, 204, 204); // Soft Pink
}

//--------------------------------------------------------------
void ofApp::update(){
	
	if(bFullscreen){
		ofHideCursor();
	} else{
		ofShowCursor();
	}

	ballPositionX += ballVelocityX;
	ballPositionY += ballVelocityY;
	
	int posx = ofGetWindowPositionX();
	int posy = ofGetWindowPositionY();
	
	if (ballPositionX < 0){
		ballPositionX = 0;
		ballVelocityX *= -1;
		if (!bFullscreen){
			ofSetWindowPosition(posx-10, posy);
		}
	} else if (ballPositionX > ofGetWidth()){
		ballPositionX = ofGetWidth();
		ballVelocityX *= -1;
		if (!bFullscreen){
			ofSetWindowPosition(posx+10, posy);
		}
	}
	
	if (ballPositionY < 0){
		ballPositionY = 0;
		ballVelocityY *= -1;
		if (!bFullscreen){
			ofSetWindowPosition(posx, posy-10);
		}
	} else if (ballPositionY > ofGetHeight()){
		ballPositionY = ofGetHeight();
		ballVelocityY *= -1;
		if (!bFullscreen){
			ofSetWindowPosition(posx, posy+10);
		}
	}
	
}

//--------------------------------------------------------------
void ofApp::draw(){
	ofSetupScreen();
	int width = ofGetWidth();
	int height = ofGetHeight();
	
	// Based on @geluso Draw 100 circles https://github.com/geluso
	for(int i = 0; i < 100; i++) {
		float speed = 1.74f;
		float sinX = sin(i + ofGetElapsedTimef() * speed) * 100;
		float sinAlpha = ofMap(sin(i + ofGetElapsedTimef() * speed),-1, 1, 0, 200);
		float sinRadius = ofMap(sin(i * ofGetElapsedTimef() * 0.05),-1, 1, 5, 30);
		ofSetColor(255,0,0,sinAlpha);
		ofDrawCircle(i * 13, sinX + ofGetWidth()*0.5, sinRadius);
	}
	
	if(bDrawGuides) { // Draw edge of transparent window for clarity
		int screenWidth = ofGetWidth();
		int screenHeight = ofGetHeight();
		ofSetColor(secondaryBtnColor);
		ofDrawRectangle(0, 0, screenWidth, 6);
		ofDrawRectangle(0, screenHeight - 6, screenWidth, 6);
		ofDrawRectangle(0, 0, 6, screenHeight);
		ofDrawRectangle(screenWidth - 6, 0, 6, screenHeight);
		// draw top rect for disabling passThrough example
		ofSetColor(highlightColor);
		ofSetColor(ofColor(80, 80, 80, 196));
		ofDrawRectangle(0, 0, screenWidth, 300);
		ofSetColor(0x000000);
		ofDrawLine(0, 0, 0, height);
		ofDrawLine(width, 0, width, height);
		ofDrawLine(0, height, width, height);
	}
	
	ofSetColor(textColor);
	centerX = ofGetWidth() / 2;
	centerY = 200;
	baseY = centerY - 2 * lineSpacing;
	if (bAllowPassThrough) {
		ofSetColor(primaryBtnColor);
		ofDrawRectangle(40, 10 + 1 * lineSpacing, 550, 200);
		ofSetColor(textColor);
		fontRenderer.drawString("Note: When GLFW mouse passThrough is Enabled", 50, 10 + 2 * lineSpacing);
		fontRenderer.drawString("GLFW Events are Disabled for that Window", 50, 10 + 3 * lineSpacing);
		fontRenderer.drawString("Disabled: Keyboard Events/Mouse Press/Release", 50, 10 + 4 * lineSpacing);
		fontRenderer.drawString("Enabled: mouseMoved/mouseExit/mouseEnter", 50, 10 + 5 * lineSpacing);
	} else {
		ofSetColor(primaryBtnColor);
		ofDrawRectangle(40, 10 + 1 * lineSpacing, 550, 200);
		ofSetColor(textColor);
		fontRenderer.drawString("Note: When GLFW mouse passThrough is Disabled", 50, 10 + 2 * lineSpacing);
		fontRenderer.drawString("GLFW Events are all enabled!", 50, 10 + 3 * lineSpacing);
		fontRenderer.drawString("This happens dynamically via GLFW Attribute", 50, 10 + 4 * lineSpacing);
	}
	
	ofSetColor(highlightColor);
	if (bAllowPassThrough) {
		fontRenderer.drawString("GLFW Mouse passThrough enabled", centerX, 2 * lineSpacing);
		if(bForcePassThrough) {
			fontRenderer.drawString("Enabled: Force Mouse passThrough - Click the Blue Circle", centerX, 1 * lineSpacing);
		}
	} else {
		fontRenderer.drawString("press h to enable GLFW passThrough or mouse below box", centerX, 2 * lineSpacing);
	}
	ofSetColor(secondaryBtnColor);
	
	if (!bFullscreen) {
		 fontRenderer.drawString("press f to enter fullscreen", centerX, baseY);
		 fontRenderer.drawString("window is normal / floating", centerX, baseY + lineSpacing);
	} else {
		 fontRenderer.drawString("press f to exit fullscreen", centerX, baseY);
		 fontRenderer.drawString("window is fullscreen", centerX, baseY + lineSpacing);
	}

	if (bDrawGuides) {
		 fontRenderer.drawString("press g to disable drawing edges", centerX, baseY + 2 * lineSpacing);
	} else {
		 fontRenderer.drawString("press g to enable drawing edges", centerX , baseY + 2 * lineSpacing);
	}
	
	if (bLogVerbose) {
		 fontRenderer.drawString("press L to disable Verbose Log", centerX , baseY + 3 * lineSpacing);
	} else {
		 fontRenderer.drawString("press L to enable Verbose Log", centerX , baseY + 3 * lineSpacing);
	}
	
	fontRenderer.drawString("window pos ("+ofToString(ofGetWindowPositionX())+", "+ofToString( ofGetWindowPositionY())+")", 20, 30);
	
	ofSetColor(primaryBtnColor);
	ofDrawCircle(ballPositionX, ballPositionY, 15);
}


//--------------------------------------------------------------
void ofApp::keyPressed(int key){ 

	if(key == 'f' || key == 'F'){
		bFullscreen = !bFullscreen;
		if(!bFullscreen){
			ofSetWindowShape(1280,720);
			ofSetFullscreen(false);
			int screenW = ofGetScreenWidth();
			int screenH = ofGetScreenHeight();
			ofSetWindowPosition(screenW/2-1280/2, screenH/2-720/2);
		} else if(bFullscreen == 1){
			ofSetFullscreen(true);
		}
		ofLogNotice("ofApp") << "bFullscreen:" << bFullscreen;
		
	} else if(key == 'g' || key == 'G'){
		bDrawGuides = !bDrawGuides;
		ofLogNotice("ofApp") << "bDrawGuides:" << bDrawGuides;
		
	} else if(key == 'h' || key == 'H'){
		bAllowPassThrough = !bAllowPassThrough;
		bForcePassThrough = !bForcePassThrough;
		ofSetWindowMousePassThrough(bAllowPassThrough);
		ofLogNotice("ofApp") << "ofSetWindowMousePassThrough:" << bAllowPassThrough << " Force:" << bForcePassThrough;
		
	} else if(key == 'l' || key == 'L' ){
		bLogVerbose = !bLogVerbose;
		if(bLogVerbose)
			ofSetLogLevel(OF_LOG_VERBOSE);
		else
			ofSetLogLevel(OF_LOG_NOTICE);
		ofLogNotice("ofApp") << "bLogVerbose:" << bLogVerbose;
	}
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
	ofLogVerbose("ofApp") << "keyReleased:" << key;
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
	ofLogVerbose("ofApp") << "mouseMoved: x:" << x << " y:" << y;
	
	float distance = ofDist(x, y, ballPositionX, ballPositionY);
	if(distance <= 15 || y < 300) {
		if(bForcePassThrough == true && distance <= 15){
			bForcePassThrough = false;
		}
		if(bAllowPassThrough && (bForcePassThrough == false || distance <= 15)) {
			bAllowPassThrough = false;
			ofLogNotice("ofApp") << "ofSetWindowMousePassThrough:" << bAllowPassThrough;
			ofSetWindowMousePassThrough(bAllowPassThrough);
		}
	} else {
		if(!bAllowPassThrough) {
			bAllowPassThrough = true;
			ofLogNotice("ofApp") << "ofSetWindowMousePassThrough:" << bAllowPassThrough;
			ofSetWindowMousePassThrough(bAllowPassThrough);
		}
	}
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
	ofLogNotice("ofApp") << "mousePressed:" << button << " x:" << x << " y:" << y;
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
	ofLogNotice("ofApp") << "mouseReleased:" << button << " x:" << x << " y:" << y;
}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){
	ofLogNotice("ofApp") << "mouseEntered the ofWindow: " << " x:" << x << " y:" << y;
}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){
	ofLogNotice("ofApp") << "mouseExited the ofWindow: " << " x:" << x << " y:" << y;
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

