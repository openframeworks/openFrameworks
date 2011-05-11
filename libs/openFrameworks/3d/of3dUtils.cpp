#include "of3dUtils.h"
#include "ofGraphics.h"

void ofDrawAxis(float size) {
	ofPushStyle();
	ofSetLineWidth(3);

	// draw x axis
	ofSetColor(ofColor::red);
	ofLine(0, 0, 0, size, 0, 0);
	
	// draw y axis
	ofSetColor(ofColor::green);
	ofLine(0, 0, 0, 0, size, 0);

	// draw z axis
	ofSetColor(ofColor::blue);
	ofLine(0, 0, 0, 0, 0, size);
	
	ofPopStyle();
}
