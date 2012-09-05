#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup() {
	quality = OF_IMAGE_QUALITY_WORST;
	maxSize = 2048;
	glitchStart = .6;
	reset();
}

//--------------------------------------------------------------
void testApp::reset() {
	generation = 0;
	img.loadImage("buses.jpg");
	// pick random for bleeding
	addX = ofRandom(0, 16);
	addY = ofRandom(0, 16);
	subX = ofRandom(0, addX);
	subY = ofRandom(0, addY);
}

//--------------------------------------------------------------
void testApp::update() {
	string curFilename = "compressed.jpg";

	int size = img.getWidth();

	// keeps the image from getting too big
	if(size < maxSize) {
		img.saveImage(curFilename, quality);

		if(ofGetKeyPressed('g')) {
			// this portion glitches the jpeg file
			// first loading the file (as binary)
			ofBuffer file = ofBufferFromFile(curFilename, true);
			int fileSize = file.size();
			char * buffer = file.getBinaryBuffer();

			// pick a byte offset that is somewhere near the end of the file
			int whichByte = (int) ofRandom(fileSize * glitchStart, fileSize);
			// and pick a bit in that byte to turn on
			int whichBit = ofRandom(8);
			char bitMask = 1 << whichBit;
			// using the OR operator |, if the bit isn't already on this will turn it on
			buffer[whichByte] |= bitMask;

			// write the file out like nothing happened
			ofBufferToFile(curFilename, file, true);
			img.loadImage(curFilename);
		} else {
			img.loadImage(curFilename);

			// this if switches every other frame
			// resizing up and down breaks the 8x8 JPEG blocks
			if(ofGetFrameNum() % 2 == 0) {
				// resize a little bigger
				img.resize(size + addX, size + addY);
			} else {
				// then resize a little smaller
				img.resize(size - subX, size - subY);
			}
		}
		generation++;
	}
}

//--------------------------------------------------------------
void testApp::draw() {
	ofSetColor(255);
	img.draw(0, 0, ofGetWidth(), ofGetHeight());

	ofSetColor(0);
	ofRect(5, 5, 290, 45);
	ofSetColor(255);
	ofDrawBitmapString("Currently on generation " + ofToString(generation), 10, 20);
	ofDrawBitmapString("Click to reset, hold 'g' to glitch.", 10, 40);
}

//--------------------------------------------------------------
void testApp::keyPressed  (int key){

}

//--------------------------------------------------------------
void testApp::keyReleased  (int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
	reset();
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
