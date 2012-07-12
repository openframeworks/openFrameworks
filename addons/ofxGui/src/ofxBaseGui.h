#pragma once

#include "ofxXmlSettings.h"
#include "ofConstants.h"
#include "ofBaseTypes.h"
#include "ofParameter.h"


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
	
	void setDefaultSerializer(ofBaseFileSerializer& serializer);

	virtual void saveTo(ofBaseFileSerializer& serializer);
	virtual void loadFrom(ofBaseFileSerializer& serializer);
	
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

	virtual ofAbstractParameter & getParameter() = 0;

protected:
	string name;
	unsigned long currentFrame;			
	ofRectangle b;
	bool bGuiActive;
	ofBaseFileSerializer * serializer;

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
