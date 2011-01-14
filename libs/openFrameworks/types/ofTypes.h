#pragma once

#include "ofConstants.h"
#include "ofColor.h"

//----------------------------------------------------------
// ofDeviceInfo
//----------------------------------------------------------
class ofSerial;
class ofSerialDeviceInfo{
	friend class ofSerial;
	
	public: 
		ofSerialDeviceInfo(string deviceNameIn, int deviceIDIn){
			deviceName = deviceNameIn;
			deviceID   = deviceIDIn;
		}

		ofSerialDeviceInfo(){
			deviceName = "device undefined";
			deviceID   = -1;
		}
		
		string getDeviceName(){
			return deviceName;
		}
		
		int getDeviceID(){
			return deviceID;
		}

	protected:
		string deviceName;
		int deviceID;
		
		//TODO: other stuff for serial ?
};


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
			sphereResolution = 20;
			lineWidth			= 1.0;
			polyMode			= OF_POLY_WINDING_ODD;
			rectMode			= OF_RECTMODE_CORNER;
		}

		virtual ~ofStyle(){}

		ofColor color;
		ofPolyWindingMode polyMode;
		ofRectMode rectMode;
		bool bFill;
    
        // one of the following GL_ZERO, GL_ONE, GL_DST_COLOR, GL_ONE_MINUS_DST_COLOR, GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_DST_ALPHA, GL_ONE_MINUS_DST_ALPHA, GL_SRC_ALPHA_SATURATE
        int blendSrc;
        int blendDst;
        bool blending;  // blending enabled?
		int blendEquation; // GL_FUNC_ADD, GL_FUNC_SUBTRACT, GL_FUNC_REVERSE_SUBTRACT, GL_MIN, GL_MAX
    
		bool smoothing;
		int circleResolution;
		int sphereResolution;
		float lineWidth;
};
