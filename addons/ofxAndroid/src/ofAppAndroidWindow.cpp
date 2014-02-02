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
#include "ofFileUtils.h"
#include "ofGLProgrammableRenderer.h"

static bool paused=true;
static bool surfaceDestroyed=false;


static int  sWindowWidth  = 480;
static int  sWindowHeight = 800;

static bool bSetupScreen = true;

static JavaVM *ofJavaVM=0;

static ofxAndroidApp * androidApp;

static ofOrientation orientation = OF_ORIENTATION_DEFAULT;

static queue<ofTouchEventArgs> touchEventArgsQueue;
static ofMutex mutex;
static bool threadedTouchEvents = false;
static bool appSetup = false;
static bool accumulateTouchEvents = false;


void ofExitCallback();
void ofGLReadyCallback();

//static ofAppAndroidWindow window;

JavaVM * ofGetJavaVMPtr(){
	return ofJavaVM;
}

JNIEnv * ofGetJNIEnv(){
	JNIEnv *env;
	JavaVM * vm = ofGetJavaVMPtr();
	if(!vm){
		ofLogError("ofAppAndroidWindow") << "couldn't get java virtual machine";
		return NULL;
	}
	if (vm->GetEnv((void**) &env, JNI_VERSION_1_4) != JNI_OK) {
		ofLogError("ofAppAndroidWindow") << "couldn't get environment using GetEnv()";
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

	jfieldID ofActivityID = env->GetStaticFieldID(OFAndroid,"ofActivity","Lcc/openframeworks/OFActivity;");
	if(!ofActivityID){
		ofLogError("ofAppAndroidWindow") << "couldn't get field ID for ofActivity";
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

void ofAppAndroidWindow::setupOpenGL(int w, int h, int screenMode){
	jclass javaClass = ofGetJNIEnv()->FindClass("cc/openframeworks/OFAndroid");

	if(javaClass==0){
		ofLogError("ofAppAndroidWindow") << "setupOpenGL(): couldn't find OFAndroid java class";
		return;
	}

	jmethodID method = ofGetJNIEnv()->GetStaticMethodID(javaClass,"setupGL","(I)V");
	if(!method){
		ofLogError("ofAppAndroidWindow") << "setupOpenGL(): couldn't find OFAndroid setupGL method";
		return;
	}

	if(ofGetGLProgrammableRenderer())
		ofGetJNIEnv()->CallStaticVoidMethod(javaClass,method,2);
	else
		ofGetJNIEnv()->CallStaticVoidMethod(javaClass,method,1);
}

void ofAppAndroidWindow::runAppViaInfiniteLoop(ofBaseApp * appPtr){
	androidApp = dynamic_cast<ofxAndroidApp*>( appPtr );
	if(androidApp){
		ofRegisterTouchEvents(androidApp);
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
		ofLogError("ofAppAndroidWindow") << "setOrientation(): couldn't find OFAndroid java class";
		return;
	}

	jmethodID setScreenOrientation = ofGetJNIEnv()->GetStaticMethodID(javaClass,"setScreenOrientation","(I)V");
	if(!setScreenOrientation){
		ofLogError("ofAppAndroidWindow") << "setOrientation(): couldn't find OFAndroid setScreenOrientation method";
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
		ofLogError("ofAppAndroidWindow") << "setFullscreen(): couldn't find OFAndroid java class";
		return;
	}

	jmethodID setFullscreen = ofGetJNIEnv()->GetStaticMethodID(javaClass,"setFullscreen","(Z)V");
	if(!setFullscreen){
		ofLogError("ofAppAndroidWindow") << "setFullScreen(): couldn't find OFAndroid setFullscreen method";
		return;
	}
	ofGetJNIEnv()->CallStaticVoidMethod(javaClass,setFullscreen,fullscreen);
}

void ofAppAndroidWindow::toggleFullscreen(){

}

void ofAppAndroidWindow::setThreadedEvents(bool threadedEvents){
	threadedTouchEvents = threadedEvents;
}


void ofAppAndroidWindow::setAccumulateTouchEvents(bool accumEvents){
	accumulateTouchEvents = accumEvents;
}

void ofPauseVideoGrabbers();
void ofResumeVideoGrabbers();

void ofxAndroidSoundStreamPause();
void ofxAndroidSoundStreamResume();

void ofPauseVideoPlayers();
void ofResumeVideoPlayers();

void ofReloadGLResources();
void ofUnloadAllFontTextures();

extern "C"{

jint JNI_OnLoad(JavaVM* vm, void* reserved)
{
	JNIEnv *env;
	ofJavaVM = vm;
	ofLog(OF_LOG_NOTICE,"JNI_OnLoad called");
	if (vm->GetEnv((void**) &env, JNI_VERSION_1_4) != JNI_OK) {
		ofLogError("ofAppAndroidWindow") << "failed to get environment using GetEnv()";
		return -1;
	}
	return JNI_VERSION_1_4;
}

void
Java_cc_openframeworks_OFAndroid_setAppDataDir( JNIEnv*  env, jobject  thiz, jstring data_dir)
{
	jboolean iscopy;
	const char *mfile = env->GetStringUTFChars(data_dir, &iscopy);
	__android_log_print(ANDROID_LOG_INFO,"ofAppAndroidWindow",("setting app dir name to: \"" + string(mfile) + "\"").c_str());
    ofSetDataPathRoot(string(mfile)+"/");
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
	ofLogNotice("ofAppAndroidWindow") << "onResume";
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
	ofLogNotice("ofAppAndroidWindow") << "onSurfaceDestroyed";
	ofUnloadAllFontTextures();
	ofPauseVideoGrabbers();
	if(androidApp){
		androidApp->unloadTextures();
	}
}

void
Java_cc_openframeworks_OFAndroid_onSurfaceCreated( JNIEnv*  env, jclass  thiz ){
	if(appSetup){
		ofLogNotice("ofAppAndroidWindow") << "onSurfaceCreated";
		if(!surfaceDestroyed){
			ofUnloadAllFontTextures();
			ofPauseVideoGrabbers();
			ofPauseVideoPlayers();
			if(androidApp){
				androidApp->unloadTextures();
			}
		}

		ofGLReadyCallback();

		ofReloadGLResources();
		ofResumeVideoGrabbers();
		ofResumeVideoPlayers();

		if(androidApp){
			androidApp->reloadTextures();
		}
		ofSetStyle(ofGetStyle());
		surfaceDestroyed = false;
	}else{
		ofGLReadyCallback();
	}

}

void
Java_cc_openframeworks_OFAndroid_setup( JNIEnv*  env, jclass  thiz, jint w, jint h  )
{
	ofLogNotice("ofAppAndroidWindow") << "setup";
	paused = false;
    sWindowWidth  = w;
    sWindowHeight = h;
	ofNotifySetup();
	appSetup = true;
}

void
Java_cc_openframeworks_OFAndroid_resize( JNIEnv*  env, jclass  thiz, jint w, jint h )
{
    sWindowWidth  = w;
    sWindowHeight = h;
    ofLogNotice("ofAppAndroidWindow") << "resize " << w << "x" << h;
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

	if(paused || surfaceDestroyed) return;

	if(!threadedTouchEvents){
		mutex.lock();
		queue<ofTouchEventArgs> events = touchEventArgsQueue;
		while(!touchEventArgsQueue.empty()) touchEventArgsQueue.pop();
		mutex.unlock();

		while(!events.empty()){
			switch(events.front().type){
			case ofTouchEventArgs::down:
				ofNotifyMousePressed(events.front().x,events.front().y,0);
				ofNotifyEvent(ofEvents().touchDown,events.front());
				break;
			case ofTouchEventArgs::up:
				ofNotifyMouseReleased(events.front().x,events.front().y,0);
				ofNotifyEvent(ofEvents().touchUp,events.front());
				break;
			case ofTouchEventArgs::move:
				ofNotifyMouseMoved(events.front().x,events.front().y);
				ofNotifyMouseDragged(events.front().x,events.front().y,0);
				ofNotifyEvent(ofEvents().touchMoved,events.front());
				break;
			case ofTouchEventArgs::doubleTap:
				ofNotifyEvent(ofEvents().touchDoubleTap,events.front());
				break;
			case ofTouchEventArgs::cancel:
				ofNotifyEvent(ofEvents().touchCancelled,events.front());
				break;
			}
			events.pop();
		}
	}

	ofNotifyUpdate();


	if(ofGetGLProgrammableRenderer()){
		ofGetGLProgrammableRenderer()->startRender();
	}
	int width, height;

	width  = sWindowWidth;
	height = sWindowHeight;

	height = height > 0 ? height : 1;
	// set viewport, clear the screen
	//glViewport( 0, 0, width, height );
	ofViewport(0, 0, width, height, false);		// used to be glViewport( 0, 0, width, height );
	float * bgPtr = ofBgColorPtr();
	bool bClearAuto = ofbClearBg();

	if ( bClearAuto == true || ofGetFrameNum() < 3){
		ofClear(bgPtr[0]*255,bgPtr[1]*255,bgPtr[2]*255, bgPtr[3]*255);
	}

	if(bSetupScreen) ofSetupScreen();
	ofNotifyDraw();

	if(ofGetGLProgrammableRenderer()){
		ofGetGLProgrammableRenderer()->finishRender();
	}

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
		if(accumulateTouchEvents && !touchEventArgsQueue.empty() && touchEventArgsQueue.back().type==ofTouchEventArgs::move){
			touchEventArgsQueue.back() = touch;
		}else{
			touchEventArgsQueue.push(touch);
		}
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
	ofLogVerbose("ofAppAndroidWindow") << "back pressed";
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


