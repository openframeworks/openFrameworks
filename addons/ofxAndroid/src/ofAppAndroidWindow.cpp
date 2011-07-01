/*
 * ofAppAndroidWindow.cpp
 *
 *  Created on: 24/05/2010
 *      Author: arturo
 */

#include "ofAppAndroidWindow.h"

extern "C"{
#include "unzip.h"
#include "miniunz.h"
}

#include <jni.h>
#include "ofGraphics.h"
#include "ofAppRunner.h"
#include "ofUtils.h"
#include "ofxAndroidApp.h"
#include "ofxAndroidUtils.h"
#include "ofxAccelerometer.h"
#include <android/log.h>
#include "ofFileUtils.h"

static bool paused=true;


static int  sWindowWidth  = 480;
static int  sWindowHeight = 800;
/*static int  sDemoStopped  = 0;
static long sTimeOffset   = 0;
static int  sTimeOffsetInit = 0;
static long sTimeStopped  = 0;*/

static bool bSetupScreen = true;

static float frameRate = 60;

static int frames = 0;
static int onesec = 0;
static int previousFrameMillis = 0;
static int nFrameCount = 0;
static float targetRate = 60;
static int oneFrameTime = 0;
static bool bFrameRateSet = false;

static double			lastFrameTime;

static JavaVM *ofJavaVM=0;


static ofPtr<ofBaseApp> OFApp;
static ofxAndroidApp * androidApp;

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

/*void ofRunApp( ofxAndroidApp * app){
	androidApp = app;
	ofRunApp((ofBaseApp*)app);
}

void ofRunApp( ofPtr<ofxAndroidApp> app){
	androidApp = app;
	ofRunApp((ofBaseApp*)app);
}*/

void ofxRegisterMultitouch(ofxAndroidApp * app){
	ofRegisterTouchEvents(app);
}




ofAppAndroidWindow::ofAppAndroidWindow() {
	// TODO Auto-generated constructor stub

}

ofAppAndroidWindow::~ofAppAndroidWindow() {
	// TODO Auto-generated destructor stub
}

void ofAppAndroidWindow::runAppViaInfiniteLoop(ofPtr<ofBaseApp> appPtr){
	androidApp = dynamic_cast<ofxAndroidApp*>( appPtr.get() );
	OFApp = appPtr;
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
	//if(orientation==_orientation) return;
	orientation = _orientation;
	jclass javaClass = ofGetJNIEnv()->FindClass("cc.openframeworks.OFAndroid");

	if(javaClass==0){
		ofLog(OF_LOG_ERROR,"setOrientation: cannot find OFAndroid java class");
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

void ofAppAndroidWindow::setFullscreen(bool fullscreen){
	jclass javaClass = ofGetJNIEnv()->FindClass("cc.openframeworks.OFAndroid");

	if(javaClass==0){
		ofLog(OF_LOG_ERROR,"setFullscreen: cannot find OFAndroid java class");
		return;
	}

	jmethodID setFullscreen = ofGetJNIEnv()->GetStaticMethodID(javaClass,"setFullscreen","(Z)V");
	if(!setFullscreen){
		ofLog(OF_LOG_ERROR,"cannot find OFAndroid setFullscreen method");
		return;
	}
	ofGetJNIEnv()->CallStaticObjectMethod(javaClass,setFullscreen,fullscreen);
}

void ofAppAndroidWindow::toggleFullscreen(){

}

void ofAppAndroidWindow::setFrameRate(float _targetRate){
	targetRate = _targetRate;
	oneFrameTime = 1000.f/targetRate;
	bFrameRateSet = true;
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
Java_cc_openframeworks_OFAndroid_setAppDataDir( JNIEnv*  env, jobject  thiz, jstring data_dir, jstring app_name )
{
	jboolean iscopy;
	const char *mfile = env->GetStringUTFChars(data_dir, &iscopy);
	__android_log_print(ANDROID_LOG_INFO,"OF",("Setting app dir name to: " + string(mfile)).c_str());
    ofSetDataPathRoot(string(mfile)+"/");
    string appname = env->GetStringUTFChars(app_name, &iscopy);
    __android_log_print(ANDROID_LOG_INFO,"OF",("app name: " + appname).c_str());
    if(appname!=""){
		string resources_name = ofToLower(appname + "resources.zip");
		__android_log_print(ANDROID_LOG_INFO,"OF",("uncompressing " + resources_name).c_str());
		ofFile resources(resources_name);
		__android_log_print(ANDROID_LOG_INFO,"OF",("uncompressing " + resources.getAbsolutePath()).c_str());
		if(resources.exists()){
			unzFile zip = unzOpen(resources.getAbsolutePath().c_str());
			char current_dir[1000];
			getcwd(current_dir,1000);
			chdir(ofToDataPath("",true).c_str());
			do_extract(zip,0,1,NULL);
			chdir(current_dir);

			resources.remove();
		}
    }
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
	ofxAndroidSoundStreamResume();
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
Java_cc_openframeworks_OFAndroid_setup( JNIEnv*  env, jclass  thiz, jint w, jint h  )
{
    //initAndroidOF();
	ofLog(OF_LOG_NOTICE,"setup");
	paused = false;
    sWindowWidth  = w;
    sWindowHeight = h;
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
	int beginFrameMillis = ofGetElapsedTimeMillis();

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

	/*timeNow = ofGetElapsedTimef();
	double diff = timeNow-timeThen;
	if( diff  > 0.00001 ){
		fps			= 1.0 / diff;
		frameRate	*= 0.9f;
		frameRate	+= 0.1f*fps;
	 }
	 lastFrameTime	= diff;
	 timeThen		= timeNow;*/
	// --------------

	int currTime = ofGetElapsedTimeMillis();
	if(currTime - onesec>=1000){
		frameRate = frames;
		frames = 0;
		onesec = currTime;
	}
	frames++;
	int frameMillis = currTime - beginFrameMillis;
	lastFrameTime = double(frameMillis)/1000.;

	previousFrameMillis = currTime;

	nFrameCount++;		// increase the overall frame count*/

	if(bFrameRateSet && frameMillis<oneFrameTime) ofSleepMillis(oneFrameTime-frameMillis);

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
Java_cc_openframeworks_OFAndroid_onTouchDoubleTap(JNIEnv*  env, jclass  thiz, jint id,jfloat x,jfloat y,jfloat pressure){
	ofNotifyMousePressed(x,y,0);
	ofTouchEventArgs touch;
	touch.id = id;
	touch.x = x;
	touch.y = y;
	touch.pressure = pressure;
	ofNotifyEvent(ofEvents.touchDoubleTap,touch);
}

void
Java_cc_openframeworks_OFAndroid_onKeyDown(JNIEnv*  env, jobject  thiz, jint  keyCode){
	ofNotifyKeyPressed(keyCode);
}

void
Java_cc_openframeworks_OFAndroid_onKeyUp(JNIEnv*  env, jobject  thiz, jint  keyCode){
	ofNotifyKeyReleased(keyCode);
}

jboolean
Java_cc_openframeworks_OFAndroid_onBackPressed(){
	ofLog(OF_LOG_VERBOSE,"back pressed");
	if(androidApp) return androidApp->backPressed();
	else return false;
}

jboolean
Java_cc_openframeworks_OFAndroid_onMenuItemSelected( JNIEnv*  env, jobject  thiz, jstring menu_id){
	jboolean iscopy;
	const char *menu_id_str = env->GetStringUTFChars(menu_id, &iscopy);
	if(androidApp) return androidApp->menuItemSelected(menu_id_str);
	else return false;
}

jboolean
Java_cc_openframeworks_OFAndroid_onMenuItemChecked( JNIEnv*  env, jobject  thiz, jstring menu_id, jboolean checked){
	jboolean iscopy;
	const char *menu_id_str = env->GetStringUTFChars(menu_id, &iscopy);
	if(androidApp) return androidApp->menuItemChecked(menu_id_str,checked);
	else return false;
}

void
Java_cc_openframeworks_OFAndroid_okPressed( JNIEnv*  env, jobject  thiz ){
	if(androidApp) androidApp->okPressed();
}

void
Java_cc_openframeworks_OFAndroid_cancelPressed( JNIEnv*  env, jobject  thiz ){
	if(androidApp) androidApp->cancelPressed();
}
}


