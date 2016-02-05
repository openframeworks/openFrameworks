#pragma once

#include "ofConstants.h"
#include "ofAppBaseWindow.h"
#include "ofBaseApp.h"
#include "ofEvents.h"

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

class ofMainLoop {
public:
	ofMainLoop();
	virtual ~ofMainLoop();

	template<typename Settings>
	shared_ptr<ofAppBaseWindow> createWindow(const Settings & settings){
		#if defined(TARGET_NODISPLAY)
		shared_ptr<ofAppNoWindow> window = shared_ptr<ofAppNoWindow>(new ofAppNoWindow());
		#elif defined(TARGET_OF_IOS)
		shared_ptr<ofAppiOSWindow> window = shared_ptr<ofAppiOSWindow>(new ofAppiOSWindow());
		#elif defined(TARGET_ANDROID)
		shared_ptr<ofAppAndroidWindow> window = shared_ptr<ofAppAndroidWindow>(new ofAppAndroidWindow());
		#elif defined(TARGET_RASPBERRY_PI)
		shared_ptr<ofAppEGLWindow> window = shared_ptr<ofAppEGLWindow>(new ofAppEGLWindow());
		#elif defined(TARGET_EMSCRIPTEN)
		shared_ptr<ofxAppEmscriptenWindow> window = shared_ptr<ofxAppEmscriptenWindow>(new ofxAppEmscriptenWindow);
		#elif defined(TARGET_OPENGLES)
		shared_ptr<ofAppGLFWWindow> window = shared_ptr<ofAppGLFWWindow>(new ofAppGLFWWindow());
		#else
		shared_ptr<ofAppGLFWWindow> window = shared_ptr<ofAppGLFWWindow>(new ofAppGLFWWindow());
		#endif
		addWindow(window);
		window->setup(settings);
		return window;
	}

	template<typename Window>
	void addWindow(shared_ptr<Window> window){
		allowMultiWindow = Window::allowsMultiWindow();
		if(Window::doesLoop()){
		    windowLoop = Window::loop;
		}
		if(Window::needsPolling()){
			windowPollEvents = Window::pollEvents;
		}
		if(!allowMultiWindow){
		    windowsApps.clear();
		}
		windowsApps[window] = shared_ptr<ofBaseApp>();
		currentWindow = window;
		ofAddListener(window->events().keyPressed,this,&ofMainLoop::keyPressed);
	}

	void run(shared_ptr<ofAppBaseWindow> window, shared_ptr<ofBaseApp> app);
	void run(shared_ptr<ofBaseApp> app);
	int loop();
	void loopOnce();
	void pollEvents();
	void exit();
	ofCoreEvents & events();
	void shouldClose(int status);
	shared_ptr<ofAppBaseWindow> getCurrentWindow();
	void setCurrentWindow(shared_ptr<ofAppBaseWindow> window);
	void setCurrentWindow(ofAppBaseWindow * window);
	shared_ptr<ofBaseApp> getCurrentApp();
	void setEscapeQuitsLoop(bool quits);

	ofEvent<void> exitEvent;
private:
	void keyPressed(ofKeyEventArgs & key);
	map<shared_ptr<ofAppBaseWindow>,shared_ptr<ofBaseApp> > windowsApps;
	bool bShouldClose;
	shared_ptr<ofAppBaseWindow> currentWindow;
	int status;
	bool allowMultiWindow;
	std::function<void()> windowLoop;
	std::function<void()> windowPollEvents;
	bool escapeQuits;
};
