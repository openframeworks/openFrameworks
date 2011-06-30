#pragma once

const ofColor
headerBackgroundColor(64),
backgroundColor(0),
textColor(255),
fillColor(128);

class ofBaseGui{
public:
	ofBaseGui(){
		bGuiActive = false;
	}
	
	virtual void mouseMoved(ofMouseEventArgs & args) = 0;
	virtual void mousePressed(ofMouseEventArgs & args) = 0;
	virtual void mouseDragged(ofMouseEventArgs & args) = 0;
	virtual void mouseReleased(ofMouseEventArgs & args) = 0;
	
	virtual void setValue(float mx, float my, bool bCheckBounds) = 0;
	virtual void draw() = 0;
	
	string name;
	unsigned long currentFrame;			
	ofRectangle b;
	bool bGuiActive;
}; 
