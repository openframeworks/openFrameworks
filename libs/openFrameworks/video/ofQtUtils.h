#pragma once

#include "ofConstants.h"

#if defined (TARGET_WIN32) || defined (TARGET_OSX)



#ifdef TARGET_OSX
	#include <QuickTime/QuickTime.h>
	#include <CoreServices/CoreServices.h>
	#include <ApplicationServices/ApplicationServices.h>
#else
	#include <QTML.h>
	#include <FixMath.h>
	#include <QuickTimeComponents.h>
	#include <TextUtils.h>
	#include <MediaHandlers.h>
	//#include <MoviesFormat.h>
#endif

#ifndef MAC_OS_X_VERSION_10_7

//p2cstr depreciation fix - thanks pickard!
#ifdef TARGET_OSX
	#define p2cstr(aStr) CFStringGetCStringPtr(CFStringCreateWithPascalString(NULL, aStr, kCFStringEncodingMacRoman),kCFStringEncodingMacRoman)
#endif

//-------------------------- helpful for rgba->rgb conversion
typedef struct{
	unsigned char r;
	unsigned char g;
	unsigned char b;
} pix24;



//----------------------------------------

void 		initializeQuicktime();
void 		closeQuicktime();
void 		convertPixels(unsigned char * gWorldPixels, unsigned char * rgbPixels, int w, int h);
Boolean 	SeqGrabberModalFilterUPP(DialogPtr theDialog, const EventRecord *theEvent, short *itemHit, long refCon);
OSErr           IsMPEGMediaHandler(MediaHandler inMediaHandler, Boolean *outIsMPEG);
ComponentResult MPEGMediaGetStaticFrameRate(MediaHandler inMPEGMediaHandler, Fixed *outStaticFrameRate);
OSErr           MediaGetStaticFrameRate(Media inMovieMedia, double *outFPS);
void            MovieGetVideoMediaAndMediaHandler(Movie inMovie, Media *outMedia,
				MediaHandler *outMediaHandler);
void            MovieGetStaticFrameRate(Movie inMovie, double *outStaticFrameRate);

#ifdef TARGET_OSX
	OSErr	GetSettingsPreference(CFStringRef inKey, UserData *outUserData);
	OSErr	SaveSettingsPreference(CFStringRef inKey, UserData inUserData);
#endif

#endif //OS 10.7 guard

#endif
