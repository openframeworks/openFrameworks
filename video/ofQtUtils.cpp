#include "ofQtUtils.h"

static bool bQuicktimeInitialized = false;

//----------------------------------------
void initializeQuicktime(){	
	if (bQuicktimeInitialized == false){	
		
		//----------------------------------
		// do we have quicktime installed at all?
		// http://www.apple.com/quicktime/download/win.html
		// can gestalt help with versions, or is that only after init?
		
		OSErr myErr 	= noErr;
		#ifdef TARGET_WIN32
			myErr = InitializeQTML(0);
			if (myErr != noErr){
				printf("----------------------------------------------------- \n");
				printf("sorry, there is a problem with quicktime starting up \nplease check!");
                OF_EXIT_APP(0);
			}
		#endif
		myErr = EnterMovies ();
		if (myErr != noErr){
			printf("----------------------------------------------------- \n");
			printf("sorry, there is a problem with quicktime starting up  \nplease check!");
			OF_EXIT_APP(0);
		}

		bQuicktimeInitialized = true;
	}
}

//----------------------------------------
void closeQuicktime(){
	if (bQuicktimeInitialized == true){
		ExitMovies();
		#ifdef TARGET_WIN32
			TerminateQTML();
		#endif
		bQuicktimeInitialized = false;
	}
}


//----------------------------------------
void convertPixels(unsigned char * gWorldPixels, unsigned char * rgbPixels, int w, int h){
	
	// ok for macs?
	// ok for intel macs?
	
	int * rgbaPtr 			= (int *) gWorldPixels;
	pix24 * rgbPtr 			= (pix24 *) rgbPixels;
	int totalPixelCount 	= w * h;
	unsigned char * rgbaStart;
	
	//	putting in the boolean, so we can work on 
	//	0,0 in top right...
	//	bool bFlipVertically 	= true;
	
	bool bFlipVertically 	= false;
	
	// -------------------------------------------
	// we flip vertically because the 0,0 position in OF 
	// is the bottom left (not top left, like processing)
	// since the 0,0 of a picture is top left
	// if we upload and drawf the data as is
	// it will be upside-down....
	// -------------------------------------------
	
	if (!bFlipVertically){
		//----- argb->rgb
		for (int i = 0; i < h; i++){
			pix24 * rgbPtr 			= (pix24 *) rgbPixels + ((i) * w);
			for (int j = 0; j < w; j++){
				rgbaStart = (unsigned char *)rgbaPtr;	
				memcpy (rgbPtr, rgbaStart+1, sizeof(pix24));
				rgbPtr++;
				rgbaPtr++;
			}
		}
	} else {
		//----- flip while argb->rgb
		for (int i = 0; i < h; i++){
			pix24 * rgbPtr 			= (pix24 *) rgbPixels + ((h-i-1) * w);
			for (int j = 0; j < w; j++){
				rgbaStart = (unsigned char *)rgbaPtr;	
				memcpy (rgbPtr, rgbaStart+1, sizeof(pix24));
				rgbPtr++;
				rgbaPtr++;
			}
		}
	}
}


//----------------------------------------
// osx needs this for modal dialogs.  
Boolean SeqGrabberModalFilterUPP (DialogPtr theDialog, const EventRecord *theEvent, short *itemHit, long refCon){
	#pragma unused(theDialog, itemHit)  
  	Boolean  handled = false;
  	if ((theEvent->what == updateEvt) && 
    ((WindowPtr) theEvent->message == (WindowPtr) refCon))
  	{
    	BeginUpdate ((WindowPtr) refCon);
    	EndUpdate ((WindowPtr) refCon);
    	handled = true;
  	}
  	return (handled);
}



//----------------------------------------

#ifdef TARGET_OSX
// GetSettingsPreference
//    Returns a preference for a specified key as QuickTime UserData
// It is your responsibility to dispose of the returned UserData
OSErr GetSettingsPreference(CFStringRef inKey, UserData *outUserData)
{
  CFPropertyListRef theCFSettings;
  Handle            theHandle = NULL;
  UserData          theUserData = NULL;
  OSErr             err = paramErr;

  // read the new setttings from our preferences
  theCFSettings = CFPreferencesCopyAppValue(inKey,
                                         kCFPreferencesCurrentApplication);
  if (theCFSettings) {
    err = PtrToHand(CFDataGetBytePtr((CFDataRef)theCFSettings), &theHandle,
                    CFDataGetLength((CFDataRef)theCFSettings));
					        
    CFRelease(theCFSettings);
    if (theHandle) {
      err = NewUserDataFromHandle(theHandle, &theUserData);
      if (theUserData) {
        *outUserData = theUserData;
      }
      DisposeHandle(theHandle);
    }
  }

  return err;
}

//----------------------------------------
// SaveSettingsPreference
//    Saves a preference for a specified key from QuickTime UserData
OSErr SaveSettingsPreference(CFStringRef inKey, UserData inUserData)
{
  CFDataRef theCFSettings;
  Handle    hSettings;
  OSErr     err;
    
  if (NULL == inUserData) return paramErr;

  hSettings = NewHandle(0);
  err = MemError();
    
  if (noErr == err) {
    err = PutUserDataIntoHandle(inUserData, hSettings); 
        
    if (noErr == err) {
      HLock(hSettings);
    
      theCFSettings = CFDataCreate(kCFAllocatorDefault,
                                   (UInt8 *)*hSettings,
                                   GetHandleSize(hSettings));
      if (theCFSettings) {
        CFPreferencesSetAppValue(inKey, theCFSettings,
                                 kCFPreferencesCurrentApplication);
        CFPreferencesAppSynchronize(kCFPreferencesCurrentApplication);
        CFRelease(theCFSettings);
      }
    }

    DisposeHandle(hSettings);
  }

  return err;
}

#endif

