#include "ofAppRunner.h"

#include "ofBaseApp.h"
#include "ofAppBaseWindow.h"

#include "ofLog.h"
#include "ofGraphicsBaseTypes.h"
#include "ofRectangle.h"
#include "ofMainLoop.h"
#include "ofEvents.h" // of::priv
#include "ofUtils.h" // initUtils

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
		ofCore.mainLoop->addWindow(windowPtr);
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

			if(ofCore.mainLoop){
				ofCore.mainLoop->shouldClose(signum);
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
	ofCore.init();
	ofCore.shutdownFunctions = {
		ofURLFileLoaderShutdown,
		ofTrueTypeShutdown,
		ofCloseFreeImage,
		// not even needed. empty function
		of::priv::endutils,
#ifndef TARGET_NO_SOUND
		ofSoundShutdown,
#endif

#if defined(OF_VIDEO_CAPTURE_QUICKTIME) || defined(OF_VIDEO_PLAYER_QUICKTIME)
		closeQuicktime,
#endif

#ifdef TARGET_LINUX
		ofGstUtils::quitGstMainLoop,
#endif
	};

//	std::cout << "ofInit, shutdown functions size = " << ofCore.shutdownFunctions.size() << std::endl;
//

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
	return;

}

//--------------------------------------
// TODO: const &
shared_ptr<ofMainLoop> ofGetMainLoop(){
	return ofCore.mainLoop;
}

//--------------------------------------
void ofSetMainLoop(const shared_ptr<ofMainLoop> & newMainLoop) {
	ofCore.mainLoop = newMainLoop;
}

//--------------------------------------
//int ofRunApp(ofBaseApp * OFSA){
//	ofCore.mainLoop->run(shared_ptr<ofBaseApp>(OFSA));
//	auto ret = ofRunMainLoop();
//#if !defined(TARGET_ANDROID) && !defined(TARGET_OF_IOS)
//	ofExitCallback();
//#endif
//	return ret;
//}
//
////--------------------------------------
int ofRunApp(const shared_ptr<ofBaseApp> & app){
	ofCore.mainLoop->run(app);
	auto ret = ofRunMainLoop();
#if !defined(TARGET_ANDROID) && !defined(TARGET_OF_IOS)
	ofExitCallback();
#endif
	return ret;
}

////--------------------------------------
//void ofRunApp(const shared_ptr<ofAppBaseWindow> & window, shared_ptr<ofBaseApp> && app){
//	ofCore.mainLoop->run(window, std::move(app));
//}

void ofRunApp(
			  const shared_ptr<ofAppBaseWindow> & window, 
			  const shared_ptr<ofBaseApp> & app){
	ofInit();
	ofCore.mainLoop->run(window, app);
}

int ofRunMainLoop(){
	auto ret = ofCore.mainLoop->loop();
	return ret;
}

//--------------------------------------
void ofSetupOpenGL(int w, int h, ofWindowMode screenMode){
	ofWindowSettings settings;
#ifdef TARGET_OPENGLES
	settings.glesVersion = 1;
#else
	settings.glVersionMajor = 2;
	settings.glVersionMinor = 1;
#endif

	settings.setSize(w, h);
	settings.windowMode = screenMode;
	ofCreateWindow(settings);
}


shared_ptr<ofAppBaseWindow> ofCreateWindow(const ofWindowSettings & settings){
	return ofCore.mainLoop->createWindow(settings);
}


void ofExitCallback(){
	ofCore.exit();
#ifdef WIN32_HIGH_RES_TIMING
	timeEndPeriod(1);
#endif
}

//--------------------------------------
// core events instance & arguments
ofCoreEvents & ofEvents(){
	auto window { ofCore.mainLoop->getCurrentWindow() };
	if(window){
		return window->events();
	}else{
		if(!ofCore.exiting){
			ofLogError("ofEvents") << "Trying to call ofEvents() before a window has been setup";
			ofLogError("ofEvents") << "We'll return a void events instance to avoid crashes but somethings might not work";
			ofLogError("ofEvents") << "Set a breakpoint in " << __FILE__ << " line " << __LINE__ << " to check where is the wrong call";
		}
		return ofCore.noopEvents;
	}
}

//--------------------------------------
void ofSetEscapeQuitsApp(bool bQuitOnEsc){
	ofCore.mainLoop->setEscapeQuitsLoop(bQuitOnEsc);
}

//--------------------------------------
shared_ptr<ofBaseRenderer> & ofGetCurrentRenderer(){
	return ofCore.mainLoop->getCurrentWindow()->renderer();
}

//--------------------------------------
ofBaseApp * ofGetAppPtr(){
	return ofCore.mainLoop->getCurrentApp().get();
}

//--------------------------------------
std::thread::id ofGetMainThreadId() {
	return ofCore.mainLoop->get_thread_id() ;
}

bool ofIsCurrentThreadTheMainThread() {
	return ofGetMainThreadId() == std::this_thread::get_id();
}

//--------------------------------------
ofAppBaseWindow * ofGetWindowPtr(){
	return ofCore.mainLoop->getCurrentWindow().get();
}

//--------------------------------------
std::shared_ptr<ofAppBaseWindow> ofGetCurrentWindow() {
	return ofCore.mainLoop->getCurrentWindow();
}

//--------------------------------------
void ofExit(int status){
	ofCore.mainLoop->shouldClose(status);
}

//--------------------------------------
void ofHideCursor(){
	ofCore.mainLoop->getCurrentWindow()->hideCursor();
}

//--------------------------------------
void ofShowCursor(){
	ofCore.mainLoop->getCurrentWindow()->showCursor();
}

//--------------------------------------
// FIXME: this is not standard with the other window modes.
void ofSetOrientation(ofOrientation orientation, bool vFlip){
	ofCore.mainLoop->getCurrentWindow()->setOrientation(orientation);
	// TODO: every window should set orientation on it's renderer
	if(ofGetCurrentRenderer()){
		ofGetCurrentRenderer()->setOrientation(orientation,vFlip);
	}
}

//--------------------------------------
ofOrientation ofGetOrientation(){
	return ofCore.mainLoop->getCurrentWindow()->getOrientation();
}

//--------------------------------------
void ofSetWindowPosition(int x, int y){
	ofCore.mainLoop->getCurrentWindow()->setWindowPosition(x,y);
}

//--------------------------------------
void ofSetWindowShape(int width, int height){
	ofCore.mainLoop->getCurrentWindow()->setWindowShape(width, height);
}

//--------------------------------------
void ofSetWindowRect(const ofRectangle & rect) {
	ofCore.mainLoop->getCurrentWindow()->setWindowRect(rect);
}

//--------------------------------------
glm::ivec2 ofGetWindowPosition() {
	return ofCore.mainLoop->getCurrentWindow()->getWindowPosition();
}

//--------------------------------------
int ofGetWindowPositionX(){
	return (int)ofCore.mainLoop->getCurrentWindow()->getWindowPosition().x;
}

//--------------------------------------
int ofGetWindowPositionY(){
	return (int)ofCore.mainLoop->getCurrentWindow()->getWindowPosition().y;
}

//--------------------------------------
int ofGetScreenWidth(){
	return ofCore.mainLoop->getCurrentWindow()->getScreenSize().x;
}

//--------------------------------------
int ofGetScreenHeight(){
	return ofCore.mainLoop->getCurrentWindow()->getScreenSize().y;
}

//--------------------------------------
glm::ivec2 ofGetScreenSize() {
	return ofCore.mainLoop->getCurrentWindow()->getScreenSize();
}

//--------------------------------------------------
int ofGetWidth(){
	return (int)ofCore.mainLoop->getCurrentWindow()->getWidth();
}

//--------------------------------------------------
int ofGetHeight(){
	return (int)ofCore.mainLoop->getCurrentWindow()->getHeight();
}

//--------------------------------------------------
int ofGetWindowWidth(){
	return (int)ofCore.mainLoop->getCurrentWindow()->getWindowSize().x;
}
//--------------------------------------------------
int ofGetWindowHeight(){
	return (int)ofCore.mainLoop->getCurrentWindow()->getWindowSize().y;
}

//--------------------------------------------------
std::string ofGetClipboardString(){
	return ofCore.mainLoop->getCurrentWindow()->getClipboardString();
}

//--------------------------------------------------
void ofSetClipboardString(const std::string & str){
	ofCore.mainLoop->getCurrentWindow()->setClipboardString(str);
}

//--------------------------------------------------
bool ofDoesHWOrientation(){
	return ofCore.mainLoop->getCurrentWindow()->doesHWOrientation();
}

//--------------------------------------------------
glm::ivec2 ofGetWindowSize() {
	//this can't return glm::vec2(ofGetWidth(), ofGetHeight()) as width and height change based on orientation.
	return ofCore.mainLoop->getCurrentWindow()->getWindowSize();
}

//--------------------------------------------------
float ofRandomWidth() {
	return of::random::uniform<float>(0.f, ofGetWidth());
}

//--------------------------------------------------
float ofRandomHeight() {
	return of::random::uniform<float>(0.f, ofGetHeight());
}

//--------------------------------------------------
// FIXME: This is wrong. doesn't consider window offset.
ofRectangle	ofGetWindowRect() {
	return ofRectangle(0, 0, ofGetWindowWidth(), ofGetWindowHeight());
}

//--------------------------------------
void ofSetWindowTitle(std::string title){
	ofCore.mainLoop->getCurrentWindow()->setWindowTitle(title);
}

//----------------------------------------------------------
void ofEnableSetupScreen(){
	ofCore.mainLoop->getCurrentWindow()->enableSetupScreen();
}

//----------------------------------------------------------
void ofDisableSetupScreen(){
	ofCore.mainLoop->getCurrentWindow()->disableSetupScreen();
}

//--------------------------------------
void ofToggleFullscreen(){
	ofCore.mainLoop->getCurrentWindow()->toggleFullscreen();
}

//--------------------------------------
void ofSetFullscreen(bool fullscreen){
	ofCore.mainLoop->getCurrentWindow()->setFullscreen(fullscreen);
}

//--------------------------------------
int ofGetWindowMode(){
	return ofCore.mainLoop->getCurrentWindow()->getWindowMode();
}

//--------------------------------------
void ofSetVerticalSync(bool bSync){
	ofCore.mainLoop->getCurrentWindow()->setVerticalSync(bSync);
}

//-------------------------- native window handles
#if defined(TARGET_LINUX) && !defined(TARGET_RASPBERRY_PI_LEGACY)
Display* ofGetX11Display(){
	return ofCore.mainLoop->getCurrentWindow()->getX11Display();
}

Window  ofGetX11Window(){
	return ofCore.mainLoop->getCurrentWindow()->getX11Window();
}
#endif

#if defined(TARGET_LINUX) && !defined(TARGET_OPENGLES)
GLXContext ofGetGLXContext(){
	return ofCore.mainLoop->getCurrentWindow()->getGLXContext();
}
#endif

#if defined(TARGET_LINUX) && defined(TARGET_OPENGLES)
EGLDisplay ofGetEGLDisplay(){
	return ofCore.mainLoop->getCurrentWindow()->getEGLDisplay();
}

EGLContext ofGetEGLContext(){
	return ofCore.mainLoop->getCurrentWindow()->getEGLContext();
}
EGLSurface ofGetEGLSurface(){
	return ofCore.mainLoop->getCurrentWindow()->getEGLSurface();
}
#endif

#if defined(TARGET_OSX)
void * ofGetNSGLContext(){
	return ofCore.mainLoop->getCurrentWindow()->getNSGLContext();
}

void * ofGetCocoaWindow(){
	return ofCore.mainLoop->getCurrentWindow()->getCocoaWindow();
}
#endif

#if defined(TARGET_WIN32)
HGLRC ofGetWGLContext(){
	return ofCore.mainLoop->getCurrentWindow()->getWGLContext();
}

HWND ofGetWin32Window(){
	return ofCore.mainLoop->getCurrentWindow()->getWin32Window();
}
#endif
