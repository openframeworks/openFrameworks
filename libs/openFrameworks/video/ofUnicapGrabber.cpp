#include "ofUnicapGrabber.h"
#include "ofUtils.h"

//--------------------------------------------------------------------
ofUnicapGrabber::ofUnicapGrabber(){
	// common
	bIsFrameNew				= false;
	bVerbose 				= false;
	bGrabberInited 			= false;
	bChooseDevice			= false;
	deviceID				= 0;
	pixels					= NULL;	
	width 					= 320;	// default setting
	height 					= 240;	// default setting
	attemptFramerate		= -1;
}


//--------------------------------------------------------------------
ofUnicapGrabber::~ofUnicapGrabber(){
	close();
	
	//---------------------------------
	#ifdef OF_VIDEO_CAPTURE_UNICAP
	//--------------------------------
		if(bGrabberInited)
			ucGrabber.close_unicap();

	//---------------------------------
	#endif
	//---------------------------------	
}

//--------------------------------------------------------------------
bool ofUnicapGrabber::initGrabber(int w, int h){

	//---------------------------------
	#ifdef OF_VIDEO_CAPTURE_UNICAP
	//--------------------------------
		if( !bGrabberInited ){
			if ( !bChooseDevice ){
				deviceID = 0;
			}

			width 	= w;
			height 	= h;
			
			clearMemory();
			pixels	= new unsigned char[width * height * 3];

			if (bUseTexture){
				// create the texture, set the pixels to black and
				// upload them to the texture (so at least we see nothing black the callback)
				tex.allocate(width,height,GL_RGB);
				memset(pixels, 0, width*height*3);
				tex.loadData(pixels, width, height, GL_RGB);
			}

			bGrabberInited = ucGrabber.open_device (deviceID);
			if( bGrabberInited ){
			ofLog(OF_LOG_NOTICE, "choosing device %i: %s", deviceID,ucGrabber.device_identifier());
				ucGrabber.set_format(w,h);
				ucGrabber.start_capture();
			}

		}
			return bGrabberInited;
	//---------------------------------
	#endif
	//---------------------------------	

}

//--------------------------------------------------------------------
void ofUnicapGrabber::listDevices(){
	
	//---------------------------------
	#ifdef OF_VIDEO_CAPTURE_UNICAP
	//--------------------------------

		ucGrabber.listUCDevices();

	//---------------------------------
	#endif
	//---------------------------------

}

//--------------------------------------------------------------------
void ofUnicapGrabber::setVerbose(bool bTalkToMe){
	bVerbose = bTalkToMe;

	//---------------------------------
	#ifdef OF_VIDEO_CAPTURE_UNICAP
	//--------------------------------

		ucGrabber.verbose=bVerbose;

	//---------------------------------
	#endif
	//---------------------------------
}


//--------------------------------------------------------------------
void ofUnicapGrabber::grabFrame(){

//---------------------------------
	#ifdef OF_VIDEO_CAPTURE_UNICAP
	//--------------------------------

		if (bGrabberInited){
			bIsFrameNew = ucGrabber.getFrameUC(&pixels);
			if(bIsFrameNew) {
				if (bUseTexture){
					tex.loadData(pixels, width, height, GL_RGB);
				}
			}
		}

	//---------------------------------
	#endif
	//---------------------------------

}

//--------------------------------------------------------------------
void ofUnicapGrabber::close(){

	//---------------------------------
	#ifdef OF_VIDEO_CAPTURE_UNICAP
	//--------------------------------

		if(bGrabberInited){
			ucGrabber.close_unicap();
			bGrabberInited 		= false;
			deviceID 			= 0;
			bIsFrameNew 		= false;
			bChooseDevice 		= false;
		}

	//---------------------------------
	#endif
	//---------------------------------
	
	clearMemory();
	
}

//---------------------------------------------------------------------------
unsigned char * ofUnicapGrabber::getPixels(){
	return pixels;
}

//--------------------------------------------------------------------
float ofUnicapGrabber::getWidth(){
	return width;
}	

//--------------------------------------------------------------------
float ofUnicapGrabber::getHeight(){
	return height;
}

//--------------------------------------------------------------------
void ofUnicapGrabber::clearMemory(){
	if (pixels != NULL){
		delete[] pixels;
		pixels = NULL;
	}
}

//---------------------------------------------------------------------------
bool  ofUnicapGrabber::isFrameNew(){
	return bIsFrameNew;
}

//--------------------------------------------------------------------
void ofUnicapGrabber::setDeviceID(int _deviceID){
	deviceID		= _deviceID;
	bChooseDevice	= true;
}

//--------------------------------------------------------------------
void ofUnicapGrabber::setDesiredFrameRate(int framerate){
	attemptFramerate = framerate;
}

//--------------------------------------------------------------------
void ofUnicapGrabber::videoSettings(void){
	
	//---------------------------------
	#ifdef OF_VIDEO_CAPTURE_UNICAP
	//--------------------------------

		ucGrabber.queryUC_imageProperties();

	//---------------------------------
	#endif
	//---------------------------------
}
