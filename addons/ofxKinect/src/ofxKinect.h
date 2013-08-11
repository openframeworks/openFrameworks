/*==============================================================================

    Copyright (c) 2010, 2011 ofxKinect Team

    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to deal
    in the Software without restriction, including without limitation the rights
    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in
    all copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
    THE SOFTWARE.
    
    ----------------------------------------------------------------------------
    
    This project uses libfreenect, copyrighted by the Open Kinect Project using
    the Apache License v2. See the file "APACHE20" in libs/libfreenect.
    
    See http://www.openkinect.org & https://github.com/OpenKinect/libfreenect 
    for documentation
    
==============================================================================*/
#pragma once

#include "ofMain.h"
#include "libfreenect.h"

#include "ofxBase3DVideo.h"

#if defined(_MSC_VER) || defined(_WIN32) || defined(WIN32) || defined(__MINGW32__)
    // do windows stuff
#else
    // mac and linux need this
    #include <libusb.h>
#endif

class ofxKinectContext;

/// \class ofxKinect
///
/// wrapper for a freenect kinect device
///
/// references:
/// - http://openkinect.org/wiki/Main_Page
/// - https://github.com/OpenKinect/libfreenect/blob/master/include/libfreenect.h
///
class ofxKinect : public ofxBase3DVideo, protected ofThread {

public:

	ofxKinect();
	virtual ~ofxKinect();

/// \section Main

	/// initialize resources, must be called before open()
	/// infrared controls whether the video image is rgb or IR
	/// set video to false to disable video image grabbing (saves bandwidth)
	/// set texture to false if you don't need to use the internal textures
	///
	/// naturally, if you disable the video image the video pixels and
	/// RGB color will be 0
	bool init(bool infrared=false, bool video=true, bool texture=true);

	/// clear resources, do not call this while ofxKinect is running!
	void clear();

	/// calibrates the depth image to align with the rgb image, disabled by default
	///
	/// call this before open(), has no effect while the connection is running
	///
	/// note: this calculation uses some cpu, leave off if not needed
	void setRegistration(bool bUseRegistration=false);

	/// open the connection and start grabbing images
	///
	/// set the id to choose a kinect, see numAvailableDevices()
	/// if you don't set the id (ie id=-1), the first available kinect will be used
	///
	/// note: this is the freenct bus id and may change each time the app is run
	bool open(int id=-1);
	
	/// open using a kinect unique serial number
	///
	/// NOTE: currently, libfreenect returns a serial number with all 0s for
	/// kinect models > 1414, so this will only work with the original xbox kinect
	bool open(string serial);

	/// close the connection and stop grabbing images
	void close();

	/// is the connection currently open?
	bool isConnected();

	/// is the current frame new?
	bool isFrameNew();
	bool isFrameNewVideo();
	bool isFrameNewDepth();

	/// updates the pixel buffers and textures
	///
	/// make sure to call this to update to the latest incoming frames
	void update();

/// \section Depth Data

	/// get the calulated distance for a depth point
	float getDistanceAt(int x, int y);
	float getDistanceAt(const ofPoint & p);

	/// calculates the coordinate in the world for the depth point (perspective calculation)
	///
	/// center of image is (0.0)
	ofVec3f getWorldCoordinateAt(int cx, int cy);
	ofVec3f getWorldCoordinateAt(float cx, float cy, float wz);

/// \section Intrinsic IR Sensor Parameters

	/// these values are used when depth registration is enabled to align the
	/// depth image to the rgb image, see http://www.ros.org/wiki/kinect_calibration/technical
	///
	/// they could also be useful for real world accurate point clouds ... weee!
	
	/// get the distance between the IR sensor and IR emitter in cm
	float getSensorEmitterDistance();
	
	/// get the distance between the IR sensor and the RGB camera in cm
	float getSensorCameraDistance();
	
	/// get the size of a single pixel on the zero plane in mm
	float getZeroPlanePixelSize();
	
	/// get the focal length of the IR sensor in mm
	float getZeroPlaneDistance();

/// \section RGB Data

	/// get the RGB value for a depth point
	///
	/// see setRegistration() for calibrated depth->RGB points
	ofColor getColorAt(int x, int y);
	ofColor getColorAt(const ofPoint & p);

/// \section Pixel Data

	/// get the pixels of the most recent rgb frame
	///
	/// see setRegistration() for a calibrated depth->RGB image
	unsigned char* getPixels();

	/// get the pixels of the most recent depth frame
	unsigned char* getDepthPixels();       ///< grayscale values
	unsigned short* getRawDepthPixels();   ///< raw 11 bit values

	/// get the distance in millimeters to a given point as a float array
	float* getDistancePixels();

	/// get the video pixels reference
	ofPixels & getPixelsRef();

	/// get the pixels of the most recent depth frame
	ofPixels & getDepthPixelsRef();       	///< grayscale values
	ofShortPixels & getRawDepthPixelsRef();	///< raw 11 bit values

	/// get the distance in millimeters to a given point as a float array
	ofFloatPixels & getDistancePixelsRef();

	/// get the video (ir or rgb) texture
	ofTexture& getTextureReference();

	/// get the grayscale depth texture
	ofTexture& getDepthTextureReference();

/// \section Grayscale Depth Value

	/// set the near value of the pixels in the grayscale depth image to white
	///
	/// bEnabled = true:  pixels closer to the camera are brighter (default)
	/// bEnabled = false: pixels closer to the camera are darker
	void enableDepthNearValueWhite(bool bEnabled=true);
	bool isDepthNearValueWhite();

	/// set the clipping planes for the depth calculations in millimeters
	///
	/// these are used for the depth value (12bit) -> grayscale (1 byte) conversion
	/// ie setting a short range will give you greater sensitivity from 0-255
	///
	/// default is 50cm - 4m
	/// note: you won't get any data < 50cm and distances > 4m start to get noisy
	void setDepthClipping(float nearClip=500, float farClip=4000);
	float getNearClipping();
	float getFarClipping();

/// \section Query Capabilities

	/// check for device capabilites ...
	/// motor, led, or accelerometer control isn't currently supported 
	/// by libfreenect with newer Kinect models (> 1414)
	bool hasAccelControl();
	bool hasCamTiltControl();
	bool hasLedControl();

/// \section Accelerometer Data

	/// get the XYZ accelerometer values
	///
	/// ... yes, the kinect has an accelerometer
	
	/// raw axis values
	ofPoint getRawAccel();
	
	/// axis-based gravity adjusted accelerometer values
	///
	/// from libfreeenect:
	///
	/// as laid out via the accelerometer data sheet, which is available at
	///
	/// http://www.kionix.com/Product%20Sheets/KXSD9%20Product%20Brief.pdf
	///
	ofPoint getMksAccel();

    /// get the current pitch (x axis) & roll (z axis) of the kinect in degrees
    ///
    /// useful to correct the 3d scene based on the camera inclination
    ///
	float getAccelPitch();
	float getAccelRoll();

/// \section Camera Tilt Motor

	/// set tilt angle of the camera in degrees
	/// 0 is flat, the range is -30 to 30
	bool setCameraTiltAngle(float angleInDegrees);

	/// get the current angle
	float getCurrentCameraTiltAngle();

	/// get the target angle (if the camera is currently moving)
	float getTargetCameraTiltAngle();
        
/// \section LED
    
	enum LedMode {
		LED_DEFAULT = -1, // yellow when not running, green when running
		LED_OFF = 0,
		LED_GREEN = 1,
		LED_RED = 2,
		LED_YELLOW = 3,
		LED_BLINK_GREEN = 4,
		LED_BLINK_YELLOW_RED = 6
	};
	
    /// set the current led color and/or blink mode,
	/// only applied while the kinect is open
    void setLed(ofxKinect::LedMode mode);

/// \section Draw

	/// enable/disable frame loading into textures on update()
	void setUseTexture(bool bUse);

	/// draw the video texture
	void draw(float x, float y, float w, float h);
	void draw(float x, float y);
	void draw(const ofPoint& point);
	void draw(const ofRectangle& rect);

	/// draw the grayscale depth texture
	void drawDepth(float x, float y, float w, float h);
	void drawDepth(float x, float y);
	void drawDepth(const ofPoint& point);
	void drawDepth(const ofRectangle& rect);

/// \section Util

	/// get the device id
	/// returns -1 if not connected
	int getDeviceId();
	
	/// get the unique serial number
	/// returns an empty string "" if not connected
	///
	/// NOTE: currently, libfreenect returns a serial number with all 0s for
	/// kinect models > 1414, so this will only work with the original xbox kinect
	string getSerial();

	/// static kinect image size
	const static int width = 640;
	const static int height = 480;
	float getHeight();
	float getWidth();

/// \section Static global kinect context functions

	/// print the device list
	static void listDevices();
	
	/// get the total number of devices
	static int numTotalDevices();

	/// get the number of available devices (not connected)
	static int numAvailableDevices();

	/// get the number of currently connected devices
	static int numConnectedDevices();

	/// is a device already connected?
	static bool isDeviceConnected(int id);
	static bool isDeviceConnected(string serial);

	/// get the id of the next available device,
	/// returns -1 if nothing found
	static int nextAvailableId();
	
	/// get the serial number of the next available device,
	/// returns an empty string "" if nothing found
	static string nextAvailableSerial();

protected:

	int deviceId;	///< -1 when not connected
	string serial;	///< unique serial number, "" when not connected
	
	bool bUseTexture;
	ofTexture depthTex; ///< the depth texture
	ofTexture videoTex; ///< the RGB texture
	bool bGrabberInited;

	ofPixels videoPixels;
	ofPixels depthPixels;
	ofShortPixels depthPixelsRaw;
	ofFloatPixels distancePixels;

	ofPoint rawAccel;
	ofPoint mksAccel;

	float targetTiltAngleDeg;
	float currentTiltAngleDeg;
	bool bTiltNeedsApplying;
    
    int currentLed;
    bool bLedNeedsApplying;
    bool bHasMotorControl; // cam tilt motor
	//bool bHasAccelContol; // for future use
	//bool bHasLedControl; // for future use
	
	// for auto connect tries
	float timeSinceOpen;
	int lastDeviceId;
	bool bGotData;
	int tryCount;

private:

	friend class ofxKinectContext;

	/// global statics shared between kinect instances
	static ofxKinectContext kinectContext;

	freenect_device* kinectDevice;      ///< kinect device handle

	ofShortPixels depthPixelsRawIntra;	///< depth back
	ofPixels videoPixelsIntra;			///< rgb back
	ofShortPixels depthPixelsRawBack;	///< depth back
	ofPixels videoPixelsBack;			///< rgb back

	vector<unsigned char> depthLookupTable;
	void updateDepthLookupTable();
	void updateDepthPixels();

	bool bIsFrameNewVideo, bIsFrameNewDepth;
	bool bNeedsUpdateVideo, bNeedsUpdateDepth;
	bool bGrabVideo;
	bool bUseRegistration;
	bool bNearWhite;

	float nearClipping, farClipping;

	bool bIsVideoInfrared;  ///< is the video image infrared or RGB?
	int videoBytesPerPixel; ///< how many bytes per pixel in the video image

	/// libfreenect callbacks
	static void grabDepthFrame(freenect_device* dev, void* depth, uint32_t timestamp);
	static void grabVideoFrame(freenect_device* dev, void* video, uint32_t timestamp);

	/// thread function
	void threadedFunction();
};

/// \class ofxKinectContext
///
/// wrapper for the freenect context
///
/// do not use this directly
///
class ofxKinectContext {

public:

	ofxKinectContext();
	~ofxKinectContext();

/// \section Main

	/// init the freenect context
	bool init();

	/// clear the freenect context
	/// closes all currently connected devices
	void clear();

	/// is the context inited?
	bool isInited();

	/// open a kinect device
	/// an id of -1 will open the first available
	bool open(ofxKinect& kinect, int id=-1);
	
	/// open a kinect device by it's unique serial number
	bool open(ofxKinect& kinect, string serial);

	/// close a kinect device
	void close(ofxKinect& kinect);

	/// closes all currently connected kinects
	void closeAll();

/// \section Util
	
	/// (re)build the list of devices
	void buildDeviceList();
	
	/// print the device list
	void listDevices(bool verbose=false);
	
	/// get the total number of devices
	int numTotal();

	/// get the number of available devices (not connected)
	int numAvailable();

	/// get the number of currently connected devices
	int numConnected();

	/// get the kinect object from a device pointer
	/// returns NULL if not found
	ofxKinect* getKinect(freenect_device* dev);
	
	/// get the deviceList index from an id
	/// returns -1 if not found
	int getDeviceIndex(int id);
	
	/// get the deviceList index from an id
	/// returns -1 if not found
	int getDeviceIndex(string serial);

	/// is a device with this id already connected?
	bool isConnected(int id);
	
	/// is a device with this serial already connected?
	bool isConnected(string serial);

	/// get the id of the next available device,
	/// returns -1 if nothing found
	int nextAvailableId();
	
	/// get the serial number of the next available device,
	/// returns an empty string "" if nothing found
	string nextAvailableSerial();

	/// get the raw pointer
	freenect_context* getContext() {return kinectContext;}

	// for auto-enumeration
    struct KinectPair{
		string serial;	///< unique serial number
		int id;			///< freenect bus id
    };
	
private:
    
	bool bInited;						///< has the context been initialized?
	freenect_context* kinectContext;    ///< kinect context handle
	std::vector<KinectPair> deviceList;	///< list of available devices, sorted by serial lexicographically
	std::map<int,ofxKinect*> kinects;   ///< the connected kinects
};
