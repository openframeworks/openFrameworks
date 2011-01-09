#include "ofGStreamerGrabber.h"
#include "ofUtils.h"

//--------------------------------------------------------------------
ofGStreamerGrabber::ofGStreamerGrabber(){
	
	// common
	bIsFrameNew				= false;
	bVerbose 				= false;
	bGrabberInited 			= false;
	bChooseDevice			= false;
	deviceID				= 0;
	width 					= 320;	// default setting
	height 					= 240;	// default setting
	attemptFramerate		= -1;

}


//--------------------------------------------------------------------
ofGStreamerGrabber::~ofGStreamerGrabber(){
	close();
}

//--------------------------------------------------------------------
bool ofGStreamerGrabber::initGrabber(int w, int h){
	
	//---------------------------------
	#ifdef OF_VIDEO_CAPTURE_GSTREAMER
	//--------------------------------
		gstUtils.setDeviceID(deviceID);

		if(gstUtils.initGrabber(w,h,attemptFramerate)){
			width 	= w;
			height 	= h;
			bGrabberInited = true;
			ofLog(OF_LOG_VERBOSE, "ofVideoGrabber: initied");
		}else{
			bGrabberInited = false;
			ofLog(OF_LOG_ERROR, "ofVideoGrabber: couldn't init");
		}
		return bGrabberInited;


	//---------------------------------
	#endif
	//---------------------------------
}

//--------------------------------------------------------------------
void ofGStreamerGrabber::listDevices(){

	//---------------------------------
	#ifdef OF_VIDEO_CAPTURE_GSTREAMER
	//--------------------------------
		gstUtils.listDevices();
	//---------------------------------
	#endif
	//---------------------------------

}

//--------------------------------------------------------------------
unsigned char * ofGStreamerGrabber::getPixels(){
	//---------------------------------
	#ifdef OF_VIDEO_CAPTURE_GSTREAMER
	//--------------------------------
		return gstUtils.getPixels();
	//---------------------------------
	#endif
	//---------------------------------
}

//--------------------------------------------------------------------
void ofGStreamerGrabber::grabFrame(){

	//---------------------------------
	#ifdef OF_VIDEO_CAPTURE_GSTREAMER
	//--------------------------------

		if (bGrabberInited){
			gstUtils.update();
			bIsFrameNew = gstUtils.isFrameNew();
		}

	//---------------------------------
	#endif
	//---------------------------------

}

//--------------------------------------------------------------------
void ofGStreamerGrabber::close(){

	//---------------------------------
	#ifdef OF_VIDEO_CAPTURE_GSTREAMER
	//--------------------------------

		gstUtils.close();

	//---------------------------------
	#endif
	//---------------------------------
	
	clearMemory();
	
}

//--------------------------------------------------------------------
float ofGStreamerGrabber::getWidth(){
	return width;
}	

//--------------------------------------------------------------------
float ofGStreamerGrabber::getHeight(){
	return height;
}

//--------------------------------------------------------------------
void ofGStreamerGrabber::clearMemory(){
	//nothing to be done?
}

//---------------------------------------------------------------------------
bool  ofGStreamerGrabber::isFrameNew(){
	return bIsFrameNew;
}

//--------------------------------------------------------------------
void ofGStreamerGrabber::setVerbose(bool bTalkToMe){
	bVerbose = bTalkToMe;
}

//--------------------------------------------------------------------
void ofGStreamerGrabber::setDeviceID(int _deviceID){
	deviceID		= _deviceID;
	bChooseDevice	= true;
}

//--------------------------------------------------------------------
void ofGStreamerGrabber::setDesiredFrameRate(int framerate){
	attemptFramerate = framerate;
}

