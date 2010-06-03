/*
 * ofAppAndroidWindow.cpp
 *
 *  Created on: 24/05/2010
 *      Author: arturo
 */

#include "ofAppAndroidWindow.h"

#include <jni.h>
#include "ofGraphics.h"
#include "ofAppRunner.h"
#include <android/log.h>

#define  LOG_TAG    "libgl2jni"
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)

static int  sWindowWidth  = 320;
static int  sWindowHeight = 480;
static int  sDemoStopped  = 0;
static long sTimeOffset   = 0;
static int  sTimeOffsetInit = 0;
static long sTimeStopped  = 0;

static bool bSetupScreen = true;
float			timeNow, timeThen, fps;
int				nFramesForFPS;
int				nFrameCount;
bool			bFrameRateSet;
int 			millisForFrame;
int 			prevMillis;
int 			diffMillis;

float 			frameRate;

double			lastFrameTime;


static ofBaseApp * OFApp = NULL;
//static ofAppAndroidWindow window;

ofAppAndroidWindow::ofAppAndroidWindow() {
	// TODO Auto-generated constructor stub

}

ofAppAndroidWindow::~ofAppAndroidWindow() {
	// TODO Auto-generated destructor stub
}

void ofAppAndroidWindow::runAppViaInfiniteLoop(ofBaseApp * appPtr){
	OFApp = appPtr;
	LOGI("runAppViaInfiniteLoop");
}

ofPoint	ofAppAndroidWindow::getWindowSize(){
	return ofPoint(sWindowWidth,sWindowHeight);
}

int	ofAppAndroidWindow::getFrameNum() {
	return nFrameCount;
}

float ofAppAndroidWindow::getFrameRate() {
	return frameRate;
}

double ofAppAndroidWindow::getLastFrameTime(){
	return lastFrameTime;
}


void ofAppAndroidWindow::enableSetupScreen(){
	bSetupScreen = true;
}

void ofAppAndroidWindow::disableSetupScreen(){
	bSetupScreen = false;
}

extern "C"{

void
Java_cc_openframeworks_OFAndroidWindow_setAppName( JNIEnv*  env, jobject  thiz, jstring app_name )
{
	jboolean iscopy;
	const char *mfile = env->GetStringUTFChars(app_name, &iscopy);

    ofSetDataPathRoot(string("/sdcard/")+mfile+"/");
}

void
Java_cc_openframeworks_OFAndroidWindow_setup( JNIEnv*  env, jobject  thiz )
{

    //initAndroidOF();
    LOGI("setup");
    if(OFApp) OFApp->setup();
}

void
Java_cc_openframeworks_OFAndroidWindow_resize( JNIEnv*  env, jobject  thiz, jint w, jint h )
{
    sWindowWidth  = w;
    sWindowHeight = h;
    LOGI("resize");
    if(OFApp)OFApp->windowResized(w,h);
}

/* Call to finalize the graphics state */
void
Java_cc_openframeworks_OFAndroidWindow_exit( JNIEnv*  env )
{
   if(OFApp) OFApp->exit();
}

/* Call to render the next GL frame */
void
Java_cc_openframeworks_OFAndroidWindow_render( JNIEnv*  env )
{
	//LOGI("update");
	if(OFApp) OFApp->update();


	int width, height;

	width  = sWindowWidth;
	height = sWindowHeight;

	height = height > 0 ? height : 1;
	// set viewport, clear the screen
	glViewport( 0, 0, width, height );
	float * bgPtr = ofBgColorPtr();
	bool bClearAuto = ofbClearBg();

	if ( bClearAuto == true ){
		glClearColor(bgPtr[0],bgPtr[1],bgPtr[2], bgPtr[3]);
		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	if(bSetupScreen) ofSetupScreen();
	if(OFApp) OFApp->draw();

	timeNow = ofGetElapsedTimef();
	double diff = timeNow-timeThen;
	if( diff  > 0.00001 ){
		fps			= 1.0 / diff;
		frameRate	*= 0.9f;
		frameRate	+= 0.1f*fps;
	 }
	 lastFrameTime	= diff;
	 timeThen		= timeNow;
	// --------------

	nFrameCount++;		// increase the overall frame count


}

void
Java_cc_openframeworks_OFGLSurfaceView_onTouchDown(JNIEnv*  env, jobject  thiz, int id,float x,float y,float pressure){
	if(OFApp)OFApp->mousePressed(x,y,0);
	ofTouchEventArgs touch;
	touch.x = x;
	touch.y = y;
	touch.pressure = pressure;
	ofNotifyEvent(ofEvents.touchDown,touch);
}

void
Java_cc_openframeworks_OFGLSurfaceView_onTouchUp(JNIEnv*  env, jobject  thiz, int id,float x,float y,float pressure){
	if(OFApp)OFApp->mouseReleased(x,y,0);
	ofTouchEventArgs touch;
	touch.x = x;
	touch.y = y;
	touch.pressure = pressure;
	ofNotifyEvent(ofEvents.touchUp,touch);
}

void
Java_cc_openframeworks_OFGLSurfaceView_onTouchMoved(JNIEnv*  env, jobject  thiz, int id,float x,float y,float pressure){
	if(OFApp)OFApp->mouseMoved(x,y);
	ofTouchEventArgs touch;
	touch.x = x;
	touch.y = y;
	touch.pressure = pressure;
	ofNotifyEvent(ofEvents.touchMoved,touch);
}
}
