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
#include "ofUtils.h"
#include "ofxAndroidApp.h"
#include "ofxAndroidUtils.h"
#include <android/log.h>

static bool paused=true;


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

static JavaVM *ofJavaVM=0;


static ofBaseApp * OFApp = NULL;
static ofxAndroidApp * androidApp = NULL;
//static ofAppAndroidWindow window;

JavaVM * ofGetJavaVMPtr(){
	return ofJavaVM;
}

JNIEnv * ofGetJNIEnv(){
	JNIEnv *env;
	JavaVM * vm = ofGetJavaVMPtr();
	if(!vm){
		ofLog(OF_LOG_ERROR,"couldn't get java vm");
		return NULL;
	}
	if (vm->GetEnv((void**) &env, JNI_VERSION_1_4) != JNI_OK) {
		ofLog(OF_LOG_ERROR,"Failed to get the environment using GetEnv()");
		return NULL;
	}
	return env;
}

void ofRunApp( ofxAndroidApp * app){
	androidApp = app;
	ofRunApp((ofBaseApp*)app);
}

ofAppAndroidWindow::ofAppAndroidWindow() {
	// TODO Auto-generated constructor stub

}

ofAppAndroidWindow::~ofAppAndroidWindow() {
	// TODO Auto-generated destructor stub
}

void ofAppAndroidWindow::runAppViaInfiniteLoop(ofBaseApp * appPtr){
	OFApp = appPtr;
	ofLog(OF_LOG_NOTICE,"runAppViaInfiniteLoop");
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

void reloadTextures(){
	ofUpdateBitmapCharacterTexture();
	ofReloadAllImageTextures();
	//ofReloadAllFontTextures();
	ofResumeVideoGrabbers();
}

extern "C"{

jint JNI_OnLoad(JavaVM* vm, void* reserved)
{
	JNIEnv *env;
	ofJavaVM = vm;
	ofLog(OF_LOG_NOTICE,"JNI_OnLoad called");
	if (vm->GetEnv((void**) &env, JNI_VERSION_1_4) != JNI_OK) {
		ofLog(OF_LOG_ERROR,"Failed to get the environment using GetEnv()");
		return -1;
	}
	return JNI_VERSION_1_4;
}

void
Java_cc_openframeworks_OFAndroid_setAppName( JNIEnv*  env, jobject  thiz, jstring app_name )
{
	jboolean iscopy;
	const char *mfile = env->GetStringUTFChars(app_name, &iscopy);

    ofSetDataPathRoot(string("/sdcard/")+mfile+"/");
}

void
Java_cc_openframeworks_OFAndroid_onRestart( JNIEnv*  env, jobject  thiz ){

}

void
Java_cc_openframeworks_OFAndroid_onPause( JNIEnv*  env, jobject  thiz ){
	paused = true;

	if(androidApp) androidApp->pause();
	//ofUnloadAllFontTextures();
	//ofPauseVideoGrabbers();
}

void
Java_cc_openframeworks_OFAndroid_onResume( JNIEnv*  env, jobject  thiz ){

	//reloadTextures();
	if(androidApp){
		androidApp->resume();
		//androidApp->reloadTextures();
	}
	paused = false;
}

void
Java_cc_openframeworks_OFAndroid_onStop( JNIEnv*  env, jobject  thiz ){

}


void
Java_cc_openframeworks_OFAndroid_onDestroy( JNIEnv*  env, jobject  thiz ){

}

void
Java_cc_openframeworks_OFAndroid_onSurfaceDestroyed( JNIEnv*  env, jobject  thiz ){
	paused = true;
	ofLog(OF_LOG_NOTICE,"onSurfaceDestroyed");
	ofUnloadAllFontTextures();
	ofPauseVideoGrabbers();
}

void
Java_cc_openframeworks_OFAndroid_onSurfaceCreated( JNIEnv*  env, jobject  thiz ){
	ofLog(OF_LOG_NOTICE,"onSurfaceCreated");
	reloadTextures();
	if(androidApp){
		androidApp->resume();
		androidApp->reloadTextures();
	}
	paused = false;
}

void
Java_cc_openframeworks_OFAndroid_setup( JNIEnv*  env, jobject  thiz )
{
    //initAndroidOF();
	ofLog(OF_LOG_NOTICE,"setup");
    if(OFApp) OFApp->setup();
}

void
Java_cc_openframeworks_OFAndroid_resize( JNIEnv*  env, jobject  thiz, jint w, jint h )
{
    sWindowWidth  = w;
    sWindowHeight = h;
    ofLog(OF_LOG_NOTICE,"resize");
    if(OFApp)OFApp->windowResized(w,h);
}

/* Call to finalize the graphics state */
void
Java_cc_openframeworks_OFAndroid_exit( JNIEnv*  env )
{
   if(OFApp) OFApp->exit();
}

/* Call to render the next GL frame */
void
Java_cc_openframeworks_OFAndroid_render( JNIEnv*  env )
{
	if(paused) return;
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
Java_cc_openframeworks_OFAndroid_onTouchDown(JNIEnv*  env, jobject  thiz, jint id,jfloat x,jfloat y,jfloat pressure){
	if(OFApp)OFApp->mousePressed(x,y,0);
	ofTouchEventArgs touch;
	touch.id = id;
	touch.x = x;
	touch.y = y;
	touch.pressure = pressure;
	ofNotifyEvent(ofEvents.touchDown,touch);
}

void
Java_cc_openframeworks_OFAndroid_onTouchUp(JNIEnv*  env, jobject  thiz, jint id,jfloat x,jfloat y,jfloat pressure){
	if(OFApp)OFApp->mouseReleased(x,y,0);
	ofTouchEventArgs touch;
	touch.id = id;
	touch.x = x;
	touch.y = y;
	touch.pressure = pressure;
	ofNotifyEvent(ofEvents.touchUp,touch);
}

void
Java_cc_openframeworks_OFAndroid_onTouchMoved(JNIEnv*  env, jobject  thiz, jint id,jfloat x,jfloat y,jfloat pressure){
	if(OFApp)OFApp->mouseMoved(x,y);
	ofTouchEventArgs touch;
	touch.id = id;
	touch.x = x;
	touch.y = y;
	touch.pressure = pressure;
	ofNotifyEvent(ofEvents.touchMoved,touch);
}
}


