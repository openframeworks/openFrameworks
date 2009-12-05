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
#ifndef TARGET_OF_IPHONE
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
#include "ofSoundPlayer.h"

//--------------------------
// video
#ifndef TARGET_OF_IPHONE			//(temp for now, until this is ported)
	#include "ofVideoGrabber.h"
	#include "ofVideoPlayer.h"
#endif

//--------------------------
// events
#include "ofEvents.h"

#endif
