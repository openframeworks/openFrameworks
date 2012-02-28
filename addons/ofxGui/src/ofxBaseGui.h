#pragma once

#include "ofxXmlSettings.h"
#include "ofConstants.h"


class ofxBaseGui{
public:
	ofxBaseGui();
	
	virtual ~ofxBaseGui();

	virtual void mouseMoved(ofMouseEventArgs & args) = 0;
	virtual void mousePressed(ofMouseEventArgs & args) = 0;
	virtual void mouseDragged(ofMouseEventArgs & args) = 0;
	virtual void mouseReleased(ofMouseEventArgs & args) = 0;
	
	virtual void setValue(float mx, float my, bool bCheckBounds) = 0;
	virtual void draw() = 0;
	
	void saveToFile(string filename);
	void loadFromFile(string filename);
	
	virtual void saveToXml(ofxXmlSettings& xml) = 0;
	virtual void loadFromXml(ofxXmlSettings& xml) = 0;
	
	string getName();
	void setName(string name);

	void setPosition(ofPoint p);
	void setPosition(float x, float y);
	void setSize(float w, float h);
	void setShape(ofRectangle r);
	void setShape(float x, float y, float w, float h);

	ofPoint getPosition();
	ofRectangle getShape();
	float getWidth();
	float getHeight();

protected:
	string name;
	unsigned long currentFrame;			
	ofRectangle b;
	bool bGuiActive;

	static const ofColor headerBackgroundColor;
	static const ofColor backgroundColor;
	static const ofColor textColor;
	static const ofColor fillColor;

	static const int textPadding;
	static const int defaultWidth;
	static const int defaultHeight;

	static string saveStencilToHex(ofImage& img);
	static void loadStencilFromHex(ofImage& img, unsigned char* data) ;
}; 
