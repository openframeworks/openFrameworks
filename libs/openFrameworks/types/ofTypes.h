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

/// \brief Describes a Serial device, including ID, name and path.
class ofSerialDeviceInfo{
	friend class ofSerial;

	public:
		/// \brief Construct an ofSerialDeviceInfo with parameters.
		/// \param devicePathIn The path to the device.
		/// \param deviceNameIn The name of the device.
		/// \param deviceIDIn The ID of the device.
		ofSerialDeviceInfo(string devicePathIn, string deviceNameIn, int deviceIDIn){
			devicePath = devicePathIn;
			deviceName = deviceNameIn;
			deviceID = deviceIDIn;
		}

		/// \brief Construct an undefined serial device.
		ofSerialDeviceInfo(){
			deviceName = "device undefined";
			deviceID   = -1;
		}

		/// \brief Gets the path to the device
		///
		/// Example: `/dev/tty.cu/usbdevice-a440`.
		///
		/// \returns the device path.
		string getDevicePath(){
			return devicePath;
		}

		/// \brief Gets the name of the device
		///
		/// Example: `usbdevice-a440` or `COM4`.
		///
		/// \returns the device name.
		string getDeviceName(){
			return deviceName;
		}

		/// \brief Gets the ID of the device
		///
		/// Example: `0`,`1`,`2`,`3` etc.
		///
		/// \returns the device ID.
		int getDeviceID(){
			return deviceID;
		}

	protected:
		/// \cond INTERNAL

		/// \brief The device path (e.g /dev/tty.cu/usbdevice-a440).
		string devicePath;

		/// \brief The device name (e.g. usbdevice-a440 / COM4).
		string deviceName;

		/// \brief The device ID (e.g. 0, 1, 2, 3, etc).
		int deviceID;

		/// \endcond
};


//----------------------------------------------------------
// ofMutex
//----------------------------------------------------------

#include "Poco/Mutex.h"

/// \brief A typedef for a cross-platform mutex.
///
/// A mutex is used to lock data when it is accessible from multiple threads.
/// Locking data with a mutex prevents data-races, deadlocks and other problems
/// associated with concurrent access to data.
///
/// The mutex can be locked with a call to ofMutex::lock(). All calls to
/// ofMutex::lock() must be paired with a call to ofMutex::unlock().
///
/// ~~~~{.cpp}
///
///     ofMutex myMutex; // Your member mutex.
///     int mySharedData; // Your member shared data.
///
///     // ...
///
///     // A method to modify some shared data.
///     void modifyMySharedData() {
///         myMutex.lock(); // Lock the mutex.
///         mySharedData++; // Modify the shared data.
///         myMutex.unlock(); // Unlock the mutex;
///     }
///
/// ~~~~
///
/// \warning Currently ofMutex is a typedef for Poco::FastMutex a fast cross-
/// platform mutex. In a future version of OF, Poco::FastMutex will be replaced
/// by std::mutex.
///
/// \sa http://www.cplusplus.com/reference/mutex/mutex/
/// \sa http://www.appinf.com/docs/poco/Poco.FastMutex.html
typedef Poco::FastMutex ofMutex;

/// \brief A typedef for a cross-platform scoped mutex.
///
/// Normally ofMutex requres explicit calls to ofMutex::lock() and
/// ofMutex::unlock() to lock and release the mutex. Sometimes, despite best
/// efforts, developers forget to unlock a mutex, leaving the data inaccessible.
/// ofScopedLock makes ofMutex easier to use by calling ofMutex::unlock when
/// the scoped lock's destructor is called. Since the destructor is called when
/// a variable goes out of scope, we call this a "scoped lock". A "scoped lock"
/// is sometimes known as a "lock guard" as well.
///
/// ofScopedLock is used to lock and unlock an existing ofMutex.
///
/// ~~~~{.cpp}
///
///     ofMutex myMutex; // Your member mutex.
///     int mySharedData; // Your member shared data.
///
///     // ...
///
///     // A method to modify some shared data.
///     void modifyMySharedData() {
///         ofScopedLock lock(myMutex); // Lock the mutex.
///         mySharedData++; // Modify the shared data.
///
///         // `lock` will unlock the mutex when it goes out of scope. 
///     }
///
/// ~~~~
///
/// \warning Currently ofScopedLock is a typedef for Poco::FastMutex::ScopedLock
/// a convenient wrapper fo Poco::FastMutex.  In a future version of OF,
/// Poco::FastMutex::ScopedLock by std::lock_guard.
///
/// \sa http://en.cppreference.com/w/cpp/thread/lock_guard
/// \sa http://www.appinf.com/docs/poco/Poco.ScopedLock.html
/// \sa ofMutex
typedef Poco::FastMutex::ScopedLock ofScopedLock;

/// \brief Contains general information about the style of ofGraphics
/// elements such as color, line width and others.
class ofStyle{
	public:
		/// \brief Create a default ofStyle.
		ofStyle(){
			bFill				= true;
			blendingMode		= OF_BLENDMODE_ALPHA;
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

		/// \brief Destroy the ofStyle.
		virtual ~ofStyle(){}

		/// \brief The color used when rendering.
		///
		/// This style depends on the state of the ofStyle::bFill.
		ofColor color;

		/// \brief The background color used when rendering.
		ofColor bgColor;

		/// \brief The current rendering mode for polygons.
		///
		/// ofPolyWindingMode determines how ovelapping parts of the same
		/// polygon are rendered.
		///
		/// \sa ofPolyWindingMode
		ofPolyWindingMode polyMode;

		/// \brief Determine how some shapes are anchored (e.g. ofDrawEllipse).
		///
		/// The rectMode applies to several shapes, including ofDrawRectangle
		/// and ofDrawEllipse.  If the current rectMode is `OF_RECTMODE_CENTER`,
		/// the rendered shape will be drawn with x, y in the center of the
		/// shape. If the current rectMode is `OF_RECTMODE_CORNER`, the
		/// rendered shape will be drawn with the x, y in the upper-left-hand
		/// corner of the shape.
		ofRectMode rectMode;

		/// \brief True if the renderer should fill a rendered shape.
		///
		/// If true, the renderer will fill the rendered shape.  If false, the
		/// renderer will draw the outline of the rendered shape.
		bool bFill;

		/// \brief The render mode for bitmaps (e.g. ofDrawBitmapString).
		///
		/// \sa ofDrawBitmapString(const T& textString, float x, float y)
		ofDrawBitmapMode drawBitmapMode;

		/// \brief The blending mode.
		ofBlendMode blendingMode;

		/// \brief True if anti-aliasing is enabled.
		bool smoothing;

		// \brief The resolution of rendered circles and arcs (e.g. ofCircle).
		int circleResolution;

		// \brief The resolution of rendered spheres (e.g. ofSphere).
		int sphereResolution;

		// \brief The resolution of rendered curves.
		int curveResolution;

		/// \brief The width of rendered lines.
		/// \warning This is not currently implemented in modern OF renderers.
		float lineWidth;

		//bool depthTest; removed since it'll break old projects setting depth test through glEnable
};


/// \brief A structure describing attributes of a video format.
///
/// An ofVideoFormat is used to describe the size, pixel format and frame rates
/// offered by a video device.
///
/// \sa ofVideoDevice
class ofVideoFormat{
public:
	/// \brief The pixel format of the video format.
	ofPixelFormat pixelFormat;

	/// \brief The width of the video format in pixels.
	int width;

	/// \brief The height of the video format in pixels.
	int height;

	/// \brief A list of framerates for this video format in frames per second.
	vector<float> framerates;
};

/// \brief A structure describing attributes of a video device.
///
/// An ofVideoDevice can represent a camera, grabber or other frame source.
class ofVideoDevice{
public:
	/// \brief The video device ID.
	int id;

	/// \brief The video device name.
	string deviceName;

	/// \brief The video device hardware name.
	string hardwareName;

	/// \brief A list of video device formats provided by the device.
	/// \sa ofVideoFormat
	vector<ofVideoFormat> formats;

	/// \brief Is true if this video device is available.
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

	OF_DEPRECATED_MSG("Use std::shared_ptr instead",ofPtr());

	  template<typename Tp1>
		explicit
		OF_DEPRECATED_MSG("Use std::shared_ptr instead",ofPtr(Tp1* __p));

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
