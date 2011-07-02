#include "ofAppRunner.h"

#include "ofBaseApp.h"
#include "ofAppBaseWindow.h"
#include "ofSoundPlayer.h"
#include "ofSoundStream.h"
#include "ofImage.h"
#include "ofUtils.h"
#include "ofEvents.h"
#include "ofMath.h"
#include "ofGraphics.h"
#include "ofGLRenderer.h"

// TODO: closing seems wonky. 
// adding this for vc2010 compile: error C3861: 'closeQuicktime': identifier not found
#if defined (TARGET_WIN32) || defined(TARGET_OSX)
	#include "ofQtUtils.h"
#endif

//========================================================================
// static variables:

ofPtr<ofBaseApp>				OFSAptr;
bool 						bMousePressed;
bool						bRightButton;
int							width, height;

static ofPtr<ofAppBaseWindow> window;


//========================================================================
// default windowing
#ifdef TARGET_OF_IPHONE
	#include "ofAppiPhoneWindow.h"
#elif defined TARGET_ANDROID
	#include "ofAppAndroidWindow.h"
#else
	#include "ofAppGlutWindow.h"
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

// the same hack but in this case the shared_ptr will delete, old versions created the testApp as new...
//--------------------------------------
void ofRunApp(ofBaseApp * OFSA){

	OFSAptr = ofPtr<ofBaseApp>(OFSA);
	if(OFSAptr){
		OFSAptr->mouseX = 0;
		OFSAptr->mouseY = 0;
	}

	#ifdef TARGET_OSX
		//this internally checks the executable path for osx
		ofSetDataPathRoot("../../../data/");
	#endif

	atexit(ofExitCallback);

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

	window->runAppViaInfiniteLoop(OFSAptr.get());
}

//--------------------------------------
void ofSetupOpenGL(ofPtr<ofAppBaseWindow> windowPtr, int w, int h, int screenMode){
	window = windowPtr;
	window->setupOpenGL(w, h, screenMode);
	
#ifndef TARGET_OPENGLES
	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		/* Problem: glewInit failed, something is seriously wrong. */
		ofLog(OF_LOG_ERROR, "Error: %s\n", glewGetErrorString(err));
	}
#endif
	ofSetCurrentRenderer(ofPtr<ofBaseRenderer>(new ofGLRenderer(false)));
	//Default colors etc are now in ofGraphics - ofSetupGraphicDefaults
	//ofSetupGraphicDefaults();
}


//--------------------------------------
void ofSetupOpenGL(int w, int h, int screenMode){
	#ifdef TARGET_OF_IPHONE
		window = ofPtr<ofAppBaseWindow>(new ofAppiPhoneWindow());
	#elif defined TARGET_ANDROID
		window = ofPtr<ofAppBaseWindow>(new ofAppAndroidWindow());
	#else
		window = ofPtr<ofAppBaseWindow>(new ofAppGlutWindow());
	#endif

	ofSetupOpenGL(window,w,h,screenMode);
}

//----------------------- 	gets called when the app exits
// 							currently looking at who to turn off
//							at the end of the application

void ofExitCallback(){

	//------------------------
	// try to close engine if needed:
	ofSoundShutdown();
	//------------------------

	// try to close quicktime, for non-linux systems:
	#if defined( TARGET_OSX ) || defined( TARGET_WIN32 )
	closeQuicktime();
	#endif


	//------------------------
	// try to close freeImage:
	ofCloseFreeImage();
	//------------------------
	// try to close free type:
	// ....

	#ifdef WIN32_HIGH_RES_TIMING
		timeEndPeriod(1);
	#endif

	ofNotifyExit();
}

//--------------------------------------
void ofRunApp(ofPtr<ofBaseApp> OFSA){

	OFSAptr = OFSA;
	if(OFSAptr){
		OFSAptr->mouseX = 0;
		OFSAptr->mouseY = 0;
	}

	#ifdef TARGET_OSX 
		//this internally checks the executable path for osx
		ofSetDataPathRoot("../../../data/");
	#endif

	atexit(ofExitCallback);

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

	window->runAppViaInfiniteLoop(OFSAptr.get());


}

//--------------------------------------
ofBaseApp * ofGetAppPtr(){
	return OFSAptr.get();
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
int ofGetFrameNum(){
	return window->getFrameNum();
}

//--------------------------------------
float ofGetFrameRate(){
	return window->getFrameRate();
}

double ofGetLastFrameTime(){
	return window->getLastFrameTime();
}

//--------------------------------------
void ofSetFrameRate(int targetRate){

	window->setFrameRate(targetRate);
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
void ofSetOrientation(ofOrientation orientation){
	window->setOrientation(orientation);
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
	//----------------------------
	#ifdef TARGET_WIN32
	//----------------------------
		if (bSync) {
			if (WGL_EXT_swap_control) wglSwapIntervalEXT (1);
		} else {
			if (WGL_EXT_swap_control) wglSwapIntervalEXT (0);
		}
	//----------------------------
	#endif
	//----------------------------

	//--------------------------------------
	#ifdef TARGET_OSX
	//--------------------------------------
		GLint sync = bSync == true ? 1 : 0;
		CGLSetParameter (CGLGetCurrentContext(), kCGLCPSwapInterval, &sync);
	//--------------------------------------
	#endif
	//--------------------------------------

	//--------------------------------------
	#ifdef TARGET_LINUX
	//--------------------------------------
		//if (GLEW_GLX_SGI_swap_control)
		void (*swapInterval)(int)  = (void (*)(int)) glXGetProcAddress((const GLubyte*) "glXSwapIntervalSGI");
		if(!swapInterval)
			swapInterval = (void (*)(int)) glXGetProcAddress((const GLubyte*) "glXSwapIntervalMESA");

		if(swapInterval)
			swapInterval(bSync ? 1 : 0);
		//glXSwapIntervalSGI(bSync ? 1 : 0);
	//--------------------------------------
	#endif
	//--------------------------------------

}
