/*
 * ofAppAndroidWindow.cpp
 *
 *  Created on: 24/05/2010
 *      Author: arturo
 */

#include "ofAppAndroidWindow.h"

#include <jni.h>
#include <queue>
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
using namespace std;

static bool stopped=true;
static bool surfaceDestroyed=false;


static int  sWindowWidth  = 480;
static int  sWindowHeight = 800;

static bool bSetupScreen = true;

static JavaVM *ofJavaVM=nullptr;

static ofAppAndroidWindow * window;

static ofOrientation orientation = OF_ORIENTATION_DEFAULT;

static queue<ofTouchEventArgs> touchEventArgsQueue;
static queue<ofKeyEventArgs> keyEventArgsQueue;
static queue<ofKeyEventArgs> axisEventArgsQueue;

static std::mutex mtx, keyMtx, controllerMtx;

static bool appSetup = false;

static bool threadedKeyEvents = false;
static bool threadedTouchEvents = false;
static bool threadedAxisEvents = false;

static bool accumulateTouchEvents = false;
static bool accumulateAxisEvents = false;

static bool multiWindowMode = false;

AAssetManager* assetManager;

void ofExitCallback();

//----- define in main.cpp---//
//JNIEXPORT void JNICALL ofAndroidApplicationInit();
//JNIEXPORT void JNICALL ofAndroidActivityInit();

//static ofAppAndroidWindow window;
JNIEXPORT JavaVM * JNICALL
ofGetJavaVMPtr(){
	return ofJavaVM;
}

JNIEXPORT JNIEnv * JNICALL
 ofGetJNIEnv(){
	JNIEnv *env;
	JavaVM * vm = ofGetJavaVMPtr();
	if(!vm){
		ofLogError("ofAppAndroidWindow") << "couldn't get java virtual machine";
		return nullptr;
	}

	int getEnvStat = vm->GetEnv((void**) &env, JNI_VERSION_1_6);

	if (getEnvStat == JNI_EDETACHED) {

		if (vm->AttachCurrentThread(&env, nullptr) != 0) {
			ofLogError("ofAppAndroidWindow") << "couldn't get environment using GetEnv()";
			return nullptr;
		}
	} else if (getEnvStat != JNI_OK) {
		ofLogError("ofAppAndroidWindow") << "couldn't get environment using GetEnv()";
		return nullptr;
	}

	return env;
}

jclass ofGetJavaOFAndroid(){
	return ofGetJNIEnv()->FindClass("cc/openframeworks/OFAndroid");
}

jclass ofGetOFLifeCycle(){
	return ofGetJNIEnv()->FindClass("cc/openframeworks/OFAndroidLifeCycle");
}

jobject ofGetOFActivityObject(){
	JNIEnv * env = ofGetJNIEnv();
	if(!env) return nullptr;

	jclass OFLifeCycle = ofGetOFLifeCycle();
	if(!OFLifeCycle) return nullptr;

	jfieldID ofActivityID = env->GetStaticFieldID(OFLifeCycle,"m_activity","Lcc/openframeworks/OFActivity;");
	if(!ofActivityID){
		ofLogError("ofAppAndroidWindow") << "couldn't get field ID for ofActivity";
		return nullptr;
	}

	return env->GetStaticObjectField(OFLifeCycle,ofActivityID);
}


ofAppAndroidWindow::ofAppAndroidWindow()  {
	//ofLogError("ofAppAndroidWindow") << "ofAppAndroidWindow() the window is this";
	window = this;
	msaaSamples = 1;
	glesVersion = 2;

	ofGetMainLoop()->setCurrentWindow(this);
}

ofAppAndroidWindow::ofAppAndroidWindow(ofAppBaseWindow & other) {
	window = this;
	msaaSamples = 1;
	glesVersion = 2;
	setMultiWindowMode(other.getWindowMode());
	ofGetMainLoop()->setCurrentWindow(this);
}

void ofAppAndroidWindow::setCurrentWindow() {
	ofLogVerbose("ofAppAndroidWindow") << "setCurrentWindow the window is this";
	window = this;
	ofGetMainLoop()->setCurrentWindow(this);
}

void ofAppAndroidWindow::setSampleSize(int samples) {
	msaaSamples = samples;
}

int	ofAppAndroidWindow::getSamples() {
	return msaaSamples;
}

AAssetManager&	ofAppAndroidWindow::getAssetManager() {
	return *assetManager;
}

void	ofAppAndroidWindow::setAssetManager(AAssetManager* aaAssetManager) {
	 assetManager = aaAssetManager;
}

ofAppAndroidWindow::~ofAppAndroidWindow() {
	if(window != nullptr && window == this) {
		window = nullptr;
		ofLogError("ofAppAndroidWindow") << "~ofAppAndroidWindow destroyed";
	}
	// TODO Auto-generated destructor stub
}

bool ofAppAndroidWindow::isSurfaceDestroyed() {
	return surfaceDestroyed;
}

void ofAppAndroidWindow::setup(const ofGLESWindowSettings & settings)
{
	setup( (const ofxAndroidWindowSettings &)settings );
}

void ofAppAndroidWindow::setup(const ofxAndroidWindowSettings & settings){

	if(window == nullptr) {
		ofLogError("ofAppAndroidWindow") << "Setup and Window is nullptr ! Fixing";
		setCurrentWindow();
	}
	glesVersion = settings.glesVersion;
	ofLogError("ofAppAndroidWindow") << "Setup OpenGLES:" << glesVersion;
	if(glesVersion<2){
		currentRenderer = std::make_shared<ofGLRenderer>(this);
	}else{
		currentRenderer = std::make_shared<ofGLProgrammableRenderer>(this);
	}

	jclass javaClass = ofGetJNIEnv()->FindClass("cc/openframeworks/OFAndroid");

	if(javaClass==nullptr){
		ofLogError("ofAppAndroidWindow") << "setupOpenGL(): couldn't find OFAndroid java class";
		return;
	}

	makeCurrent();

	jmethodID method = ofGetJNIEnv()->GetStaticMethodID(javaClass,"setupGL","(IZ)V");
	if(!method){
		ofLogError("ofAppAndroidWindow") << "setupOpenGL(): couldn't find OFAndroid setupGL method";
		return;
	}

	ofGetJNIEnv()->CallStaticVoidMethod(javaClass,method,glesVersion,settings.preserveContextOnPause);


}

void ofAppAndroidWindow::update(){

}

void ofAppAndroidWindow::draw(){

}

glm::vec2 ofAppAndroidWindow::getWindowSize(){
	return glm::vec2(sWindowWidth,sWindowHeight);
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

	if(javaClass==nullptr){
		ofLogError("ofAppAndroidWindow") << "setOrientation(): couldn't find OFAndroid java class";
		return;
	}

    bSetupScreen = true;

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


void ofAppAndroidWindow::makeCurrent(){
	shared_ptr<ofMainLoop> mainLoop = ofGetMainLoop();
	if(mainLoop){
		mainLoop->setCurrentWindow(this);
	}
}

void ofAppAndroidWindow::setFullscreen(bool fullscreen){
	jclass javaClass = ofGetJNIEnv()->FindClass("cc/openframeworks/OFAndroid");

	if(javaClass==nullptr){
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
	threadedKeyEvents = threadedEvents;
	threadedAxisEvents = threadedEvents;
}


void ofAppAndroidWindow::setAccumulateTouchEvents(bool accumEvents){
	accumulateTouchEvents = accumEvents;
}

void ofAppAndroidWindow::setMultiWindowMode(bool multiWindow) {
	multiWindowMode = multiWindow;
}

bool ofAppAndroidWindow::getIsThreadedEvents() {
	return threadedTouchEvents;
}
bool ofAppAndroidWindow::getIsAccumulateTouchEvents() {
	return accumulateTouchEvents;
}

bool ofAppAndroidWindow::getIsMultiWindowMode() {
	return multiWindowMode;
}


ofCoreEvents & ofAppAndroidWindow::events(){
	return coreEvents;
}

shared_ptr<ofBaseRenderer> & ofAppAndroidWindow::renderer(){
	return currentRenderer;
}

int ofAppAndroidWindow::getGlesVersion()
{
	return glesVersion;
}


extern "C"{

JNIEXPORT jint JNICALL
JNI_OnLoad(JavaVM* vm, void* reserved)
{
	JNIEnv *env;
	ofJavaVM = vm;
	ofLogVerbose() << "JNI_OnLoad called";
	if (vm->GetEnv((void**) &env, JNI_VERSION_1_4) != JNI_OK) {
		ofLogError("ofAppAndroidWindow") << "failed to get environment using GetEnv()";
		return -1;
	}
	return JNI_VERSION_1_4;
}

JNIEXPORT void JNICALL
Java_cc_openframeworks_OFAndroid_setAppDataDir( JNIEnv*  env, jclass thiz, jstring data_dir)
{
	jboolean iscopy;
	const char *mfile = env->GetStringUTFChars(data_dir, &iscopy);
	ofLogVerbose("ofAppAndroidWindow") << "setting app dir name to: " << string(mfile);
    ofSetDataPathRoot(string(mfile)+"/");
    env->ReleaseStringUTFChars(data_dir, mfile);
}

//JNIEXPORT void JNICALL
//Java_cc_openframeworks_OFAndroid_init( JNIEnv*  env, jclass  clazz)
//{
//	ofLogVerbose("ofAppAndroidWindow") << "OFAndroid_init";
//	//ofAndroidApplicationInit();
//}
//
//JNIEXPORT void JNICALL
//Java_cc_openframeworks_OFAndroid_onCreate( JNIEnv*  env, jclass  clazz)
//{
//    ofLogVerbose("ofAppAndroidWindow") << "OFAndroid_onCreate";
//	ofAndroidActivityInit();
//}

JNIEXPORT void JNICALL
Java_cc_openframeworks_OFAndroid_onRestart( JNIEnv*  env, jclass thiz){
	ofLogVerbose("ofAppAndroidWindow") << "onRestart";
}

JNIEXPORT void JNICALL
Java_cc_openframeworks_OFAndroid_onStart( JNIEnv*  env, jclass thiz){
	ofLogVerbose("ofAppAndroidWindow") << "onStart";
	stopped = false;
	bSetupScreen = true;
	ofNotifyEvent(ofxAndroidEvents().start);
}

JNIEXPORT void JNICALL
Java_cc_openframeworks_OFAndroid_onStop( JNIEnv*  env, jclass thiz){
	ofLogVerbose("ofAppAndroidWindow") << "onStop";
	ofNotifyEvent( ofxAndroidEvents().stop );
	stopped = true;
}

JNIEXPORT void JNICALL
Java_cc_openframeworks_OFAndroid_onResume( JNIEnv*  env, jclass thiz){
	ofLogVerbose("ofAppAndroidWindow") << "onResume";
	bSetupScreen = true;
	stopped = false;

	if(!threadedTouchEvents){
		mtx.lock();
			while (!touchEventArgsQueue.empty())
			{
			touchEventArgsQueue.pop();
			}
		mtx.unlock();
	}
	if(!threadedKeyEvents){
		keyMtx.lock();
				while (!keyEventArgsQueue.empty())
				{
					keyEventArgsQueue.pop();
				}
		keyMtx.unlock();
	}
	ofNotifyEvent(ofxAndroidEvents().resume);
}


JNIEXPORT void JNICALL
Java_cc_openframeworks_OFAndroid_onPause( JNIEnv*  env, jclass thiz){
	ofLogVerbose("ofAppAndroidWindow") << "onPause";
	ofNotifyEvent(ofxAndroidEvents().pause);
}


JNIEXPORT void JNICALL
Java_cc_openframeworks_OFAndroid_onDestroy( JNIEnv*  env, jclass  thiz ){
	appSetup = false;
	stopped = true;
	ofEvents().notifyExit();
	ofExitCallback();
}

JNIEXPORT void JNICALL
Java_cc_openframeworks_OFAndroid_onSurfaceDestroyed( JNIEnv*  env, jclass  thiz ){
	if(!surfaceDestroyed) {
		surfaceDestroyed = true;
		ofLogVerbose("ofAppAndroidWindow") << "onSurfaceDestroyed";
		ofNotifyEvent(ofxAndroidEvents().unloadGL);
		bSetupScreen = true;
	} else {
		ofLogVerbose("ofAppAndroidWindow") << "onSurfaceDestroyed already destroyed though";
	}
}

JNIEXPORT void JNICALL
Java_cc_openframeworks_OFAndroid_onSurfaceCreated( JNIEnv*  env, jclass  thiz ){
	if(appSetup){
		ofLogVerbose("ofAppAndroidWindow") << "onSurfaceCreated";
		if(!surfaceDestroyed){
			ofNotifyEvent(ofxAndroidEvents().unloadGL);
		}
		ofNotifyEvent(ofxAndroidEvents().reloadGL);
		window->renderer()->pushStyle();
		window->renderer()->setupGraphicDefaults();
		window->renderer()->popStyle();
		window->setThreadedEvents(false);
        int glesVersion = window->getGlesVersion();
		bSetupScreen = true;
		if( glesVersion < 2 )
		{
			ofLogVerbose("ofAppAndroidWindow") << "onSurfaceCreated OpenGLES 1";
			dynamic_cast<ofGLRenderer*>(window->renderer().get())->setup();
		}
		else
		{
			ofLogVerbose("ofAppAndroidWindow") << "onSurfaceCreated OpenGLES 2.0";
			dynamic_cast<ofGLProgrammableRenderer*>(window->renderer().get())->setup(glesVersion,0);
		}

	}else{
		if(window != nullptr) {
			int glesVersion = window->getGlesVersion();
			bSetupScreen = true;
			if (glesVersion < 2) {
				ofLogVerbose("ofAppAndroidWindow") << "onSurfaceCreated OpenGLES 1";
				dynamic_cast<ofGLRenderer *>(window->renderer().get())->setup();
			} else {
				ofLogVerbose("ofAppAndroidWindow") << "onSurfaceCreated OpenGLES 2.0";
				dynamic_cast<ofGLProgrammableRenderer *>(window->renderer().get())->setup(
						glesVersion, 0);
			}
		}
	}

	surfaceDestroyed = false;
}

JNIEXPORT jboolean JNICALL
Java_cc_openframeworks_OFAndroid_isWindowReady( JNIEnv*  env, jclass  thiz) {

          if(window != nullptr && window->renderer() != nullptr) {
            return true;
      } else {
        return false;
        }
}

JNIEXPORT void JNICALL
Java_cc_openframeworks_OFAndroid_setup( JNIEnv*  env, jclass  thiz, jint w, jint h  )
{
	ofLogNotice("ofAppAndroidWindow") << "setup " << w << "x" << h;
	stopped = false;
    sWindowWidth  = w;
    sWindowHeight = h;
    if(window != nullptr && window->renderer() != nullptr)
	    window->renderer()->startRender();
	if(bSetupScreen) {
        if(window != nullptr && window->renderer() != nullptr) {
			ofLogNotice("ofAppAndroidWindow") << "setupScreen - Logical Resolution:" << w << "x" << h << " Rendering at Resolution:" << w * window->getSamples() << "x" << h * window->getSamples() << " with MSAA:" << window->getSamples();
			window->renderer()->setupScreen();
            bSetupScreen = false;
        } else {
            ofLogError("ofAppAndroidWindow") << "No Window or Renderer for Logical Resolution:" << w << "x" << h;
        }
	}

    if(window != nullptr) {
    	window->makeCurrent();
    	if(assetManager != nullptr)
			window->setAssetManager(assetManager);
	    window->events().notifySetup();

    }
    if(window != nullptr && window->renderer() != nullptr)
	    window->renderer()->finishRender();
	appSetup = true;
}

JNIEXPORT void JNICALL
Java_cc_openframeworks_OFAndroid_resize( JNIEnv*  env, jclass  thiz, jint w, jint h )
{
    sWindowWidth  = w;
    sWindowHeight = h;
    bSetupScreen = true;
    if(window != nullptr) {
		ofLogNotice("ofAppAndroidWindow") << "resize to Logical Resolution:" << w << "x" << h << " Hardware Resolution: " << w * window->getSamples() << "x" << h * window->getSamples()  << " with MSAA:" << window->getSamples();
		window->events().notifyWindowResized(w,h);
    }
}

/* Call to finalize the graphics state */
JNIEXPORT void JNICALL
Java_cc_openframeworks_OFAndroid_exit( JNIEnv*  env, jclass  thiz )
{
    ofLogVerbose("ofAppAndroidWindow") << "OFAndroid_exit";
	exit(0);
	//window->events().notifyExit();
}

//
JNIEXPORT void JNICALL
Java_cc_openframeworks_OFAndroid_setAssetManager(JNIEnv *env, jclass thiz,
		jobject jAssetManager) {

	env->NewGlobalRef(jAssetManager);

	AAssetManager *aaAssetManager = AAssetManager_fromJava(env, jAssetManager);
	if (aaAssetManager == nullptr) {
		ofLogError("ofAppAndroidWindow") << "Could not obtain the AAssetManager";
		return;
	}

	assetManager = aaAssetManager;

	if(window == nullptr || (window != nullptr && window->renderer() == nullptr)) {
		ofLogVerbose("ofAppAndroidWindow") << "setAssetManager window is null";
		return;
	}

	window->setAssetManager(assetManager);




}

/* Call to render the next GL frame */
JNIEXPORT void JNICALL
Java_cc_openframeworks_OFAndroid_render( JNIEnv*  env, jclass  thiz )
{
	if(stopped || surfaceDestroyed) {
        ofLogVerbose("ofAppAndroidWindow") << "OFAndroid_render  stopped:" << stopped << "surfaceDestroyed" << surfaceDestroyed;
	    return;
	}

	if(window == nullptr || (window != nullptr && window->renderer() == nullptr)) {
		ofLogVerbose("ofAppAndroidWindow") << "OFAndroid_render window is null";
		return;
	}
	if(!threadedTouchEvents){ // process touch events in render loop thread to prevent latency
		mtx.lock();
		queue<ofTouchEventArgs> events = touchEventArgsQueue;
		while(!touchEventArgsQueue.empty()) touchEventArgsQueue.pop();
		mtx.unlock();
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

	if(!threadedKeyEvents) { // process key events in render loop thread to prevent latency
		keyMtx.lock();
		queue<ofKeyEventArgs> keyEvents = keyEventArgsQueue;
		while (!keyEventArgsQueue.empty()) keyEventArgsQueue.pop();
		keyMtx.unlock();

		while (!keyEvents.empty()) {
			switch (keyEvents.front().type) {
				case ofKeyEventArgs::Type::Pressed:
					ofNotifyEvent(window->events().keyPressed, keyEvents.front());
					break;
				case ofKeyEventArgs::Type::Released:
					ofNotifyEvent(window->events().keyReleased, keyEvents.front());
					break;
			}
			keyEvents.pop();
		}
	}

	if(!threadedAxisEvents) {
//		axisMtx.lock();
//		queue<ofKeyEventArgs> keyEvents = keyEventArgsQueue;
//		while (!keyEventArgsQueue.empty()) keyEventArgsQueue.pop();
//		axisMtx.unlock();
	}


	if (bSetupScreen) {
		window->renderer()->startRender();
		window->renderer()->setupScreen();
		window->events().notifyUpdate();
		window->events().notifyDraw();
		window->renderer()->finishRender();
		bSetupScreen = false;
	} else {
		window->renderer()->startRender();
		window->events().notifyUpdate();
		window->events().notifyDraw();
		window->renderer()->finishRender();
	}

}

JNIEXPORT void JNICALL
Java_cc_openframeworks_OFAndroid_onTouchDown(JNIEnv*  env, jclass  thiz, jint id,jfloat x,jfloat y,jfloat pressure,jfloat majoraxis,jfloat minoraxis,jfloat angle){
	if(window == nullptr || (window != nullptr && window->renderer() == nullptr)) {
		ofLogNotice("oF") << "Java_cc_openframeworks_OFAndroid_onTouchDown no window or renderer";
		return;
	}

	ofTouchEventArgs touch;
	touch.id = id;
	touch.x = x;
	touch.y = y;
	touch.pressure = pressure;
	touch.majoraxis = majoraxis;
	touch.minoraxis = minoraxis;
	touch.angle = angle;
	touch.type = ofTouchEventArgs::down;
	if(threadedTouchEvents){
		window->events().notifyMousePressed(x,y,0);
		ofNotifyEvent(window->events().touchDown,touch);
	}else{
		mtx.lock();
		touchEventArgsQueue.push(touch);
		mtx.unlock();
	}
}

JNIEXPORT void JNICALL
Java_cc_openframeworks_OFAndroid_onTouchUp(JNIEnv*  env, jclass  thiz, jint id,jfloat x,jfloat y,jfloat pressure,jfloat majoraxis,jfloat minoraxis,jfloat angle){
	if(window == nullptr || (window != nullptr && window->renderer() == nullptr)) return;
	ofTouchEventArgs touch;
	touch.id = id;
	touch.x = x;
	touch.y = y;
	touch.pressure = pressure;
	touch.majoraxis = majoraxis;
	touch.minoraxis = minoraxis;
	touch.angle = angle;
	touch.type = ofTouchEventArgs::up;
	if(threadedTouchEvents){
		window->events().notifyMouseReleased(x,y,0);
		ofNotifyEvent(window->events().touchUp,touch);
	}else{
		mtx.lock();
		touchEventArgsQueue.push(touch);
		mtx.unlock();
	}
}

JNIEXPORT void JNICALL
Java_cc_openframeworks_OFAndroid_onTouchCancelled(JNIEnv*  env, jclass  thiz, jint id,jfloat x,jfloat y){
	if(window == nullptr || (window != nullptr && window->renderer() == nullptr)) return;
	ofTouchEventArgs touch;
	touch.id = id;
	touch.x = x;
	touch.y = y;
	touch.pressure = 0;
	touch.type = ofTouchEventArgs::cancel;
	if(threadedTouchEvents){
		ofNotifyEvent(window->events().touchCancelled,touch);
	}else{
		mtx.lock();
		touchEventArgsQueue.push(touch);
		mtx.unlock();
	}
}

JNIEXPORT void JNICALL
Java_cc_openframeworks_OFAndroid_onTouchMoved(JNIEnv*  env, jclass  thiz, jint id,jfloat x,jfloat y,jfloat pressure,jfloat majoraxis,jfloat minoraxis,jfloat angle){
	if(window == nullptr || (window != nullptr && window->renderer() == nullptr)) return;
	ofTouchEventArgs touch;
	touch.id = id;
	touch.x = x;
	touch.y = y;
	touch.pressure = pressure;
	touch.majoraxis = majoraxis;
	touch.minoraxis = minoraxis;
	touch.angle = angle;
	touch.type = ofTouchEventArgs::move;
	if(threadedTouchEvents){
		window->events().notifyMouseMoved(x,y);
		window->events().notifyMouseDragged(x,y,0);
		ofNotifyEvent(window->events().touchMoved,touch);
	}else{
		mtx.lock();
		if(accumulateTouchEvents && !touchEventArgsQueue.empty() && touchEventArgsQueue.back().type==ofTouchEventArgs::move){
			touchEventArgsQueue.back() = touch;
		}else{
			touchEventArgsQueue.push(touch);
		}
		mtx.unlock();
	}
}

JNIEXPORT void JNICALL
Java_cc_openframeworks_OFAndroid_onTouchDoubleTap(JNIEnv*  env, jclass  thiz, jint id,jfloat x,jfloat y,jfloat pressure){
	if(window == nullptr || (window != nullptr && window->renderer() == nullptr)) return;
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
		mtx.lock();
		touchEventArgsQueue.push(touch);
		mtx.unlock();
	}
}

JNIEXPORT void JNICALL
Java_cc_openframeworks_OFAndroid_onSwipe(JNIEnv*  env, jclass  thiz, jint id, jint swipeDir){
	if(window == nullptr || (window != nullptr && window->renderer() == nullptr)) return;
	ofxAndroidSwipeEventArgs swipe{(ofxAndroidSwipeDir)swipeDir,id};
	ofNotifyEvent(ofxAndroidEvents().swipe,swipe);
}

JNIEXPORT jboolean JNICALL
Java_cc_openframeworks_OFAndroid_onScale(JNIEnv*  env, jclass  thiz, jobject detector){
	if(window == nullptr || (window != nullptr && window->renderer() == nullptr)) return false;
	ofxAndroidScaleEventArgs scale(detector);
	return ofxAndroidEvents().scale.notify(nullptr,scale);
}

JNIEXPORT jboolean JNICALL
Java_cc_openframeworks_OFAndroid_onScaleBegin(JNIEnv*  env, jclass  thiz, jobject detector){
	if(window == nullptr || (window != nullptr && window->renderer() == nullptr)) return false;
	ofxAndroidScaleEventArgs scale(detector);
	return ofxAndroidEvents().scaleBegin.notify(nullptr,scale);
}

JNIEXPORT void JNICALL
Java_cc_openframeworks_OFAndroid_onScaleEnd(JNIEnv*  env, jclass  thiz, jobject detector){
	if(window == nullptr || (window != nullptr && window->renderer() == nullptr)) return;
	ofxAndroidScaleEventArgs scale(detector);
    ofxAndroidEvents().scaleEnd.notify(nullptr,scale);
}

JNIEXPORT jboolean JNICALL
Java_cc_openframeworks_OFAndroid_onKeyDown(JNIEnv*  env, jclass thiz, jint  keyCode, jint unicode){
	if(window == nullptr || (window != nullptr && window->renderer() == nullptr)) return true;
    ofKeyEventArgs key;
	key.type = ofKeyEventArgs::Pressed;
    key.key = unicode;
    key.keycode = keyCode;
    key.scancode = keyCode;
    key.codepoint = unicode;
	if(threadedKeyEvents){
		return window->events().notifyKeyEvent(key);
	}else{
		keyMtx.lock();
		keyEventArgsQueue.push(key);
		keyMtx.unlock();
		return true; // returning key states always true in non-threaded
	}

}

JNIEXPORT jboolean JNICALL
Java_cc_openframeworks_OFAndroid_onKeyUp(JNIEnv*  env, jclass thiz, jint  keyCode, jint unicode){
	if(window == nullptr || (window != nullptr && window->renderer() == nullptr)) return true;
    ofKeyEventArgs key;
	key.type = ofKeyEventArgs::Released;
    key.key = unicode;
    key.keycode = keyCode;
    key.scancode = keyCode;
    key.codepoint = unicode;
	if(threadedKeyEvents){
		return window->events().notifyKeyEvent(key);
	}else{
		keyMtx.lock();
		keyEventArgsQueue.push(key);
		keyMtx.unlock();
		return true; // returning key states always true in non-threaded
	}
}

JNIEXPORT jboolean JNICALL
Java_cc_openframeworks_OFAndroid_onBackPressed(JNIEnv *env, jclass clazz){
	return ofxAndroidEvents().backPressed.notify(nullptr);
}

JNIEXPORT jboolean JNICALL
Java_cc_openframeworks_OFAndroid_onMenuItemSelected( JNIEnv*  env, jclass thiz, jstring menu_id){
	jboolean iscopy;
	const char * menu_id_str = env->GetStringUTFChars(menu_id, &iscopy);
	if(!menu_id_str) return false;
	string id_str(menu_id_str);
	return ofxAndroidEvents().menuItemSelected.notify(nullptr,id_str);
}

JNIEXPORT jboolean JNICALL
Java_cc_openframeworks_OFAndroid_onMenuItemChecked( JNIEnv*  env, jclass thiz, jstring menu_id, jboolean checked){
	jboolean iscopy;
	const char *menu_id_str = env->GetStringUTFChars(menu_id, &iscopy);
	if(!menu_id_str) return false;
	string id_str(menu_id_str);
	return ofxAndroidEvents().menuItemChecked.notify(nullptr,id_str);
}

JNIEXPORT void JNICALL
Java_cc_openframeworks_OFAndroid_okPressed( JNIEnv*  env, jclass thiz){
	ofNotifyEvent(ofxAndroidEvents().okPressed);
}

JNIEXPORT void JNICALL
Java_cc_openframeworks_OFAndroid_cancelPressed( JNIEnv*  env, jclass thiz){
	ofNotifyEvent(ofxAndroidEvents().cancelPressed);
}

JNIEXPORT void JNICALL
Java_cc_openframeworks_OFAndroid_networkConnected( JNIEnv*  env, jclass thiz, jboolean connected){
	bool bConnected = (bool)connected;
	ofNotifyEvent(ofxAndroidEvents().networkConnected,bConnected);
}

JNIEXPORT void JNICALL
Java_cc_openframeworks_OFAndroid_deviceOrientationChanged(JNIEnv*  env, jclass  thiz, jint orientation){
	if(window == nullptr || (window != nullptr && window->renderer() == nullptr)) return;
	ofOrientation _orientation = (ofOrientation) orientation;
	ofNotifyEvent(ofxAndroidEvents().deviceOrientationChanged,_orientation );
}

JNIEXPORT void JNICALL
Java_cc_openframeworks_OFAndroid_deviceHighestRefreshRate(JNIEnv*  env, jclass  thiz, jint refreshRate){
	if(window == nullptr || (window != nullptr && window->renderer() == nullptr)) return;
    int _refreshRate = (int) refreshRate;
	ofLogNotice("oF") << "deviceHighestRefreshRate:" << _refreshRate;
   // ofNotifyEvent(ofxAndroidEvents().deviceHighestRefreshRate,_refreshRate );
}


JNIEXPORT void JNICALL
Java_cc_openframeworks_OFAndroid_deviceRefreshRate(JNIEnv*  env, jclass  thiz, jint refreshRate){
	if(window == nullptr || (window != nullptr && window->renderer() == nullptr)) return;
	ofLogNotice("oF") << "deviceRefreshRateChanged:" << refreshRate;
	int _refreshRate = (int) refreshRate;
	//ofNotifyEvent(ofxAndroidEvents().deviceRefreshRate,_refreshRate );
}

JNIEXPORT void JNICALL
Java_cc_openframeworks_OFAndroid_setSampleSize(JNIEnv*  env, jclass  thiz, jint sampleSize){
	if(window == nullptr || (window != nullptr && window->renderer() == nullptr)) return;
	ofLogNotice("oF") << "setSampleSize:" << sampleSize;
	window->setSampleSize(sampleSize);
}

JNIEXPORT void JNICALL
Java_cc_openframeworks_OFAndroid_onAxisMoved(JNIEnv*  env, jclass  thiz, jint id, jint deviceid, jint productid, jfloat x, jfloat y){
	if(window == nullptr || (window != nullptr && window->renderer() == nullptr)) return;
	//ofLogNotice("oF") << "axisMoved:[" << id << "] x:" << x << " y:" << y;

}


JNIEXPORT void JNICALL
Java_cc_openframeworks_OFAndroid_setMultiWindowMode(JNIEnv*  env, jclass  thiz, jboolean multiWindow){
	if(window == nullptr || (window != nullptr && window->renderer() == nullptr)) return;
    bool bMultiWindow = (bool)multiWindow;
	ofLogNotice("oF") << "setMultiWindowMode:" << bMultiWindow;
	window->setMultiWindowMode(bMultiWindow);
}


}
