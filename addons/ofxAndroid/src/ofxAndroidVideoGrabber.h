/*
 * ofxAndroidVideoGrabber.h
 *
 *  Created on: 17/01/2011
 *      Author: arturo
 */

#pragma once

#include "ofBaseTypes.h"
#include "ofPixels.h"
#include "ofEvents.h"
#include "ofTypes.h"
#include "ofTexture.h"
#include <jni.h>

class ofxAndroidVideoGrabber: public ofBaseVideoGrabber{
public:
	ofxAndroidVideoGrabber();
	~ofxAndroidVideoGrabber();

	//needs implementing
	vector<ofVideoDevice>	listDevices() const;
	bool	setup(int w, int h);
	bool	isInitialized() const;

	bool	isFrameNew() const;
	void	update();

	ofPixels& getPixels();
	const ofPixels&	getPixels() const;

	void	close();

	float	getHeight() const;
	float	getWidth() const;

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

	/// Get number of cameras available
	int getNumCameras();

	/// Get device id of back facing camera.
	/// Returns -1 if no match is found
	int getBackCamera();

	/// Get device id of front facing (selfie) camera.
	/// Returns -1 if no match is found
	int getFrontCamera();

	/// Get the physical orientation of the camera. Typically on a phone the camera mounted in
	/// landscape mode, this returns
	int getCameraOrientation();

	/// Returns true if the camera is frontal facing. Use getFrontCamera() or getBackCamera()
	/// to specify the camera during initialization
	int getIsCameraFacingFront();

	bool setAutoFocus(bool autofocus);

	ofTexture *	getTexturePtr();

	ofEvent<ofPixels> newFrameE;

	struct Data;
private:
	int getCameraFacing(int facing);
	bool initCamera();

	// only to be used internally to resize;
	ofPixelsRef getAuxBuffer();
	shared_ptr<Data> data;
};
