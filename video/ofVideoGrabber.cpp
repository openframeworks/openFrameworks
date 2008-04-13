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
		bSgInited		= false;

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
	#ifdef OF_VIDEO_CAPTURE_UNICAP
	//--------------------------------


		auxPixels			= NULL;

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

		//initGrabbingComponent(vgd, true);		// if necessary, start the sg object

		if (!bSgInited){
			printf("error in list devices, couldn't allocate grabbing component\n");
			return;
		}
		printf("-------------------------------------\n");
		SGDeviceList deviceList;
		SGGetChannelDeviceList (gVideoChannel, sgDeviceListIncludeInputs, &deviceList);
		unsigned char pascalName[64];
		if(true)
		{
			printf("listing available capture devices\n");
			for(int i = 0 ; i < (*deviceList)->count ; ++i)
			{
				SGDeviceName nameRec;
				nameRec = (*deviceList)->entry[i];
				memcpy(pascalName, (*deviceList)->entry[i].name, sizeof(char) * 64);
				if(nameRec.flags != sgDeviceNameFlagDeviceUnavailable) printf("device[%i] %s \n",  i, p2cstr(pascalName) );
				else printf("(unavailable) device[%i] %s \n",  i, p2cstr(pascalName) );

			}
		}
		printf("-------------------------------------\n");

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


			if (bDoWeNeedToResize == true){
				bIsFrameNew = ucGrabber.getFrameUC(&auxPixels);
				int inputW = ucGrabber.getUC_Width();
				int inputH = ucGrabber.getUC_Height();

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

						pixels[(j*width*3) + i*3    ] = auxPixels[posPix  ];
						pixels[(j*width*3) + i*3 + 1] = auxPixels[posPix+1];
						pixels[(j*width*3) + i*3 + 2] = auxPixels[posPix+2];

					}
				}

			} else {

				bIsFrameNew = ucGrabber.getFrameUC(&pixels);

			}
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

		//thanks eloi!
		if (gSeqGrabber != 0L){
			CloseComponent (gSeqGrabber);
			gSeqGrabber = 0L;
		}

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
			if(auxPixels != NULL){
				delete auxPixels;
			}
		}

	//---------------------------------
	#endif
	//---------------------------------
	if (pixels != NULL){
		delete pixels;
	}

	tex.clear();

}

//--------------------------------------------------------------------
void ofVideoGrabber::videoSettings(void){

	//---------------------------------
	#ifdef OF_VIDEO_CAPTURE_QUICKTIME
	//---------------------------------

		Rect	curBounds, curVideoRect;
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

   if (bGrabberInited != true) return false;

   ComponentResult   err;
   UserData mySGVideoSettings = NULL;

   // get the settings using the key "ofVideoSettings-the name of the device"
   string pref = "ofVideoSettings-"+deviceName;
   CFStringRef cameraString = CFStringCreateWithCString(kCFAllocatorDefault,pref.c_str(),kCFStringEncodingMacRoman);

   GetSettingsPreference(cameraString, &mySGVideoSettings);
   if (mySGVideoSettings) {

      Rect   curBounds, curVideoRect;
      ComponentResult   err;

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


//---------------------------------
#endif
//---------------------------------


//--------------------------------------------------------------------
void ofVideoGrabber::initGrabber(int w, int h){

	//---------------------------------
	#ifdef OF_VIDEO_CAPTURE_QUICKTIME
	//---------------------------------


		OSStatus err;

		///initGrabbingComponent(vgd, true);

		if (bSgInited != true){

			OSErr err = noErr;

			gSeqGrabber = OpenDefaultComponent(SeqGrabComponentType, 0);

			err = GetMoviesError ();
			if (gSeqGrabber == NULL || err) {
				printf("error: can't get default sequence grabber component");
				return;
			}

			err = SGInitialize(gSeqGrabber);
			if (err != noErr) {
				printf("error: can't initialize sequence grabber component");
				return;
			}

			err = SGSetDataRef(gSeqGrabber, 0, 0, seqGrabDontMakeMovie);
			if (err != noErr) {
				printf("error: can't set the destination data reference");
				return;
			}

			// windows crashes w/ out gworld, make a dummy for now...
			// this took a long time to figure out.
			err = SGSetGWorld(gSeqGrabber, 0, 0);
			if (err != noErr) {
				printf("error: setting up the gworld");
				return;
			}

			err = SGNewChannel(gSeqGrabber, VideoMediaType, &(gVideoChannel));
			if (err != noErr) {
				printf("error: creating a channel.  Check if you have any qt capable cameras attached\n");
				return;
			}

			bSgInited = true;

		}

		width 		= w;
		height 		= h;

		MacSetRect(&(videoRect),0, 0, width, height);

		//---------------------------------- buffer allocation
		// Create a buffer big enough to hold the video data,
		// make sure the pointer is 32-byte aligned.
		// also the rgb image that people will grab

		offscreenGWorldPixels 	= (unsigned char*)malloc(4 * width * height + 32);
		pixels					= new unsigned char[width*height*3];
		QTNewGWorldFromPtr (&videogworld, k32ARGBPixelFormat, &videoRect, NULL, NULL, 0, offscreenGWorldPixels, 4 * width);
		LockPixels(GetGWorldPixMap(videogworld));
		SetGWorld (videogworld, NULL);
		SGSetGWorld(gSeqGrabber, videogworld, nil);


		//---------------------------------- choose a specific device to setup
		bool didWeChooseADevice = false;

		if(!bChooseDevice) setDeviceID(0);
		else didWeChooseADevice = true;

		if(bChooseDevice)
		{

			//Generate a device list and enumerate
			//all devices availble to the channel
			SGDeviceList deviceList;
			SGGetChannelDeviceList (gVideoChannel, sgDeviceListIncludeInputs, &deviceList);

			int deviceCount = (*deviceList)->count;
			if(deviceCount == 0){
				printf("No catpure devices found\n");
				goto bail;
			}

			unsigned char pascalName[128];
			//List all available devices
			if(bVerbose && didWeChooseADevice)
			{
				printf("listing available capture devices\n");
				for(int i = 0 ; i < deviceCount; ++i){
					memcpy(pascalName, (*deviceList)->entry[i].name, sizeof(char) * 128);
					printf("device[%i] %s \n",  i, p2cstr(pascalName) );
				}
			}

			for(int i = 0 ; i < deviceCount; i++){

					if(deviceID >= deviceCount) deviceID -= deviceCount;

					SGDeviceName nameRec;
					nameRec = (*deviceList)->entry[deviceID];
					if(nameRec.flags != sgDeviceNameFlagDeviceUnavailable){

						memcpy(pascalName, (*deviceList)->entry[deviceID].name, sizeof(char) * 128);
						int len = strlen( (char *)p2cstr(pascalName) );

						if(len > 0){
							string str;
							str = (char *)p2cstr(pascalName);

							if(bVerbose)printf("attempting to setup device[%i] - %s \n",  deviceID, str.c_str());

							err = SGSetChannelDevice(gVideoChannel, pascalName);
							if ( err != noErr){
								if(bVerbose && didWeChooseADevice)printf("problems setting device[%i] %s *****\n", deviceID, str.c_str());
							}else{
								deviceName = str;
								if(bVerbose)printf("using device[%i] - %s\n", deviceID,  deviceName.c_str());
								break;
							}
						}
					} else {
						if(bVerbose && didWeChooseADevice)printf("device[%i] in use - using next default device\n", deviceID);
					}

				deviceID++;
			}
		}

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


		if (bUseTexture){
			// create the texture, set the pixels to black and
			// upload them to the texture (so at least we see nothing black the callback)
			tex.allocate(width,height,GL_RGB);
			memset(pixels, 0, width*height*3);
			tex.loadData(pixels, width, height, GL_RGB);
		}



		return;				// good catch eloi/theo

		//--------------------- (bail) something's wrong -----
		bail:

			if (bVerbose){
				printf ("***** ofVideoGrabber error *****\n");
				printf ("-------------------------------------\n");
				printf ("\n");
			}

			bGrabberInited = false;
			return;

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
		} else {
			printf("error allocating a video device  \nplease check your camera with AMCAP or other software\n");
			bGrabberInited = false;
		}

	//---------------------------------
	#endif
	//---------------------------------



	//---------------------------------
	#ifdef OF_VIDEO_CAPTURE_UNICAP
	//--------------------------------
		if(!bGrabberInited){
			if (!bChooseDevice){
				deviceID = 0;
			}
			ucGrabber.open_device (deviceID);
			printf("choosing device %i: %s\n", deviceID,ucGrabber.device_identifier());
			ucGrabber.set_format(w,h);
			width 	= w;
			height 	= h;
			if(w==ucGrabber.getUC_Width() && h==ucGrabber.getUC_Height()){
				bDoWeNeedToResize=false;
			}else{
				bDoWeNeedToResize=true;
				auxPixels	= new unsigned char[ucGrabber.getUC_Width() * ucGrabber.getUC_Height() * 3];
			}
			pixels	= new unsigned char[width * height * 3];

			if (bUseTexture){
				// create the texture, set the pixels to black and
				// upload them to the texture (so at least we see nothing black the callback)
				tex.allocate(width,height,GL_RGB);
				memset(pixels, 0, width*height*3);
				tex.loadData(pixels, width, height, GL_RGB);
			}
			ucGrabber.start_capture();
			bGrabberInited=true;
		}
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
		} else {
			printf("error allocating a video device \nplease check your camera and verify that your driver is correctly installed.\n");
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

