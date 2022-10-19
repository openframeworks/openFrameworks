#ifndef OF_MAIN_H
#define OF_MAIN_H

//--------------------------
// utils
#include "ofConstants.h"
#include "ofFileUtils.h"
#include "ofLog.h"
#include "ofSystemUtils.h"

#include "ofURLFileLoader.h"

#include "ofUtils.h"

#if !defined(TARGET_EMSCRIPTEN)
#include "ofThread.h"
#include "ofThreadChannel.h"
#endif

#include "ofFpsCounter.h"
#include "ofJson.h"
#include "ofXml.h"

//--------------------------
// types
#include "ofGraphicsBaseTypes.h"
#include "ofTypes.h"
#include "ofColor.h"
#include "ofPoint.h"
#include "ofRectangle.h"
#include "ofParameter.h"
#include "ofParameterGroup.h"

//--------------------------
// math
#include "ofMath.h"
#include "ofVectorMath.h"

//--------------------------
// communication
#if !defined( TARGET_OF_IOS ) & !defined(TARGET_ANDROID) & !defined(TARGET_EMSCRIPTEN)
	#include "ofSerial.h"
	#include "ofArduino.h"
#endif

//--------------------------
// gl
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
// 	#include "ofGLRenderer.h"
// #endif

//--------------------------
// graphics
#if !defined( TARGET_OF_IOS ) & !defined(TARGET_ANDROID) & !defined(TARGET_EMSCRIPTEN)
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
#include "ofBaseApp.h"
#include "ofAppRunner.h"
#include "ofAppBaseWindow.h"
#include "ofWindowSettings.h"
#include "ofMainLoop.h"
#if !defined( TARGET_OF_IOS ) & !defined(TARGET_ANDROID) & !defined(TARGET_EMSCRIPTEN) & !defined(TARGET_RASPBERRY_PI_LEGACY)
	#include "ofAppGLFWWindow.h"
	#if !defined( TARGET_LINUX_ARM )
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
using namespace std;

#endif
