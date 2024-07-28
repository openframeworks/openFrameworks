#pragma once

//--------------------------
// utils
#include "ofConstants.h"
#include "ofFileUtils.h"
#include "ofLog.h"
#include "ofSystemUtils.h"

#include "ofURLFileLoader.h"

#include "ofUtils.h"

#include "ofThread.h"
#include "ofThreadChannel.h"

#include "ofFpsCounter.h"
#include "ofJson.h"
#include "ofXml.h"

//--------------------------
// types
#include "ofColor.h"
#include "ofGraphicsBaseTypes.h"
#include "ofParameter.h"
#include "ofPoint.h"
#include "ofRectangle.h"
#include "ofTypes.h"

//--------------------------
// math
#include "ofMath.h"
#include "ofVectorMath.h"

//--------------------------
// communication
#if !defined(TARGET_OF_IOS) & !defined(TARGET_ANDROID) & !defined(TARGET_EMSCRIPTEN)
    #include "ofSerial.h"
    #include "ofArduino.h"
#endif

//--------------------------
// gl
#include "ofCubeMap.h"
#include "ofFbo.h"
#include "ofGLRenderer.h"
#include "ofGLUtils.h"
#include "ofLight.h"
#include "ofMaterial.h"
#include "ofShader.h"
#include "ofTexture.h"
#include "ofVbo.h"
#include "ofVboMesh.h"
// #include "ofGLProgrammableRenderer.h"
// #ifndef TARGET_PROGRAMMABLE_GL
//     #include "ofGLRenderer.h"
// #endif

//--------------------------
// graphics
#if !defined(TARGET_OF_IOS) & !defined(TARGET_ANDROID) & !defined(TARGET_EMSCRIPTEN)
    #include "ofCairoRenderer.h"
    #include "ofGraphicsCairo.h"
#endif
#include "ofGraphics.h"
#include "ofImage.h"
#include "ofPath.h"
#include "ofPixels.h"
#include "ofPolyline.h"
#include "ofRendererCollection.h"
#include "ofTessellator.h"
#include "ofTrueTypeFont.h"

//--------------------------
// app
#include "ofAppBaseWindow.h"
#include "ofAppRunner.h"
#include "ofBaseApp.h"
#include "ofMainLoop.h"
#include "ofWindowSettings.h"
#if !defined(TARGET_OF_IOS) & !defined(TARGET_ANDROID) & !defined(TARGET_EMSCRIPTEN) & !defined(TARGET_RASPBERRY_PI_LEGACY)
    #include "ofAppGLFWWindow.h"
    #if !defined(TARGET_LINUX_ARM)
        #include "ofAppGlutWindow.h"
    #endif
#endif

//--------------------------
// audio
#ifndef TARGET_NO_SOUND
    #include "ofSoundStream.h"
    #include "ofSoundPlayer.h"
    #include "ofSoundBuffer.h"
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
#include "of3dUtils.h"
#include "ofCamera.h"
#include "ofEasyCam.h"
#include "ofMesh.h"
#include "ofNode.h"

//--------------------------
#ifdef OF_LEGACY_INCLUDE_STD
using namespace std;
#else

// this will eventually be disabled by default
#define OF_USE_MINIMAL_STD
    #ifdef OF_USE_MINIMAL_STD
using std::cout;
using std::deque;
using std::endl;
using std::make_shared;
using std::map;
using std::max;
using std::pair;
using std::shared_ptr;
using std::string;
using std::stringstream;
using std::swap;
using std::to_string;
using std::vector;
using std::weak_ptr;
    #endif
#endif
