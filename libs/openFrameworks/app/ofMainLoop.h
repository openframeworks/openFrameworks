#pragma once

#include "ofConstants.h"
#include "ofAppBaseWindow.h"
#include "ofBaseApp.h"

class ofMainLoop {
public:
	ofMainLoop();
	virtual ~ofMainLoop();

	shared_ptr<ofAppBaseWindow> createWindow(const ofWindowSettings & settings);
	void run(shared_ptr<ofAppBaseWindow> window, shared_ptr<ofBaseApp> app);
	int loop();
	void loopOnce();
	ofCoreEvents & events();
	void shouldClose(int status);
	shared_ptr<ofAppBaseWindow> getCurrentWindow();
	shared_ptr<ofBaseApp> getCurrentApp();
private:
	map<shared_ptr<ofAppBaseWindow>,shared_ptr<ofBaseApp>> windowsApps;
	bool bShouldClose;
	shared_ptr<ofAppBaseWindow> currentWindow;
	int status;
};
