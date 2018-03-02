#pragma once

#include "ofConstants.h"
#include "ofAppBaseWindow.h"

class ofBaseApp;
class ofCoreEvents;
class ofPath;
class of3dGraphics;
class ofBaseRenderer;

class ofAppNoWindow : public ofAppBaseWindow {

public:
	ofAppNoWindow();
	~ofAppNoWindow(){}

	static bool doesLoop(){ return false; }
	static bool allowsMultiWindow(){ return false; }
	static void loop(){};
	static bool needsPolling(){ return false; }
	static void pollEvents(){};

	void run(ofBaseApp * appPtr);

	static void exitApp();
	void setup(const ofWindowSettings & settings);
	void update();
	void draw();

	glm::vec2	getWindowPosition();
	glm::vec2	getWindowSize();
	glm::vec2	getScreenSize();

	int			getWidth();
	int			getHeight();

	ofCoreEvents & events();
	std::shared_ptr<ofBaseRenderer> & renderer();

private:
	int width, height;

    ofBaseApp *		ofAppPtr;
	std::unique_ptr<ofCoreEvents> coreEvents;
    std::shared_ptr<ofBaseRenderer> currentRenderer;
};
