#pragma once

#include "ofConstants.h"
#include "ofColor.h"

#if (_MSC_VER)
#include <memory>
#else
#include <tr1/memory>
#endif

//----------------------------------------------------------
// ofDeviceInfo
//----------------------------------------------------------
class ofSerial;
class ofSerialDeviceInfo{
	friend class ofSerial;

	public:

		ofSerialDeviceInfo(string devicePathIn, string deviceNameIn, int deviceIDIn){
			devicePath			= devicePathIn;
			deviceName			= deviceNameIn;
			deviceID			= deviceIDIn;
		}

		ofSerialDeviceInfo(){
			deviceName = "device undefined";
			deviceID   = -1;
		}

		string getDevicePath(){
			return devicePath;
		}

		string getDeviceName(){
			return deviceName;
		}

		int getDeviceID(){
			return deviceID;
		}

	protected:
		string devicePath;			//eg: /dev/tty.cu/usbdevice-a440
		string deviceName;			//eg: usbdevice-a440 / COM4
		int deviceID;				//eg: 0,1,2,3 etc

		//TODO: other stuff for serial ?
};


//----------------------------------------------------------
// ofMutex
//----------------------------------------------------------

#include "Poco/Mutex.h"
typedef Poco::FastMutex ofMutex;

//----------------------------------------------------------
// ofSemaphore
//----------------------------------------------------------

#include "Poco/Semaphore.h"
typedef Poco::Semaphore ofSemaphore;

//----------------------------------------------------------
// ofStyle
//----------------------------------------------------------

class ofStyle{
	public:
		ofStyle(){
			bFill				= true;
			blendingMode		= OF_BLENDMODE_DISABLED;
			smoothing			= false;
			circleResolution	= 20;
			sphereResolution	= 20;
			curveResolution		= 20;
			lineWidth			= 1.0;
			polyMode			= OF_POLY_WINDING_ODD;
			rectMode			= OF_RECTMODE_CORNER;
			drawBitmapMode		= OF_BITMAPMODE_SIMPLE;
			bgColor.set(200,200,200);
		}

		virtual ~ofStyle(){}

		ofColor color;
		ofColor bgColor;
		ofPolyWindingMode polyMode;
		ofRectMode rectMode;
		bool bFill;
		ofDrawBitmapMode drawBitmapMode;

		ofBlendMode blendingMode;

		bool smoothing;
		int circleResolution;
		int sphereResolution;
		int curveResolution;
		float lineWidth;
};

//----------------------------------------------------------
// ofPtr
//----------------------------------------------------------
template <typename T>
class ofPtr: public std::tr1::shared_ptr<T>
{

public:

	ofPtr()
	  : std::tr1::shared_ptr<T>() { }

	  template<typename Tp1>
		explicit
		ofPtr(Tp1* __p)
	: std::tr1::shared_ptr<T>(__p) { }

	  template<typename Tp1, typename _Deleter>
		ofPtr(Tp1* __p, _Deleter __d)
	: std::tr1::shared_ptr<T>(__p, __d) { }

	  template<typename Tp1, typename _Deleter, typename _Alloc>
		ofPtr(Tp1* __p, _Deleter __d, const _Alloc& __a)
	: std::tr1::shared_ptr<T>(__p, __d, __a) { }

	  // Aliasing constructor
	  template<typename Tp1>
		ofPtr(const ofPtr<Tp1>& __r, T* __p)
	: std::tr1::shared_ptr<T>(__r, __p) { }

	  template<typename Tp1>
		ofPtr(const ofPtr<Tp1>& __r)
	: std::tr1::shared_ptr<T>(__r) { }

	  /*ofPtr(ofPtr&& __r)
	  : std::tr1::shared_ptr<T>(std::move(__r)) { }

	  template<typename Tp1>
		ofPtr(ofPtr<Tp1>&& __r)
		: std::tr1::shared_ptr<T>(std::move(__r)) { }*/

	  template<typename Tp1>
		explicit
		ofPtr(const std::tr1::weak_ptr<Tp1>& __r)
	: std::tr1::shared_ptr<T>(__r) { }

	  /*template<typename Tp1, typename Del>
		explicit
		ofPtr(const std::tr1::unique_ptr<Tp1, Del>&) = delete;

	  template<typename Tp1, typename Del>
		explicit
		ofPtr(std::tr1::unique_ptr<Tp1, Del>&& __r)
	: std::tr1::shared_ptr<T>(std::move(__r)) { }*/
};
