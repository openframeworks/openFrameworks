#pragma once

#include "ofConstants.h"
#include "ofBaseTypes.h"
#include "ofParameter.h"
#include "ofTrueTypeFont.h"

class ofxBaseGui{
public:
	ofxBaseGui();
	
	virtual ~ofxBaseGui();
	virtual void draw() = 0;
	
	void saveToFile(string filename);
	void loadFromFile(string filename);
	
	void setDefaultSerializer(ofBaseFileSerializer& serializer);

	virtual void saveTo(ofBaseFileSerializer& serializer);
	virtual void loadFrom(ofBaseFileSerializer& serializer);
	
	string getName();
	void setName(string name);

	virtual void setPosition(ofPoint p);
	virtual void setPosition(float x, float y);
	virtual void setSize(float w, float h);
	virtual void setShape(ofRectangle r);
	virtual void setShape(float x, float y, float w, float h);

	ofPoint getPosition();
	ofRectangle getShape();
	float getWidth();
	float getHeight();

	ofColor getHeaderBackgroundColor();
	ofColor getBackgroundColor();
	ofColor getBorderColor();
	ofColor getTextColor();
	ofColor getFillColor();

	void setHeaderBackgroundColor(const ofColor & color);
	void setBackgroundColor(const ofColor & color);
	void setBorderColor(const ofColor & color);
	void setTextColor(const ofColor & color);
	void setFillColor(const ofColor & color);

	virtual ofAbstractParameter & getParameter() = 0;
	static void loadFont(string filename, int fontsize, bool _bAntiAliased=true, bool _bFullCharacterSet=false, int dpi=0);


	virtual void mouseMoved(ofMouseEventArgs & args) = 0;
	virtual void mousePressed(ofMouseEventArgs & args) = 0;
	virtual void mouseDragged(ofMouseEventArgs & args) = 0;
	virtual void mouseReleased(ofMouseEventArgs & args) = 0;
protected:

	virtual void setValue(float mx, float my, bool bCheckBounds) = 0;

	unsigned long currentFrame;			
	ofRectangle b;
	static ofTrueTypeFont font;
	static bool fontLoaded;
	bool bGuiActive;
	ofBaseFileSerializer * serializer;

	static const ofColor headerBackgroundColor;
	static const ofColor backgroundColor;
	static const ofColor borderColor;
	static const ofColor textColor;
	static const ofColor fillColor;

	ofColor thisHeaderBackgroundColor;
	ofColor thisBackgroundColor;
	ofColor thisBorderColor;
	ofColor thisTextColor;
	ofColor thisFillColor;

	static const int textPadding;
	static const int defaultWidth;
	static const int defaultHeight;

	static string saveStencilToHex(ofImage& img);
	static void loadStencilFromHex(ofImage& img, unsigned char* data) ;

	virtual void generateDraw(){};
}; 
