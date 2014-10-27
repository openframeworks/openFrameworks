#include "ofVideoGrabber.h"
#include "ofUtils.h"
#include "ofBaseTypes.h"
#include "ofConstants.h"
#include "ofAppRunner.h"

#ifdef TARGET_ANDROID
	extern bool ofxAndroidInitGrabber(ofVideoGrabber * grabber);
	extern bool ofxAndroidCloseGrabber(ofVideoGrabber * grabber);
#endif

//--------------------------------------------------------------------
ofVideoGrabber::ofVideoGrabber(){
	bUseTexture			= false;
	requestedDeviceID	= -1;
	internalPixelFormat = OF_PIXELS_RGB;
	desiredFramerate 	= -1;
	height				= 0;
	width				= 0;
	tex.resize(1);

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
	if(!grabber){
		setGrabber( shared_ptr<OF_VID_GRABBER_TYPE>(new OF_VID_GRABBER_TYPE) );
	}
	return grabber;
}

const shared_ptr<ofBaseVideoGrabber> ofVideoGrabber::getGrabber() const{
	return grabber;
}

//--------------------------------------------------------------------
bool ofVideoGrabber::setup(int w, int h, bool setUseTexture){

#ifndef OF_VID_GRABBER_TYPE
#error OF_VID_GRABBER_TYPE is not #defined!
#endif

	if(!grabber){
		setGrabber( shared_ptr<OF_VID_GRABBER_TYPE>(new OF_VID_GRABBER_TYPE) );
	}

	bUseTexture = setUseTexture;

	if( requestedDeviceID >= 0 ){
		grabber->setDeviceID(requestedDeviceID);
	}

	setPixelFormat(internalPixelFormat); //this safely handles checks for supported format

	if( desiredFramerate!=-1 ){
		grabber->setDesiredFrameRate(desiredFramerate);
	}

	grabber->setup(w, h);
	width			= (int)grabber->getWidth();
	height			= (int)grabber->getHeight();

	if( grabber->isInitialized() && bUseTexture ){
		if(!grabber->getTexturePtr()){
			for(int i=0;i<grabber->getPixels().getNumPlanes();i++){
				ofPixels plane = grabber->getPixels().getPlane(i);
				tex.push_back(ofTexture());
				tex[i].allocate(plane);
				if(ofIsGLProgrammableRenderer() && plane.getPixelFormat() == OF_PIXELS_GRAY){
					tex[i].setRGToRGBASwizzles(true);
				}
			}
		}
	}

	return grabber->isInitialized();
}

//--------------------------------------------------------------------
bool ofVideoGrabber::initGrabber(int w, int h, bool setUseTexture){
	return setup(w,h,setUseTexture);
}

//--------------------------------------------------------------------
bool ofVideoGrabber::setPixelFormat(ofPixelFormat pixelFormat) {
	if(grabber){
		if( grabber->isInitialized() ){
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
ofPixelFormat ofVideoGrabber::getPixelFormat() const{
	if(grabber){
		internalPixelFormat = grabber->getPixelFormat();
	}
	return internalPixelFormat;
}

//--------------------------------------------------------------------
vector<ofVideoDevice> ofVideoGrabber::listDevices() const{
	if(!grabber){
		ofVideoGrabber * mutThis = const_cast<ofVideoGrabber*>(this);
		mutThis->setGrabber( shared_ptr<OF_VID_GRABBER_TYPE>(new OF_VID_GRABBER_TYPE) );
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
	requestedDeviceID = _deviceID;
	if( grabber && grabber->isInitialized() ){
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
ofPixels & ofVideoGrabber::getPixels(){
	return getGrabber()->getPixels();
}

//---------------------------------------------------------------------------
const ofPixels & ofVideoGrabber::getPixels() const{
	return getGrabber()->getPixels();
}

//---------------------------------------------------------------------------
ofPixels& ofVideoGrabber::getPixelsRef(){
	return getGrabber()->getPixels();
}

//---------------------------------------------------------------------------
const ofPixels& ofVideoGrabber::getPixelsRef() const{
	return getGrabber()->getPixels();
}

//------------------------------------
ofTexture & ofVideoGrabber::getTexture(){
	if(grabber->getTexturePtr() == NULL){
		return tex[0];
	}
	else{
		return *grabber->getTexturePtr();
	}
}

//------------------------------------
const ofTexture & ofVideoGrabber::getTexture() const{
	if(grabber->getTexturePtr() == NULL){
		return tex[0];
	}
	else{
		return *grabber->getTexturePtr();
	}
}

//------------------------------------
ofTexture & ofVideoGrabber::getTextureReference(){
	return getTexture();
}

//------------------------------------
const ofTexture & ofVideoGrabber::getTextureReference() const{
	return getTexture();
}

//------------------------------------
vector<ofTexture> & ofVideoGrabber::getTexturePlanes(){
	if(grabber->getTexturePtr() != NULL){
		tex.clear();
		tex.push_back(*grabber->getTexturePtr());
	}
	return tex;
}

//------------------------------------
const vector<ofTexture> & ofVideoGrabber::getTexturePlanes() const{
	if(grabber->getTexturePtr() != NULL){
		ofVideoGrabber* mutThis = const_cast<ofVideoGrabber*>(this);
		mutThis->tex.clear();
		mutThis->tex.push_back(*grabber->getTexturePtr());
	}
	return tex;
}

//---------------------------------------------------------------------------
bool  ofVideoGrabber::isFrameNew() const{
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
		if( bUseTexture && !grabber->getTexturePtr() && grabber->isFrameNew() ){
			if(int(tex.size())!=grabber->getPixels().getNumPlanes()){
				tex.resize(grabber->getPixels().getNumPlanes());
			}
			for(int i=0;i<grabber->getPixels().getNumPlanes();i++){
				ofPixels plane = grabber->getPixels().getPlane(i);
				bool bDiffPixFormat = ( tex[i].isAllocated() && tex[i].texData.glTypeInternal != ofGetGLInternalFormatFromPixelFormat(plane.getPixelFormat()) );
				if(width==0 || height==0 || bDiffPixFormat || !tex[i].isAllocated() ){
					tex[i].allocate(plane);
					if(ofIsGLProgrammableRenderer() && plane.getPixelFormat() == OF_PIXELS_GRAY){
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
	}
	if(!grabber->getTexturePtr()) tex.clear();
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
bool ofVideoGrabber::isUsingTexture() const{
	return bUseTexture;
}


//----------------------------------------------------------
void ofVideoGrabber::setAnchorPercent(float xPct, float yPct){
	getTexture().setAnchorPercent(xPct, yPct);
}

//----------------------------------------------------------
void ofVideoGrabber::setAnchorPoint(float x, float y){
	getTexture().setAnchorPoint(x, y);
}

//----------------------------------------------------------
void ofVideoGrabber::resetAnchor(){
	getTexture().resetAnchor();
}

//------------------------------------
void ofVideoGrabber::draw(float _x, float _y, float _w, float _h) const{
	ofGetCurrentRenderer()->draw(*this,_x,_y,_w,_h);
}

//------------------------------------
void ofVideoGrabber::draw(float _x, float _y) const{
	draw(_x, _y,width,height);
}


//------------------------------------
void ofVideoGrabber::bind() const{
	shared_ptr<ofBaseGLRenderer> renderer = ofGetGLRenderer();
	if(renderer){
		renderer->bind(*this);
	}
}

//------------------------------------
void ofVideoGrabber::unbind() const{
	shared_ptr<ofBaseGLRenderer> renderer = ofGetGLRenderer();
	if(renderer){
		renderer->unbind(*this);
	}
}

//----------------------------------------------------------
float ofVideoGrabber::getHeight() const{
	if(grabber){
		height = grabber->getHeight();
	}
	return (float)height;
}

//----------------------------------------------------------
float ofVideoGrabber::getWidth() const{
	if(grabber){
		width = grabber->getWidth();
	}
	return (float)width;
}

//----------------------------------------------------------
bool ofVideoGrabber::isInitialized() const{
	return grabber->isInitialized() && (!bUseTexture || tex[0].isAllocated() || grabber->getTexturePtr());
}
