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
#include "ofxKinect.h"
#include "ofMain.h"

#include "libfreenect_registration.h"
#include "freenect_internal.h" // for access to freenect_device.registration.zero_plane_info

#include "ofxKinectExtras.h"

#ifndef BUILD_AUDIO
	#undef OFX_KINECT_EXTRA_FW //Audio / Motor via Audio support is not currently working with libfreenect on win32
#endif 

#define OFX_KINECT_GRAVITY 9.80665

// context static
ofxKinectContext ofxKinect::kinectContext;

//--------------------------------------------------------------------
ofxKinect::ofxKinect() {
	ofLogVerbose("ofxKinect") <<" creating ofxKinect";

	deviceId = -1;
	serial = "";
	
	bUseTexture = true;
	bGrabVideo = true;

	// set defaults
	bGrabberInited = false;

	bNeedsUpdateVideo = false;
	bNeedsUpdateDepth = false;
	bIsFrameNewVideo = false;
	bIsFrameNewDepth = false;
    
	bIsVideoInfrared = false;
	videoBytesPerPixel = 3;

	kinectDevice = NULL;

	targetTiltAngleDeg = 0;
	currentTiltAngleDeg = 0;
	bTiltNeedsApplying = false;
    
    currentLed = -1;
    bLedNeedsApplying = false;
	bHasMotorControl = false;
	
	lastDeviceId = -1;
	tryCount = 0;
	timeSinceOpen = 0;
	bGotData = false;

	bUseRegistration = false;
	bNearWhite = true;

	pixelFormat = OF_PIXELS_RGB;

	setDepthClipping();
}

//--------------------------------------------------------------------
ofxKinect::~ofxKinect() {
	close();
	clear();
}

//--------------------------------------------------------------------
bool ofxKinect::init(bool infrared, bool video, bool texture) {
	if(isConnected()) {
		ofLogWarning("ofxKinect") << "init(): do not call init while ofxKinect is running!";
		return false;
	}

	clear();

	bIsVideoInfrared = infrared;
	bGrabVideo = video;
	videoBytesPerPixel = infrared?1:3;
	if(infrared){
		pixelFormat = OF_PIXELS_GRAY;
	}

	bUseTexture = texture;

	// allocate
	depthPixelsRaw.allocate(width, height, 1);
	depthPixelsRawBack.allocate(width, height, 1);
	depthPixelsRawIntra.allocate(width, height, 1);
    
    //We have to do this as freenect has 488 pixels for the IR image height.
    //Instead of having slightly different sizes depending on capture we will crop the last 8 rows of pixels which are empty.
    int videoHeight = height;
    if( infrared ){
        videoHeight = 488;
    }
    
	videoPixels.allocate(width, height, videoBytesPerPixel);
	videoPixelsBack.allocate(width, videoHeight, videoBytesPerPixel);
	videoPixelsIntra.allocate(width, videoHeight, videoBytesPerPixel);

	depthPixels.allocate(width, height, 1);
	distancePixels.allocate(width, height, 1);

	 // set
	depthPixelsRaw.set(0);
	depthPixelsRawBack.set(0);

	videoPixels.set(0);
	videoPixelsBack.set(0);

	depthPixels.set(0);    
	distancePixels.set(0);

	if(bUseTexture) {
		depthTex.allocate(width, height, GL_LUMINANCE);
		videoTex.allocate(width, height, infrared ? GL_LUMINANCE : GL_RGB);
	}

	if(!kinectContext.isInited()) {
		if(!kinectContext.init()) {
			return false;
		}
	}

	bGrabberInited = true;

	return bGrabberInited;
}

//---------------------------------------------------------------------------
void ofxKinect::clear() {
	if(isConnected()) {
		ofLogWarning("ofxKinect") << "clear(): do not call clear while ofxKinect is running!";
		return;
	}

	depthPixelsRaw.clear();
	depthPixelsRawBack.clear();

	videoPixels.clear();
	videoPixelsBack.clear();

	depthPixels.clear();
	distancePixels.clear();

	depthTex.clear();
	videoTex.clear();

	bGrabberInited = false;
}

//--------------------------------------------------------------------
void ofxKinect::setRegistration(bool bUseRegistration) {
	this->bUseRegistration = bUseRegistration;
}

//--------------------------------------------------------------------
bool ofxKinect::open(int id) {
	if(!bGrabberInited) {
		ofLogWarning("ofxKinect") << "open(): cannot open, init not called";
		return false;
	}

	if(!kinectContext.open(*this, id)) {
		return false;
	}

	if(serial == "0000000000000000") {
        bHasMotorControl = false;
        //if we do motor control via the audio device ( ie: 1473 or k4w ) and we have firmware uploaded
        //then we can do motor stuff! :)
        if( kinectDevice->motor_control_with_audio_enabled ){
            bHasMotorControl = true;
        }else{
            ofLogVerbose("ofxKinect") << "open(): device " << deviceId << " does not have motor control";
        }
    }
	else {
		bHasMotorControl = true;
	}

	lastDeviceId = deviceId;
	timeSinceOpen = ofGetElapsedTimef();
	bGotData = false;

	freenect_set_user(kinectDevice, this);
	freenect_set_depth_buffer(kinectDevice, depthPixelsRawBack.getData());
	freenect_set_video_buffer(kinectDevice, videoPixelsBack.getData());
	freenect_set_depth_callback(kinectDevice, &grabDepthFrame);
	freenect_set_video_callback(kinectDevice, &grabVideoFrame);

	startThread(); // blocking, not verbose

	return true;
}

//--------------------------------------------------------------------
bool ofxKinect::open(string serial) {
	if(!bGrabberInited) {
		ofLogVerbose("ofxKinect") << "open(): cannot open, init not called";
		return false;
	}
	
	if(!kinectContext.open(*this, serial)) {
		return false;
	}
	
	if(serial == "0000000000000000") {
        bHasMotorControl = false;
        //if we do motor control via the audio device ( ie: 1473 or k4w ) and we have firmware uploaded
        //then we can do motor stuff! :)
        if( kinectDevice->motor_control_with_audio_enabled ){
            bHasMotorControl = true;
        }else{
            ofLogVerbose("ofxKinect") << "open(): device " << deviceId << " does not have motor control";
        }
    }
	else {
		bHasMotorControl = true;
	}
    
	lastDeviceId = deviceId;
	timeSinceOpen = ofGetElapsedTimef();
	bGotData = false;
	
	freenect_set_user(kinectDevice, this);
	freenect_set_depth_callback(kinectDevice, &grabDepthFrame);
	freenect_set_video_callback(kinectDevice, &grabVideoFrame);
	
	startThread(); // blocking, not verbose
	
	return true;
}

//---------------------------------------------------------------------------
void ofxKinect::close() {
	if(isThreadRunning()) {
		stopThread();
		ofSleepMillis(10);
		waitForThread(false);
	}

	deviceId = -1;
	serial = "";
	bIsFrameNewVideo = false;
	bNeedsUpdateVideo = false;
	bIsFrameNewDepth = false;
	bNeedsUpdateDepth = false;
}

//---------------------------------------------------------------------------
bool ofxKinect::isConnected() const{
	return isThreadRunning();
}

//---------------------------------------------------------------------------
bool ofxKinect::isInitialized() const{
	return kinectContext.isInited();
}

//--------------------------------------------------------------------
bool ofxKinect::isFrameNew()  const{
	return isFrameNewVideo() || isFrameNewDepth();
}

//--------------------------------------------------------------------
bool ofxKinect::isFrameNewVideo() const{
	return bIsFrameNewVideo;
}

//--------------------------------------------------------------------
bool ofxKinect::isFrameNewDepth() const{
	return bIsFrameNewDepth;
}

//--------------------------------------------------------------------
bool ofxKinect::setPixelFormat(ofPixelFormat pixelFormat){
	if(!bIsVideoInfrared && pixelFormat==OF_PIXELS_RGB){
		return true;
	}else if(pixelFormat == OF_PIXELS_GRAY){
		return true;
	}else{
		return false;
	}
}

//--------------------------------------------------------------------
ofPixelFormat ofxKinect::getPixelFormat() const{
	if(!bIsVideoInfrared){
		return OF_PIXELS_RGB;
	}else{
		return OF_PIXELS_GRAY;
	}
}

//----------------------------------------------------------
void ofxKinect::update() {
	if(!bGrabberInited) {
		return;
	}

	if(!bNeedsUpdateVideo && !bNeedsUpdateDepth && !bGotData && tryCount < 5 && ofGetElapsedTimef() - timeSinceOpen > 2.0 ){
		close();
		ofLogWarning("ofxKinect") << "update(): device " << lastDeviceId << " isn't delivering data, reconnecting tries: " << tryCount+1;
		kinectContext.buildDeviceList();
		open(lastDeviceId);
		tryCount++;
		timeSinceOpen = ofGetElapsedTimef();
		return;
	}

	if(bNeedsUpdateVideo){
		bIsFrameNewVideo = true;
		bGotData = true;
		tryCount = 0;
		if(this->lock()) {
            if( videoPixels.getHeight() == videoPixelsIntra.getHeight() ){
                swap(videoPixels,videoPixelsIntra);
            }else{
				int minimumSize = MIN(videoPixels.size(), videoPixelsIntra.size());
				memcpy(videoPixels.getData(), videoPixelsIntra.getData(), minimumSize);
            }
			bNeedsUpdateVideo = false;
			this->unlock();
		}

		if(bUseTexture) {
			videoTex.loadData(videoPixels.getData(), width, height, bIsVideoInfrared?GL_LUMINANCE:GL_RGB);
		}
	} else {
		bIsFrameNewVideo = false;
	}

	if(bNeedsUpdateDepth){
		bIsFrameNewDepth = true;
		bGotData = true;
		tryCount = 0;
		if(this->lock()) {
			swap(depthPixelsRaw, depthPixelsRawIntra);
			bNeedsUpdateDepth = false;
			this->unlock();

			updateDepthPixels();
		}

		if(bUseTexture) {
			depthTex.loadData(depthPixels.getData(), width, height, GL_LUMINANCE);
		}
	} else {
		bIsFrameNewDepth = false;
	}

}

//------------------------------------
float ofxKinect::getDistanceAt(int x, int y)  const{
	return depthPixelsRaw[y * width + x];
}

//------------------------------------
float ofxKinect::getDistanceAt(const ofPoint & p)  const{
	return getDistanceAt(p.x, p.y);
}

//------------------------------------
ofVec3f ofxKinect::getWorldCoordinateAt(int x, int y)  const{
	return getWorldCoordinateAt(x, y, getDistanceAt(x, y));
}

//------------------------------------
ofVec3f ofxKinect::getWorldCoordinateAt(float cx, float cy, float wz)  const{
	double wx, wy;
	freenect_camera_to_world(kinectDevice, cx, cy, wz, &wx, &wy);
	return ofVec3f(wx, wy, wz);
}

//------------------------------------
float ofxKinect::getSensorEmitterDistance()  const{
	return kinectDevice->registration.zero_plane_info.dcmos_emitter_dist;
}

//------------------------------------
float ofxKinect::getSensorCameraDistance()  const{
	return kinectDevice->registration.zero_plane_info.dcmos_rcmos_dist;
}

//------------------------------------
float ofxKinect::getZeroPlanePixelSize()  const{
	return kinectDevice->registration.zero_plane_info.reference_pixel_size;
}

//------------------------------------
float ofxKinect::getZeroPlaneDistance()  const{
	return kinectDevice->registration.zero_plane_info.reference_distance;
}

//------------------------------------
ofColor ofxKinect::getColorAt(int x, int y)  const{
	int index = (y * width + x) * videoBytesPerPixel;
	ofColor c;
	c.r = videoPixels[index + 0];
	c.g = videoPixels[index + (videoBytesPerPixel-1)/2];
	c.b = videoPixels[index + (videoBytesPerPixel-1)];
	c.a = 255;

	return c;
}

//------------------------------------
ofColor ofxKinect::getColorAt(const ofPoint & p)  const{
	return getColorAt(p.x, p.y);
}

ofPixels & ofxKinect::getPixels(){
	return videoPixels;
}

ofPixels & ofxKinect::getDepthPixels(){
	return depthPixels;
}

ofShortPixels & ofxKinect::getRawDepthPixels(){
	return depthPixelsRaw;
}

ofFloatPixels & ofxKinect::getDistancePixels(){
	return distancePixels;
}

const ofPixels & ofxKinect::getPixels() const{
	return videoPixels;
}

const ofPixels & ofxKinect::getDepthPixels() const{
	return depthPixels;
}

const ofShortPixels & ofxKinect::getRawDepthPixels() const{
	return depthPixelsRaw;
}

const ofFloatPixels & ofxKinect::getDistancePixels() const{
	return distancePixels;
}

//------------------------------------
ofTexture& ofxKinect::getTexture(){
	if(!videoTex.isAllocated()){
		ofLogWarning("ofxKinect") << "getTexture(): device " << deviceId << " video texture not allocated";
	}
	return videoTex;
}

//---------------------------------------------------------------------------
ofTexture& ofxKinect::getDepthTexture(){
	if(!depthTex.isAllocated()){
		ofLogWarning("ofxKinect") << "getDepthTexture(): device " << deviceId << " depth texture not allocated";
	}
	return depthTex;
}

//------------------------------------
const ofTexture& ofxKinect::getTexture() const{
	if(!videoTex.isAllocated()){
		ofLogWarning("ofxKinect") << "getTexture(): device " << deviceId << " video texture not allocated";
	}
	return videoTex;
}

//---------------------------------------------------------------------------
const ofTexture& ofxKinect::getDepthTexture() const{
	if(!depthTex.isAllocated()){
		ofLogWarning("ofxKinect") << "getDepthTexture(): device " << deviceId << " depth texture not allocated";
	}
	return depthTex;
}

//------------------------------------
ofTexture& ofxKinect::getTextureReference(){
	return getTexture();
}

//---------------------------------------------------------------------------
ofTexture& ofxKinect::getDepthTextureReference(){
	return getDepthTexture();
}

//------------------------------------
const ofTexture& ofxKinect::getTextureReference() const{
	return getTexture();
}

//---------------------------------------------------------------------------
const ofTexture& ofxKinect::getDepthTextureReference() const{
	return getDepthTexture();
}

//---------------------------------------------------------------------------
void ofxKinect::enableDepthNearValueWhite(bool bEnabled) {
	bNearWhite = bEnabled;
	updateDepthLookupTable();
}

//---------------------------------------------------------------------------
bool ofxKinect::isDepthNearValueWhite() const{
	return bNearWhite;
}

//---------------------------------------------------------------------------
void ofxKinect::setDepthClipping(float nearClip, float farClip) {
	nearClipping = nearClip;
	farClipping = farClip;
	updateDepthLookupTable();
}

//---------------------------------------------------------------------------
float ofxKinect::getNearClipping() const{
    return nearClipping;
}

//---------------------------------------------------------------------------
float ofxKinect::getFarClipping() const{
    return farClipping;
}

//--------------------------------------------------------------------
bool ofxKinect::hasAccelControl() const{
	return bHasMotorControl; // depends on motor for now
}

bool ofxKinect::hasCamTiltControl() const{
	return bHasMotorControl; // depends on motor for now
}

bool ofxKinect::hasLedControl() const{
	return bHasMotorControl; // depends on motor for now
}

//---------------------------------------------------------------------------
ofPoint ofxKinect::getRawAccel() const{
	return rawAccel;
}

//---------------------------------------------------------------------------
ofPoint ofxKinect::getMksAccel() const{
	return mksAccel;
}

//---------------------------------------------------------------------------
float ofxKinect::getAccelPitch() const{
	return ofRadToDeg(asin(getMksAccel().z/OFX_KINECT_GRAVITY));
}

//---------------------------------------------------------------------------
float ofxKinect::getAccelRoll() const{
	return ofRadToDeg(asin(getMksAccel().x/OFX_KINECT_GRAVITY));
}

// we update the value here, but apply it in kinect thread
//--------------------------------------------------------------------
bool ofxKinect::setCameraTiltAngle(float angleInDegrees) {

	if(!hasCamTiltControl() || !bGrabberInited) {
		return false;
	}

	targetTiltAngleDeg = ofClamp(angleInDegrees, -30, 30);
	bTiltNeedsApplying = true;

	return true;
}

//--------------------------------------------------------------------
float ofxKinect::getTargetCameraTiltAngle() const{
	return targetTiltAngleDeg;
}

float ofxKinect::getCurrentCameraTiltAngle() const{
	return currentTiltAngleDeg;
}

//--------------------------------------------------------------------

void ofxKinect::setLed(ofxKinect::LedMode mode) {
	if(!hasLedControl() || mode == currentLed) {
		return;
	}
    bLedNeedsApplying = true;
    currentLed = mode;
}

//------------------------------------
void ofxKinect::setUseTexture(bool bUse){
	bUseTexture = bUse;
}

//------------------------------------
bool ofxKinect::isUsingTexture() const{
	return bUseTexture;
}

//----------------------------------------------------------
void ofxKinect::draw(float _x, float _y, float _w, float _h) const{
	if(bUseTexture && bGrabVideo) {
		videoTex.draw(_x, _y, _w, _h);
	}
}

//----------------------------------------------------------
void ofxKinect::draw(float _x, float _y) const{
	draw(_x, _y, (float)width, (float)height);
}

//----------------------------------------------------------
void ofxKinect::draw(const ofPoint & point) const{
	draw(point.x, point.y);
}

//----------------------------------------------------------
void ofxKinect::draw(const ofRectangle & rect) const{
	draw(rect.x, rect.y, rect.width, rect.height);
}

//----------------------------------------------------------
void ofxKinect::drawDepth(float _x, float _y, float _w, float _h) const{
	if(bUseTexture) {
		depthTex.draw(_x, _y, _w, _h);
	}
}

//---------------------------------------------------------------------------
void ofxKinect::drawDepth(float _x, float _y) const{
	drawDepth(_x, _y, (float)width, (float)height);
}

//----------------------------------------------------------
void ofxKinect::drawDepth(const ofPoint & point) const{
	drawDepth(point.x, point.y);
}

//----------------------------------------------------------
void ofxKinect::drawDepth(const ofRectangle & rect) const{
	drawDepth(rect.x, rect.y, rect.width, rect.height);
}

//---------------------------------------------------------------------------
int ofxKinect::getDeviceId() const{
	return deviceId;
}

//---------------------------------------------------------------------------
string ofxKinect::getSerial() const{
	return serial;
}

//----------------------------------------------------------
float ofxKinect::getHeight() const{
	return (float) height;
}

//---------------------------------------------------------------------------
float ofxKinect::getWidth() const{
	return (float) width;
}

//----------------------------------------------------------
void ofxKinect::listDevices() {
	kinectContext.listDevices();
}

//---------------------------------------------------------------------------
int ofxKinect::numTotalDevices() {
	return kinectContext.numTotal();
}

//---------------------------------------------------------------------------
int ofxKinect::numAvailableDevices() {
	return kinectContext.numAvailable();
}

//---------------------------------------------------------------------------
int ofxKinect::numConnectedDevices() {
	return kinectContext.numConnected();
}

//---------------------------------------------------------------------------
bool ofxKinect::isDeviceConnected(int id) {
	return kinectContext.isConnected(id);
}

//---------------------------------------------------------------------------
bool ofxKinect::isDeviceConnected(string serial) {
	return kinectContext.isConnected(serial);
}

//---------------------------------------------------------------------------
int ofxKinect::nextAvailableId() {
	return kinectContext.nextAvailableId();
}

//---------------------------------------------------------------------------
string ofxKinect::nextAvailableSerial() {
	return kinectContext.nextAvailableSerial();
}

/* ***** PRIVATE ***** */

//---------------------------------------------------------------------------
void ofxKinect::updateDepthLookupTable() {
	unsigned char nearColor = bNearWhite ? 255 : 0;
	unsigned char farColor = bNearWhite ? 0 : 255;
	unsigned int maxDepthLevels = 10001;
	depthLookupTable.resize(maxDepthLevels);
	depthLookupTable[0] = 0;
	for(unsigned int i = 1; i < maxDepthLevels; i++) {
		depthLookupTable[i] = ofMap(i, nearClipping, farClipping, nearColor, farColor, true);
	}
}

//----------------------------------------------------------
void ofxKinect::updateDepthPixels() {
	int n = width * height;
	for(int i = 0; i < n; i++) {
		distancePixels[i] = depthPixelsRaw[i];
	}
	for(int i = 0; i < n; i++) {
		depthPixels[i] = depthLookupTable[depthPixelsRaw[i]];
	}
}

//---------------------------------------------------------------------------
void ofxKinect::grabDepthFrame(freenect_device *dev, void *depth, uint32_t timestamp) {

	ofxKinect* kinect = kinectContext.getKinect(dev);

	if(kinect->kinectDevice == dev) {
		kinect->lock();
		swap(kinect->depthPixelsRawBack,kinect->depthPixelsRawIntra);
		kinect->bNeedsUpdateDepth = true;
		kinect->unlock();
		freenect_set_depth_buffer(kinect->kinectDevice,kinect->depthPixelsRawBack.getData());
    }
}

//---------------------------------------------------------------------------
void ofxKinect::grabVideoFrame(freenect_device *dev, void *video, uint32_t timestamp) {

	ofxKinect* kinect = kinectContext.getKinect(dev);

	if(kinect->kinectDevice == dev) {
		kinect->lock();
		swap(kinect->videoPixelsBack,kinect->videoPixelsIntra);
		kinect->bNeedsUpdateVideo = true;
		kinect->unlock();
		freenect_set_video_buffer(kinect->kinectDevice,kinect->videoPixelsBack.getData());
	}
}

//---------------------------------------------------------------------------
void ofxKinect::threadedFunction(){

	if(currentLed < 0) { 
        freenect_set_led(kinectDevice, (freenect_led_options)ofxKinect::LED_GREEN); 
    }
	
	freenect_frame_mode videoMode = freenect_find_video_mode(FREENECT_RESOLUTION_MEDIUM, bIsVideoInfrared?FREENECT_VIDEO_IR_8BIT:FREENECT_VIDEO_RGB);
	freenect_set_video_mode(kinectDevice, videoMode);
	freenect_frame_mode depthMode = freenect_find_depth_mode(FREENECT_RESOLUTION_MEDIUM, bUseRegistration?FREENECT_DEPTH_REGISTERED:FREENECT_DEPTH_MM);
	freenect_set_depth_mode(kinectDevice, depthMode);

	ofLogVerbose("ofxKinect") << "device " << deviceId << " " << serial << " connection opened";

	freenect_start_depth(kinectDevice);
	if(bGrabVideo) {
		freenect_start_video(kinectDevice);
	}

	while(isThreadRunning() && freenect_process_events(kinectContext.getContext()) >= 0) {        
		if(bTiltNeedsApplying) {
			freenect_set_tilt_degs(kinectDevice, targetTiltAngleDeg);
			bTiltNeedsApplying = false;
		}
		
		if(bLedNeedsApplying) {
			if(currentLed == ofxKinect::LED_DEFAULT) {
				freenect_set_led(kinectDevice, (freenect_led_options)ofxKinect::LED_GREEN);
			}
			else {
				freenect_set_led(kinectDevice, (freenect_led_options)currentLed);
			}
			bLedNeedsApplying = false;
		}

		freenect_update_tilt_state(kinectDevice);
		freenect_raw_tilt_state * tilt = freenect_get_tilt_state(kinectDevice);
		currentTiltAngleDeg = freenect_get_tilt_degs(tilt);

		rawAccel.set(tilt->accelerometer_x, tilt->accelerometer_y, tilt->accelerometer_z);

		double dx,dy,dz;
		freenect_get_mks_accel(tilt, &dx, &dy, &dz);
		mksAccel.set(dx, dy, dz);
	}
    
	// finish up a tilt on exit
	if(bTiltNeedsApplying) {
		freenect_set_tilt_degs(kinectDevice, targetTiltAngleDeg);
		bTiltNeedsApplying = false;
	}
    
	freenect_stop_depth(kinectDevice);
	freenect_stop_video(kinectDevice);
	if(currentLed < 0) {
        freenect_set_led(kinectDevice, (freenect_led_options)ofxKinect::LED_RED);
    }
    
	kinectContext.close(*this);
	ofLogVerbose("ofxKinect") << "device " << deviceId << " connection closed";
}

//---------------------------------------------------------------------------
// ofxKinectContext
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
ofxKinectContext::ofxKinectContext() {
	bInited = false;
	kinectContext = NULL;
}
ofxKinectContext::~ofxKinectContext() {
	closeAll();
	clear();
}

//---------------------------------------------------------------------------
static bool sortKinectPairs(ofxKinectContext::KinectPair A, ofxKinectContext::KinectPair B){
	return A.serial < B.serial;
}
        
//---------------------------------------------------------------------------
bool ofxKinectContext::init() {
	
	if(freenect_init(&kinectContext, NULL) < 0) {
		ofLogError("ofxKinect") << "init(): freenect_init failed";
		bInited = false;
		return false;
	}
    
    #ifdef OFX_KINECT_EXTRA_FW
        freenect_set_fw_address_nui(kinectContext, ofxKinectExtras::getFWData1473(), ofxKinectExtras::getFWSize1473());
        freenect_set_fw_address_k4w(kinectContext, ofxKinectExtras::getFWDatak4w(), ofxKinectExtras::getFWSizek4w());
    #endif
    
	freenect_set_log_level(kinectContext, FREENECT_LOG_WARNING);
	freenect_select_subdevices(kinectContext, (freenect_device_flags)(FREENECT_DEVICE_MOTOR | FREENECT_DEVICE_CAMERA));

	bInited = true;
	ofLogVerbose("ofxKinect") << "context inited";
	
	buildDeviceList();
	listDevices(true);

	return true;
}

//---------------------------------------------------------------------------
void ofxKinectContext::clear() {
	if(isInited() && numConnected() < 1) {
		freenect_shutdown(kinectContext);
		kinectContext = NULL;
		bInited = false;
		ofLogVerbose("ofxKinect") << "context cleared";
	}
}

bool ofxKinectContext::isInited() {
	return bInited;
}

bool ofxKinectContext::open(ofxKinect& kinect, int id) {
	
	// rebuild if necessary (aka new kinects plugged in)
	buildDeviceList();
	
	if(numConnected() >= numTotal()) {
		ofLogWarning("ofxKinect") << "no available devices found";
		return false;
	}
	
	// is the id available?
	if(id < 0) {
		id = nextAvailableId();
	}
	if(isConnected(id)) {
		ofLogWarning("ofxKinect") << "device " << id << " already connected";
		return false;
	}
	
	// open and add to vector
	if(freenect_open_device(kinectContext, &kinect.kinectDevice, id) < 0) {
		ofLogError("ofxKinect") << "could not open device " <<  id;
		return false;
	}
	kinects.insert(pair<int,ofxKinect*>(id, &kinect));
	
	// set kinect id & serial from bus id
	kinect.deviceId = id;
	kinect.serial = deviceList[getDeviceIndex(id)].serial;

	return true;
}

bool ofxKinectContext::open(ofxKinect& kinect, string serial) {
	
	// rebuild if necessary (aka new kinects plugged in)
	buildDeviceList();
	
	if(numConnected() >= numTotal()) {
		ofLogWarning("ofxKinect") << "no available devices found";
		return false;
	}
	
	// is the serial available?
	if(isConnected(serial)) {
		ofLogWarning("ofxKinect") << "device " << serial << " already connected";
		return false;
	}
	
	// open and add to vector
	if(freenect_open_device_by_camera_serial(kinectContext, &kinect.kinectDevice, serial.c_str()) < 0) {
		ofLogError("ofxKinect") << "could not open device " << serial;
		return false;
	}
	int index = getDeviceIndex(serial);
	kinects.insert(pair<int,ofxKinect*>(deviceList[index].id, &kinect));
	kinect.deviceId = deviceList[index].id;
	kinect.serial = serial;
	
	return true;
}

void ofxKinectContext::close(ofxKinect& kinect) {

	// check if it's already closed
	int id = -1;
	std::map<int,ofxKinect*>::iterator iter;
	for(iter = kinects.begin(); iter != kinects.end(); ++iter) {
		if(iter->second == &kinect) {
			id = iter->first;
			break;
		}
	}
	if(id == -1)
		return;

	// remove connected device and close
	iter = kinects.find(id);
	if(iter != kinects.end()) {
		kinects.erase(iter);
		freenect_close_device(kinect.kinectDevice);
	}
}

void ofxKinectContext::closeAll() {
	// make copy of map to avoid invalidating iter when calling close()
	std::map<int,ofxKinect*> kinectsCopy(kinects);
    std::map<int,ofxKinect*>::iterator iter;
    for(iter = kinectsCopy.begin(); iter != kinectsCopy.end(); ++iter) {
        iter->second->close();
    }
}

//---------------------------------------------------------------------------
void ofxKinectContext::buildDeviceList() {
	
	deviceList.clear();
	
	// build the device list from freenect
	freenect_device_attributes * devAttrib; 
	int numDevices = freenect_list_device_attributes(kinectContext, &devAttrib);
	
	// save bus ids ...
	for(int i = 0; i < numDevices; i++){
		KinectPair kp;
		kp.id = i;
		kp.serial = (string) devAttrib->camera_serial; 
		deviceList.push_back(kp);
		devAttrib = devAttrib->next;
	}
	freenect_free_device_attributes(devAttrib);
	
	// sort devices by serial number
	sort(deviceList.begin(), deviceList.end(), sortKinectPairs);
}

void ofxKinectContext::listDevices(bool verbose) {
    if(!isInited())
		init();
	
	stringstream stream;
	
	if(numTotal() == 0) {
		stream << "no devices found";
		return;
	}
	else if(numTotal() == 1) {
		stream << 1 << " device found";
	}
	else {
		stream << deviceList.size() << " devices found";
	}
	
	if(verbose) {
		ofLogVerbose("ofxKinect") << stream.str();
	}
	else {
		ofLogNotice("ofxKinect") << stream.str();
	}
	stream.str("");
	
	for(unsigned int i = 0; i < deviceList.size(); ++i) {
		stream << "    id: " << deviceList[i].id << " serial: " << deviceList[i].serial;
		if(verbose) {
			ofLogVerbose("ofxKinect") << stream.str();
		}
		else {
			ofLogNotice("ofxKinect") << stream.str();
		}
		stream.str("");
	}
}

int ofxKinectContext::numTotal() {
    if(!isInited())
		init();
    return freenect_num_devices(kinectContext);
}

int ofxKinectContext::numAvailable() {
	if(!isInited())
		init();
    return freenect_num_devices(kinectContext) - kinects.size();
}

int ofxKinectContext::numConnected() {
	return kinects.size();
}

ofxKinect* ofxKinectContext::getKinect(freenect_device* dev) {
	std::map<int,ofxKinect*>::iterator iter;
	for(iter = kinects.begin(); iter != kinects.end(); ++iter) {
		if(iter->second->kinectDevice == dev)
			return iter->second;
	}
	return NULL;
}

int ofxKinectContext::getDeviceIndex(int id) {
	for(unsigned int i = 0; i < deviceList.size(); ++i) {
		if(deviceList[i].id == id)
			return i;
	}
	return -1;
}

int ofxKinectContext::getDeviceIndex(string serial) {
	for(unsigned int i = 0; i < deviceList.size(); ++i) {
		if(deviceList[i].serial == serial)
			return i;
	}
	return -1;
}

bool ofxKinectContext::isConnected(int id) {
	std::map<int,ofxKinect*>::iterator iter = kinects.find(id);
	return iter != kinects.end();
}

bool ofxKinectContext::isConnected(string serial) {
	std::map<int,ofxKinect*>::iterator iter;
	for(iter = kinects.begin(); iter != kinects.end(); ++iter) {
		if(iter->second->getSerial() == serial)
			return true;
	}
	return false;
}

int ofxKinectContext::nextAvailableId() {
	if(!isInited())
		init();
	
	// a brute force free index finder :D
	std::map<int,ofxKinect*>::iterator iter;
	for(unsigned int i = 0; i < deviceList.size(); ++i) {
		iter = kinects.find(deviceList[i].id);
		if(iter == kinects.end())
			return deviceList[i].id;
	}
	return -1;
}

string ofxKinectContext::nextAvailableSerial() {
	if(!isInited())
		init();
	
	int id = nextAvailableId();
	if(id == -1) {
		return "";
	}
	return deviceList[getDeviceIndex(id)].serial;
}

