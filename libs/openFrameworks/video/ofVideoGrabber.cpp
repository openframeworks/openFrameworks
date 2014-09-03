#include "ofVideoGrabber.h"
#include "ofUtils.h"
#include "ofBaseTypes.h"
#include "ofConstants.h"
#include "ofGraphics.h"

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
void ofVideoGrabber::setGrabber(shared_ptr<ofBaseVideoGrabber> newGrabber){
	grabber = newGrabber;
}

//--------------------------------------------------------------------
shared_ptr<ofBaseVideoGrabber> ofVideoGrabber::getGrabber(){
	return grabber;
}

//--------------------------------------------------------------------
bool ofVideoGrabber::initGrabber(int w, int h, bool setUseTexture){

#ifndef OF_VID_GRABBER_TYPE
#error OF_VID_GRABBER_TYPE is not #defined! 
#endif
    
	if(!grabber){
		setGrabber( shared_ptr<OF_VID_GRABBER_TYPE>(new OF_VID_GRABBER_TYPE) );
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
		if(!grabber->getTexture()){
			for(int i=0;i<grabber->getPixelsRef().getNumPlanes();i++){
				ofPixels plane = grabber->getPixelsRef().getPlane(i);
				tex.push_back(ofTexture());
				tex[i].allocate(plane);
				if(ofGetGLProgrammableRenderer() && plane.getPixelFormat() == OF_PIXELS_GRAY){
					tex[i].setRGToRGBASwizzles(true);
				}
			}
		}
	}

	return grabberRunning;
}

//--------------------------------------------------------------------
bool ofVideoGrabber::setPixelFormat(ofPixelFormat pixelFormat) {
	if(grabber){
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
	if(grabber){
		internalPixelFormat = grabber->getPixelFormat();
	}
	return internalPixelFormat;
}

//--------------------------------------------------------------------
vector<ofVideoDevice> ofVideoGrabber::listDevices(){
	if(!grabber){
		setGrabber( shared_ptr<OF_VID_GRABBER_TYPE>(new OF_VID_GRABBER_TYPE) );
	}
	return grabber->listDevices();
}

//--------------------------------------------------------------------
void ofVideoGrabber::setVerbose(bool bTalkToMe){
	if(grabber){
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
	if(grabber){
		grabber->setDesiredFrameRate(framerate);
	}
}

//---------------------------------------------------------------------------
unsigned char * ofVideoGrabber::getPixels(){
	if(grabber){
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
	if(grabber->getTexture() == NULL){
		return tex[0];
	}
	else{
		return grabber->getTexture()->at(0);
	}
}

vector<ofTexture> & ofVideoGrabber::getTexturePlanes(){
	if(grabber->getTexture() == NULL){
		return tex;
	}
	else{
		return *grabber->getTexture();
	}
}

//---------------------------------------------------------------------------
bool  ofVideoGrabber::isFrameNew(){
	if(grabber){
		return grabber->isFrameNew();
	}
	return false;
}

//--------------------------------------------------------------------
void ofVideoGrabber::update(){
	if(grabber){
		grabber->update();
		width = grabber->getWidth();
		height = grabber->getHeight();
		if( bUseTexture && !grabber->getTexture() && grabber->isFrameNew() ){
			if(int(tex.size())!=grabber->getPixelsRef().getNumPlanes()){
				tex.resize(grabber->getPixelsRef().getNumPlanes());
			}
			for(int i=0;i<grabber->getPixelsRef().getNumPlanes();i++){
				ofPixels plane = grabber->getPixelsRef().getPlane(i);
				bool bDiffPixFormat = ( tex[i].isAllocated() && tex[i].texData.glTypeInternal != ofGetGLInternalFormatFromPixelFormat(plane.getPixelFormat()) );
				if(width==0 || height==0 || bDiffPixFormat || !tex[i].isAllocated() ){
					tex[i].allocate(plane);
					if(ofGetGLProgrammableRenderer() && plane.getPixelFormat() == OF_PIXELS_GRAY){
						tex[i].setRGToRGBASwizzles(true);
					}
				}
				tex[i].loadData(plane);
			}
		}
	}
}

//--------------------------------------------------------------------
void ofVideoGrabber::close(){
	if(grabber){
		grabber->close();
		bInitialized=false;
		grabberRunning = false;
	}
	if(!grabber->getTexture()) tex.clear();
}

//--------------------------------------------------------------------
void ofVideoGrabber::videoSettings(void){
	if(grabber){
		grabber->videoSettings();
	}
}

//------------------------------------
void ofVideoGrabber::setUseTexture(bool bUse){
	bUseTexture = bUse;
}

//------------------------------------
bool ofVideoGrabber::isUsingTexture(){
	return bUseTexture;
}


//----------------------------------------------------------
void ofVideoGrabber::setAnchorPercent(float xPct, float yPct){
	getTextureReference().setAnchorPercent(xPct, yPct);
}

//----------------------------------------------------------
void ofVideoGrabber::setAnchorPoint(float x, float y){
	getTextureReference().setAnchorPoint(x, y);
}

//----------------------------------------------------------
void ofVideoGrabber::resetAnchor(){
	getTextureReference().resetAnchor();
}

//------------------------------------
void ofVideoGrabber::draw(float _x, float _y, float _w, float _h){
	ofGetCurrentRenderer()->draw(*this,_x,_y,_w,_h);
}

//------------------------------------
void ofVideoGrabber::draw(float _x, float _y){
	draw(_x, _y,width,height);
}


//------------------------------------
void ofVideoGrabber::bind(){
	ofGetCurrentRenderer()->bind(*this);
}

//------------------------------------
void ofVideoGrabber::unbind(){
	ofGetCurrentRenderer()->unbind(*this);
}

//----------------------------------------------------------
float ofVideoGrabber::getHeight(){
	if(grabber){
		height = grabber->getHeight();
	}
	return (float)height;
}

//----------------------------------------------------------
float ofVideoGrabber::getWidth(){
	if(grabber){
		width = grabber->getWidth();
	}
	return (float)width;
}

//----------------------------------------------------------
bool ofVideoGrabber::isInitialized(){
	return bInitialized;
}
