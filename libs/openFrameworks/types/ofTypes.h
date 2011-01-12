#ifndef _OF_TYPES
#define _OF_TYPES

#include "ofConstants.h"
#include "ofUtils.h"
#include "ofRectangle.h"
#include "ofPoint.h"
#include "ofColor.h"
#include "ofBaseTypes.h"


//----------------------------------------------------------
// ofMutex
//----------------------------------------------------------

#include "Poco/Mutex.h"
typedef Poco::FastMutex ofMutex;

//----------------------------------------------------------
// ofStyle
//----------------------------------------------------------

class ofStyle{
	public:
		ofStyle(){
			bFill				= true;
            blending            = false;
			blendEquation		= 0;
            blendSrc            = GL_SRC_ALPHA;
            blendDst            = GL_ONE_MINUS_SRC_ALPHA;
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
    
        // one of the following GL_ZERO, GL_ONE, GL_DST_COLOR, GL_ONE_MINUS_DST_COLOR, GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_DST_ALPHA, GL_ONE_MINUS_DST_ALPHA, GL_SRC_ALPHA_SATURATE
        int blendSrc;
        int blendDst;
        bool blending;  // blending enabled?
		int blendEquation; // GL_FUNC_ADD, GL_FUNC_SUBTRACT, GL_FUNC_REVERSE_SUBTRACT, GL_MIN, GL_MAX
    
		bool smoothing;
		int circleResolution;
		float lineWidth;
};

//----------------------------------------------------------
// ofBuffer
//----------------------------------------------------------

class ofBuffer{
	vector<char> 	buffer;
	long 			nextLinePos;
public:

	ofBuffer();
	ofBuffer(int size, char * buffer);
	ofBuffer(istream & stream);
	ofBuffer(const ofBuffer & mom);

	~ofBuffer();

	bool set(istream & stream);
	void set(int _size, char * _buffer);

	void clear();

	void allocate(long _size);

	char * getBuffer();

	long getSize() const;

	string getNextLine();
	string getFirstLine();
};


#endif
