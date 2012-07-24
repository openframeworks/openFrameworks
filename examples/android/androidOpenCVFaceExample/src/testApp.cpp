#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
	ofBackground(255,255,255);
	ofSetLogLevel(OF_LOG_NOTICE);
	ofSetOrientation(OF_ORIENTATION_90_LEFT);
	// this makes the camera directly return grayscale image, faster!
	grabber.setPixelFormat(OF_PIXELS_MONO);
	int grabberWidth = 320;
	int grabberHeight = 240;
	grabber.initGrabber(grabberWidth,grabberHeight);
	gray.allocate(grabber.getWidth(),grabber.getHeight());
	bg.allocate(grabber.getWidth(),grabber.getHeight());
	//diff.allocate(grabber.getWidth(),grabber.getHeight());
	//contourFinder.allocate(grabber.getWidth(),grabber.getHeight());

	faceFinder.setup("haarcascade_frontalface_default.xml");
	faceFinder.setNeighbors(1);
	faceFinder.setScaleHaar(1.5);

	one_second_time = ofGetSystemTime();
	camera_fps = 0;
	frames_one_sec = 0;
	captureBg = true;
	threshold = 80;

	minusOn = false;
	plusOn = false;

	buttonWidth = grabberWidth/2 - 60;
	buttonHeight = 50;

	minusX = 20;
	plusX = minusX + grabberWidth/2 + 40/2;
	minusY = grabberHeight + 10;
	plusY = grabberHeight + 10;
}

//--------------------------------------------------------------
void testApp::update(){
	grabber.update();
	if(grabber.isFrameNew()){
		frames_one_sec++;
		if( ofGetSystemTime() - one_second_time >= 1000){
			camera_fps = frames_one_sec;
			frames_one_sec = 0;
			one_second_time = ofGetSystemTime();
		}
		gray.setFromPixels(grabber.getPixels(),grabber.getWidth(),grabber.getHeight());
		if(captureBg){
			bg = gray;
			captureBg = false;
		}

		//gray.absDiff(bg);
		gray.threshold(threshold);
		faceFinder.findHaarObjects(gray);
		faces = faceFinder.blobs;
	}
}

//--------------------------------------------------------------
void testApp::draw(){
	ofSetColor(0xFFFFFF);
	gray.draw(5,5);
	//contourFinder.draw(5,5);
	ofPushStyle();
	ofNoFill();
	ofSetColor(255, 0, 255);
	for (int i = 0; i < faces.size(); i++) {
		ofxCvBlob& face = faces[i];
		ofRectangle rect(face.boundingRect.x, face.boundingRect.y, face.boundingRect.width, face.boundingRect.height);
		ofRect(rect);
	}
	ofPopStyle();

	ofSetColor(0x000000);
	ofDrawBitmapString("fps: " + ofToString(ofGetFrameRate()),330,10);
	ofDrawBitmapString("camera fps: " + ofToString(camera_fps),330,30);
	ofDrawBitmapString("threshold: " + ofToString(threshold), 330, 50);

	// TODO: eventually replace with ofxGui code when it supports big buttons
	drawButton("-", minusX, minusY, buttonWidth, buttonHeight, minusOn);
	drawButton("+", plusX, plusY, buttonWidth, buttonHeight, plusOn);
	ofPopStyle();
}

void testApp::drawButton(const string& s, int x, int y, int w, int h, bool isToggled) {
	// this doesn't consider the length of the string when drawing the button,
	// it just assumes it's short enough not to be a problem

	ofPushStyle();
	ofColor outlineColor(isToggled ? 0xffffff : 0x000000);
	ofColor invColor(isToggled ? 0x000000 : 0xffffff);
	ofSetColor(invColor);
	ofFill();
	ofRect(x, y, w, h);

	ofSetColor(outlineColor);
	ofDrawBitmapString(s, x + w/2, y + h/2);
	ofNoFill();
	ofRect(x, y, w, h);

	ofPopStyle();
}

//--------------------------------------------------------------
void testApp::keyPressed  (int key){ 
	
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){ 
	
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){
	if (x >= minusX && x < minusX + buttonWidth &&
		y >= minusY && y < minusY + buttonHeight) {
		minusOn = true;
	}
	else
	{
		minusOn = false;
	}

	if (x >= plusX && x < plusX + buttonWidth &&
		y >= plusY && y < plusY + buttonHeight) {
		plusOn = true;
	}
	else
	{
		plusOn = false;
	}

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
	if (x >= minusX && x < minusX + buttonWidth &&
		y >= minusY && y < minusY + buttonHeight) {
		minusOn = true;
	}
	if (x >= plusX && x < plusX + buttonWidth &&
		y >= plusY && y < plusY + buttonHeight) {
		plusOn = true;
	}
	if (minusOn) {
		if (threshold >= 20) {
			threshold--;
		}
	}
	if (plusOn) {
		if (threshold <= 120) {
			threshold++;
		}
	}

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){
	if (x >= minusX && x < minusX + buttonWidth &&
		y >= minusY && y < minusY + buttonHeight) {
		minusOn = false;
	}
	if (x >= plusX && x < plusX + buttonWidth &&
		y >= plusY && y < plusY + buttonHeight) {
		plusOn = false;
	}

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

