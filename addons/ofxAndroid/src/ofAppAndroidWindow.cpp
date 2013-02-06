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
static bool surfaceDestroyed=false;


static int  sWindowWidth  = 480;
static int  sWindowHeight = 800;

static bool bSetupScreen = true;

static float frameRate = 60;

static int frames = 0;
static unsigned long onesec = 0;
static unsigned long previousFrameMicros = 0;
static int nFrameCount = 0;
static float targetRate = 60;
static unsigned long oneFrameTime = 0;
static bool bFrameRateSet = false;

static double			lastFrameTime;

static JavaVM *ofJavaVM=0;

static ofxAndroidApp * androidApp;

static ofOrientation orientation = OF_ORIENTATION_DEFAULT;

static queue<ofTouchEventArgs> touchEventArgsQueue;
static ofMutex mutex;
static bool threadedTouchEvents = false;


void ofExitCallback();

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
	return ofGetJNIEnv()->FindClass("cc/openframeworks/OFAndroid");
}

jobject ofGetOFActivityObject(){
	JNIEnv * env = ofGetJNIEnv();
	if(!env) return NULL;

	jclass OFAndroid = ofGetJavaOFAndroid();
	if(!OFAndroid) return NULL;

	jfieldID ofActivityID = env->GetStaticFieldID(OFAndroid,"ofActivity","Landroid/app/Activity;");
	if(!ofActivityID){
		ofLogError() << "Failed to get field ID for ofActivity";
		return NULL;
	}

	return env->GetStaticObjectField(OFAndroid,ofActivityID);
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

void ofAppAndroidWindow::runAppViaInfiniteLoop(ofBaseApp * appPtr){
	androidApp = dynamic_cast<ofxAndroidApp*>( appPtr );
	if(androidApp){
		ofxRegisterMultitouch(androidApp);
	}
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
	jclass javaClass = ofGetJNIEnv()->FindClass("cc/openframeworks/OFAndroid");

	if(javaClass==0){
		ofLog(OF_LOG_ERROR,"setOrientation: cannot find OFAndroid java class");
		return;
	}

	jmethodID setScreenOrientation = ofGetJNIEnv()->GetStaticMethodID(javaClass,"setScreenOrientation","(I)V");
	if(!setScreenOrientation){
		ofLog(OF_LOG_ERROR,"cannot find OFAndroid setScreenOrientation method");
		return;
	}
	if(orientation==OF_ORIENTATION_UNKNOWN)
		ofGetJNIEnv()->CallStaticVoidMethod(javaClass,setScreenOrientation,-1);
	else
		ofGetJNIEnv()->CallStaticVoidMethod(javaClass,setScreenOrientation,ofOrientationToDegrees(orientation));
}

ofOrientation ofAppAndroidWindow::getOrientation(){
	return orientation;
}

void ofAppAndroidWindow::setFullscreen(bool fullscreen){
	jclass javaClass = ofGetJNIEnv()->FindClass("cc/openframeworks/OFAndroid");

	if(javaClass==0){
		ofLog(OF_LOG_ERROR,"setFullscreen: cannot find OFAndroid java class");
		return;
	}

	jmethodID setFullscreen = ofGetJNIEnv()->GetStaticMethodID(javaClass,"setFullscreen","(Z)V");
	if(!setFullscreen){
		ofLog(OF_LOG_ERROR,"cannot find OFAndroid setFullscreen method");
		return;
	}
	ofGetJNIEnv()->CallStaticVoidMethod(javaClass,setFullscreen,fullscreen);
}

void ofAppAndroidWindow::toggleFullscreen(){

}

void ofAppAndroidWindow::setFrameRate(float _targetRate){
	targetRate = _targetRate;
	oneFrameTime = 1000000.f/targetRate;
	bFrameRateSet = true;
}

void ofAppAndroidWindow::setThreadedEvents(bool threadedEvents){
	threadedTouchEvents = threadedEvents;
}

void reloadTextures(){
	ofUpdateBitmapCharacterTexture();
	ofReloadAllImageTextures();
	ofReloadAllFontTextures();
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
		ofFile resources(resources_name);
		if(resources.exists()){
			__android_log_print(ANDROID_LOG_DEBUG,"OF",("uncompressing " + resources.getAbsolutePath()).c_str());
			unzFile zip = unzOpen(resources.getAbsolutePath().c_str());
			char current_dir[1000];
			getcwd(current_dir,1000);
			chdir(ofToDataPath("",true).c_str());
			do_extract(zip,0,1,NULL);
			chdir(current_dir);

			resources.remove();
		}else{
			__android_log_print(ANDROID_LOG_DEBUG,"OF",("no resources found in " + resources.getAbsolutePath()).c_str());
		}
    }
}

void
Java_cc_openframeworks_OFAndroid_onRestart( JNIEnv*  env, jobject  thiz ){

}

void
Java_cc_openframeworks_OFAndroid_onPause( JNIEnv*  env, jobject  thiz ){
	paused = true;
	ofxAndroidSoundStreamPause();
	if(androidApp) androidApp->pause();
}

void
Java_cc_openframeworks_OFAndroid_onResume( JNIEnv*  env, jobject  thiz ){
	if(paused){
		if(androidApp){
			androidApp->resume();
		}
		ofxAndroidSoundStreamResume();

		paused = false;
	}
}

void
Java_cc_openframeworks_OFAndroid_onStop( JNIEnv*  env, jobject  thiz ){

}


void
Java_cc_openframeworks_OFAndroid_onDestroy( JNIEnv*  env, jclass  thiz ){
	ofExitCallback();
}

void
Java_cc_openframeworks_OFAndroid_onSurfaceDestroyed( JNIEnv*  env, jclass  thiz ){
	surfaceDestroyed = true;
	ofLog(OF_LOG_NOTICE,"onSurfaceDestroyed");
	ofUnloadAllFontTextures();
	ofPauseVideoGrabbers();
	if(androidApp){
		androidApp->unloadTextures();
	}
}

void
Java_cc_openframeworks_OFAndroid_onSurfaceCreated( JNIEnv*  env, jclass  thiz ){
	ofLog(OF_LOG_NOTICE,"onSurfaceCreated");
	if(!surfaceDestroyed){
		ofUnloadAllFontTextures();
		ofPauseVideoGrabbers();
		if(androidApp){
			androidApp->unloadTextures();
		}
	}
	reloadTextures();
	if(androidApp){
		androidApp->reloadTextures();
	}
	ofSetStyle(ofGetStyle());
	surfaceDestroyed = false;

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
	unsigned long beginFrameMicros = ofGetElapsedTimeMicros();

	if(paused || surfaceDestroyed) return;

	lastFrameTime = double(beginFrameMicros - previousFrameMicros)/1000000.;

	previousFrameMicros = beginFrameMicros;

	if(!threadedTouchEvents){
		mutex.lock();
		while(!touchEventArgsQueue.empty()){
			switch(touchEventArgsQueue.front().type){
			case ofTouchEventArgs::down:
				ofNotifyMousePressed(touchEventArgsQueue.front().x,touchEventArgsQueue.front().y,0);
				ofNotifyEvent(ofEvents().touchDown,touchEventArgsQueue.front());
				break;
			case ofTouchEventArgs::up:
				ofNotifyMouseReleased(touchEventArgsQueue.front().x,touchEventArgsQueue.front().y,0);
				ofNotifyEvent(ofEvents().touchUp,touchEventArgsQueue.front());
				break;
			case ofTouchEventArgs::move:
				ofNotifyMouseMoved(touchEventArgsQueue.front().x,touchEventArgsQueue.front().y);
				ofNotifyMouseDragged(touchEventArgsQueue.front().x,touchEventArgsQueue.front().y,0);
				ofNotifyEvent(ofEvents().touchMoved,touchEventArgsQueue.front());
				break;
			case ofTouchEventArgs::doubleTap:
				ofNotifyEvent(ofEvents().touchDoubleTap,touchEventArgsQueue.front());
				break;
			case ofTouchEventArgs::cancel:
				ofNotifyEvent(ofEvents().touchCancelled,touchEventArgsQueue.front());
				break;
			}
			touchEventArgsQueue.pop();
		}
		mutex.unlock();
	}

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

	unsigned long currTime = ofGetElapsedTimeMicros();
	unsigned long frameMicros = currTime - beginFrameMicros;

	nFrameCount++;		// increase the overall frame count*/
	frames++;

	if(currTime - onesec>=1000000){
		frameRate = frames;
		frames = 0;
		onesec = currTime;
	}

	if(bFrameRateSet && frameMicros<oneFrameTime) usleep(oneFrameTime-frameMicros);

}

void
Java_cc_openframeworks_OFAndroid_onTouchDown(JNIEnv*  env, jclass  thiz, jint id,jfloat x,jfloat y,jfloat pressure){
	ofTouchEventArgs touch;
	touch.id = id;
	touch.x = x;
	touch.y = y;
	touch.pressure = pressure;
	touch.type = ofTouchEventArgs::down;
	if(threadedTouchEvents){
		ofNotifyMousePressed(x,y,0);
		ofNotifyEvent(ofEvents().touchDown,touch);
	}else{
		mutex.lock();
		touchEventArgsQueue.push(touch);
		mutex.unlock();
	}
}

void
Java_cc_openframeworks_OFAndroid_onTouchUp(JNIEnv*  env, jclass  thiz, jint id,jfloat x,jfloat y,jfloat pressure){
	ofTouchEventArgs touch;
	touch.id = id;
	touch.x = x;
	touch.y = y;
	touch.pressure = pressure;
	touch.type = ofTouchEventArgs::up;
	if(threadedTouchEvents){
		ofNotifyMouseReleased(x,y,0);
		ofNotifyEvent(ofEvents().touchUp,touch);
	}else{
		mutex.lock();
		touchEventArgsQueue.push(touch);
		mutex.unlock();
	}
}

void
Java_cc_openframeworks_OFAndroid_onTouchCancelled(JNIEnv*  env, jclass  thiz, jint id,jfloat x,jfloat y){
	ofTouchEventArgs touch;
	touch.id = id;
	touch.x = x;
	touch.y = y;
	touch.pressure = 0;
	touch.type = ofTouchEventArgs::cancel;
	if(threadedTouchEvents){
		ofNotifyEvent(ofEvents().touchCancelled,touch);
	}else{
		mutex.lock();
		touchEventArgsQueue.push(touch);
		mutex.unlock();
	}
}

void
Java_cc_openframeworks_OFAndroid_onTouchMoved(JNIEnv*  env, jclass  thiz, jint id,jfloat x,jfloat y,jfloat pressure){
	ofTouchEventArgs touch;
	touch.id = id;
	touch.x = x;
	touch.y = y;
	touch.pressure = pressure;
	touch.type = ofTouchEventArgs::move;
	if(threadedTouchEvents){
		ofNotifyMouseMoved(x,y);
		ofNotifyMouseDragged(x,y,0);
		ofNotifyEvent(ofEvents().touchMoved,touch);
	}else{
		mutex.lock();
		touchEventArgsQueue.push(touch);
		mutex.unlock();
	}
}

void
Java_cc_openframeworks_OFAndroid_onTouchDoubleTap(JNIEnv*  env, jclass  thiz, jint id,jfloat x,jfloat y,jfloat pressure){
	ofTouchEventArgs touch;
	touch.id = id;
	touch.x = x;
	touch.y = y;
	touch.pressure = pressure;
	touch.type = ofTouchEventArgs::doubleTap;
	if(threadedTouchEvents){
		ofNotifyMousePressed(x,y,0);
		ofNotifyEvent(ofEvents().touchDoubleTap,touch);
	}else{
		mutex.lock();
		touchEventArgsQueue.push(touch);
		mutex.unlock();
	}
}

void
Java_cc_openframeworks_OFAndroid_onSwipe(JNIEnv*  env, jclass  thiz, jint id, jint swipeDir){
	if(androidApp){
		androidApp->swipe((ofxAndroidSwipeDir)swipeDir,id);
	}
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
	if(androidApp && menu_id_str) return androidApp->menuItemChecked(menu_id_str,checked);
	else return false;
}

void
Java_cc_openframeworks_OFAndroid_okPressed( JNIEnv*  env, jobject  thiz ){
	if(androidApp) androidApp->okPressed();
	bool yes = true;
	ofNotifyEvent(ofxAndroidEvents().okPressed,yes);
}

void
Java_cc_openframeworks_OFAndroid_cancelPressed( JNIEnv*  env, jobject  thiz ){
	if(androidApp) androidApp->cancelPressed();
	bool yes = true;
	ofNotifyEvent(ofxAndroidEvents().cancelPressed,yes);
}

void
Java_cc_openframeworks_OFAndroid_networkConnected( JNIEnv*  env, jobject  thiz, jboolean connected){
	bool bConnected = (bool)connected;
	if(androidApp) androidApp->networkConnected(bConnected);
	ofNotifyEvent(ofxAndroidEvents().networkConnected,bConnected);
}
}


