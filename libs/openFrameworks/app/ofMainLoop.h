#pragma once

#include "ofEvents.h"
//#include <unordered_map>

class ofBaseApp;
class ofAppBaseWindow;
class ofWindowSettings;

class ofMainLoop {
public:
	ofMainLoop();
	virtual ~ofMainLoop();

	std::shared_ptr<ofAppBaseWindow> createWindow(const ofWindowSettings & settings);

	template<typename Window>
	void addWindow(const std::shared_ptr<Window> & window){
		allowMultiWindow = Window::allowsMultiWindow();
		if(Window::doesLoop()){
			windowLoop = Window::loop;
		}
		if(Window::needsPolling()){
			windowPollEvents = Window::pollEvents;
		}
		if(!allowMultiWindow){
			windows.clear();
//			windowsApps.clear();
		}
		windows.emplace_back(window);
//		windowsApps[window] = std::shared_ptr<ofBaseApp>();
		currentWindow = window;
		ofAddListener(window->events().keyPressed,this,&ofMainLoop::keyPressed);
	}

	void run(const std::shared_ptr<ofAppBaseWindow> & window, const std::shared_ptr<ofBaseApp> & app);
//	std::shared_ptr<ofBaseApp> && app);
	void run(const std::shared_ptr<ofBaseApp> & app);
	int loop();
	void loopOnce();
	void pollEvents();
	void exit();
	void shouldClose(int status);
	std::shared_ptr<ofAppBaseWindow> getCurrentWindow();
	void setCurrentWindow(const std::shared_ptr<ofAppBaseWindow> & window);
	void setCurrentWindow(ofAppBaseWindow * window);
	std::shared_ptr<ofBaseApp> getCurrentApp();
	void setEscapeQuitsLoop(bool quits);

	ofEvent<void> exitEvent;
	ofEvent<void> loopEvent;
	
	std::thread::id get_thread_id() { return thread_id; };
	
	std::vector <std::shared_ptr<ofAppBaseWindow> > getWindows() { return windows; }

	void ofBeginWindow(int n);
	void ofEndWindow();

	
public:
	std::weak_ptr<ofAppBaseWindow> currentWindow;

private:
	std::thread::id thread_id { std::this_thread::get_id() };

	void keyPressed(ofKeyEventArgs & key);
	std::vector <std::shared_ptr<ofAppBaseWindow> > windows;
	std::shared_ptr<ofBaseApp> mainApp;
	
	bool bShouldClose;
	int status;
	bool allowMultiWindow;
	std::function<void()> windowLoop;
	std::function<void()> windowPollEvents;
	bool escapeQuits;
	

};
