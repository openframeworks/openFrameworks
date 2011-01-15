#include "ofDirectShowGrabber.h"
#include "ofUtils.h"

//--------------------------------------------------------------------
ofDirectShowGrabber::ofDirectShowGrabber(){

	//---------------------------------
	#ifdef OF_VIDEO_CAPTURE_DIRECTSHOW
	//---------------------------------

		bVerbose 			= false;
		bDoWeNeedToResize 	= false;

	//---------------------------------
	#endif
	//---------------------------------

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
ofDirectShowGrabber::~ofDirectShowGrabber(){
	close();
}


//--------------------------------------------------------------------
bool ofDirectShowGrabber::initGrabber(int w, int h){
	
	//---------------------------------
	#ifdef OF_VIDEO_CAPTURE_DIRECTSHOW
	//---------------------------------

		if (bChooseDevice){
			device = deviceID;
			ofLog(OF_LOG_NOTICE, "choosing %i", deviceID);
		} else {
			device = 0;
		}

		width = w;
		height = h;
		bGrabberInited = false;
		
		if( attemptFramerate >= 0){
			VI.setIdealFramerate(device, attemptFramerate);
		}
		bool bOk = VI.setupDevice(device, width, height);

		int ourRequestedWidth = width;
		int ourRequestedHeight = height;

		if (bOk == true){
			bGrabberInited = true;
			width 	= VI.getWidth(device);
			height 	= VI.getHeight(device);

			if (width == ourRequestedWidth && height == ourRequestedHeight){
				bDoWeNeedToResize = false;
			} else {
				bDoWeNeedToResize = true;
				width = ourRequestedWidth;
				height = ourRequestedHeight;
			}


			pixels.allocate(width, height, 24);
			return true;
		} else {
			ofLog(OF_LOG_ERROR, "error allocating a video device");
			ofLog(OF_LOG_ERROR, "please check your camera with AMCAP or other software");
			bGrabberInited = false;
			return false;
		}

	//---------------------------------
	#endif
	//---------------------------------

}

//--------------------------------------------------------------------
void ofDirectShowGrabber::listDevices(){

	//---------------------------------
	#ifdef OF_VIDEO_CAPTURE_DIRECTSHOW
	//---------------------------------
		ofLog(OF_LOG_NOTICE, "---");
		VI.listDevices();
		ofLog(OF_LOG_NOTICE, "---");

	//---------------------------------
	#endif
	//---------------------------------

}

//--------------------------------------------------------------------
void ofDirectShowGrabber::update(){

	//---------------------------------
	#ifdef OF_VIDEO_CAPTURE_DIRECTSHOW
	//---------------------------------

		if (bGrabberInited == true){
			bIsFrameNew = false;
			if (VI.isFrameNew(device)){

				bIsFrameNew = true;


				/*
				 	rescale --
				 	currently this is nearest neighbor scaling
				 	not the greatest, but fast
				 	this can be optimized too
				 	with pointers, etc

				 	better --
				 	make sure that you ask for a "good" size....

				*/

				unsigned char * viPixels = VI.getPixels(device, true, true);


				if (bDoWeNeedToResize == true){

					int inputW = VI.getWidth(device);
					int inputH = VI.getHeight(device);

					float scaleW =	(float)inputW / (float)width;
					float scaleH =	(float)inputH / (float)height;

					for(int i=0;i<width;i++){
						for(int j=0;j<height;j++){

							float posx = i * scaleW;
							float posy = j * scaleH;

							/*

							// start of calculating
							// for linear interpolation

							int xbase = (int)floor(posx);
							int xhigh = (int)ceil(posx);
							float pctx = (posx - xbase);

							int ybase = (int)floor(posy);
							int yhigh = (int)ceil(posy);
							float pcty = (posy - ybase);
							*/

							int posPix = (((int)posy * inputW * 3) + ((int)posx * 3));

							pixels.getPixels()[(j*width*3) + i*3    ] = viPixels[posPix  ];
							pixels.getPixels()[(j*width*3) + i*3 + 1] = viPixels[posPix+1];
							pixels.getPixels()[(j*width*3) + i*3 + 2] = viPixels[posPix+2];

						}
					}

				} else {

					pixels.setFromPixels(viPixels,width,height,OF_IMAGE_COLOR);

				}

				
			}
		}

	//---------------------------------
	#endif
	//---------------------------------

}

//--------------------------------------------------------------------
void ofDirectShowGrabber::close(){

	//---------------------------------
	#ifdef OF_VIDEO_CAPTURE_DIRECTSHOW
	//---------------------------------

		if (bGrabberInited == true){
			VI.stopDevice(device);
			bGrabberInited = false;
		}

	//---------------------------------
	#endif
	//---------------------------------

	clearMemory();
	
}


//--------------------------------------------------------------------
void ofDirectShowGrabber::clearMemory(){
	pixels.clear();
}

//---------------------------------------------------------------------------
unsigned char * ofDirectShowGrabber::getPixels(){
	return pixels.getPixels();
}
//--------------------------------------------------------------------
ofPixels ofDirectShowGrabber::getOFPixels(){
	return pixels;
}

//--------------------------------------------------------------------
ofPixels ofDirectShowGrabber::getOFPixels() const{
	return pixels;
}

//--------------------------------------------------------------------
float ofDirectShowGrabber::getWidth(){
	return width;
}	

//--------------------------------------------------------------------
float ofDirectShowGrabber::getHeight(){
	return height;
}

//---------------------------------------------------------------------------
bool  ofDirectShowGrabber::isFrameNew(){
	return bIsFrameNew;
}

//--------------------------------------------------------------------
void ofDirectShowGrabber::setVerbose(bool bTalkToMe){
	bVerbose = bTalkToMe;
}

//--------------------------------------------------------------------
void ofDirectShowGrabber::setDeviceID(int _deviceID){
	deviceID		= _deviceID;
	bChooseDevice	= true;
}

//--------------------------------------------------------------------
void ofDirectShowGrabber::setDesiredFrameRate(int framerate){
	attemptFramerate = framerate;
}


//--------------------------------------------------------------------
void ofDirectShowGrabber::videoSettings(void){

	//---------------------------------
	#ifdef OF_VIDEO_CAPTURE_DIRECTSHOW
	//---------------------------------

		if (bGrabberInited == true) VI.showSettingsWindow(device);

	//---------------------------------
	#endif
	//---------------------------------
}
