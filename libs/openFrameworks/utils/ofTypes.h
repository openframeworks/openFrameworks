#ifndef _OF_TYPES
#define _OF_TYPES

#include "ofConstants.h"
#include "ofUtils.h"
#include "ofRectangle.h"
#include "ofPoint.h"
#include "ofColor.h"

//----------------------------------------------------------
// ofStyle
//----------------------------------------------------------

class ofStyle{
	public:
		ofStyle(){
			bFill				= true;
			blending			= false;
			smoothing			= false;
			circleResolution	= 20;
			lineWidth			= 1.0;
			polyMode			= OF_POLY_WINDING_ODD;
			rectMode			= OF_RECTMODE_CORNER;
		}

		virtual ~ofStyle(){}

		ofColor color;
		int polyMode;
		int rectMode;
		bool bFill;
		bool blending;
		bool smoothing;
		int circleResolution;
		float lineWidth;
};

//----------------------------------------------------------
// ofBuffer
//----------------------------------------------------------

class ofBuffer{

	long 	size;
	char * 	buffer;
	long 	nextLinePos;
public:

	ofBuffer(){
		size 	= 0;
		buffer 	= NULL;
		nextLinePos = 0;
	}

	ofBuffer(int _size, char * _buffer){
		size 	= _size;
		buffer 	= _buffer;
		nextLinePos = 0;
	}

	ofBuffer(const ofBuffer & mom){
		size = mom.size;
		nextLinePos = mom.nextLinePos;
		memcpy(buffer,mom.buffer,size);
	}

	~ofBuffer(){
		if(buffer) delete[] buffer;
	}

	void allocate(long _size){
		if(buffer) delete[] buffer;
		buffer = new char[_size];
		size = _size;
	}

	char * getBuffer(){
		return buffer;
	}

	long getSize(){
		return size;
	}

	string getNextLine(){
		if( size <= 0 ) return "";
		long currentLinePos = nextLinePos;
		while(nextLinePos<size && buffer[nextLinePos]!='\n') nextLinePos++;
		string line(buffer + currentLinePos,nextLinePos-currentLinePos);
		if(nextLinePos<size-1) nextLinePos++;
		return line;
	}

	string getFirstLine(){
		nextLinePos = 0;
		return getNextLine();
	}
};


//----------------------------------------------------------
// ofBaseDraws
//----------------------------------------------------------

class ofBaseDraws{
public:
	virtual ~ofBaseDraws(){}
	virtual void draw(float x,float y)=0;
	virtual void draw(float x,float y,float w, float h)=0;
	
	virtual void draw(const ofPoint & point){
		draw( point.x, point.y);
	}
	
	virtual void draw(const ofRectangle & rect){
		draw(rect.x, rect.y, rect.width, rect.height); 
	}

	virtual float getHeight()=0;
	virtual float getWidth()=0;

	virtual void setAnchorPercent(float xPct, float yPct){};
    virtual void setAnchorPoint(float x, float y){};
	virtual void resetAnchor(){};

};

//----------------------------------------------------------
// ofBaseUpdates
//----------------------------------------------------------

class ofBaseUpdates{
public:
	virtual ~ofBaseUpdates(){}
	virtual void update()=0;
};


//----------------------------------------------------------
// ofBaseHasTexture
//----------------------------------------------------------
class ofTexture;

class ofBaseHasTexture{
public:
	virtual ~ofBaseHasTexture(){}
	virtual ofTexture & getTextureReference()=0;
	virtual void setUseTexture(bool bUseTex)=0;
};

//----------------------------------------------------------
// ofBaseHasPixels
//----------------------------------------------------------
class ofBaseHasPixels{
public:
	virtual ~ofBaseHasPixels(){}
	virtual unsigned char * getPixels()=0;
};

//----------------------------------------------------------
// ofBaseImage
//----------------------------------------------------------
class ofBaseImage: public ofBaseDraws, public ofBaseHasTexture, public ofBaseHasPixels{
public:

};

//----------------------------------------------------------
// ofBaseVideo
//----------------------------------------------------------
class ofBaseVideo: public ofBaseImage, public ofBaseUpdates{
public:
	virtual ~ofBaseVideo(){}
	virtual bool isFrameNew()=0;
	virtual void close()=0;
};
#endif
