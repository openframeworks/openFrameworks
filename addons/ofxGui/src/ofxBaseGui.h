#pragma once

#include "ofConstants.h"
#include "ofBaseTypes.h"
#include "ofParameter.h"
#include "ofTrueTypeFont.h"
#include "ofBitmapFont.h"

class ofxBaseGui{
public:
	ofxBaseGui();
	
	virtual ~ofxBaseGui();
	void draw();
	
	void saveToFile(string filename);
	void loadFromFile(string filename);
	
	void setDefaultSerializer(std::shared_ptr<ofBaseFileSerializer> serializer);

	virtual void saveTo(ofBaseSerializer& serializer);
	virtual void loadFrom(ofBaseSerializer& serializer);
	
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

	static void setDefaultHeaderBackgroundColor(const ofColor & color);
	static void setDefaultBackgroundColor(const ofColor & color);
	static void setDefaultBorderColor(const ofColor & color);
	static void setDefaultTextColor(const ofColor & color);
	static void setDefaultFillColor(const ofColor & color);

	static void setDefaultTextPadding(int padding);
	static void setDefaultWidth(int width);
	static void setDefaultHeight(int height);

	virtual ofAbstractParameter & getParameter() = 0;
	static void loadFont(string filename, int fontsize, bool _bAntiAliased=true, bool _bFullCharacterSet=false, int dpi=0);
	static void setUseTTF(bool bUseTTF);
    
    void registerMouseEvents();
    void unregisterMouseEvents();

	virtual bool mouseMoved(ofMouseEventArgs & args) = 0;
	virtual bool mousePressed(ofMouseEventArgs & args) = 0;
	virtual bool mouseDragged(ofMouseEventArgs & args) = 0;
	virtual bool mouseReleased(ofMouseEventArgs & args) = 0;
	virtual bool mouseScrolled(ofMouseEventArgs & args) = 0;
protected:
	virtual void render()=0;
	bool isGuiDrawing();
	virtual bool setValue(float mx, float my, bool bCheckBounds) = 0;
	void bindFontTexture();
	void unbindFontTexture();
	ofMesh getTextMesh(const string & text, float x, float y);
	ofRectangle getTextBoundingBox(const string & text,float x, float y);

	ofRectangle b;
	static ofTrueTypeFont font;
	static bool fontLoaded;
	static bool useTTF;
	static ofBitmapFont bitmapFont;
	shared_ptr<ofBaseFileSerializer> serializer;

	static ofColor headerBackgroundColor;
	static ofColor backgroundColor;
	static ofColor borderColor;
	static ofColor textColor;
	static ofColor fillColor;

	ofColor thisHeaderBackgroundColor;
	ofColor thisBackgroundColor;
	ofColor thisBorderColor;
	ofColor thisTextColor;
	ofColor thisFillColor;

	static int textPadding;
	static int defaultWidth;
	static int defaultHeight;

	static string saveStencilToHex(ofImage& img);
	static void loadStencilFromHex(ofImage& img, unsigned char* data) ;

	virtual void generateDraw(){};

private:
	unsigned long currentFrame;
    bool bRegisteredForMouseEvents;
}; 
