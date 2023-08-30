#include "ofAppRunner.h"

#include "ofBaseApp.h"
#include "ofAppBaseWindow.h"

// IDEA BRANCH - remove this comment
#include "ofLog.h"
#include "ofMath.h"
#include "ofGraphicsBaseTypes.h"
#include "ofRectangle.h"

// MASTER - TODO: Remove commented out code
// #ifndef TARGET_NO_SOUND
// #include "ofSoundPlayer.h"
// #endif

// #include "ofImage.h"
// #include "ofTrueTypeFont.h"


#include "ofMainLoop.h"

using std::shared_ptr;

#if !defined(TARGET_NODISPLAY)
	#if !defined( TARGET_OF_IOS ) & !defined(TARGET_ANDROID) & !defined(TARGET_EMSCRIPTEN) & !defined(TARGET_RASPBERRY_PI_LEGACY)
	#include "ofAppGLFWWindow.h"
	//special case so we preserve supplied settngs
	//TODO: remove me when we remove the ofAppGLFWWindow setters.
	//--------------------------------------
	void ofSetupOpenGL(const shared_ptr<ofAppGLFWWindow> & windowPtr, int w, int h, ofWindowMode screenMode){
		ofInit();
		auto settings = windowPtr->getSettings();
		settings.setSize(w,h);
		settings.windowMode = screenMode;
		ofGetMainLoop()->addWindow(windowPtr);
		windowPtr->setup(settings);
	}
	#endif
#endif

#ifdef TARGET_LINUX
#include "ofGstUtils.h"
#endif

// adding this for vc2010 compile: error C3861: 'closeQuicktime': identifier not found
#if defined(OF_VIDEO_CAPTURE_QUICKTIME) || defined(OF_VIDEO_PLAYER_QUICKTIME)
	#include "ofQtUtils.h"
#endif

#if defined (TARGET_WIN32)
#include <mmsystem.h>
#endif


//--------------------------------------
namespace{

    shared_ptr<ofMainLoop> & mainLoop(){
        static shared_ptr<ofMainLoop> * mainLoop(new shared_ptr<ofMainLoop>(new ofMainLoop));
        return *mainLoop;
    }

    bool & initialized(){
        static bool * initialized = new bool(false);
        return *initialized;
    }

	bool & exiting(){
		static bool * exiting = new bool(false);
		return *exiting;
	}

	ofCoreEvents & noopEvents(){
		static auto * noopEvents = new ofCoreEvents();
		return *noopEvents;
	}

    #if defined(TARGET_LINUX) || defined(TARGET_OSX)
        #include <signal.h>
        #include <string.h>
        void ofSignalHandler(int signum){
            char* pSignalString = strsignal(signum);

            if(pSignalString){
                ofLogVerbose("ofSignalHandler") << pSignalString;
            }else{
                ofLogVerbose("ofSignalHandler") << "Unknown: " << signum;
            }

            signal(SIGTERM, nullptr);
            signal(SIGQUIT, nullptr);
            signal(SIGINT,  nullptr);
            signal(SIGHUP,  nullptr);
            signal(SIGABRT, nullptr);

            if(mainLoop()){
                mainLoop()->shouldClose(signum);
            }
        }
    #endif
}


void ofExitCallback();
void ofURLFileLoaderShutdown();
void ofTrueTypeShutdown();
void ofCloseFreeImage();

#if defined(TARGET_ANDROID) || defined (TARGET_LINUX_ARM)
	inline void ofSoundShutdown(){}
#else
	void ofSoundShutdown();
#endif


void ofInit(){
	if(initialized()) return;
	initialized() = true;
	exiting() = false;

#if defined(TARGET_ANDROID) || defined(TARGET_OF_IOS)
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

    of::priv::initutils();
    of::priv::initfileutils();

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

#ifdef TARGET_LINUX
	if(std::locale().name() == "C"){
		try{
            std::locale::global(std::locale("C.UTF-8"));
		}catch(...){
			if(ofToLower(std::locale("").name()).find("utf-8")==std::string::npos){
				ofLogWarning("ofInit") << "Couldn't set UTF-8 locale, string manipulation functions\n"
						"won't work correctly for non ansi characters unless you specify a UTF-8 locale\n"
						"manually using std::locale::global(std::locale(\"locale\"))\n"
						"available locales can be queried with 'locale -a' in a terminal.";
			}
		}
	}
#endif

#if defined(TARGET_WIN32) && !_MSC_VER //MSYS2 UTF-8 limited support
    setlocale(LC_ALL,"");
    ofLogWarning("ofInit") << "MSYS2 has limited support for UTF-8. using "<< std::string( setlocale(LC_ALL,NULL) );
#endif
}

//--------------------------------------
shared_ptr<ofMainLoop> ofGetMainLoop(){
	return mainLoop();
}

//--------------------------------------
void ofSetMainLoop(const shared_ptr<ofMainLoop> & newMainLoop) {
	mainLoop() = newMainLoop;
}

//--------------------------------------
int ofRunApp(ofBaseApp * OFSA){
	mainLoop()->run(shared_ptr<ofBaseApp>(OFSA));
	auto ret = ofRunMainLoop();
#if !defined(TARGET_ANDROID) && !defined(TARGET_OF_IOS)
	ofExitCallback();
#endif
	return ret;
}

//--------------------------------------
int ofRunApp(shared_ptr<ofBaseApp> && app){
	mainLoop()->run(std::move(app));
	auto ret = ofRunMainLoop();
#if !defined(TARGET_ANDROID) && !defined(TARGET_OF_IOS)
	ofExitCallback();
#endif
	return ret;
}

//--------------------------------------
void ofRunApp(const shared_ptr<ofAppBaseWindow> & window, shared_ptr<ofBaseApp> && app){
	mainLoop()->run(window, std::move(app));
}

int ofRunMainLoop(){
	auto ret = mainLoop()->loop();
	return ret;
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

	settings.setSize(w, h);
	settings.windowMode = screenMode;
	ofCreateWindow(settings);
}

shared_ptr<ofAppBaseWindow> ofCreateWindow(const ofWindowSettings & settings){
	ofInit();
	return mainLoop()->createWindow(settings);
}

//-----------------------	gets called when the app exits
//							currently looking at who to turn off
//							at the end of the application

void ofExitCallback(){
	if(!initialized()) return;

	// controlled destruction of the mainLoop before
	// any other deinitialization
	mainLoop()->exit();

	// everything should be destroyed here, except for
	// static objects


	// finish every library and subsystem
	ofURLFileLoaderShutdown();

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

	//------------------------
	// try to close gstreamer
	#ifdef TARGET_LINUX
		ofGstUtils::quitGstMainLoop();
	#endif

	//------------------------
	// try to close font libraries
	ofTrueTypeShutdown();

	// static deinitialization happens after this finishes
	// every object should have ended by now and won't receive any
	// events
	of::priv::endutils();

	initialized() = false;
	exiting() = true;
}

//--------------------------------------
// core events instance & arguments
ofCoreEvents & ofEvents(){
	auto window = mainLoop()->getCurrentWindow();
	if(window){
		return window->events();
	}else{
		if(!exiting()){
			ofLogError("ofEvents") << "Trying to call ofEvents() before a window has been setup";
			ofLogError("ofEvents") << "We'll return a void events instance to avoid crashes but somethings might not work";
			ofLogError("ofEvents") << "Set a breakpoint in " << __FILE__ << " line " << __LINE__ << " to check where is the wrong call";
		}
		return noopEvents();
	}
}

//--------------------------------------
void ofSetEscapeQuitsApp(bool bQuitOnEsc){
	mainLoop()->setEscapeQuitsLoop(bQuitOnEsc);
}

//--------------------------------------
shared_ptr<ofBaseRenderer> & ofGetCurrentRenderer(){
	return mainLoop()->getCurrentWindow()->renderer();
}

//--------------------------------------
ofBaseApp * ofGetAppPtr(){
	return mainLoop()->getCurrentApp().get();
}

//--------------------------------------
std::thread::id ofGetMainThreadId() {
	return ofGetMainLoop()->get_thread_id() ;
}

bool ofIsCurrentThreadTheMainThread() {
	return ofGetMainThreadId() == std::this_thread::get_id();
}

//--------------------------------------
ofAppBaseWindow * ofGetWindowPtr(){
	return mainLoop()->getCurrentWindow().get();
}

//--------------------------------------
std::shared_ptr<ofAppBaseWindow> ofGetCurrentWindow() {
	return mainLoop()->getCurrentWindow();
}

//--------------------------------------
void ofExit(int status){
	mainLoop()->shouldClose(status);
}

//--------------------------------------
void ofHideCursor(){
	mainLoop()->getCurrentWindow()->hideCursor();
}

//--------------------------------------
void ofShowCursor(){
	mainLoop()->getCurrentWindow()->showCursor();
}

//--------------------------------------
void ofSetOrientation(ofOrientation orientation, bool vFlip){
	mainLoop()->getCurrentWindow()->setOrientation(orientation);
	// TODO: every window should set orientation on it's renderer
	if(ofGetCurrentRenderer()){
		ofGetCurrentRenderer()->setOrientation(orientation,vFlip);
	}
}

//--------------------------------------
ofOrientation ofGetOrientation(){
	return mainLoop()->getCurrentWindow()->getOrientation();
}

//--------------------------------------
void ofSetWindowPosition(int x, int y){
	mainLoop()->getCurrentWindow()->setWindowPosition(x,y);
}

//--------------------------------------
void ofSetWindowShape(int width, int height){
	mainLoop()->getCurrentWindow()->setWindowShape(width, height);
}

//--------------------------------------
int ofGetWindowPositionX(){
	return (int)mainLoop()->getCurrentWindow()->getWindowPosition().x;
}

//--------------------------------------
int ofGetWindowPositionY(){
	return (int)mainLoop()->getCurrentWindow()->getWindowPosition().y;
}

//--------------------------------------
int ofGetScreenWidth(){
	return (int)mainLoop()->getCurrentWindow()->getScreenSize().x;
}

//--------------------------------------
int ofGetScreenHeight(){
	return (int)mainLoop()->getCurrentWindow()->getScreenSize().y;
}

//--------------------------------------------------
int ofGetWidth(){
	return (int)mainLoop()->getCurrentWindow()->getWidth();
}
//--------------------------------------------------
int ofGetHeight(){
	return (int)mainLoop()->getCurrentWindow()->getHeight();
}

//--------------------------------------------------
int ofGetWindowWidth(){
	return (int)mainLoop()->getCurrentWindow()->getWindowSize().x;
}
//--------------------------------------------------
int ofGetWindowHeight(){
	return (int)mainLoop()->getCurrentWindow()->getWindowSize().y;
}

//--------------------------------------------------
std::string ofGetClipboardString(){
	return mainLoop()->getCurrentWindow()->getClipboardString();
}

//--------------------------------------------------
void ofSetClipboardString(const std::string & str){
	mainLoop()->getCurrentWindow()->setClipboardString(str);
}

//--------------------------------------------------
bool ofDoesHWOrientation(){
	return mainLoop()->getCurrentWindow()->doesHWOrientation();
}

//--------------------------------------------------
glm::vec2 ofGetWindowSize() {
	//this can't return glm::vec2(ofGetWidth(), ofGetHeight()) as width and height change based on orientation.
	return mainLoop()->getCurrentWindow()->getWindowSize();
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
void ofSetWindowTitle(std::string title){
	mainLoop()->getCurrentWindow()->setWindowTitle(title);
}

//----------------------------------------------------------
void ofEnableSetupScreen(){
	mainLoop()->getCurrentWindow()->enableSetupScreen();
}

//----------------------------------------------------------
void ofDisableSetupScreen(){
	mainLoop()->getCurrentWindow()->disableSetupScreen();
}

//--------------------------------------
void ofToggleFullscreen(){
	mainLoop()->getCurrentWindow()->toggleFullscreen();
}

//--------------------------------------
void ofSetFullscreen(bool fullscreen){
	mainLoop()->getCurrentWindow()->setFullscreen(fullscreen);
}

//--------------------------------------
int ofGetWindowMode(){
	return mainLoop()->getCurrentWindow()->getWindowMode();
}

//--------------------------------------
void ofSetVerticalSync(bool bSync){
	mainLoop()->getCurrentWindow()->setVerticalSync(bSync);
}

//-------------------------- native window handles
#if defined(TARGET_LINUX) && !defined(TARGET_RASPBERRY_PI_LEGACY)
Display* ofGetX11Display(){
	return mainLoop()->getCurrentWindow()->getX11Display();
}

Window  ofGetX11Window(){
	return mainLoop()->getCurrentWindow()->getX11Window();
}
#endif

#if defined(TARGET_LINUX) && !defined(TARGET_OPENGLES)
GLXContext ofGetGLXContext(){
	return mainLoop()->getCurrentWindow()->getGLXContext();
}
#endif

#if defined(TARGET_LINUX) && defined(TARGET_OPENGLES)
EGLDisplay ofGetEGLDisplay(){
	return mainLoop()->getCurrentWindow()->getEGLDisplay();
}

EGLContext ofGetEGLContext(){
	return mainLoop()->getCurrentWindow()->getEGLContext();
}
EGLSurface ofGetEGLSurface(){
	return mainLoop()->getCurrentWindow()->getEGLSurface();
}
#endif

#if defined(TARGET_OSX)
void * ofGetNSGLContext(){
	return mainLoop()->getCurrentWindow()->getNSGLContext();
}

void * ofGetCocoaWindow(){
	return mainLoop()->getCurrentWindow()->getCocoaWindow();
}
#endif

#if defined(TARGET_WIN32)
HGLRC ofGetWGLContext(){
	return mainLoop()->getCurrentWindow()->getWGLContext();
}

HWND ofGetWin32Window(){
	return mainLoop()->getCurrentWindow()->getWin32Window();
}
#endif
