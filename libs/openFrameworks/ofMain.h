#ifndef OF_MAIN
#define OF_MAIN

//--------------------------
// utils
#include "ofConstants.h"
#include "ofMath.h"
#include "ofUtils.h"
#include "ofTypes.h"

//--------------------------
// communication
#if !defined( TARGET_OF_IPHONE ) & !defined(TARGET_ANDROID)
	#include "ofSerial.h"
	#include "ofArduino.h"
#endif

//--------------------------
// graphics
#include "ofTexture.h"
#include "ofTrueTypeFont.h"
#include "ofGraphics.h"
#include "ofImage.h"

//--------------------------
// app
#include "ofBaseApp.h"
#include "ofAppRunner.h"

//--------------------------
// audio
#include "ofSoundStream.h"
#ifndef TARGET_ANDROID
#include "ofSoundPlayer.h"
#endif

//--------------------------
// video
#include "ofVideoGrabber.h"
#if !defined( TARGET_OF_IPHONE ) & !defined(TARGET_ANDROID)		//(temp for now, until this is ported)
	#include "ofVideoPlayer.h"
#endif

//--------------------------
// events
#include "ofEvents.h"

#endif
