#pragma once

#include "ofMainLoop.h"
#include "ofWindowSettings.h"

class ofRectangle;
class ofAppBaseWindow;
class ofAppGLFWWindow;
class ofBaseApp;
class ofBaseRenderer;
class ofCoreEvents;

using std::cout;
using std::endl;

static struct ofCoreInternal {
public:
	ofCoreInternal() {};
	~ofCoreInternal() {};
	
	std::string name = "virgem";
	
	std::vector <std::function<void()>> shutdownFunctions;
	// ofAppRunner
	bool initialized = false;
	bool exiting = false;
	ofCoreEvents noopEvents;
//	ofMainLoop mainLoop;
	std::shared_ptr<ofMainLoop> mainLoop { std::make_shared<ofMainLoop>() };
	
	// ofFileUtils
	
	void exit() {
		if(!initialized) return;
		
		// controlled destruction of the mainLoop before
		// any other deinitialization
		// mainLoop->exit();
//		mainLoop.exit();

		// all shutdown functions called
		for (const auto & func : shutdownFunctions) {
			func();
		}

		initialized = false;
		exiting = true;
	}
	
	void init() {
		name = "inicializado";
		if (initialized) return;
		initialized  = true;
		exiting = false;
	}
	
//std::shared_ptr<ofAppBaseWindow> ofGetCurrentWindow() {

//	std::shared_ptr<ofAppBaseWindow> ofMainLoop::getCurrentWindow(){
//		return currentWindow.lock();
//	}
		
		std::shared_ptr<ofAppBaseWindow> getCurrentWindow(){
		if (mainLoop) {
			return mainLoop->getCurrentWindow();
//			return mainLoop->currentWindow.lock();
		}
		cout << "getCurrentWindow ofCore nullptr name=" << name << endl;
		return nullptr;
	}
	
} ofCore;





void ofInit();
void ofSetupOpenGL(int w, int h, ofWindowMode screenMode); // sets up the opengl context!
std::shared_ptr<ofAppBaseWindow> ofCreateWindow(const ofWindowSettings & settings); // sets up the opengl context!
std::shared_ptr<ofMainLoop> ofGetMainLoop();
void ofSetMainLoop(const std::shared_ptr<ofMainLoop> & mainLoop);

template <typename Window>
void ofSetupOpenGL(const std::shared_ptr<Window> & windowPtr, int w, int h, ofWindowMode screenMode) {
	ofInit();
	ofWindowSettings settings;
	settings.setSize(w, h);
	settings.windowMode = screenMode;
//	ofCore.mainLoop.addWindow(windowPtr);
	windowPtr->setup(settings);
}

//special case so we preserve supplied settngs
//TODO: remove me when we remove the ofSetupOpenGL legacy approach.
void ofSetupOpenGL(const std::shared_ptr<ofAppGLFWWindow> & windowPtr, int w, int h, ofWindowMode screenMode);

template <typename Window>
static void noopDeleter(Window *) { }

template <typename Window>
void ofSetupOpenGL(Window * windowPtr, int w, int h, ofWindowMode screenMode) {
	std::shared_ptr<Window> window = std::shared_ptr<Window>(windowPtr, std::function<void(Window *)>(noopDeleter<Window>));
	ofSetupOpenGL(window, w, h, screenMode);
}

int ofRunApp(const std::shared_ptr<ofBaseApp> & OFSA);
//int ofRunApp(ofBaseApp * OFSA = nullptr); // will be deprecated
void ofRunApp(const std::shared_ptr<ofAppBaseWindow> & window,
			  const std::shared_ptr<ofBaseApp> & app);
int ofRunMainLoop();

ofBaseApp * ofGetAppPtr();

void ofExit(int status = 0);

bool ofIsCurrentThreadTheMainThread();
std::thread::id ofGetMainThreadId();

//-------------------------- time
float ofGetFrameRate();
float ofGetTargetFrameRate();
bool ofGetTargetFrameRateEnabled();
uint64_t ofGetFrameNum();
void ofSetFrameRate(int targetRate);
double ofGetLastFrameTime();
void ofSetTimeModeSystem();
ofTimeMode ofGetTimeMode();
uint64_t ofGetFixedStepForFps(double fps);
void ofSetTimeModeFixedRate(uint64_t stepNanos = ofGetFixedStepForFps(60)); //default nanos for 1 frame at 60fps
void ofSetTimeModeFiltered(float alpha = 0.9);

void ofSetOrientation(ofOrientation orientation, bool vFlip = true);
ofOrientation ofGetOrientation();

//-------------------------- cursor
void ofHideCursor();
void ofShowCursor();
//-------------------------- window / screen
glm::ivec2 ofGetWindowPosition();
int ofGetWindowPositionX();
int ofGetWindowPositionY();
int ofGetScreenWidth();
int ofGetScreenHeight();
glm::ivec2 ofGetScreenSize();
int ofGetWindowMode();
int ofGetWidth();
int ofGetHeight();
int ofGetWindowWidth();
int ofGetWindowHeight();

std::string ofGetClipboardString();
void ofSetClipboardString(const std::string & str);

/// \returns a random number between 0 and the width of the window.
float ofRandomWidth();

/// \returns a random number between 0 and the height of the window.
float ofRandomHeight();
bool ofDoesHWOrientation();
glm::ivec2 ofGetWindowSize();
ofRectangle ofGetWindowRect();
ofAppBaseWindow * ofGetWindowPtr();
std::shared_ptr<ofAppBaseWindow> ofGetCurrentWindow();

void ofSetWindowPosition(int x, int y);
void ofSetWindowShape(int width, int height);
void ofSetWindowRect(const ofRectangle & rect);
void ofSetWindowTitle(std::string title);
void ofEnableSetupScreen();
void ofDisableSetupScreen();
void ofSetFullscreen(bool fullscreen);
void ofToggleFullscreen();
//-------------------------- sync
void ofSetVerticalSync(bool bSync);

ofCoreEvents & ofEvents();
void ofSetCurrentRenderer(const std::shared_ptr<ofBaseRenderer> & renderer, bool setDefaults = false);
std::shared_ptr<ofBaseRenderer> & ofGetCurrentRenderer();
void ofSetEscapeQuitsApp(bool bQuitOnEsc);

//-------------------------- native window handles
#if defined(TARGET_LINUX) && !defined(TARGET_RASPBERRY_PI_LEGACY)
typedef unsigned long Window;
struct _XDisplay;
typedef struct _XDisplay Display;
Display * ofGetX11Display();
Window ofGetX11Window();
#endif

#if defined(TARGET_LINUX) && !defined(TARGET_OPENGLES)
struct __GLXcontextRec;
typedef __GLXcontextRec * GLXContext;
GLXContext ofGetGLXContext();
#endif

#if defined(TARGET_LINUX) && defined(TARGET_OPENGLES)
EGLDisplay ofGetEGLDisplay();
EGLContext ofGetEGLContext();
EGLSurface ofGetEGLSurface();
#endif

#if defined(TARGET_OSX)
void * ofGetNSGLContext();
void * ofGetCocoaWindow();
#endif

#if defined(TARGET_WIN32)
HGLRC ofGetWGLContext();
HWND ofGetWin32Window();
#endif


