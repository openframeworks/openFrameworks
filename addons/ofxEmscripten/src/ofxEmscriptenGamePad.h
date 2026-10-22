#pragma once

#include "ofConstants.h"
#include "emscripten/html5.h"

class ofxEmscriptenGamePad {
public:
	ofxEmscriptenGamePad();
	~ofxEmscriptenGamePad();

	void gamePadData();
private:
	static int gamepadconnected_cb(int eventType, const EmscriptenGamepadEvent *gamepadEvent, void *userData);
	static int gamepaddisconnected_cb(int eventType, const EmscriptenGamepadEvent *gamepadEvent, void *userData);
	
	EmscriptenGamepadEvent prevState[32];
	int prevNumGamepads = 0;
};
