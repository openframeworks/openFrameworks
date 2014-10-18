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


// adding this for vc2010 compile: error C3861: 'closeQuicktime': identifier not found
#if defined (TARGET_WIN32) || defined(TARGET_OSX)
	#include "ofQtUtils.h"
#endif

#if defined (TARGET_WIN32)
#include <mmsystem.h>
#endif

//========================================================================
// static variables:

static shared_ptr<ofBaseApp>			OFSAptr;
static shared_ptr<ofAppBaseWindow> 		window;
static ofThreadErrorLogger threadErrorLogger;

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
#elif defined(TARGET_EMSCRIPTEN)
	#include "ofxAppEmscriptenWindow.h"
#else
	#include "ofAppGLFWWindow.h"
#endif

// this is hacky only to provide bw compatibility, a shared_ptr should always be initialized using a shared_ptr
// it shouldn't be a problem since it's only called from main and never deleted from outside
// also since old versions created the window in the stack, if this function is called we create a shared_ptr that never deletes
//--------------------------------------
#ifdef TARGET_OPENGLES
static void noopDeleter(ofAppBaseGLESWindow*){}
void ofSetupOpenGL(ofAppBaseGLESWindow * windowPtr, int w, int h, ofWindowMode screenMode){
	ofSetupOpenGL(shared_ptr<ofAppBaseGLESWindow>(windowPtr,std::ptr_fun(noopDeleter)),w,h,screenMode);
}
#else
static void noopDeleter(ofAppBaseGLWindow*){}
void ofSetupOpenGL(ofAppBaseGLWindow * windowPtr, int w, int h, ofWindowMode screenMode){
	ofSetupOpenGL(shared_ptr<ofAppBaseGLWindow>(windowPtr,std::ptr_fun(noopDeleter)),w,h,screenMode);
}
#endif

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

		ofAppBaseWindow * w = window.get();
		if(w){
			w->windowShouldClose();
		}
	}
#endif

//--------------------------------------
void ofRunApp(ofBaseApp * OFSA){
	ofRunApp(shared_ptr<ofBaseApp>(OFSA));
}

//--------------------------------------
void ofRunApp(shared_ptr<ofBaseApp> OFSA){
	Poco::ErrorHandler::set(&threadErrorLogger);
	OFSAptr = OFSA;
	if(OFSAptr){
		OFSAptr->mouseX = 0;
		OFSAptr->mouseY = 0;
	}

#ifndef TARGET_ANDROID
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

	window->initializeWindow();

	ofSeedRandom();
	ofResetElapsedTimeCounter();
	ofSetWorkingDirectoryToDefault();

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
    ofAddListener(ofEvents().mouseScrolled,OFSA.get(),&ofBaseApp::mouseScrolled,OF_EVENT_ORDER_APP);
    ofAddListener(ofEvents().windowEntered,OFSA.get(),&ofBaseApp::windowEntry,OF_EVENT_ORDER_APP);
    ofAddListener(ofEvents().windowResized,OFSA.get(),&ofBaseApp::windowResized,OF_EVENT_ORDER_APP);
    ofAddListener(ofEvents().messageEvent,OFSA.get(),&ofBaseApp::messageReceived,OF_EVENT_ORDER_APP);
    ofAddListener(ofEvents().fileDragEvent,OFSA.get(),&ofBaseApp::dragged,OF_EVENT_ORDER_APP);

	window->runAppViaInfiniteLoop(OFSAptr.get());
}

#ifdef TARGET_OPENGLES
static int glVersionMajor = 1;
static int glVersionMinor = 0;
#else
static int glVersionMajor = 2;
static int glVersionMinor = 1;
#endif

static string glslVersionFromGL(int major, int minor){
	switch(major){
	case 3:
		if(minor==0){
			return "130";
		}else if(minor==1){
			return "140";
		}else if(minor==2){
			return "150";
		}else{
			return ofToString(major*100+minor*10);
		}
	case 4:
		return ofToString(major*100+minor*10);
	default:
		return "120";
	}
}

//--------------------------------------
#ifdef TARGET_OPENGLES
void ofSetOpenGLESVersion(int version){
	glVersionMajor = version;
	glVersionMinor = 0;
	if(version>1){
		ofSetCurrentRenderer(ofGLProgrammableRenderer::TYPE);
	}
}

int	ofGetOpenGLESVersion(){
	return glVersionMajor;
}

string ofGetGLSLVersion(){
	return "1ES";
}
#else
void ofSetOpenGLVersion(int major, int minor){
	glVersionMajor = major;
	glVersionMinor = minor;
	if(major>2){
		ofSetCurrentRenderer(ofGLProgrammableRenderer::TYPE);
	}
}

int	ofGetOpenGLVersionMajor(){
	return glVersionMajor;
}

int	ofGetOpenGLVersionMinor(){
	return glVersionMinor;
}

string ofGetGLSLVersion(){
	return glslVersionFromGL(glVersionMajor,glVersionMinor);
}
#endif

//--------------------------------------
#ifdef TARGET_OPENGLES
void ofSetupOpenGL(shared_ptr<ofAppBaseGLESWindow> windowPtr, int w, int h, ofWindowMode screenMode){
#else
void ofSetupOpenGL(shared_ptr<ofAppBaseGLWindow> windowPtr, int w, int h, ofWindowMode screenMode){
#endif
    if(!ofGetCurrentRenderer()) {
	#ifdef TARGET_PROGRAMMABLE_GL
	    ofPtr<ofBaseRenderer> renderer(new ofGLProgrammableRenderer(false));
	#else
	    shared_ptr<ofBaseRenderer> renderer(new ofGLRenderer(false));
	#endif
	    ofSetCurrentRenderer(renderer,false);
    }

	window = windowPtr;

	if(ofIsGLProgrammableRenderer()){
		#ifdef TARGET_OPENGLES
			if(glVersionMajor<2){
				glVersionMajor=2;
				glVersionMinor=0;
			}
		#else
			if(glVersionMajor<3){
				glVersionMajor=3;
				glVersionMinor=2;
			}
		#endif
	}

	#if defined(TARGET_OPENGLES)
		windowPtr->setGLESVersion(glVersionMajor);
	#else
		windowPtr->setOpenGLVersion(glVersionMajor,glVersionMinor);
	#endif

	windowPtr->setupOpenGL(w, h, screenMode);
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
	char* glslVer = (char*)glGetString(GL_SHADING_LANGUAGE_VERSION);
	ofLogVerbose("ofAppRunner") << "GLSL:     " << (glslVer ? glslVer : "Error getting GLSL version.");

    if(ofIsGLProgrammableRenderer()){
    	ofGetGLProgrammableRenderer()->setup(glslVersionFromGL(glVersionMajor,glVersionMinor));
    }

	//Default colors etc are now in ofGraphics - ofSetupGraphicDefaults
	ofSetupGraphicDefaults();
	ofBackground(200);
	ofSetVerticalSync(true);
	ofEnableAlphaBlending();
}

//--------------------------------------
void ofSetupOpenGL(int w, int h, ofWindowMode screenMode){
	#ifdef TARGET_NODISPLAY
		shared_ptr<ofAppBaseWindow> window = shared_ptr<ofAppBaseWindow>(new ofAppNoWindow());
	#else
		#if defined(TARGET_OF_IOS)
			shared_ptr<ofAppBaseGLESWindow> glWindow = shared_ptr<ofAppBaseGLESWindow>(new ofAppiOSWindow());
		#elif defined(TARGET_ANDROID)
			shared_ptr<ofAppBaseGLESWindow> glWindow = shared_ptr<ofAppBaseGLESWindow>(new ofAppAndroidWindow());
		#elif defined(TARGET_RASPBERRY_PI)
			shared_ptr<ofAppBaseGLESWindow> glWindow = shared_ptr<ofAppBaseGLESWindow>(new ofAppEGLWindow());
		#elif defined(TARGET_EMSCRIPTEN)
			shared_ptr<ofAppBaseGLESWindow> glWindow = shared_ptr<ofAppBaseGLESWindow>(new ofxAppEmscriptenWindow);
		#elif defined(TARGET_OPENGLES)
			shared_ptr<ofAppBaseGLESWindow> glWindow = shared_ptr<ofAppBaseGLESWindow>(new ofAppGLFWWindow());
		#else
			shared_ptr<ofAppBaseGLWindow> glWindow = shared_ptr<ofAppBaseGLWindow>(new ofAppGLFWWindow());
		#endif
		window = glWindow;
		ofSetupOpenGL(glWindow,w,h,screenMode);
	#endif
}

void ofSetWindow(ofAppBaseWindow * windowPtr){
	ofSetWindow(shared_ptr<ofAppBaseWindow>(windowPtr));
}

void ofSetWindow(shared_ptr<ofAppBaseWindow> windowPtr){
	window = windowPtr;
}

void ofSetupOpenGL(ofAppBaseWindow * windowPtr, int w, int h, ofWindowMode screenMode){
	ofSetWindow(windowPtr);
}

//-----------------------	gets called when the app exits
//							currently looking at who to turn off
//							at the end of the application

void ofExitCallback(){
	// first notify we are exiting so every object has a chance to
	// stop threads, deallocate...
	ofNotifyExit();

	// then disable all core events to avoid crashes
	ofEvents().disable();

	// controlled destruction of the app before
	// any other deinitialization
	OFSAptr.reset();

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
ofBaseApp * ofGetAppPtr(){
	return OFSAptr.get();
}

//--------------------------------------
ofAppBaseWindow * ofGetWindowPtr(){
	return window.get();
}

//--------------------------------------
void ofSetAppPtr(shared_ptr<ofBaseApp> appPtr) {
	OFSAptr = appPtr;
}

//--------------------------------------
void ofExit(int status){
	std::exit(status);
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
