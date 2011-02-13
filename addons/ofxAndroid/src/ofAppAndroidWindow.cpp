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
#include "ofxAccelerometer.h"
#include <android/log.h>

static bool paused=true;


static int  sWindowWidth  = 320;
static int  sWindowHeight = 480;
/*static int  sDemoStopped  = 0;
static long sTimeOffset   = 0;
static int  sTimeOffsetInit = 0;
static long sTimeStopped  = 0;*/

static bool bSetupScreen = true;
static float			timeNow, timeThen, fps;
static int				nFramesForFPS;
static int				nFrameCount;
static bool			bFrameRateSet;
static int 			millisForFrame;
static int 			prevMillis;
static int 			diffMillis;

static float 			frameRate;

static double			lastFrameTime;

static JavaVM *ofJavaVM=0;


static ofBaseApp * OFApp = NULL;
static ofxAndroidApp * androidApp = NULL;

static ofOrientation orientation = OF_ORIENTATION_DEFAULT;
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

jclass ofGetJavaOFAndroid(){
	return ofGetJNIEnv()->FindClass("cc.openframeworks.OFAndroid");
}

void ofRunApp( ofxAndroidApp * app){
	androidApp = app;
	ofRunApp((ofBaseApp*)app);
}

void ofxRegisterMultitouch(ofxAndroidApp * app){
	ofRegisterTouchEvents(app);
}


void ofxAndroidPauseApp(){
	jclass javaClass = ofGetJavaOFAndroid();

	if(javaClass==0){
		ofLog(OF_LOG_ERROR,"cannot find OFAndroid java class");
		return;
	}

	jmethodID pauseApp = ofGetJNIEnv()->GetStaticMethodID(javaClass,"pauseApp","()V");
	if(!pauseApp){
		ofLog(OF_LOG_ERROR,"cannot find OFAndroid pauseApp method");
		return;
	}
	ofGetJNIEnv()->CallStaticObjectMethod(javaClass,pauseApp);
}

void ofxAndroidAlertBox(string msg){
	jclass javaClass = ofGetJavaOFAndroid();

	if(javaClass==0){
		ofLog(OF_LOG_ERROR,"cannot find OFAndroid java class");
		return;
	}

	jmethodID alertBox = ofGetJNIEnv()->GetStaticMethodID(javaClass,"alertBox","(Ljava/lang/String;)V");
	if(!alertBox){
		ofLog(OF_LOG_ERROR,"cannot find OFAndroid alertBox method");
		return;
	}
	jstring jMsg = ofGetJNIEnv()->NewStringUTF(msg.c_str());
	ofGetJNIEnv()->CallStaticObjectMethod(javaClass,alertBox,jMsg);
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

int	ofAppAndroidWindow::getWidth(){
	return sWindowWidth;
}

int	ofAppAndroidWindow::getHeight(){
	return sWindowHeight;
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

void ofAppAndroidWindow::setOrientation(ofOrientation _orientation){
	if(orientation==_orientation) return;
	orientation = _orientation;
	jclass javaClass = ofGetJNIEnv()->FindClass("cc.openframeworks.OFAndroid");

	if(javaClass==0){
		ofLog(OF_LOG_ERROR,"cannot find OFAndroid java class");
		return;
	}

	jmethodID setScreenOrientation = ofGetJNIEnv()->GetStaticMethodID(javaClass,"setScreenOrientation","(I)V");
	if(!setScreenOrientation){
		ofLog(OF_LOG_ERROR,"cannot find OFAndroid setScreenOrientation method");
		return;
	}
	ofGetJNIEnv()->CallStaticObjectMethod(javaClass,setScreenOrientation,orientation);
}

ofOrientation ofAppAndroidWindow::getOrientation(){
	return orientation;
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
Java_cc_openframeworks_OFAndroid_setAppDataDir( JNIEnv*  env, jobject  thiz, jstring data_dir )
{
	jboolean iscopy;
	const char *mfile = env->GetStringUTFChars(data_dir, &iscopy);
	ofLog(OF_LOG_NOTICE,"Setting app dir name to: " + string(mfile));
    ofSetDataPathRoot(string(mfile)+"/");
}

void
Java_cc_openframeworks_OFAndroid_onRestart( JNIEnv*  env, jobject  thiz ){

}

void
Java_cc_openframeworks_OFAndroid_onPause( JNIEnv*  env, jobject  thiz ){
	paused = true;

	if(androidApp) androidApp->pause();
	ofUnloadAllFontTextures();
	ofPauseVideoGrabbers();
	ofxAndroidSoundStreamPause();
}

void
Java_cc_openframeworks_OFAndroid_onResume( JNIEnv*  env, jobject  thiz ){

	reloadTextures();
	if(androidApp){
		androidApp->resume();
		androidApp->reloadTextures();
	}
	paused = false;
}

void
Java_cc_openframeworks_OFAndroid_onStop( JNIEnv*  env, jobject  thiz ){

}


void
Java_cc_openframeworks_OFAndroid_onDestroy( JNIEnv*  env, jclass  thiz ){

}

void
Java_cc_openframeworks_OFAndroid_onSurfaceDestroyed( JNIEnv*  env, jclass  thiz ){
	paused = true;
	ofLog(OF_LOG_NOTICE,"onSurfaceDestroyed");
	ofUnloadAllFontTextures();
	ofPauseVideoGrabbers();
	ofPushStyle();
}

void
Java_cc_openframeworks_OFAndroid_onSurfaceCreated( JNIEnv*  env, jclass  thiz ){
	ofLog(OF_LOG_NOTICE,"onSurfaceCreated");
	reloadTextures();
	if(androidApp){
		androidApp->resume();
		androidApp->reloadTextures();
	}
	ofPopStyle();
	paused = false;
}

void
Java_cc_openframeworks_OFAndroid_setup( JNIEnv*  env, jclass  thiz )
{
    //initAndroidOF();
	ofLog(OF_LOG_NOTICE,"setup");
	ofNotifySetup();
}

void
Java_cc_openframeworks_OFAndroid_resize( JNIEnv*  env, jclass  thiz, jint w, jint h )
{
    sWindowWidth  = w;
    sWindowHeight = h;
    ofLog(OF_LOG_NOTICE,"resize %i,%i",w,h);
    ofNotifyWindowResized(w,h);
}

/* Call to finalize the graphics state */
void
Java_cc_openframeworks_OFAndroid_exit( JNIEnv*  env, jclass  thiz )
{
   ofNotifyExit();
}

/* Call to render the next GL frame */
void
Java_cc_openframeworks_OFAndroid_render( JNIEnv*  env, jclass  thiz )
{
	if(paused) return;
	//LOGI("update");
	ofNotifyUpdate();


	int width, height;

	width  = sWindowWidth;
	height = sWindowHeight;

	height = height > 0 ? height : 1;
	// set viewport, clear the screen
	//glViewport( 0, 0, width, height );
	ofViewport(0, 0, width, height, false);		// used to be glViewport( 0, 0, width, height );
	float * bgPtr = ofBgColorPtr();
	bool bClearAuto = ofbClearBg();

	if ( bClearAuto == true || nFrameCount < 3){
		ofClear(bgPtr[0]*255,bgPtr[1]*255,bgPtr[2]*255, bgPtr[3]*255);
	}

	if(bSetupScreen) ofSetupScreen();
	ofNotifyDraw();

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

	nFrameCount++;		// increase the overall frame count*/


}

void
Java_cc_openframeworks_OFAndroid_onTouchDown(JNIEnv*  env, jclass  thiz, jint id,jfloat x,jfloat y,jfloat pressure){
	ofNotifyMousePressed(x,y,0);
	ofTouchEventArgs touch;
	touch.id = id;
	touch.x = x;
	touch.y = y;
	touch.pressure = pressure;
	ofNotifyEvent(ofEvents.touchDown,touch);
}

void
Java_cc_openframeworks_OFAndroid_onTouchUp(JNIEnv*  env, jclass  thiz, jint id,jfloat x,jfloat y,jfloat pressure){
	ofNotifyMouseReleased(x,y,0);
	ofTouchEventArgs touch;
	touch.id = id;
	touch.x = x;
	touch.y = y;
	touch.pressure = pressure;
	ofNotifyEvent(ofEvents.touchUp,touch);
}

void
Java_cc_openframeworks_OFAndroid_onTouchMoved(JNIEnv*  env, jclass  thiz, jint id,jfloat x,jfloat y,jfloat pressure){
	ofNotifyMouseMoved(x,y);
	ofNotifyMouseDragged(x,y,0);
	ofTouchEventArgs touch;
	touch.id = id;
	touch.x = x;
	touch.y = y;
	touch.pressure = pressure;
	ofNotifyEvent(ofEvents.touchMoved,touch);
}

void
Java_cc_openframeworks_OFAndroid_onKeyDown(JNIEnv*  env, jobject  thiz, jint  keyCode){
	if(OFApp)OFApp->keyPressed(keyCode);
	ofKeyEventArgs key;
	key.key = keyCode;
	ofNotifyEvent(ofEvents.keyPressed,key);
}

void
Java_cc_openframeworks_OFAndroid_onKeyUp(JNIEnv*  env, jobject  thiz, jint  keyCode){
	if(OFApp)OFApp->keyReleased(keyCode);
	ofKeyEventArgs key;
	key.key = keyCode;
	ofNotifyEvent(ofEvents.keyReleased,key);
}

jboolean
Java_cc_openframeworks_OFAndroid_onBackPressed(){
	ofLog(OF_LOG_NOTICE,"back pressed");
	if(androidApp) return androidApp->backPressed();
	else return false;
}
}


