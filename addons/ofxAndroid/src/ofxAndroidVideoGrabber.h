/*
 * ofxAndroidVideoGrabber.h
 *
 *  Created on: 17/01/2011
 *      Author: arturo
 */

#pragma once

#include "ofVideoBaseTypes.h"
#include "ofPixels.h"
#include "ofEvents.h"
#include "ofTypes.h"
#include "ofTexture.h"
#include <jni.h>

class ofxAndroidVideoGrabber: public ofBaseVideoGrabber{
public:
	ofxAndroidVideoGrabber();
	~ofxAndroidVideoGrabber();

	std::vector<ofVideoDevice>	listDevices() const;
	bool setup(int w, int h);
	bool isInitialized() const;

	bool isFrameNew() const;
	void update();

	ofPixels& getPixels();
	const ofPixels&	getPixels() const;

	void setUsePixels(bool usePixels);

	void close();

	float getHeight() const;
	float getWidth() const;

	//should implement!
	void setVerbose(bool bTalkToMe);


	/// Set desired frame rate of the camera.
	/// By default will the camera pick the highest frame rate available
	void setDesiredFrameRate(int framerate);

	/// Set specific camera device id.
	/// Must be a value between 0 and numCameras.
	/// Default is first back facing camera
	void setDeviceID(int _deviceID);

	void videoSettings();
	bool setPixelFormat(ofPixelFormat pixelFormat);
	ofPixelFormat getPixelFormat() const;

	// specifics android

	/// Get device id of back facing camera.
	/// Returns -1 if no match is found
	int getBackCamera()const;

	/// Get device id of front facing (selfie) camera.
	/// Returns -1 if no match is found
	int getFrontCamera()const;

	/// Get the physical orientation of the camera. Typically on a phone the camera mounted in
	/// landscape mode, this returns 90
	int getCameraOrientation(int device=-1)const;

	/// Get facing of camera.
	/// Leave device = -1 to get selected cameras facing
	///
	/// Returns 0 on backfacing camera, and 1 on frontal facing camera.
	int getFacingOfCamera(int device=-1)const;

	bool setAutoFocus(bool autofocus);

	ofTexture *	getTexturePtr();

	bool supportsTextureRendering();

	struct Data;
private:
	int getCameraFacing(int facing)const;

	/// Get number of cameras available
	int getNumCameras()const;

	bool initCamera();

	// only to be used internally to resize;
	ofPixelsRef getAuxBuffer();
	std::shared_ptr<Data> data;
};
