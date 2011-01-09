#ifndef _OF_TYPES
#define _OF_TYPES

#include "ofConstants.h"
#include "ofUtils.h"
#include "ofRectangle.h"
#include "ofPoint.h"
#include "ofColor.h"
#include "ofBaseTypes.h"


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


#endif
