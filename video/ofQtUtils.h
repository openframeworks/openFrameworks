#ifndef _QT_UTILS
#define _QT_UTILS


#include "ofConstants.h"
#include "ofGraphics.h" 	


#if TARGET_OS_MAC
	#include <Quicktime/QuickTime.h>
	#include <CoreServices/CoreServices.h>
	#include <ApplicationServices/ApplicationServices.h>
#else
	#include <QTML.h>
	#include <FixMath.h>
	#include <QuickTimeComponents.h>
	#include <TextUtils.h>	
#endif

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

#ifdef TARGET_OSX
	OSErr	GetSettingsPreference(CFStringRef inKey, UserData *outUserData);
	OSErr	SaveSettingsPreference(CFStringRef inKey, UserData inUserData);
#endif

#endif
