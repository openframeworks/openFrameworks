#pragma once

//--------------------------
// utils
#include "ofConstants.h"
#include "ofFileUtils.h"
#include "ofSystemUtils.h"
#include "ofThread.h"
#include "ofURLFileLoader.h"
#include "ofUtils.h"

//--------------------------
// types
#include "ofBaseTypes.h"
#include "ofTypes.h"
#include "ofColor.h"
#include "ofPoint.h"
#include "ofRectangle.h"

//--------------------------
// math
#include "ofMath.h"
#include "ofVectorMath.h"

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
#include "ofFbo.h"
#include "ofShader.h"
#include "ofPixels.h"

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
#include "ofVideoPlayer.h"

//--------------------------
// events
#include "ofEvents.h"

//--------------------------
// 3d
#include "ofVbo.h"
#include "of3dUtils.h"
#include "ofNode.h"
#include "ofCamera.h"
#include "ofMesh.h"
#include "ofEasyCam.h"
#include "ofLight.h"

