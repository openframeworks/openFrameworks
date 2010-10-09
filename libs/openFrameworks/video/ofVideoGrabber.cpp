#include "ofVideoGrabber.h"
#include "ofUtils.h"


//---------------------------------
#ifdef OF_VIDEO_CAPTURE_QUICKTIME
//---------------------------------
//--------------------------------------------------------------
static ComponentResult 	frameIsGrabbedProc(SGChannel sgChan, short nBufferNum, Boolean *pbDone, long lRefCon);
static ComponentResult  frameIsGrabbedProc(SGChannel sgChan, short nBufferNum, Boolean *pbDone, long lRefCon){

 	ComponentResult err = SGGrabFrameComplete( sgChan, nBufferNum, pbDone );
	
	bool * havePixChanged = (bool *)lRefCon;
	*havePixChanged = true;

	return err;
}
//---------------------------------
#endif
//---------------------------------


//--------------------------------------------------------------------
ofVideoGrabber::ofVideoGrabber(){

	//---------------------------------
	#ifdef OF_VIDEO_CAPTURE_QUICKTIME
	//---------------------------------

		initializeQuicktime();
		bSgInited				= false;
		pixels					= NULL;
		gSeqGrabber				= NULL;
		offscreenGWorldPixels	= NULL;

	//---------------------------------
	#endif
	//---------------------------------


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
	bUseTexture				= true;
	bChooseDevice			= false;
	deviceID				= 0;
	width 					= 320;	// default setting
	height 					= 240;	// default setting
	pixels					= NULL;
	attemptFramerate		= -1;
}


//--------------------------------------------------------------------
ofVideoGrabber::~ofVideoGrabber(){

	close();

	//---------------------------------
	#ifdef OF_VIDEO_CAPTURE_QUICKTIME
	//---------------------------------
		if (offscreenGWorldPixels != NULL){
			delete[] offscreenGWorldPixels;
		}
	//---------------------------------
	#endif
	//---------------------------------
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
void ofVideoGrabber::listDevices(){

	//---------------------------------
	#ifdef OF_VIDEO_CAPTURE_QUICKTIME
	//---------------------------------

		bool bNeedToInitGrabberFirst = false;

		if (!bSgInited) bNeedToInitGrabberFirst = true;

		//if we need to initialize the grabbing component then do it
		if( bNeedToInitGrabberFirst ){
			if( !qtInitSeqGrabber() ){
				return;
			}
		}

		printf("-------------------------------------\n");

		/*
			//input selection stuff (ie multiple webcams)
			//from http://developer.apple.com/samplecode/SGDevices/listing13.html
			//and originally http://lists.apple.com/archives/QuickTime-API/2008/Jan/msg00178.html
		*/

		SGDeviceList deviceList;
		SGGetChannelDeviceList (gVideoChannel, sgDeviceListIncludeInputs, &deviceList);
		unsigned char pascalName[64];
		unsigned char pascalNameInput[64];

		//this is our new way of enumerating devices
		//quicktime can have multiple capture 'inputs' on the same capture 'device'
		//ie the USB Video Class Video 'device' - can have multiple usb webcams attached on what QT calls 'inputs'
		//The isight for example will show up as:
		//USB Video Class Video - Built-in iSight ('input' 1 of the USB Video Class Video 'device')
		//Where as another webcam also plugged whill show up as
		//USB Video Class Video - Philips SPC 1000NC Webcam ('input' 2 of the USB Video Class Video 'device')

		//this means our the device ID we use for selection has to count both capture 'devices' and their 'inputs'
		//this needs to be the same in our init grabber method so that we select the device we ask for
		int deviceCount = 0;

		ofLog(OF_LOG_NOTICE, "listing available capture devices");
		for(int i = 0 ; i < (*deviceList)->count ; ++i)
		{
			SGDeviceName nameRec;
			nameRec = (*deviceList)->entry[i];
			SGDeviceInputList deviceInputList = nameRec.inputs;

			int numInputs = 0;
			if( deviceInputList ) numInputs = ((*deviceInputList)->count);

			memcpy(pascalName, (*deviceList)->entry[i].name, sizeof(char) * 64);

			//this means we can use the capture method
			if(nameRec.flags != sgDeviceNameFlagDeviceUnavailable){

				//if we have a capture 'device' (qt's word not mine - I would prefer 'system' ) that is ready to be used
				//we go through its inputs to list all physical devices - as there could be more than one!
				for(int j = 0; j < numInputs; j++){


					//if our 'device' has inputs we get their names here
					if( deviceInputList ){
						SGDeviceInputName inputNameRec  = (*deviceInputList)->entry[j];
						memcpy(pascalNameInput, inputNameRec.name, sizeof(char) * 64);
					}

					printf( "device[%i] %s - %s\n",  deviceCount, p2cstr(pascalName), p2cstr(pascalNameInput) );

					//we count this way as we need to be able to distinguish multiple inputs as devices
					deviceCount++;
				}

			}else{
				printf( "(unavailable) device[%i] %s\n",  deviceCount, p2cstr(pascalName) );
				deviceCount++;
			}
		}
		printf( "-------------------------------------\n");

		//if we initialized the grabbing component then close it
		if( bNeedToInitGrabberFirst ){
			qtCloseSeqGrabber();
		}

	//---------------------------------
	#endif
	//---------------------------------


	//---------------------------------
	#ifdef OF_VIDEO_CAPTURE_DIRECTSHOW
	//---------------------------------
		ofLog(OF_LOG_NOTICE, "---");
		VI.listDevices();
		ofLog(OF_LOG_NOTICE, "---");

	//---------------------------------
	#endif
	//---------------------------------


	//---------------------------------
	#ifdef OF_VIDEO_CAPTURE_UNICAP
	//--------------------------------


		ucGrabber.listUCDevices();

	//---------------------------------
	#endif
	//---------------------------------

	//---------------------------------
	#ifdef OF_VIDEO_CAPTURE_GSTREAMER
	//--------------------------------


		gstUtils.listDevices();

	//---------------------------------
	#endif
	//---------------------------------

}

//--------------------------------------------------------------------
void ofVideoGrabber::setVerbose(bool bTalkToMe){
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
void ofVideoGrabber::setDeviceID(int _deviceID){
	deviceID		= _deviceID;
	bChooseDevice	= true;
}

//--------------------------------------------------------------------
void ofVideoGrabber::setDesiredFrameRate(int framerate){
	attemptFramerate = framerate;
}

//---------------------------------------------------------------------------
unsigned char * ofVideoGrabber::getPixels(){

	//---------------------------------
	#ifdef OF_VIDEO_CAPTURE_GSTREAMER
	//--------------------------------
		return gstUtils.getPixels();

	//---------------------------------
	#else
	//---------------------------------
		return pixels;
	//---------------------------------
	#endif
	//---------------------------------
}

//------------------------------------
//for getting a reference to the texture
ofTexture & ofVideoGrabber::getTextureReference(){
	if(!tex.bAllocated() ){
		ofLog(OF_LOG_WARNING, "ofVideoGrabber - getTextureReference - texture is not allocated");
	}
	return tex;
}

//---------------------------------------------------------------------------
bool  ofVideoGrabber::isFrameNew(){
	return bIsFrameNew;
}

//--------------------------------------------------------------------
void ofVideoGrabber::update(){
	grabFrame();
}

//--------------------------------------------------------------------
void ofVideoGrabber::grabFrame(){

	//---------------------------------
	#ifdef OF_VIDEO_CAPTURE_QUICKTIME
	//---------------------------------

		if (bGrabberInited == true){
			SGIdle(gSeqGrabber);
			// set the top pixel alpha = 0, so we can know if it
			// was a new frame or not..
			// or else we will process way more than necessary
			// (ie opengl is running at 60fps +, capture at 30fps)
			if (bHavePixelsChanged){
				
				#if defined(TARGET_OSX) && defined(__BIG_ENDIAN__)
					convertPixels(offscreenGWorldPixels, pixels, width, height);
				#endif
				
				if (bUseTexture){
					tex.loadData(pixels, width, height, GL_RGB);
				}
			}
		}

		// newness test for quicktime:
		if (bGrabberInited == true){
			bIsFrameNew = false;
			if (bHavePixelsChanged == true){
				bIsFrameNew = true;
				bHavePixelsChanged = false;
			}
		}


	//---------------------------------
	#endif
	//---------------------------------

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

							pixels[(j*width*3) + i*3    ] = viPixels[posPix  ];
							pixels[(j*width*3) + i*3 + 1] = viPixels[posPix+1];
							pixels[(j*width*3) + i*3 + 2] = viPixels[posPix+2];

						}
					}

				} else {

					memcpy(pixels, viPixels, width*height*3);

				}

				if (bUseTexture){
					tex.loadData(pixels, width, height, GL_RGB);
				}
			}
		}

	//---------------------------------
	#endif
	//---------------------------------


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

	//---------------------------------
	#ifdef OF_VIDEO_CAPTURE_GSTREAMER
	//--------------------------------

		if (bGrabberInited){
			gstUtils.update();
			bIsFrameNew = gstUtils.isFrameNew();
			if(bIsFrameNew) {
				if (bUseTexture){
					tex.loadData(gstUtils.getPixels(), width, height, GL_RGB);
				}
			}
		}

	//---------------------------------
	#endif
	//---------------------------------

}

//--------------------------------------------------------------------
void ofVideoGrabber::close(){

	//---------------------------------
	#ifdef OF_VIDEO_CAPTURE_QUICKTIME
	//---------------------------------

		qtCloseSeqGrabber();
		DisposeSGGrabCompleteBottleUPP(myGrabCompleteProc);

	//---------------------------------
	#endif
	//---------------------------------


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

	//---------------------------------
	#ifdef OF_VIDEO_CAPTURE_GSTREAMER
	//--------------------------------

		gstUtils.close();

	//---------------------------------
	#endif
	//---------------------------------
	if (pixels != NULL){
		delete[] pixels;
		pixels = NULL;
	}

	tex.clear();

}

//--------------------------------------------------------------------
void ofVideoGrabber::videoSettings(void){

	//---------------------------------
	#ifdef OF_VIDEO_CAPTURE_QUICKTIME
	//---------------------------------

		Rect curBounds, curVideoRect;
		ComponentResult	err;

		// Get our current state
		err = SGGetChannelBounds (gVideoChannel, &curBounds);
		if (err != noErr){
			ofLog(OF_LOG_ERROR, "Error in SGGetChannelBounds");
			return;
		}
		err = SGGetVideoRect (gVideoChannel, &curVideoRect);
		if (err != noErr){
			ofLog(OF_LOG_ERROR, "Error in SGGetVideoRect");
			return;
		}

		// Pause
		err = SGPause (gSeqGrabber, true);
		if (err != noErr){
			ofLog(OF_LOG_ERROR, "Error in SGPause");
			return;
		}

		#ifdef TARGET_OSX
			//load any saved camera settings from file
			loadSettings();

			static SGModalFilterUPP gSeqGrabberModalFilterUPP = NewSGModalFilterUPP(SeqGrabberModalFilterUPP);
			ComponentResult result = SGSettingsDialog(gSeqGrabber,  gVideoChannel, 0, nil, 0, gSeqGrabberModalFilterUPP, nil);
			if (result != noErr){
				ofLog(OF_LOG_ERROR, "error in  dialogue");
				return;
			}

			//save any changed settings to file
			saveSettings();
		#else
			SGSettingsDialog(gSeqGrabber, gVideoChannel, 0, nil, seqGrabSettingsPreviewOnly, NULL, 0);
		#endif

		SGSetChannelBounds(gVideoChannel, &videoRect);
		SGPause (gSeqGrabber, false);

	//---------------------------------
	#endif
	//---------------------------------

	//---------------------------------
	#ifdef OF_VIDEO_CAPTURE_DIRECTSHOW
	//---------------------------------

		if (bGrabberInited == true) VI.showSettingsWindow(device);

	//---------------------------------
	#endif
	//---------------------------------


	//---------------------------------
	#ifdef OF_VIDEO_CAPTURE_UNICAP
	//--------------------------------

		ucGrabber.queryUC_imageProperties();

	//---------------------------------
	#endif
	//---------------------------------

	//TODO: add some kind of show settings for gstreamer

}


//--------------------------------------------------------------------
#ifdef TARGET_OSX
//--------------------------------------------------------------------

//---------------------------------------------------------------------
bool ofVideoGrabber::saveSettings(){

		if (bGrabberInited != true) return false;
		ComponentResult	err;

		UserData mySGVideoSettings = NULL;
		// get the SGChannel settings cofigured by the user
		err = SGGetChannelSettings(gSeqGrabber, gVideoChannel, &mySGVideoSettings, 0);
		if ( err != noErr ){
			ofLog(OF_LOG_ERROR, "Error getting camera settings %i",err);
			return false;
		}
		string pref = "ofVideoSettings-"+deviceName;
		CFStringRef cameraString = CFStringCreateWithCString(kCFAllocatorDefault,pref.c_str(),kCFStringEncodingMacRoman);

		//get the settings using the key "ofVideoSettings-the name of the device"
		SaveSettingsPreference( cameraString, mySGVideoSettings);
		DisposeUserData(mySGVideoSettings);
		return true;

}

//---------------------------------------------------------------------
bool ofVideoGrabber::loadSettings(){

   if (bGrabberInited != true || deviceName.length() == 0) return false;

   ComponentResult   err;
   UserData mySGVideoSettings = NULL;

   // get the settings using the key "ofVideoSettings-the name of the device"
   string pref = "ofVideoSettings-"+deviceName;
   CFStringRef cameraString = CFStringCreateWithCString(kCFAllocatorDefault,pref.c_str(),kCFStringEncodingMacRoman);

   GetSettingsPreference(cameraString, &mySGVideoSettings);
   if (mySGVideoSettings){

      Rect   curBounds, curVideoRect;

      //we need to make sure the dimensions don't get effected
      //by our preferences

      // Get our current state
      err = SGGetChannelBounds (gVideoChannel, &curBounds);
      if (err != noErr){
         ofLog(OF_LOG_ERROR, "Error in SGGetChannelBounds");
      }
      err = SGGetVideoRect (gVideoChannel, &curVideoRect);
      if (err != noErr){
         ofLog(OF_LOG_ERROR, "Error in SGGetVideoRect");
      }

      // use the saved settings preference to configure the SGChannel
      err = SGSetChannelSettings(gSeqGrabber, gVideoChannel, mySGVideoSettings, 0);
      if ( err != noErr ) {
         ofLog(OF_LOG_ERROR, "Error applying stored settings %i", err);
         return false;
      }
      DisposeUserData(mySGVideoSettings);

      // Pause
      err = SGPause (gSeqGrabber, true);
      if (err != noErr){
         ofLog(OF_LOG_ERROR, "Error in SGPause");
      }
      SGSetChannelBounds(gVideoChannel, &videoRect);
      SGPause (gSeqGrabber, false);

   }else{
      ofLog(OF_LOG_WARNING, "No camera settings to load");
      return false;
   }
   return true;
}


//------------------------------------------------------
bool ofVideoGrabber::qtInitSeqGrabber(){

		if (bSgInited != true){

			OSErr err = noErr;

			ComponentDescription	theDesc;
			Component				sgCompID;

			// this crashes when we get to
			// SGNewChannel
			// we get -9405 as error code for the channel
			// -----------------------------------------
			// gSeqGrabber = OpenDefaultComponent(SeqGrabComponentType, 0);

			// this seems to work instead (got it from hackTV)
			// -----------------------------------------
			theDesc.componentType = SeqGrabComponentType;
			theDesc.componentSubType = NULL;
			theDesc.componentManufacturer = 'appl';
			theDesc.componentFlags = NULL;
			theDesc.componentFlagsMask = NULL;
			sgCompID = FindNextComponent (NULL, &theDesc);
			// -----------------------------------------

			if (sgCompID == NULL){
				ofLog(OF_LOG_ERROR, "error:FindNextComponent did not return a valid component");
				return false;
			}

			gSeqGrabber = OpenComponent(sgCompID);

			err = GetMoviesError();
			if (gSeqGrabber == NULL || err) {
				ofLog(OF_LOG_ERROR, "error: can't get default sequence grabber component");
				return false;
			}

			err = SGInitialize(gSeqGrabber);
			if (err != noErr) {
				ofLog(OF_LOG_ERROR, "error: can't initialize sequence grabber component");
				return false;
			}

			err = SGSetDataRef(gSeqGrabber, 0, 0, seqGrabDontMakeMovie);
			if (err != noErr) {
				ofLog(OF_LOG_ERROR, "error: can't set the destination data reference");
				return false;
			}

			// windows crashes w/ out gworld, make a dummy for now...
			// this took a long time to figure out.
			err = SGSetGWorld(gSeqGrabber, 0, 0);
			if (err != noErr) {
				ofLog(OF_LOG_ERROR, "error: setting up the gworld");
				return false;
			}

			err = SGNewChannel(gSeqGrabber, VideoMediaType, &(gVideoChannel));
			if (err != noErr) {
				ofLog(OF_LOG_ERROR, "error: creating a channel.  Check if you have any qt capable cameras attached");
				return false;
			}

			bSgInited = true;
			return true;
		}

		return false;
}

//--------------------------------------------------------------------
bool ofVideoGrabber::qtCloseSeqGrabber(){

	if (gSeqGrabber != NULL){
		SGStop (gSeqGrabber);
		CloseComponent (gSeqGrabber);
		gSeqGrabber = NULL;
		bSgInited = false;
		return true;
	}

	return false;

}

//--------------------------------------------------------------------
bool ofVideoGrabber::qtSelectDevice(int deviceNumber, bool didWeChooseADevice){

	//note - check for memory freeing possibly needed for the all SGGetChannelDeviceList mac stuff
	// also see notes in listDevices() regarding new enunemeration method.

	//Generate a device list and enumerate
	//all devices availble to the channel
	SGDeviceList deviceList;
	SGGetChannelDeviceList(gVideoChannel, sgDeviceListIncludeInputs, &deviceList);

	unsigned char pascalName[64];
	unsigned char pascalNameInput[64];

	int numDevices = (*deviceList)->count;
	if(numDevices == 0){
		ofLog(OF_LOG_ERROR, "error: No catpure devices found");
		return false;
	}

	int deviceCount = 0;
	for(int i = 0 ; i < numDevices; ++i)
	{
		SGDeviceName nameRec;
		nameRec = (*deviceList)->entry[i];
		SGDeviceInputList deviceInputList = nameRec.inputs;

		int numInputs = 0;
		if( deviceInputList ) numInputs = ((*deviceInputList)->count);

		memcpy(pascalName, (*deviceList)->entry[i].name, sizeof(char) * 64);
		memset(pascalNameInput, 0, sizeof(char)*64);

		//this means we can use the capture method
		if(nameRec.flags != sgDeviceNameFlagDeviceUnavailable){

			//if we have a capture 'device' (qt's word not mine - I would prefer 'system' ) that is ready to be used
			//we go through its inputs to list all physical devices - as there could be more than one!
			for(int j = 0; j < numInputs; j++){

				//if our 'device' has inputs we get their names here
				if( deviceInputList ){
					SGDeviceInputName inputNameRec  = (*deviceInputList)->entry[j];
					memcpy(pascalNameInput, inputNameRec.name, sizeof(char) * 64);
				}

				//if the device number matches we try and setup the device
				//if we didn't specifiy a device then we will try all devices till one works!
				if( deviceCount == deviceNumber || !didWeChooseADevice ){
					ofLog(OF_LOG_NOTICE, "attempting to open device[%i] %s   -   %s",  deviceCount, p2cstr(pascalName), p2cstr(pascalNameInput) );

					OSErr err1 = SGSetChannelDevice(gVideoChannel, pascalName);
					OSErr err2 = SGSetChannelDeviceInput(gVideoChannel, j);

					int successLevel = 0;

					//if there were no errors then we have opened the device without issue
					if ( err1 == noErr && err2 == noErr){
						successLevel = 2;
					}
						//parameter errors are not fatal so we will try and open but will caution the user
					else if ( (err1 == paramErr || err1 == noErr) && (err2 == noErr || err2 == paramErr) ){
						successLevel = 1;
					}

					//the device is opened!
					if ( successLevel > 0 ){

						deviceName = (char *)p2cstr(pascalName);
						deviceName  += "-";
						deviceName +=  (char *)p2cstr(pascalNameInput);

						if(successLevel == 2)ofLog(OF_LOG_NOTICE, "device opened successfully");
						else ofLog(OF_LOG_WARNING, "device opened with some paramater errors - should be fine though!");

						//no need to keep searching - return that we have opened a device!
						return true;

					}else{
						//if we selected a device in particular but failed we want to go through the whole list again - starting from 0 and try any device.
						//so we return false - and try one more time without a preference
						if( didWeChooseADevice ){
							ofLog(OF_LOG_WARNING, "problems setting device[%i] %s - %s *****", deviceNumber, p2cstr(pascalName), p2cstr(pascalNameInput));
							return false;
						}else{
							ofLog(OF_LOG_WARNING, "unable to open device, trying next device");
						}
					}

				}

				//we count this way as we need to be able to distinguish multiple inputs as devices
				deviceCount++;
			}
		}else{
			//ofLog(OF_LOG_ERROR, "(unavailable) device[%i] %s",  deviceCount, p2cstr(pascalName) );
			deviceCount++;
		}
	}

	return false;
}

//---------------------------------
#endif
//---------------------------------


//--------------------------------------------------------------------
bool ofVideoGrabber::initGrabber(int w, int h, bool setUseTexture){
	bUseTexture = setUseTexture;

	//---------------------------------
	#ifdef OF_VIDEO_CAPTURE_QUICKTIME
	//---------------------------------

		//---------------------------------- 1 - open the sequence grabber
		if( !qtInitSeqGrabber() ){
			ofLog(OF_LOG_ERROR, "error: unable to initialize the seq grabber");
			return false;
		}

		//---------------------------------- 2 - set the dimensions
		width 		= w;
		height 		= h;

		MacSetRect(&videoRect, 0, 0, width, height);

		//---------------------------------- 3 - buffer allocation
		// Create a buffer big enough to hold the video data,
		// make sure the pointer is 32-byte aligned.
		// also the rgb image that people will grab

		offscreenGWorldPixels 	= (unsigned char*)malloc(4 * width * height + 32);
		pixels					= new unsigned char[width*height*3];
		
		#if defined(TARGET_OSX) && defined(__BIG_ENDIAN__)
			QTNewGWorldFromPtr (&(videogworld), k32ARGBPixelFormat, &(videoRect), NULL, NULL, 0, (offscreenGWorldPixels), 4 * width);		
		#else
			QTNewGWorldFromPtr (&(videogworld), k24RGBPixelFormat, &(videoRect), NULL, NULL, 0, (pixels), 3 * width);
		#endif		
		
		LockPixels(GetGWorldPixMap(videogworld));
		SetGWorld (videogworld, NULL);
		SGSetGWorld(gSeqGrabber, videogworld, nil);

		
		//---------------------------------- 4 - device selection
		bool didWeChooseADevice = bChooseDevice;
		bool deviceIsSelected	=  false;

		//if we have a device selected then try first to setup
		//that device
		if(didWeChooseADevice){
			deviceIsSelected = qtSelectDevice(deviceID, true);
			if(!deviceIsSelected && bVerbose) ofLog(OF_LOG_WARNING, "unable to open device[%i] - will attempt other devices", deviceID);
		}

		//if we couldn't select our required device
		//or we aren't specifiying a device to setup
		//then lets try to setup ANY device!
		if(deviceIsSelected == false){
			//lets list available devices
			listDevices();

			setDeviceID(0);
			deviceIsSelected = qtSelectDevice(deviceID, false);
		}

		//if we still haven't been able to setup a device
		//we should error and stop!
		if( deviceIsSelected == false){
			goto bail;
		}

		//---------------------------------- 5 - final initialization steps
		OSStatus err;

	 	err = SGSetChannelUsage(gVideoChannel,seqGrabPreview);
		if ( err != noErr ) goto bail;

	
		//----------------- callback method for notifying new frame
		err = SGSetChannelRefCon(gVideoChannel, (long)&bHavePixelsChanged );
		if(!err) {

			VideoBottles vb; 
			/* get the current bottlenecks */ 
			vb.procCount = 9; 
			err = SGGetVideoBottlenecks(gVideoChannel, &vb); 
			if (!err) { 			
				myGrabCompleteProc = NewSGGrabCompleteBottleUPP(frameIsGrabbedProc);
				vb.grabCompleteProc = myGrabCompleteProc;
			
				/* add our GrabFrameComplete function */ 
				err = SGSetVideoBottlenecks(gVideoChannel, &vb); 	
			}
		
		}
				
		err = SGSetChannelBounds(gVideoChannel, &videoRect);
		if ( err != noErr ) goto bail;

		err = SGPrepare(gSeqGrabber,  true, false); //theo swapped so preview is true and capture is false
		if ( err != noErr ) goto bail;

		err = SGStartPreview(gSeqGrabber);
		if ( err != noErr ) goto bail;

		bGrabberInited = true;
		loadSettings();
		
		if( attemptFramerate >= 0 ){
			err = SGSetFrameRate(gVideoChannel, IntToFixed(attemptFramerate) );
			if ( err != noErr ){
				ofLog(OF_LOG_ERROR,"initGrabber error setting framerate to %i", attemptFramerate);
			}		
		}
		
		ofLog(OF_LOG_NOTICE,"end setup ofVideoGrabber");
		ofLog(OF_LOG_NOTICE,"-------------------------------------\n");


		//---------------------------------- 6 - setup texture if needed

		if (bUseTexture){
			// create the texture, set the pixels to black and
			// upload them to the texture (so at least we see nothing black the callback)
			tex.allocate(width,height,GL_RGB);
			memset(pixels, 0, width*height*3);
			tex.loadData(pixels, width, height, GL_RGB);
		}

		// we are done
		return true;


		//--------------------- (bail) something's wrong -----
		bail:

			ofLog(OF_LOG_ERROR, "***** ofVideoGrabber error *****");
			ofLog(OF_LOG_ERROR, "-------------------------------------\n");

			//if we don't close this - it messes up the next device!
			if(bSgInited) qtCloseSeqGrabber();

			bGrabberInited = false;
			return false;

	//---------------------------------
	#endif
	//---------------------------------


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


			pixels	= new unsigned char[width * height * 3];

			if (bUseTexture){
				// create the texture, set the pixels to black and
				// upload them to the texture (so at least we see nothing black the callback)
				tex.allocate(width,height,GL_RGB);
				memset(pixels, 0, width*height*3);
				tex.loadData(pixels, width, height, GL_RGB);
			}

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



	//---------------------------------
	#ifdef OF_VIDEO_CAPTURE_UNICAP
	//--------------------------------
		if( !bGrabberInited ){
			if ( !bChooseDevice ){
				deviceID = 0;
			}

			width 	= w;
			height 	= h;
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

	//---------------------------------
	#ifdef OF_VIDEO_CAPTURE_GSTREAMER
	//--------------------------------
		gstUtils.setDeviceID(deviceID);

		if(gstUtils.initGrabber(w,h,attemptFramerate)){
			width 	= w;
			height 	= h;
			if (bUseTexture){
				// create the texture, set the pixels to black and
				// upload them to the texture (so at least we see nothing black the callback)
				tex.allocate(width,height,GL_RGB);
				tex.loadData(gstUtils.getPixels(), width, height, GL_RGB);
			}
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

//------------------------------------
void ofVideoGrabber::setUseTexture(bool bUse){
	bUseTexture = bUse;
}

//we could cap these values - but it might be more useful
//to be able to set anchor points outside the image

//----------------------------------------------------------
void ofVideoGrabber::setAnchorPercent(float xPct, float yPct){
    if (bUseTexture)tex.setAnchorPercent(xPct, yPct);
}

//----------------------------------------------------------
void ofVideoGrabber::setAnchorPoint(int x, int y){
    if (bUseTexture)tex.setAnchorPoint(x, y);
}

//----------------------------------------------------------
void ofVideoGrabber::resetAnchor(){
   	if (bUseTexture)tex.resetAnchor();
}

//------------------------------------
void ofVideoGrabber::draw(float _x, float _y, float _w, float _h){
	if (bUseTexture){
		tex.draw(_x, _y, _w, _h);
	}
}

//------------------------------------
void ofVideoGrabber::draw(float _x, float _y){
	draw(_x, _y, (float)width, (float)height);
}


//----------------------------------------------------------
float ofVideoGrabber::getHeight(){
	return (float)height;
}

//----------------------------------------------------------
float ofVideoGrabber::getWidth(){
	return (float)width;
}
