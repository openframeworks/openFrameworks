#include "ofVideoGrabber.h"
#include "ofUtils.h"

//TODO: allow for non rgb pixel formats to work with textures
//TODO: getImageBytes() 
//TODO: getBytesPerPixel()
//TODO: add ofPixels support which would take care of the above

//--------------------------------------------------------------------
ofVideoGrabber::ofVideoGrabber(){
	grabber				= NULL;
	bUseTexture			= false;
	bInitialized		= false;
	RequestedDeviceID	= -1;
	internalPixelFormat = OF_PIXELS_RGB;
	
	if( grabber == NULL ){
		setGrabber( new OF_VID_GRABBER_TYPE );
	}
}

//--------------------------------------------------------------------
ofVideoGrabber::~ofVideoGrabber(){
	close();

	if(	grabber != NULL ){
		delete grabber;
		grabber = NULL;
	}	
		
	tex.clear();
}

//--------------------------------------------------------------------
bool ofVideoGrabber::setGrabber(ofBaseVideoGrabber * newGrabber){
	if( grabber == NULL ){
		grabber = newGrabber;
		return true;
	}else{
		//TODO: should we delete grabberPtr? This is why we need smart pointers. 
		ofLog(OF_LOG_ERROR, "ofVideoGrabber::setGrabber - grabber already set!");
	}
	return false;
}

//--------------------------------------------------------------------
ofBaseVideoGrabber * ofVideoGrabber::getGrabber(){
	return grabber;
}

//--------------------------------------------------------------------
bool ofVideoGrabber::initGrabber(int w, int h, bool setUseTexture){
	bInitialized = true;
	bUseTexture = setUseTexture;

	if( RequestedDeviceID >= 0 ){
		grabber->setDeviceID(RequestedDeviceID);
	}

	grabber->setPixelFormat(internalPixelFormat);
	
	bool bOk = grabber->initGrabber(w, h);
	width	 = grabber->getWidth();
	height	 = grabber->getHeight();	
	
	if( bOk && bUseTexture ){
		if(internalPixelFormat == OF_PIXELS_RGB)
			tex.allocate(width, height, GL_RGB);
		else if(internalPixelFormat == OF_PIXELS_RGBA)
			tex.allocate(width, height, GL_RGBA);
		else if(internalPixelFormat == OF_PIXELS_BGRA)
			tex.allocate(width, height, GL_RGBA); // for some reason if we allcoate as GL_BGRA we get a white texture
	}
	
	return bOk;	
}

//--------------------------------------------------------------------
void ofVideoGrabber::setPixelFormat(ofPixelFormat pixelFormat) {
	internalPixelFormat = pixelFormat;
}

//--------------------------------------------------------------------
void ofVideoGrabber::listDevices(){
	if(	grabber != NULL ){
		grabber->listDevices();
	}
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
		ofLog(OF_LOG_WARNING, "call setDeviceID before grabber is started!");
	}
}

//--------------------------------------------------------------------
void ofVideoGrabber::setDesiredFrameRate(int framerate){
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
			//note we should look at ways to do other pixel formats. 
			if(internalPixelFormat == OF_PIXELS_RGB)
				tex.loadData(grabber->getPixels(), tex.getWidth(), tex.getHeight(), GL_RGB);
			else if(internalPixelFormat == OF_PIXELS_RGBA)
				tex.loadData(grabber->getPixels(), tex.getWidth(), tex.getHeight(), GL_RGBA);
			else if(internalPixelFormat == OF_PIXELS_BGRA)
				tex.loadData(grabber->getPixels(), tex.getWidth(), tex.getHeight(), GL_BGRA);
			
		}
	}
}

//--------------------------------------------------------------------
void ofVideoGrabber::grabFrame(){
	update();
}

//--------------------------------------------------------------------
void ofVideoGrabber::close(){
	if(	grabber != NULL ){
		grabber->close();
		bInitialized=false;
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
void ofVideoGrabber::setAnchorPoint(int x, int y){
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
