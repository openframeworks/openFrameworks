#include "ofQtUtils.h"

#if defined (TARGET_WIN32) || (defined TARGET_OSX && !defined(MAC_OS_X_VERSION_10_7))
#include "ofUtils.h"
#include "ofGraphics.h"
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
				ofLogFatalError("ofQtUtils.h") << "------------------------------------------------------";
				ofLogFatalError("ofQtUtils.h") << "sorry, there wasa problem initing quicktime... exiting";
                OF_EXIT_APP(0);
			}
		#endif
		myErr = EnterMovies ();
		if (myErr != noErr){
			ofLogFatalError("ofQtUtils.h") << "------------------------------------------------------";
			ofLogFatalError("ofQtUtils.h") << "sorry, there is a problem initing quicktime... exiting";
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


#define   kCharacteristicHasVideoFrameRate		FOUR_CHAR_CODE('vfrr')
#define   kCharacteristicIsAnMpegTrack			FOUR_CHAR_CODE('mpeg')

/*

Calculate the static frame rate for a given movie.

*/
void MovieGetStaticFrameRate(Movie inMovie, double *outStaticFrameRate)
{

  *outStaticFrameRate = 0;

  Media movieMedia;
  MediaHandler movieMediaHandler;
  /* get the media identifier for the media that contains the first
    video track's sample data, and also get the media handler for
    this media. */
  MovieGetVideoMediaAndMediaHandler(inMovie, &movieMedia, &movieMediaHandler);
  if (movieMedia && movieMediaHandler)
  {
    Boolean isMPEG = false;
    /* is this the MPEG-1/MPEG-2 media handler? */
    OSErr err = IsMPEGMediaHandler(movieMediaHandler, &isMPEG);
    if (err == noErr)
    {
      if (isMPEG) /* working with MPEG-1/MPEG-2 media */
      {
        Fixed staticFrameRate;
        ComponentResult err = MPEGMediaGetStaticFrameRate(movieMediaHandler, &staticFrameRate);
        if (err == noErr)
        {
          /* convert Fixed data result to type double */
          *outStaticFrameRate = Fix2X(staticFrameRate);
        }
      }
      else  /* working with non-MPEG-1/MPEG-2 media */
      {
        OSErr err = MediaGetStaticFrameRate(movieMedia, outStaticFrameRate);
        if (err != noErr) {
			ofLogError("ofQtUtils") << "MovieGetStaticFrameRate(): couldn't get static frame rate: OSErr " << err;
		}
        //assert(err == noErr);
      }
    }
  }
}

/*

Get the media identifier for the media that contains the first
video track's sample data, and also get the media handler for
this media.

*/
void MovieGetVideoMediaAndMediaHandler(Movie inMovie, Media *outMedia, MediaHandler *outMediaHandler)
{

  *outMedia = nullptr;
  *outMediaHandler = nullptr;

  /* get first video track */
  Track videoTrack = GetMovieIndTrackType(inMovie, 1, kCharacteristicHasVideoFrameRate,
              movieTrackCharacteristic | movieTrackEnabledOnly);
  if (videoTrack != nullptr)
  {
    /* get media ref. for track's sample data */
    *outMedia = GetTrackMedia(videoTrack);
    if (*outMedia)
    {
      /* get a reference to the media handler component */
      *outMediaHandler = GetMediaHandler(*outMedia);
    }
  }
}

/*

Return true if media handler reference is from the MPEG-1/MPEG-2 media handler.
Return false otherwise.

*/
OSErr IsMPEGMediaHandler(MediaHandler inMediaHandler, Boolean *outIsMPEG)
{

  	/* is this the MPEG-1/MPEG-2 media handler? */
	return((OSErr) MediaHasCharacteristic(inMediaHandler,
					kCharacteristicIsAnMpegTrack,
					outIsMPEG));
}

/*

Given a reference to the media handler used for media in a MPEG-1/MPEG-2
track, return the static frame rate.

*/
ComponentResult MPEGMediaGetStaticFrameRate(MediaHandler inMPEGMediaHandler, Fixed *outStaticFrameRate)
{
  *outStaticFrameRate = 0;

  MHInfoEncodedFrameRateRecord encodedFrameRate;
  Size encodedFrameRateSize = sizeof(encodedFrameRate);

    /* get the static frame rate */
  ComponentResult err = MediaGetPublicInfo(inMPEGMediaHandler,
                       kMHInfoEncodedFrameRate,
                       &encodedFrameRate,
                       &encodedFrameRateSize);
  if (err == noErr)
  {
    /* return frame rate at which the track was encoded */
    *outStaticFrameRate = encodedFrameRate.encodedFrameRate;
  }

  return err;
}

/*

Given a reference to the media that contains the sample data for a track,
calculate the static frame rate.

*/
OSErr MediaGetStaticFrameRate(Media inMovieMedia, double *outFPS)
{
  *outFPS = 0;

    /* get the number of samples in the media */
  long sampleCount = GetMediaSampleCount(inMovieMedia);
  OSErr err = GetMoviesError();

  if (sampleCount && err == noErr)
  {
      /* find the media duration */

    //Quicktime 7.0 code
    //TimeValue64 duration = GetMediaDisplayDuration(inMovieMedia);
    TimeValue64 duration = GetMediaDuration(inMovieMedia);


    err = GetMoviesError();
    if (err == noErr)
    {
        /* get the media time scale */
      TimeValue64 timeScale = GetMediaTimeScale(inMovieMedia);
      err = GetMoviesError();
      if (err == noErr)
      {
        /* calculate the frame rate:
          frame rate = (sample count * media time scale) / media duration
          */
        *outFPS = (double)sampleCount * (double)timeScale / (double)duration;
      }
    }
  }

  return err;
}


//----------------------------------------

#ifdef TARGET_OSX
// GetSettingsPreference
//    Returns a preference for a specified key as QuickTime UserData
// It is your responsibility to dispose of the returned UserData
OSErr GetSettingsPreference(CFStringRef inKey, UserData *outUserData)
{
  CFPropertyListRef theCFSettings;
  Handle            theHandle = nullptr;
  UserData          theUserData = nullptr;
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

  if (nullptr == inUserData) return paramErr;

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

//end mac specific stuff
#endif

#endif
