#include "ofVideoGrabber.h"

#ifdef OF_VIDEO_CAPTURE_V4L
	#include "ofV4LUtils.h"
#endif


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

		
	//---------------------------------
	#ifdef OF_VIDEO_CAPTURE_V4L				// kept around if people have unicap issues...
	//--------------------------------

		bV4LGrabberInited 	= false;

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
}


//--------------------------------------------------------------------
ofVideoGrabber::~ofVideoGrabber(){

	close();

	//---------------------------------
	#ifdef OF_VIDEO_CAPTURE_QUICKTIME
	//---------------------------------
		if (offscreenGWorldPixels != NULL){
			delete offscreenGWorldPixels;
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
		unsigned char pascalName[256];
		unsigned char pascalNameInput[256];
		
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
		
		printf("listing available capture devices\n");
		for(int i = 0 ; i < (*deviceList)->count ; ++i)
		{
			SGDeviceName nameRec;
			nameRec = (*deviceList)->entry[i];
			SGDeviceInputList deviceInputList = nameRec.inputs;
			
			int numInputs = 0;
			if( deviceInputList ) numInputs = ((*deviceInputList)->count);
		
			memcpy(pascalName, (*deviceList)->entry[i].name, sizeof(char) * 256);
						
			//this means we can use the capture method
			if(nameRec.flags != sgDeviceNameFlagDeviceUnavailable){
			
				//if we have a capture 'device' (qt's word not mine - I would prefer 'system' ) that is ready to be used 
				//we go through its inputs to list all physical devices - as there could be more than one!
				for(int j = 0; j < numInputs; j++){
				
					
					//if our 'device' has inputs we get their names here
					if( deviceInputList ){
						SGDeviceInputName inputNameRec  = (*deviceInputList)->entry[j];
						memcpy(pascalNameInput, inputNameRec.name, sizeof(char) * 256);
					}
					
					printf("device[%i] %s - %s\n",  deviceCount, p2cstr(pascalName), p2cstr(pascalNameInput) );

					//we count this way as we need to be able to distinguish multiple inputs as devices
					deviceCount++;
				}
				
			}else{
				printf("(unavailable) device[%i] %s \n",  deviceCount, p2cstr(pascalName) );
				deviceCount++;
			}
		}
		printf("-------------------------------------\n");
		
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
		printf("---\n");
		VI.listDevices();
		printf("---\n");

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
	#ifdef OF_VIDEO_CAPTURE_V4L
	//--------------------------------

	struct stat st;

	printf("listing available capture devices\n");
	printf("---\n");
	for (int i = 0; i < 8; i++)
	{
		sprintf(dev_name, "/dev/video%i", i);
		if (stat (dev_name, &st) == 0) {
			printf("Video device %i = /dev/video%i\n",i,i);
		} else {

		}
	}
	printf("---\n");

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

//---------------------------------------------------------------------------
unsigned char * ofVideoGrabber::getPixels(){
	return pixels;
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
			if (offscreenGWorldPixels[0] != 0x00){
				offscreenGWorldPixels[0] = 0x00;
				bHavePixelsChanged = true;
				convertPixels(offscreenGWorldPixels, pixels, width, height);
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
	#ifdef OF_VIDEO_CAPTURE_V4L
	//--------------------------------

		if (bV4LGrabberInited == true){
			bIsFrameNew = getFrameV4L(pixels);
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
void ofVideoGrabber::close(){

	//---------------------------------
	#ifdef OF_VIDEO_CAPTURE_QUICKTIME
	//---------------------------------

		qtCloseSeqGrabber();

	//---------------------------------
	#endif
	//---------------------------------


	//---------------------------------
	#ifdef OF_VIDEO_CAPTURE_V4L
	//--------------------------------

		closeV4L();

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
			printf("Error in SGGetChannelBounds \n");
			return;
		}
		err = SGGetVideoRect (gVideoChannel, &curVideoRect);
		if (err != noErr){
			printf("Error in SGGetVideoRect \n");
			return;
		}

		// Pause
		err = SGPause (gSeqGrabber, true);
		if (err != noErr){
			printf("Error in SGPause\n");
			return;
		}

		#ifdef TARGET_OSX
			//load any saved camera settings from file
			loadSettings();

			static SGModalFilterUPP gSeqGrabberModalFilterUPP = NewSGModalFilterUPP(SeqGrabberModalFilterUPP);
			ComponentResult result = SGSettingsDialog(gSeqGrabber,  gVideoChannel, 0, nil, seqGrabSettingsPreviewOnly, gSeqGrabberModalFilterUPP, nil);
			if (result != noErr){
				printf("error in  dialogue\n");
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

	//---------------------------------
	#ifdef OF_VIDEO_CAPTURE_V4L
	//--------------------------------

		queryV4L_imageProperties();

	//---------------------------------
	#endif
	//---------------------------------

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
			printf("Error getting camera settings %i\n",err);
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
         printf("Error in SGGetChannelBounds \n");
      }
      err = SGGetVideoRect (gVideoChannel, &curVideoRect);
      if (err != noErr){
         printf("Error in SGGetVideoRect \n");
      }

      // use the saved settings preference to configure the SGChannel
      err = SGSetChannelSettings(gSeqGrabber, gVideoChannel, mySGVideoSettings, 0);
      if ( err != noErr ) {
         printf("Error applying stored settings %i \n", err);
         return false;
      }
      DisposeUserData(mySGVideoSettings);

      // Pause
      err = SGPause (gSeqGrabber, true);
      if (err != noErr){
         printf("Error in SGPause\n");
      }
      SGSetChannelBounds(gVideoChannel, &videoRect);
      SGPause (gSeqGrabber, false);

   }else{
      printf("No camera settings to load \n");
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
				printf("error:FindNextComponent did not return a valid component");
				return false;
			}
			
			gSeqGrabber = OpenComponent(sgCompID);
						
			err = GetMoviesError();
			if (gSeqGrabber == NULL || err) {
				printf("error: can't get default sequence grabber component\n");
				return false;
			}

			err = SGInitialize(gSeqGrabber);
			if (err != noErr) {
				printf("error: can't initialize sequence grabber component");
				return false;
			}

			err = SGSetDataRef(gSeqGrabber, 0, 0, seqGrabDontMakeMovie);
			if (err != noErr) {
				printf("error: can't set the destination data reference");
				return false;
			}

			// windows crashes w/ out gworld, make a dummy for now...
			// this took a long time to figure out.
			err = SGSetGWorld(gSeqGrabber, 0, 0);
			if (err != noErr) {
				printf("error: setting up the gworld");
				return false;
			}

			err = SGNewChannel(gSeqGrabber, VideoMediaType, &(gVideoChannel));
			if (err != noErr) {
				printf("error: creating a channel.  Check if you have any qt capable cameras attached\n");
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
	
	unsigned char pascalName[256];
	unsigned char pascalNameInput[256];

	int numDevices = (*deviceList)->count;
	if(numDevices == 0){
		printf("error: No catpure devices found\n");
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

		memcpy(pascalName, (*deviceList)->entry[i].name, sizeof(char) * 256);
				
		//this means we can use the capture method
		if(nameRec.flags != sgDeviceNameFlagDeviceUnavailable){
		
			//if we have a capture 'device' (qt's word not mine - I would prefer 'system' ) that is ready to be used 
			//we go through its inputs to list all physical devices - as there could be more than one!
			for(int j = 0; j < numInputs; j++){
			
				//if our 'device' has inputs we get their names here
				if( deviceInputList ){
					SGDeviceInputName inputNameRec  = (*deviceInputList)->entry[j];
					memcpy(pascalNameInput, inputNameRec.name, sizeof(char) * 256);
				}
				
				//if the device number matches we try and setup the device
				//if we didn't specifiy a device then we will try all devices till one works!
				if( deviceCount == deviceNumber || !didWeChooseADevice ){
					printf("attempting to open device[%i] %s   -   %s\n",  deviceCount, p2cstr(pascalName), p2cstr(pascalNameInput) );
					
					OSErr err1 = SGSetChannelDevice(gVideoChannel, pascalNameInput);
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
						
						if(bVerbose){
							if(successLevel == 2)printf("device opened successfully\n");
							else printf("device opened with some paramater errors - should be fine though!\n");
						}
						
						//no need to keep searching - return that we have opened a device!
						return true;
						
					}else{
						//if we selected a device in particular but failed we want to go through the whole list again - starting from 0 and try any device.
						//so we return false - and try one more time without a preference
						if( didWeChooseADevice ){
							if(bVerbose)printf("problems setting device[%i] %s - %s *****\n", deviceNumber, p2cstr(pascalName), p2cstr(pascalNameInput));
							return false;
						}else{
							if(bVerbose)printf("unable to open device, trying next device\n");
						}
					}
					
				}

				//we count this way as we need to be able to distinguish multiple inputs as devices
				deviceCount++;
			}
		}else{
			//printf("(unavailable) device[%i] %s \n",  deviceCount, p2cstr(pascalName) );
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
			printf("error: unable to initialize the seq grabber\n");
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
		QTNewGWorldFromPtr (&videogworld, k32ARGBPixelFormat, &videoRect, NULL, NULL, 0, offscreenGWorldPixels, 4 * width);
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
			if(!deviceIsSelected && bVerbose) printf("unable to open device[%i] - will attempt other devices\n", deviceID);
		}
		
		//if we couldn't select our required device
		//or we aren't specifiying a device to setup
		//then lets try to setup ANY device!
		if(deviceIsSelected == false){
			//lets list available devices 
			if(bVerbose)listDevices();
		
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

		err = SGSetChannelBounds(gVideoChannel, &videoRect);
		if ( err != noErr ) goto bail;

		err = SGPrepare(gSeqGrabber,  true, false); //theo swapped so preview is true and capture is false
		if ( err != noErr ) goto bail;

		err = SGStartPreview(gSeqGrabber);
		if ( err != noErr ) goto bail;

		bGrabberInited = true;
		loadSettings();

		if (bVerbose){
			printf ("end setup ofVideoGrabber\n");
			printf ("-------------------------------------\n");
			printf ("\n");
		}


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

			if (bVerbose){
				printf ("***** ofVideoGrabber error *****\n");
				printf ("-------------------------------------\n");
				printf ("\n");
			}

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
			printf("choosing %i  \n", deviceID);
		} else {
			device = 0;
		}

		width = w;
		height = h;
		bGrabberInited = false;

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
			printf("error allocating a video device  \nplease check your camera with AMCAP or other software\n");
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
				printf("choosing device %i: %s\n", deviceID,ucGrabber.device_identifier());
				ucGrabber.set_format(w,h);
				ucGrabber.start_capture();
			}
			
		}
			return bGrabberInited;
	//---------------------------------
	#endif
	//---------------------------------


	//---------------------------------
	#ifdef OF_VIDEO_CAPTURE_V4L
	//--------------------------------
		if (bChooseDevice){
			device = deviceID;
		} else {
			device = 0;
		}
		sprintf(dev_name, "/dev/video%i", device);
		printf("choosing device %s\n", dev_name);

		bool bOk = initV4L(w, h, dev_name);

		if (bOk == true){
			bV4LGrabberInited = true;
			width 	= getV4L_Width();
			height 	= getV4L_Height();
			pixels	= new unsigned char[width * height * 3];

			if (bUseTexture){
				// create the texture, set the pixels to black and
				// upload them to the texture (so at least we see nothing black the callback)
				tex.allocate(width,height,GL_RGB);
				//memset(pixels, 0, width*height*3);
				//tex.loadData(pixels, width, height, GL_RGB);
			}
			
			printf("success allocating a video device \n");
			return true;
		} else {
			printf("error allocating a video device \nplease check your camera and verify that your driver is correctly installed.\n");
			return false;
		}	//---------------------------------


	//---------------------------------
	#endif
	//---------------------------------



}

//------------------------------------
void ofVideoGrabber::setUseTexture(bool bUse){
	bUseTexture = bUse;
}


//------------------------------------
void ofVideoGrabber::draw(float _x, float _y, float _w, float _h){
	if (bUseTexture){
		tex.draw(_x, _y, _w, _h);
	}
}

//------------------------------------
void ofVideoGrabber::draw(float _x, float _y){
	draw(_x, _y, width, height);
}

