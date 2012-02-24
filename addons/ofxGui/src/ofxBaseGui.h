#pragma once

#include "ofxXmlSettings.h"
#include "ofConstants.h"

const ofColor
headerBackgroundColor(64),
backgroundColor(0),
textColor(255),
fillColor(128);

const int textPadding = 4;
const int defaultWidth = 200;
const int defaultHeight = 16;

class ofxBaseGui{
public:
	ofxBaseGui(){
		bGuiActive = false;
	}
	
	virtual ~ofxBaseGui(){}

	virtual void mouseMoved(ofMouseEventArgs & args) = 0;
	virtual void mousePressed(ofMouseEventArgs & args) = 0;
	virtual void mouseDragged(ofMouseEventArgs & args) = 0;
	virtual void mouseReleased(ofMouseEventArgs & args) = 0;
	
	virtual void setValue(float mx, float my, bool bCheckBounds) = 0;
	virtual void draw() = 0;
	
	void saveToFile(string filename) {
		ofxXmlSettings xml;
		xml.loadFile(filename);
		saveToXml(xml);
		xml.saveFile(filename);
	}
	
	void loadFromFile(string filename) {
		ofxXmlSettings xml;
		xml.loadFile(filename);
		loadFromXml(xml);
	}
	
	virtual void saveToXml(ofxXmlSettings& xml) = 0;
	virtual void loadFromXml(ofxXmlSettings& xml) = 0;
	
	string name;
	unsigned long currentFrame;			
	ofRectangle b;
	bool bGuiActive;
}; 
