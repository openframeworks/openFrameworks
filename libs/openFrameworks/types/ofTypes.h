#pragma once

#include "ofConstants.h"
#include "ofColor.h"

#if (_MSC_VER) || ((defined(TARGET_EMSCRIPTEN) || defined(TARGET_LINUX)) && __cplusplus>=201103L)
#include <memory>
#else
#include <tr1/memory>
// import smart pointers utils into std
namespace std {
#if __cplusplus<201103L
	using std::tr1::shared_ptr;
	using std::tr1::weak_ptr;
	using std::tr1::enable_shared_from_this;
#endif
	using std::tr1::static_pointer_cast;
	using std::tr1::dynamic_pointer_cast;
	using std::tr1::const_pointer_cast;
	using std::tr1::__dynamic_cast_tag;
}
#endif

class ofSerial;

/// \brief Describes a Serial device, what ID it has, what name it has, 
/// and the path to the device.
class ofSerialDeviceInfo{
	friend class ofSerial;

	public:

		/// \brief Constructor
		/// \param devicePathIn The path to the device
		/// \param deviceNameIn The name of the device
		/// \param deviceIDIn  	The ID of the device
		ofSerialDeviceInfo(string devicePathIn, string deviceNameIn, int deviceIDIn){
			devicePath			= devicePathIn;
			deviceName			= deviceNameIn;
			deviceID			= deviceIDIn;
		}

		/// \brief Constructs a undefined serial device
		ofSerialDeviceInfo(){
			deviceName = "device undefined";
			deviceID   = -1;
		}

		/// \brief Gets the path to the device
		///
		/// Example: `/dev/tty.cu/usbdevice-a440`
		string getDevicePath(){
			return devicePath;
		}

		/// \brief Gets the name of the device
		///
		/// Example: `usbdevice-a440` / `COM4`
		string getDeviceName(){
			return deviceName;
		}

		/// \brief Gets the ID of the device
		///
		/// Example: `0`,`1`,`2`,`3` etc
		int getDeviceID(){
			return deviceID;
		}

	protected:
		/// \cond INTERNAL
		string devicePath;			//eg: /dev/tty.cu/usbdevice-a440
		string deviceName;			//eg: usbdevice-a440 / COM4
		int deviceID;				//eg: 0,1,2,3 etc
		/// \endcond

		//TODO: other stuff for serial ?
};


//----------------------------------------------------------
// ofMutex
//----------------------------------------------------------

#include "Poco/Mutex.h"
typedef Poco::FastMutex ofMutex;
typedef Poco::FastMutex::ScopedLock ofScopedLock;

/// \brief Contains general information about the style of ofGraphics
/// elements such as color, line width and others.
class ofStyle{
	public:
		/// \brief Constructor for ofStyle
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
			#ifdef TARGET_OPENGLES
				drawBitmapMode		= OF_BITMAPMODE_SIMPLE;
			#else
				drawBitmapMode		= OF_BITMAPMODE_MODEL_BILLBOARD;
			#endif
			bgColor.set(200,200,200);
			//depthTest = false;
		}

		/// \brief Destructor of ofStyle
		virtual ~ofStyle(){}

		/// \brief Contains the color information for the style object.
		ofColor color;
		
		/// \brief Contains the background color information for the style object.
		ofColor bgColor;

		/// \brief Contains information about the drawing behavior for 
		/// ovelapping parts of the same polygon when a shape is created.
		/// 
		/// Possible modes are:
		/// - `OF_POLY_WINDING_ODD`
		/// - `OF_POLY_WINDING_NONZERO`
		/// - `OF_POLY_WINDING_POSITIVE`
		/// - `OF_POLY_WINDING_NEGATIVE`
		/// - `OF_POLY_WINDING_ABS_GEQ_TWO`
		ofPolyWindingMode polyMode;
		
		/// \brief Contains information about the mode for drawing rectangles.
		/// 
		/// If they are corner aligned, or drawn so that the x,y position is
		/// the center of the rectangle. Possible options are
		/// `OF_RECTMODE_CENTER` and `OF_RECTMODE_CORNER`
		ofRectMode rectMode;

		/// \brief Contains information about how the graphics have to 
		/// be drawed: Fill or noFill(empty)
		bool bFill;

		/// \brief Contains the ofDrawBitmapMode information
		ofDrawBitmapMode drawBitmapMode;

		/// \brief Contains the blending mode information
		ofBlendMode blendingMode;

		/// \brief Contains the information about if smoothing
		/// effect is activated or not.
		bool smoothing;

		// \brief Contains the resolution of a ofCircle.
		int circleResolution;
		
		// \brief Contains the resolution of a ofSphere.
		int sphereResolution;
		
		// \brief Contains the resolution of a curves drawn.	
		int curveResolution;

		/// \brief Contains information about the width of the line for our ofLine.
		float lineWidth;

		//bool depthTest; removed since it'll break old projects setting depth test through glEnable
};

class ofVideoFormat{
public:
	  ofPixelFormat pixelFormat;
	  int    width;
	  int    height;
	  vector<float> framerates;
};

class ofVideoDevice{
public:
	  int id;
	  string deviceName;
	  string hardwareName;
	  vector<ofVideoFormat> formats;
      bool bAvailable;
};


//----------------------------------------------------------
// ofPtr
//----------------------------------------------------------
#if __cplusplus >= 201103L
template <typename T>
using ofPtr = std::shared_ptr<T>;
#else
template <typename T>
class ofPtr: public std::shared_ptr<T>
{

public:

	OF_DEPRECATED_MSG("Use shared_ptr instead",ofPtr());

	  template<typename Tp1>
		explicit
		OF_DEPRECATED_MSG("Use shared_ptr instead",ofPtr(Tp1* __p));

	  template<typename Tp1, typename _Deleter>
		ofPtr(Tp1* __p, _Deleter __d)
	: std::shared_ptr<T>(__p, __d) { }

	  template<typename Tp1, typename _Deleter, typename _Alloc>
		ofPtr(Tp1* __p, _Deleter __d, const _Alloc& __a)
	: std::shared_ptr<T>(__p, __d, __a) { }

	  // Aliasing constructor
	  template<typename Tp1>
		ofPtr(const ofPtr<Tp1>& __r, T* __p)
	: std::shared_ptr<T>(__r, __p) { }

	  template<typename Tp1>
		ofPtr(const ofPtr<Tp1>& __r)
	: std::shared_ptr<T>(__r) { }

	  template<typename Tp1>
		ofPtr(const std::shared_ptr<Tp1>& __r)
	: std::shared_ptr<T>(__r) { }

	  template<typename Tp1>
		explicit
		ofPtr(const std::weak_ptr<Tp1>& __r)
	: std::shared_ptr<T>(__r) { }

	// tgfrerer: extends ofPtr facade to allow dynamic_pointer_cast, pt.1
#if (_MSC_VER)
	template<typename Tp1>
	ofPtr(const ofPtr<Tp1>& __r, std::_Dynamic_tag)
	: std::shared_ptr<T>(__r, std:::_Dynamic_tag()) { }
#elif !defined(TARGET_EMSCRIPTEN) && !defined(TARGET_LINUX)
	template<typename Tp1>
	ofPtr(const ofPtr<Tp1>& __r, std::__dynamic_cast_tag)
	: std::shared_ptr<T>(__r, std::__dynamic_cast_tag()) { }
#endif
};

template<typename T>
ofPtr<T>::ofPtr()
: std::shared_ptr<T>() { }

template<typename T>
template<typename Tp1>
ofPtr<T>::ofPtr(Tp1* __p)
: std::shared_ptr<T>(__p) { }

// tgfrerer: extends ofPtr facade to allow dynamic_pointer_cast, pt. 2
#if (_MSC_VER)
template<typename _Tp, typename _Tp1>
ofPtr<_Tp>
	dynamic_pointer_cast(const ofPtr<_Tp1>& __r)
{ return ofPtr<_Tp>(__r, std::_Dynamic_tag()); }
#elif !defined(TARGET_EMSCRIPTEN) && !defined(TARGET_LINUX)
template<typename _Tp, typename _Tp1>
ofPtr<_Tp>
	dynamic_pointer_cast(const ofPtr<_Tp1>& __r)
{ return ofPtr<_Tp>(__r, std::__dynamic_cast_tag()); }
#endif
#endif
