#include "ofVideoGrabber.h"
#include "ofUtils.h"
#include "ofBaseTypes.h"
#include "ofConstants.h"

//TODO: allow for non rgb pixel formats to work with textures
//TODO: getImageBytes()
//TODO: getBytesPerPixel()
//TODO: add ofPixels support which would take care of the above

#ifdef TARGET_ANDROID
	extern bool ofxAndroidInitGrabber(ofVideoGrabber * grabber);
	extern bool ofxAndroidCloseGrabber(ofVideoGrabber * grabber);
#endif

//--------------------------------------------------------------------
ofVideoGrabber::ofVideoGrabber(){
	bUseTexture			= false;
	bInitialized		= false;
	grabberRunning		= false;
	RequestedDeviceID	= -1;
	internalPixelFormat = OF_PIXELS_RGB;
	desiredFramerate 	= -1;
	height				= 0;
	width				= 0;

#ifdef TARGET_ANDROID
	if(!ofxAndroidInitGrabber(this)) return;
#endif

}

//--------------------------------------------------------------------
ofVideoGrabber::~ofVideoGrabber(){
#ifdef TARGET_ANDROID
	ofxAndroidCloseGrabber(this);
#endif
}

//--------------------------------------------------------------------
void ofVideoGrabber::setGrabber(ofPtr<ofBaseVideoGrabber> newGrabber){
	grabber = newGrabber;
}

//--------------------------------------------------------------------
ofPtr<ofBaseVideoGrabber> ofVideoGrabber::getGrabber(){
	return grabber;
}

//--------------------------------------------------------------------
bool ofVideoGrabber::initGrabber(int w, int h, bool setUseTexture){

#ifndef OF_VID_GRABBER_TYPE
#error OF_VID_GRABBER_TYPE is not #defined! 
#endif
    
	if( grabber == NULL ){
		setGrabber( ofPtr<OF_VID_GRABBER_TYPE>(new OF_VID_GRABBER_TYPE) );
	}

	bInitialized = true;
	bUseTexture = setUseTexture;

	if( RequestedDeviceID >= 0 ){
		grabber->setDeviceID(RequestedDeviceID);
	}

	setPixelFormat(internalPixelFormat); //this safely handles checks for supported format

	if( desiredFramerate!=-1 ){
		grabber->setDesiredFrameRate(desiredFramerate);
	}

	grabberRunning	= grabber->initGrabber(w, h);
	width			= (int)grabber->getWidth();
	height			= (int)grabber->getHeight();

	if( grabberRunning && bUseTexture ){
		tex.allocate(width, height, ofGetGLInternalFormatFromPixelFormat(internalPixelFormat));
		if(ofGetGLProgrammableRenderer() && internalPixelFormat == OF_PIXELS_MONO){
			tex.setRGToRGBASwizzles(true);
		}
	}

	return grabberRunning;
}

//--------------------------------------------------------------------
bool ofVideoGrabber::setPixelFormat(ofPixelFormat pixelFormat) {
	if( grabber != NULL ){
		if( grabberRunning ){
			ofLogWarning("ofVideoGrabber") << "setPixelFormat(): can't set pixel format while grabber is running";
			internalPixelFormat = grabber->getPixelFormat(); 
			return false;
		}else{
			if( grabber->setPixelFormat(pixelFormat) ){		
				internalPixelFormat = grabber->getPixelFormat();  //we do this as either way we want the grabbers format
			}else{
				internalPixelFormat = grabber->getPixelFormat();  //we do this as either way we want the grabbers format
				return false; 					
			}
		}
	}else{
		internalPixelFormat = pixelFormat;	
	}
	return true;
}

//---------------------------------------------------------------------------
ofPixelFormat ofVideoGrabber::getPixelFormat(){
	if( grabber != NULL ){
		internalPixelFormat = grabber->getPixelFormat();
	}
	return internalPixelFormat;
}

//--------------------------------------------------------------------
vector<ofVideoDevice> ofVideoGrabber::listDevices(){
	if( grabber == NULL ){
		setGrabber( ofPtr<OF_VID_GRABBER_TYPE>(new OF_VID_GRABBER_TYPE) );
	}
	return grabber->listDevices();
}

//--------------------------------------------------------------------
void ofVideoGrabber::setVerbose(bool bTalkToMe){
	if(	grabber != NULL ){
		grabber->setVerbose(bTalkToMe);
	}
}

//--------------------------------------------------------------------
void ofVideoGrabber::setDeviceID(int _deviceID){
	RequestedDeviceID = _deviceID;
	if( bInitialized ){
		ofLogWarning("ofxVideoGrabber") << "setDeviceID(): can't set device while grabber is running";
	}
}

//--------------------------------------------------------------------
void ofVideoGrabber::setDesiredFrameRate(int framerate){
	desiredFramerate = framerate;
	if(	grabber != NULL ){
		grabber->setDesiredFrameRate(framerate);
	}
}

//---------------------------------------------------------------------------
unsigned char * ofVideoGrabber::getPixels(){
	if(	grabber != NULL ){
		return grabber->getPixels();
	}
	return NULL;
}

//---------------------------------------------------------------------------
ofPixelsRef ofVideoGrabber::getPixelsRef(){
	return grabber->getPixelsRef();
}

//------------------------------------
//for getting a reference to the texture
ofTexture & ofVideoGrabber::getTextureReference(){
	return tex;
}

//---------------------------------------------------------------------------
bool  ofVideoGrabber::isFrameNew(){
	if(	grabber != NULL ){
		return grabber->isFrameNew();
	}
	return false;
}

//--------------------------------------------------------------------
void ofVideoGrabber::update(){
	if(	grabber != NULL ){
		grabber->update();
		if( bUseTexture && grabber->isFrameNew() ){
			tex.loadData(grabber->getPixels(), (int)tex.getWidth(), (int)tex.getHeight(), ofGetGLTypeFromPixelFormat(internalPixelFormat));
		}
	}
}

//--------------------------------------------------------------------
void ofVideoGrabber::close(){
	if(	grabber != NULL ){
		grabber->close();
		bInitialized=false;
		grabberRunning = false;
	}
	tex.clear();
}

//--------------------------------------------------------------------
void ofVideoGrabber::videoSettings(void){
	if(	grabber != NULL ){
		grabber->videoSettings();
	}
}

//------------------------------------
void ofVideoGrabber::setUseTexture(bool bUse){
	bUseTexture = bUse;
}


//----------------------------------------------------------
void ofVideoGrabber::setAnchorPercent(float xPct, float yPct){
	tex.setAnchorPercent(xPct, yPct);
}

//----------------------------------------------------------
void ofVideoGrabber::setAnchorPoint(float x, float y){
	tex.setAnchorPoint(x, y);
}

//----------------------------------------------------------
void ofVideoGrabber::resetAnchor(){
	tex.resetAnchor();
}

//------------------------------------
void ofVideoGrabber::draw(float _x, float _y, float _w, float _h){
	tex.draw(_x, _y, _w, _h);
}

//------------------------------------
void ofVideoGrabber::draw(float _x, float _y){
	tex.draw(_x, _y);
}

//----------------------------------------------------------
float ofVideoGrabber::getHeight(){
	if(	grabber != NULL ){
		height = grabber->getHeight();
	}
	return (float)height;
}

//----------------------------------------------------------
float ofVideoGrabber::getWidth(){
	if(	grabber != NULL ){
		width = grabber->getWidth();
	}
	return (float)width;
}

//----------------------------------------------------------
bool ofVideoGrabber::isInitialized(){
	return bInitialized;
}
