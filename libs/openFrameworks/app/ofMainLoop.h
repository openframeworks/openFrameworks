#pragma once

#include "ofConstants.h"
#include "ofAppBaseWindow.h"
#include "ofBaseApp.h"
#include "ofEvents.h"

class ofMainLoop {
public:
	ofMainLoop();
	virtual ~ofMainLoop();

	std::shared_ptr<ofAppBaseWindow> createWindow(const ofWindowSettings & settings);
	template<typename Window>
	void addWindow(std::shared_ptr<Window> window){
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
		windowsApps[window] = std::shared_ptr<ofBaseApp>();
		currentWindow = window;
		ofAddListener(window->events().keyPressed,this,&ofMainLoop::keyPressed);
	}

	void run(std::shared_ptr<ofAppBaseWindow> window, std::shared_ptr<ofBaseApp> && app);
	void run(std::shared_ptr<ofBaseApp> && app);
	int loop();
	void loopOnce();
	void pollEvents();
	void exit();
	void shouldClose(int status);
	std::shared_ptr<ofAppBaseWindow> getCurrentWindow();
	void setCurrentWindow(std::shared_ptr<ofAppBaseWindow> window);
	void setCurrentWindow(ofAppBaseWindow * window);
	std::shared_ptr<ofBaseApp> getCurrentApp();
	void setEscapeQuitsLoop(bool quits);

	ofEvent<void> exitEvent;
private:
	void keyPressed(ofKeyEventArgs & key);
	map<std::shared_ptr<ofAppBaseWindow>,std::shared_ptr<ofBaseApp> > windowsApps;
	bool bShouldClose;
	std::weak_ptr<ofAppBaseWindow> currentWindow;
	int status;
	bool allowMultiWindow;
	std::function<void()> windowLoop;
	std::function<void()> windowPollEvents;
	bool escapeQuits;
};
