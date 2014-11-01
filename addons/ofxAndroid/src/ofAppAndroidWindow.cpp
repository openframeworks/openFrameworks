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
#include "ofGLRenderer.h"
#include "ofBaseTypes.h"

static bool paused=true;
static bool surfaceDestroyed=false;


static int  sWindowWidth  = 480;
static int  sWindowHeight = 800;

static bool bSetupScreen = true;

static JavaVM *ofJavaVM=0;

static ofxAndroidApp * androidApp;
static ofAppAndroidWindow * window;

static ofOrientation orientation = OF_ORIENTATION_DEFAULT;

static queue<ofTouchEventArgs> touchEventArgsQueue;
static ofMutex mutex;
static bool threadedTouchEvents = false;
static bool appSetup = false;
static bool accumulateTouchEvents = false;


void ofExitCallback();

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




ofAppAndroidWindow::ofAppAndroidWindow()
:currentRenderer(new ofGLRenderer(this))
,glesVersion(1){
	window = this;

}

ofAppAndroidWindow::~ofAppAndroidWindow() {
	// TODO Auto-generated destructor stub
}

void ofAppAndroidWindow::setup(const ofGLESWindowSettings & settings){
	glesVersion = settings.glesVersion;
	ofLogError() << "setup gles" << glesVersion;
	if(glesVersion<2){
		currentRenderer = make_shared<ofGLRenderer>(this);
	}else{
		currentRenderer = make_shared<ofGLProgrammableRenderer>(this);
	}

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

	ofGetJNIEnv()->CallStaticVoidMethod(javaClass,method,glesVersion);
}

void ofAppAndroidWindow::update(){

}

void ofAppAndroidWindow::draw(){

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


ofCoreEvents & ofAppAndroidWindow::events(){
	return coreEvents;
}

shared_ptr<ofBaseRenderer> & ofAppAndroidWindow::renderer(){
	return currentRenderer;
}

void ofPauseVideoGrabbers();
void ofResumeVideoGrabbers();

void ofxAndroidSoundStreamPause();
void ofxAndroidSoundStreamResume();

void ofPauseVideoPlayers();
void ofResumeVideoPlayers();

void ofReloadGLResources();

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
			ofPauseVideoGrabbers();
			ofPauseVideoPlayers();
			if(androidApp){
				androidApp->unloadTextures();
			}
		}

		//ofGLReadyCallback();

		ofReloadGLResources();
		ofResumeVideoGrabbers();
		ofResumeVideoPlayers();

		if(androidApp){
			androidApp->reloadTextures();
		}
		window->renderer()->setupGraphicDefaults();
		surfaceDestroyed = false;
	}else{
	    if(window->renderer()->getType()==ofGLProgrammableRenderer::TYPE){
	    	static_cast<ofGLProgrammableRenderer*>(window->renderer().get())->setup(2,0);
	    }else{
	    	static_cast<ofGLRenderer*>(window->renderer().get())->setup();
	    }
	    ofLogNotice() << "renderer created";
	}
}

void
Java_cc_openframeworks_OFAndroid_setup( JNIEnv*  env, jclass  thiz, jint w, jint h  )
{
	ofLogNotice("ofAppAndroidWindow") << "setup";
	paused = false;
    sWindowWidth  = w;
    sWindowHeight = h;
	window->events().notifySetup();
	appSetup = true;
}

void
Java_cc_openframeworks_OFAndroid_resize( JNIEnv*  env, jclass  thiz, jint w, jint h )
{
    sWindowWidth  = w;
    sWindowHeight = h;
    ofLogNotice("ofAppAndroidWindow") << "resize " << w << "x" << h;
    window->events().notifyWindowResized(w,h);
}

/* Call to finalize the graphics state */
void
Java_cc_openframeworks_OFAndroid_exit( JNIEnv*  env, jclass  thiz )
{
	window->events().notifyExit();
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
				window->events().notifyMousePressed(events.front().x,events.front().y,0);
				ofNotifyEvent(window->events().touchDown,events.front());
				break;
			case ofTouchEventArgs::up:
				window->events().notifyMouseReleased(events.front().x,events.front().y,0);
				ofNotifyEvent(window->events().touchUp,events.front());
				break;
			case ofTouchEventArgs::move:
				window->events().notifyMouseMoved(events.front().x,events.front().y);
				window->events().notifyMouseDragged(events.front().x,events.front().y,0);
				ofNotifyEvent(window->events().touchMoved,events.front());
				break;
			case ofTouchEventArgs::doubleTap:
				ofNotifyEvent(window->events().touchDoubleTap,events.front());
				break;
			case ofTouchEventArgs::cancel:
				ofNotifyEvent(window->events().touchCancelled,events.front());
				break;
			}
			events.pop();
		}
	}

	window->events().notifyUpdate();


	window->renderer()->startRender();
	if(bSetupScreen) window->renderer()->setupScreen();
	window->events().notifyDraw();
	window->renderer()->finishRender();

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
		window->events().notifyMousePressed(x,y,0);
		ofNotifyEvent(window->events().touchDown,touch);
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
		window->events().notifyMouseReleased(x,y,0);
		ofNotifyEvent(window->events().touchUp,touch);
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
		ofNotifyEvent(window->events().touchCancelled,touch);
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
		window->events().notifyMouseMoved(x,y);
		window->events().notifyMouseDragged(x,y,0);
		ofNotifyEvent(window->events().touchMoved,touch);
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
		window->events().notifyMousePressed(x,y,0);
		ofNotifyEvent(window->events().touchDoubleTap,touch);
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
	window->events().notifyKeyPressed(keyCode);
}

void
Java_cc_openframeworks_OFAndroid_onKeyUp(JNIEnv*  env, jobject  thiz, jint  keyCode){
	window->events().notifyKeyReleased(keyCode);
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


