#include "ofAppRunner.h"

#include "ofBaseApp.h"
#include "ofAppBaseWindow.h"

#ifndef TARGET_NO_SOUND
#include "ofSoundPlayer.h"
#include "ofSoundStream.h"
#endif

#include "ofImage.h"
#include "ofUtils.h"
#include "ofEvents.h"
#include "ofMath.h"
#include "ofGraphics.h"
#include "ofGLRenderer.h"
#include "ofGLProgrammableRenderer.h"
#include "ofTrueTypeFont.h"
#include "ofURLFileLoader.h"
#include "Poco/Net/NetSSL.h"


// TODO: closing seems wonky.
// adding this for vc2010 compile: error C3861: 'closeQuicktime': identifier not found
#if defined (TARGET_WIN32) || defined(TARGET_OSX)
	#include "ofQtUtils.h"
#endif

//========================================================================
// static variables:

static ofPtr<ofBaseApp>				OFSAptr;
static ofPtr<ofAppBaseWindow> 		window;

//#define USE_PROGRAMMABLE_GL

//========================================================================
// default windowing
#ifdef TARGET_NODISPLAY
	#include "ofAppNoWindow.h"
#elif defined(TARGET_OF_IOS)
	#include "ofAppiOSWindow.h"
#elif defined(TARGET_ANDROID)
	#include "ofAppAndroidWindow.h"
#elif defined(TARGET_RASPBERRY_PI)
	#include "ofAppEGLWindow.h"
#else
	#include "ofAppGLFWWindow.h"
#endif

// this is hacky only to provide bw compatibility, a shared_ptr should always be initialized using a shared_ptr
// it shouldn't be a problem since it's only called from main and never deleted from outside
// also since old versions created the window in the stack, if this function is called we create a shared_ptr that never deletes
//--------------------------------------
static void noopDeleter(ofAppBaseWindow*){}
void ofSetupOpenGL(ofAppBaseWindow * windowPtr, int w, int h, int screenMode){
	ofSetupOpenGL(ofPtr<ofAppBaseWindow>(windowPtr,std::ptr_fun(noopDeleter)),w,h,screenMode);
}

void ofExitCallback();
void ofURLFileLoaderShutdown();

#if defined(TARGET_LINUX) || defined(TARGET_OSX)
	#include <signal.h>

	static bool bExitCalled = false;
	void sighandler(int sig) {
		ofLogVerbose("ofAppRunner") << "sighandler caught: " << sig;
		if(!bExitCalled) {
			bExitCalled = true;
			std::exit(0);
		}
	}
#endif

// the same hack but in this case the shared_ptr will delete, old versions created the testApp as new...
//--------------------------------------
void ofRunApp(ofBaseApp * OFSA){

	OFSAptr = ofPtr<ofBaseApp>(OFSA);
	if(OFSAptr){
		OFSAptr->mouseX = 0;
		OFSAptr->mouseY = 0;
	}

#ifndef TARGET_ANDROID
	atexit(ofExitCallback);
#endif

#if defined(TARGET_LINUX) || defined(TARGET_OSX)
	// see http://www.gnu.org/software/libc/manual/html_node/Termination-Signals.html#Termination-Signals
	signal(SIGTERM, &sighandler);
    signal(SIGQUIT, &sighandler);
	signal(SIGINT,  &sighandler);

	signal(SIGKILL, &sighandler); // not much to be done here
	signal(SIGHUP,  &sighandler); // not much to be done here

	// http://www.gnu.org/software/libc/manual/html_node/Program-Error-Signals.html#Program-Error-Signals
    signal(SIGABRT, &sighandler);  // abort signal
#endif


	#ifdef WIN32_HIGH_RES_TIMING
		timeBeginPeriod(1);		// ! experimental, sets high res time
								// you need to call timeEndPeriod.
								// if you quit the app other than "esc"
								// (ie, close the console, kill the process, etc)
								// at exit wont get called, and the time will
								// remain high res, that could mess things
								// up on your system.
								// info here:http://www.geisswerks.com/ryan/FAQS/timing.html

	#endif

	window->initializeWindow();

	ofSeedRandom();
	ofResetElapsedTimeCounter();
	ofSetWorkingDirectoryToDefault();
	

    ofAddListener(ofEvents().setup,OFSAptr.get(),&ofBaseApp::setup,OF_EVENT_ORDER_APP);
    ofAddListener(ofEvents().update,OFSAptr.get(),&ofBaseApp::update,OF_EVENT_ORDER_APP);
    ofAddListener(ofEvents().draw,OFSAptr.get(),&ofBaseApp::draw,OF_EVENT_ORDER_APP);
    ofAddListener(ofEvents().exit,OFSAptr.get(),&ofBaseApp::exit,OF_EVENT_ORDER_APP);
    ofAddListener(ofEvents().keyPressed,OFSAptr.get(),&ofBaseApp::keyPressed,OF_EVENT_ORDER_APP);
    ofAddListener(ofEvents().keyReleased,OFSAptr.get(),&ofBaseApp::keyReleased,OF_EVENT_ORDER_APP);
    ofAddListener(ofEvents().mouseMoved,OFSAptr.get(),&ofBaseApp::mouseMoved,OF_EVENT_ORDER_APP);
    ofAddListener(ofEvents().mouseDragged,OFSAptr.get(),&ofBaseApp::mouseDragged,OF_EVENT_ORDER_APP);
    ofAddListener(ofEvents().mousePressed,OFSAptr.get(),&ofBaseApp::mousePressed,OF_EVENT_ORDER_APP);
    ofAddListener(ofEvents().mouseReleased,OFSAptr.get(),&ofBaseApp::mouseReleased,OF_EVENT_ORDER_APP);
    ofAddListener(ofEvents().windowEntered,OFSAptr.get(),&ofBaseApp::windowEntry,OF_EVENT_ORDER_APP);
    ofAddListener(ofEvents().windowResized,OFSAptr.get(),&ofBaseApp::windowResized,OF_EVENT_ORDER_APP);
    ofAddListener(ofEvents().messageEvent,OFSAptr.get(),&ofBaseApp::messageReceived,OF_EVENT_ORDER_APP);
    ofAddListener(ofEvents().fileDragEvent,OFSAptr.get(),&ofBaseApp::dragged,OF_EVENT_ORDER_APP);

	window->runAppViaInfiniteLoop(OFSAptr.get());
}

//--------------------------------------
void ofSetupOpenGL(ofPtr<ofAppBaseWindow> windowPtr, int w, int h, int screenMode){
    if(!ofGetCurrentRenderer()) {
	#ifdef USE_PROGRAMMABLE_GL
	    ofPtr<ofBaseRenderer> renderer(new ofGLProgrammableRenderer(false));
	#else
	    ofPtr<ofBaseRenderer> renderer(new ofGLRenderer(false));
	#endif
	    ofSetCurrentRenderer(renderer,false);
    }

	window = windowPtr;

	if(ofIsGLProgrammableRenderer()){
        #if defined(TARGET_RASPBERRY_PI)
		static_cast<ofAppEGLWindow*>(window.get())->setGLESVersion(2);
		#elif defined(TARGET_LINUX_ARM)
		static_cast<ofAppGLFWWindow*>(window.get())->setOpenGLVersion(2,0);
		#elif !defined(TARGET_OPENGLES)
		static_cast<ofAppGLFWWindow*>(window.get())->setOpenGLVersion(3,2);
		#endif
	}else{
	    #if defined(TARGET_LINUX_ARM) && !defined(TARGET_RASPBERRY_PI)
		static_cast<ofAppGLFWWindow*>(window.get())->setOpenGLVersion(1,0);
		#endif
	}

	window->setupOpenGL(w, h, screenMode);
}

void ofGLReadyCallback(){

#ifndef TARGET_OPENGLES
	glewExperimental = GL_TRUE;
	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		/* Problem: glewInit failed, something is seriously wrong. */
		ofLogError("ofAppRunner") << "couldn't init GLEW: " << glewGetErrorString(err);
		return;
	}
#endif

	ofLogVerbose("ofAppRunner") << "GL ready";
	ofLogVerbose("ofAppRunner") << "Vendor:   " << (char*)glGetString(GL_VENDOR);
	ofLogVerbose("ofAppRunner") << "Renderer: " << (char*)glGetString(GL_RENDERER);
	ofLogVerbose("ofAppRunner") << "Version:  " << (char*)glGetString(GL_VERSION);
	ofLogVerbose("ofAppRunner") << "GLSL:     " << (char*)glGetString(GL_SHADING_LANGUAGE_VERSION);

    if(ofGetGLProgrammableRenderer()){
    	ofGetGLProgrammableRenderer()->setup();
    }

	//Default colors etc are now in ofGraphics - ofSetupGraphicDefaults
	ofSetupGraphicDefaults();
	ofBackground(200);
	ofSetVerticalSync(true);
	ofEnableAlphaBlending();
}

//--------------------------------------
void ofSetupOpenGL(int w, int h, int screenMode){
	#ifdef TARGET_NODISPLAY
		window = ofPtr<ofAppBaseWindow>(new ofAppNoWindow());
	#elif defined(TARGET_OF_IOS)
		window = ofPtr<ofAppBaseWindow>(new ofAppiOSWindow());
	#elif defined(TARGET_ANDROID)
		window = ofPtr<ofAppBaseWindow>(new ofAppAndroidWindow());
	#elif defined(TARGET_RASPBERRY_PI)
		window = ofPtr<ofAppBaseWindow>(new ofAppEGLWindow());
    #else
		window = ofPtr<ofAppBaseWindow>(new ofAppGLFWWindow());
	#endif

	ofSetupOpenGL(window,w,h,screenMode);
}

//-----------------------	gets called when the app exits
//							currently looking at who to turn off
//							at the end of the application

void ofExitCallback(){

	ofNotifyExit();

	ofURLFileLoaderShutdown();

    ofRemoveListener(ofEvents().setup,OFSAptr.get(),&ofBaseApp::setup,OF_EVENT_ORDER_APP);
    ofRemoveListener(ofEvents().update,OFSAptr.get(),&ofBaseApp::update,OF_EVENT_ORDER_APP);
    ofRemoveListener(ofEvents().draw,OFSAptr.get(),&ofBaseApp::draw,OF_EVENT_ORDER_APP);
    ofRemoveListener(ofEvents().exit,OFSAptr.get(),&ofBaseApp::exit,OF_EVENT_ORDER_APP);
    ofRemoveListener(ofEvents().keyPressed,OFSAptr.get(),&ofBaseApp::keyPressed,OF_EVENT_ORDER_APP);
    ofRemoveListener(ofEvents().keyReleased,OFSAptr.get(),&ofBaseApp::keyReleased,OF_EVENT_ORDER_APP);
    ofRemoveListener(ofEvents().mouseMoved,OFSAptr.get(),&ofBaseApp::mouseMoved,OF_EVENT_ORDER_APP);
    ofRemoveListener(ofEvents().mouseDragged,OFSAptr.get(),&ofBaseApp::mouseDragged,OF_EVENT_ORDER_APP);
    ofRemoveListener(ofEvents().mousePressed,OFSAptr.get(),&ofBaseApp::mousePressed,OF_EVENT_ORDER_APP);
    ofRemoveListener(ofEvents().mouseReleased,OFSAptr.get(),&ofBaseApp::mouseReleased,OF_EVENT_ORDER_APP);
    ofRemoveListener(ofEvents().windowResized,OFSAptr.get(),&ofBaseApp::windowResized,OF_EVENT_ORDER_APP);
    ofRemoveListener(ofEvents().windowEntered,OFSAptr.get(),&ofBaseApp::windowEntry,OF_EVENT_ORDER_APP);
    ofRemoveListener(ofEvents().messageEvent,OFSAptr.get(),&ofBaseApp::messageReceived,OF_EVENT_ORDER_APP);
    ofRemoveListener(ofEvents().fileDragEvent,OFSAptr.get(),&ofBaseApp::dragged,OF_EVENT_ORDER_APP);

	#ifndef TARGET_NO_SOUND
	//------------------------
	// try to close engine if needed:
	ofSoundShutdown();
	//------------------------
	#endif

	// try to close quicktime, for non-linux systems:
	#if defined(OF_VIDEO_CAPTURE_QUICKTIME) || defined(OF_VIDEO_PLAYER_QUICKTIME)
	closeQuicktime();
	#endif


	//------------------------
	// try to close freeImage:
	ofCloseFreeImage();

	//------------------------
	// try to close free type:
	ofTrueTypeFont::finishLibraries();

	#ifdef WIN32_HIGH_RES_TIMING
		timeEndPeriod(1);
	#endif

}

//--------------------------------------
void ofRunApp(ofPtr<ofBaseApp> OFSA){

	OFSAptr = OFSA;
	if(OFSAptr){
		OFSAptr->mouseX = 0;
		OFSAptr->mouseY = 0;
	}

#ifndef TARGET_ANDROID
	atexit(ofExitCallback);
#endif

	#ifdef WIN32_HIGH_RES_TIMING
		timeBeginPeriod(1);		// ! experimental, sets high res time
								// you need to call timeEndPeriod.
								// if you quit the app other than "esc"
								// (ie, close the console, kill the process, etc)
								// at exit wont get called, and the time will
								// remain high res, that could mess things
								// up on your system.
								// info here:http://www.geisswerks.com/ryan/FAQS/timing.html
	#endif

	window->initializeWindow();

	ofSeedRandom();
	ofResetElapsedTimeCounter();

    ofAddListener(ofEvents().setup,OFSA.get(),&ofBaseApp::setup,OF_EVENT_ORDER_APP);
    ofAddListener(ofEvents().update,OFSA.get(),&ofBaseApp::update,OF_EVENT_ORDER_APP);
    ofAddListener(ofEvents().draw,OFSA.get(),&ofBaseApp::draw,OF_EVENT_ORDER_APP);
    ofAddListener(ofEvents().exit,OFSA.get(),&ofBaseApp::exit,OF_EVENT_ORDER_APP);
    ofAddListener(ofEvents().keyPressed,OFSA.get(),&ofBaseApp::keyPressed,OF_EVENT_ORDER_APP);
    ofAddListener(ofEvents().keyReleased,OFSA.get(),&ofBaseApp::keyReleased,OF_EVENT_ORDER_APP);
    ofAddListener(ofEvents().mouseMoved,OFSA.get(),&ofBaseApp::mouseMoved,OF_EVENT_ORDER_APP);
    ofAddListener(ofEvents().mouseDragged,OFSA.get(),&ofBaseApp::mouseDragged,OF_EVENT_ORDER_APP);
    ofAddListener(ofEvents().mousePressed,OFSA.get(),&ofBaseApp::mousePressed,OF_EVENT_ORDER_APP);
    ofAddListener(ofEvents().mouseReleased,OFSA.get(),&ofBaseApp::mouseReleased,OF_EVENT_ORDER_APP);
    ofAddListener(ofEvents().windowEntered,OFSA.get(),&ofBaseApp::windowEntry,OF_EVENT_ORDER_APP);
    ofAddListener(ofEvents().windowResized,OFSA.get(),&ofBaseApp::windowResized,OF_EVENT_ORDER_APP);
    ofAddListener(ofEvents().messageEvent,OFSA.get(),&ofBaseApp::messageReceived,OF_EVENT_ORDER_APP);
    ofAddListener(ofEvents().fileDragEvent,OFSA.get(),&ofBaseApp::dragged,OF_EVENT_ORDER_APP);

	window->runAppViaInfiniteLoop(OFSAptr.get());

}

//--------------------------------------
ofBaseApp * ofGetAppPtr(){
	return OFSAptr.get();
}

//--------------------------------------
ofAppBaseWindow * ofGetWindowPtr(){
	return window.get();
}

//--------------------------------------
void ofSetAppPtr(ofPtr<ofBaseApp> appPtr) {
	OFSAptr = appPtr;
}

//--------------------------------------
void ofExit(int status){
	std::exit(status);
}

//--------------------------------------
void ofSleepMillis(int millis){
	#ifdef TARGET_WIN32
		Sleep(millis);			//windows sleep in milliseconds
	#else
		usleep(millis * 1000);	//mac sleep in microseconds - cooler :)
	#endif
}

//--------------------------------------
void ofHideCursor(){
	window->hideCursor();
}

//--------------------------------------
void ofShowCursor(){
	window->showCursor();
}

//--------------------------------------
void ofSetOrientation(ofOrientation orientation, bool vFlip){
	window->setOrientation(orientation);
	if(ofGetCurrentRenderer()){
		ofGetCurrentRenderer()->setOrientation(orientation,vFlip);
	}
}

//--------------------------------------
ofOrientation ofGetOrientation(){
	return window->getOrientation();
}

//--------------------------------------
void ofSetWindowPosition(int x, int y){
	window->setWindowPosition(x,y);
}

//--------------------------------------
void ofSetWindowShape(int width, int height){
	window->setWindowShape(width, height);
}

//--------------------------------------
int ofGetWindowPositionX(){
	return (int)window->getWindowPosition().x;
}

//--------------------------------------
int ofGetWindowPositionY(){
	return (int)window->getWindowPosition().y;
}

//--------------------------------------
int ofGetScreenWidth(){
	return (int)window->getScreenSize().x;
}

//--------------------------------------
int ofGetScreenHeight(){
	return (int)window->getScreenSize().y;
}

//--------------------------------------------------
int ofGetWidth(){
	return (int)window->getWidth();
}
//--------------------------------------------------
int ofGetHeight(){
	return (int)window->getHeight();
}

//--------------------------------------------------
int ofGetWindowWidth(){
	return (int)window->getWindowSize().x;
}
//--------------------------------------------------
int ofGetWindowHeight(){
	return (int)window->getWindowSize().y;
}

//--------------------------------------------------
bool ofDoesHWOrientation(){
	return window->doesHWOrientation();
}

//--------------------------------------------------
ofPoint	ofGetWindowSize() {
	//this can't be return ofPoint(ofGetWidth(), ofGetHeight()) as width and height change based on orientation.
	return window->getWindowSize();
}

//--------------------------------------------------
ofRectangle	ofGetWindowRect() {
	return ofRectangle(0, 0, ofGetWindowWidth(), ofGetWindowHeight());
}

//--------------------------------------
void ofSetWindowTitle(string title){
	window->setWindowTitle(title);
}

//----------------------------------------------------------
void ofEnableSetupScreen(){
	window->enableSetupScreen();
}

//----------------------------------------------------------
void ofDisableSetupScreen(){
	window->disableSetupScreen();
}

//--------------------------------------
void ofToggleFullscreen(){
	window->toggleFullscreen();
}

//--------------------------------------
void ofSetFullscreen(bool fullscreen){
	window->setFullscreen(fullscreen);
}

//--------------------------------------
int ofGetWindowMode(){
	return window->getWindowMode();
}

//--------------------------------------
void ofSetVerticalSync(bool bSync){
	window->setVerticalSync(bSync);
}

//-------------------------- native window handles
#if defined(TARGET_LINUX) && !defined(TARGET_RASPBERRY_PI)
Display* ofGetX11Display(){
	return window->getX11Display();
}

Window  ofGetX11Window(){
	return window->getX11Window();
}
#endif

#if defined(TARGET_LINUX) && !defined(TARGET_OPENGLES)
GLXContext ofGetGLXContext(){
	return window->getGLXContext();
}
#endif

#if defined(TARGET_LINUX) && defined(TARGET_OPENGLES)
EGLDisplay ofGetEGLDisplay(){
	return window->getEGLDisplay();
}

EGLContext ofGetEGLContext(){
	return window->getEGLContext();
}
EGLSurface ofGetEGLSurface(){
	return window->getEGLSurface();
}
#endif

#if defined(TARGET_OSX)
void * ofGetNSGLContext(){
	return window->getNSGLContext();
}

void * ofGetCocoaWindow(){
	return window->getCocoaWindow();
}
#endif

#if defined(TARGET_WIN32)
HGLRC ofGetWGLContext(){
	return window->getWGLContext();
}

HWND ofGetWin32Window(){
	return window->getWin32Window();
}
#endif
