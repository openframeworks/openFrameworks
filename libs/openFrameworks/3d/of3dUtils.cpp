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

void ofDrawGrid(float size) {
	
	
	const float lineCountMajor = 3;
	const float lineCountMinor = lineCountMajor*3;
	const float lineStepMajor = size / lineCountMajor;
	const float lineStepMinor = size / lineCountMinor;
	
	
	ofPushStyle();
	
	//draw minor lines
	ofSetLineWidth(1);
	for (float s=-size; s<=size; s+=lineStepMinor)
	{
		ofLine(-size, s, size, s);
		ofLine(s, -size, s, size);		
	}
	
	
	//draw major lines
	ofSetLineWidth(3);
	for (float s=-size; s<=size; s+=lineStepMajor)
	{
		ofLine(-size, s, size, s);
		ofLine(s, -size, s, size);		
	}
	
	ofPopStyle();
	
}