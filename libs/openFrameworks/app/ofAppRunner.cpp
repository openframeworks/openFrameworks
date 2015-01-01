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
#include "ofMainLoop.h"


// adding this for vc2010 compile: error C3861: 'closeQuicktime': identifier not found
#if defined (TARGET_WIN32) || defined(TARGET_OSX)
	#include "ofQtUtils.h"
#endif

#if defined (TARGET_WIN32)
#include <mmsystem.h>
#endif

//========================================================================
// static variables:

static shared_ptr<ofMainLoop> mainLoop;


void ofExitCallback();
void ofURLFileLoaderShutdown();

#if defined(TARGET_LINUX) || defined(TARGET_OSX)
	#include <signal.h>
	#include <string.h>

	static void ofSignalHandler(int signum){

		char* pSignalString = strsignal(signum);

		if(pSignalString){
			ofLogVerbose("ofSignalHandler") << pSignalString;
		}else{
			ofLogVerbose("ofSignalHandler") << "Unknown: " << signum;
		}

		signal(SIGTERM, NULL);
		signal(SIGQUIT, NULL);
		signal(SIGINT,  NULL);
		signal(SIGHUP,  NULL);
		signal(SIGABRT, NULL);

		if(mainLoop){
			mainLoop->shouldClose(signum);
		}
	}
#endif

void ofInit(){
	static bool initialized = false;
	if(initialized) return;
	initialized = true;
	Poco::ErrorHandler::set(new ofThreadErrorLogger);

#if defined(TARGET_ANDROID) || defined(TARGET_IOS)
    // manage own exit
#else
	atexit(ofExitCallback);
#endif

#if defined(TARGET_LINUX) || defined(TARGET_OSX)
	// see http://www.gnu.org/software/libc/manual/html_node/Termination-Signals.html#Termination-Signals
	signal(SIGTERM, &ofSignalHandler);
	signal(SIGQUIT, &ofSignalHandler);
	signal(SIGINT,  &ofSignalHandler);

	signal(SIGHUP,  &ofSignalHandler); // not much to be done here

	// http://www.gnu.org/software/libc/manual/html_node/Program-Error-Signals.html#Program-Error-Signals
	signal(SIGABRT, &ofSignalHandler);  // abort signal
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

	ofSeedRandom();
	ofResetElapsedTimeCounter();
	ofSetWorkingDirectoryToDefault();
}


//--------------------------------------
shared_ptr<ofMainLoop> ofGetMainLoop(){
	return mainLoop;
}

//--------------------------------------
int ofRunApp(ofBaseApp * OFSA){
	return ofRunApp(shared_ptr<ofBaseApp>(OFSA));
}

//--------------------------------------
int ofRunApp(shared_ptr<ofBaseApp> app){
	mainLoop->run(app);
	return mainLoop->loop();
}


//--------------------------------------
void ofRunApp(shared_ptr<ofAppBaseWindow> window, shared_ptr<ofBaseApp> app){
	mainLoop->run(window,app);
}

int ofRunMainLoop(){
	return mainLoop->loop();
}

//--------------------------------------
void ofSetupOpenGL(int w, int h, ofWindowMode screenMode){
#ifdef TARGET_OPENGLES
	ofGLESWindowSettings settings;
	settings.glesVersion = 1;
#else
	ofGLWindowSettings settings;
	settings.glVersionMajor = 2;
	settings.glVersionMinor = 1;
#endif

	settings.width = w;
	settings.height = h;
	settings.windowMode = screenMode;
	ofCreateWindow(settings);
}

shared_ptr<ofAppBaseWindow> ofCreateWindow(const ofWindowSettings & settings){
	ofInit();
	if(!mainLoop) mainLoop = shared_ptr<ofMainLoop>(new ofMainLoop);
	return mainLoop->createWindow(settings);
}

//-----------------------	gets called when the app exits
//							currently looking at who to turn off
//							at the end of the application

void ofExitCallback(){
	// controlled destruction of the mainLoop before
	// any other deinitialization
	mainLoop.reset();

	// everything should be destroyed here, except for
	// static objects


	// finish every library and subsystem
	#ifndef TARGET_EMSCRIPTEN
		ofURLFileLoaderShutdown();
	#endif

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


	#ifdef WIN32_HIGH_RES_TIMING
		timeEndPeriod(1);
	#endif

	// static deinitialization happens after this finishes
	// every object should have ended by now and won't receive any
	// events
}

//--------------------------------------
// core events instance & arguments
ofCoreEvents & ofEvents(){
	return mainLoop->events();
}

//--------------------------------------
void ofSetEscapeQuitsApp(bool bQuitOnEsc){
	mainLoop->setEscapeQuitsLoop(bQuitOnEsc);
}

//--------------------------------------
shared_ptr<ofBaseRenderer> & ofGetCurrentRenderer(){
	return mainLoop->getCurrentWindow()->renderer();
}

//--------------------------------------
ofBaseApp * ofGetAppPtr(){
	return mainLoop->getCurrentApp().get();
}

//--------------------------------------
ofAppBaseWindow * ofGetWindowPtr(){
	return mainLoop->getCurrentWindow().get();
}

//--------------------------------------
void ofSetAppPtr(shared_ptr<ofBaseApp> appPtr) {
	//OFSAptr = appPtr;
}

//--------------------------------------
void ofExit(int status){
	mainLoop->shouldClose(status);
}

//--------------------------------------
void ofHideCursor(){
	mainLoop->getCurrentWindow()->hideCursor();
}

//--------------------------------------
void ofShowCursor(){
	mainLoop->getCurrentWindow()->showCursor();
}

//--------------------------------------
void ofSetOrientation(ofOrientation orientation, bool vFlip){
	mainLoop->getCurrentWindow()->setOrientation(orientation);
	// TODO: every window should set orientation on it's renderer
	if(ofGetCurrentRenderer()){
		ofGetCurrentRenderer()->setOrientation(orientation,vFlip);
	}
}

//--------------------------------------
ofOrientation ofGetOrientation(){
	return mainLoop->getCurrentWindow()->getOrientation();
}

//--------------------------------------
void ofSetWindowPosition(int x, int y){
	mainLoop->getCurrentWindow()->setWindowPosition(x,y);
}

//--------------------------------------
void ofSetWindowShape(int width, int height){
	mainLoop->getCurrentWindow()->setWindowShape(width, height);
}

//--------------------------------------
int ofGetWindowPositionX(){
	return (int)mainLoop->getCurrentWindow()->getWindowPosition().x;
}

//--------------------------------------
int ofGetWindowPositionY(){
	return (int)mainLoop->getCurrentWindow()->getWindowPosition().y;
}

//--------------------------------------
int ofGetScreenWidth(){
	return (int)mainLoop->getCurrentWindow()->getScreenSize().x;
}

//--------------------------------------
int ofGetScreenHeight(){
	return (int)mainLoop->getCurrentWindow()->getScreenSize().y;
}

//--------------------------------------------------
int ofGetWidth(){
	return (int)mainLoop->getCurrentWindow()->getWidth();
}
//--------------------------------------------------
int ofGetHeight(){
	return (int)mainLoop->getCurrentWindow()->getHeight();
}

//--------------------------------------------------
int ofGetWindowWidth(){
	return (int)mainLoop->getCurrentWindow()->getWindowSize().x;
}
//--------------------------------------------------
int ofGetWindowHeight(){
	return (int)mainLoop->getCurrentWindow()->getWindowSize().y;
}

//--------------------------------------------------
bool ofDoesHWOrientation(){
	return mainLoop->getCurrentWindow()->doesHWOrientation();
}

//--------------------------------------------------
ofPoint	ofGetWindowSize() {
	//this can't be return ofPoint(ofGetWidth(), ofGetHeight()) as width and height change based on orientation.
	return mainLoop->getCurrentWindow()->getWindowSize();
}

//--------------------------------------------------
float ofRandomWidth() {
	return ofRandom(0.f, ofGetWidth());
}

//--------------------------------------------------
float ofRandomHeight() {
	return ofRandom(0.f, ofGetHeight());
}

//--------------------------------------------------
ofRectangle	ofGetWindowRect() {
	return ofRectangle(0, 0, ofGetWindowWidth(), ofGetWindowHeight());
}

//--------------------------------------
void ofSetWindowTitle(string title){
	mainLoop->getCurrentWindow()->setWindowTitle(title);
}

//----------------------------------------------------------
void ofEnableSetupScreen(){
	mainLoop->getCurrentWindow()->enableSetupScreen();
}

//----------------------------------------------------------
void ofDisableSetupScreen(){
	mainLoop->getCurrentWindow()->disableSetupScreen();
}

//--------------------------------------
void ofToggleFullscreen(){
	mainLoop->getCurrentWindow()->toggleFullscreen();
}

//--------------------------------------
void ofSetFullscreen(bool fullscreen){
	mainLoop->getCurrentWindow()->setFullscreen(fullscreen);
}

//--------------------------------------
int ofGetWindowMode(){
	return mainLoop->getCurrentWindow()->getWindowMode();
}

//--------------------------------------
void ofSetVerticalSync(bool bSync){
	mainLoop->getCurrentWindow()->setVerticalSync(bSync);
}

//-------------------------- native window handles
#if defined(TARGET_LINUX) && !defined(TARGET_RASPBERRY_PI)
Display* ofGetX11Display(){
	return mainLoop->getCurrentWindow()->getX11Display();
}

Window  ofGetX11Window(){
	return mainLoop->getCurrentWindow()->getX11Window();
}
#endif

#if defined(TARGET_LINUX) && !defined(TARGET_OPENGLES)
GLXContext ofGetGLXContext(){
	return mainLoop->getCurrentWindow()->getGLXContext();
}
#endif

#if defined(TARGET_LINUX) && defined(TARGET_OPENGLES)
EGLDisplay ofGetEGLDisplay(){
	return mainLoop->getCurrentWindow()->getEGLDisplay();
}

EGLContext ofGetEGLContext(){
	return mainLoop->getCurrentWindow()->getEGLContext();
}
EGLSurface ofGetEGLSurface(){
	return mainLoop->getCurrentWindow()->getEGLSurface();
}
#endif

#if defined(TARGET_OSX)
void * ofGetNSGLContext(){
	return mainLoop->getCurrentWindow()->getNSGLContext();
}

void * ofGetCocoaWindow(){
	return mainLoop->getCurrentWindow()->getCocoaWindow();
}
#endif

#if defined(TARGET_WIN32)
HGLRC ofGetWGLContext(){
	return mainLoop->getCurrentWindow()->getWGLContext();
}

HWND ofGetWin32Window(){
	return mainLoop->getCurrentWindow()->getWin32Window();
}
#endif
