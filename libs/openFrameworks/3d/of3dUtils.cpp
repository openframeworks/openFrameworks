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

//--------------------------------------------------------------
void ofDrawGrid(float scale, float ticks, bool labels, bool x, bool y, bool z) {
	
	ofColor c = ofGetStyle().color;
	
	ofPushStyle();
	
	if (x) {
		c.setHue(0.0f);
		ofSetColor(c);
		ofDrawGridPlane(scale, ticks, labels);
	}
	if (y) {
		c.setHue(255.0f / 3.0f);
		ofSetColor(c);
		ofPushMatrix();
		ofRotate(90, 0, 0, 1);
		ofDrawGridPlane(scale, ticks, labels);
		ofPopMatrix();
	}
	if (z) {
		c.setHue(255.0f * 2.0f / 3.0f);
		ofSetColor(c);
		ofPushMatrix();
		ofRotate(90, 0, 1, 0);
		ofDrawGridPlane(scale, ticks, labels);
		ofPopMatrix();
	}
	
	ofPopStyle();
}


//--------------------------------------------------------------
void drawGridPlane(float scale, float ticks, bool labels) {
	
	float minor = scale / ticks;
	float major =  minor * 2.0f;
	
	ofPushStyle();
	for (int iDimension=0; iDimension<2; iDimension++)
	{
		for (float yz=-scale; yz<=scale; yz+= minor)
		{
			//major major
			if (fabs(yz) == scale || yz == 0)
				ofSetLineWidth(4);
			
			//major
			else if (yz / major == floor(yz / major) )
				ofSetLineWidth(2);
			
			//minor
			else
				ofSetLineWidth(1);
			if (iDimension==0)
				ofLine(0, yz, -scale, 0, yz, scale);
			else
				ofLine(0, -scale, yz, 0, scale, yz);
		}
	}
	ofPopStyle();
	
	if (labels) {
		//draw numbers on axes
		ofPushStyle();
		ofSetColor(255, 255, 255);
		
		ofSetDrawBitmapMode(OF_BITMAPMODE_MODEL_BILLBOARD);
		for (float yz = -scale; yz<=scale; yz+=minor)
		{
			ofDrawBitmapString(ofToString(yz, 0), 0, yz, 0);
			ofDrawBitmapString(ofToString(yz, 0), 0, 0, yz);		
		}
		ofPopStyle();
	}
	
}
