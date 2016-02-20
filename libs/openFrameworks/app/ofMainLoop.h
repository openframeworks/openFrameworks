#pragma once

#include "ofConstants.h"
#include "ofAppBaseWindow.h"
#include "ofBaseApp.h"
#include "ofEvents.h"

class ofMainLoop {
public:
	ofMainLoop();
	virtual ~ofMainLoop();

	shared_ptr<ofAppBaseWindow> createWindow(const ofWindowSettings & settings);
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
