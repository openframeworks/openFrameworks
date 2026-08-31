#include "ofxEmscriptenGamePad.h"

using namespace std;

static inline const char* emscripten_event_type_to_string(int eventType) {
	const char *events[] = { "(invalid)", "(none)", "keypress", "keydown", "keyup", "click", "mousedown", "mouseup", "dblclick", "mousemove", "wheel", "resize", 
	"scroll", "blur", "focus", "focusin", "focusout", "deviceorientation", "devicemotion", "orientationchange", "fullscreenchange", "pointerlockchange", 
	"visibilitychange", "touchstart", "touchend", "touchmove", "touchcancel", "gamepadconnected", "gamepaddisconnected", "beforeunload", 
	"batterychargingchange", "batterylevelchange", "webglcontextlost", "webglcontextrestored", "mouseenter", "mouseleave", "mouseover", "mouseout", "(invalid)" };
	++eventType;
	if (eventType < 0) eventType = 0;
	if (eventType >= sizeof(events)/sizeof(events[0])) eventType = sizeof(events)/sizeof(events[0])-1;
	return events[eventType];
}

const char* emscripten_result_to_string(EMSCRIPTEN_RESULT result) {
	if (result == EMSCRIPTEN_RESULT_SUCCESS) return "EMSCRIPTEN_RESULT_SUCCESS";
	if (result == EMSCRIPTEN_RESULT_DEFERRED) return "EMSCRIPTEN_RESULT_DEFERRED";
	if (result == EMSCRIPTEN_RESULT_NOT_SUPPORTED) return "EMSCRIPTEN_RESULT_NOT_SUPPORTED";
	if (result == EMSCRIPTEN_RESULT_FAILED_NOT_DEFERRED) return "EMSCRIPTEN_RESULT_FAILED_NOT_DEFERRED";
	if (result == EMSCRIPTEN_RESULT_INVALID_TARGET) return "EMSCRIPTEN_RESULT_INVALID_TARGET";
	if (result == EMSCRIPTEN_RESULT_UNKNOWN_TARGET) return "EMSCRIPTEN_RESULT_UNKNOWN_TARGET";
	if (result == EMSCRIPTEN_RESULT_INVALID_PARAM) return "EMSCRIPTEN_RESULT_INVALID_PARAM";
	if (result == EMSCRIPTEN_RESULT_FAILED) return "EMSCRIPTEN_RESULT_FAILED";
	if (result == EMSCRIPTEN_RESULT_NO_DATA) return "EMSCRIPTEN_RESULT_NO_DATA";
	return "Unknown EMSCRIPTEN_RESULT!";
}


ofxEmscriptenGamePad::ofxEmscriptenGamePad() {
	emscripten_set_gamepadconnected_callback(this,1,&gamepadconnected_cb);
	emscripten_set_gamepaddisconnected_callback(this,1,&gamepaddisconnected_cb);
}

ofxEmscriptenGamePad::~ofxEmscriptenGamePad() {}

int ofxEmscriptenGamePad::gamepadconnected_cb(int eventType, const EmscriptenGamepadEvent *gamepadEvent, void *userData){
	printf("%s: timeStamp: %g, connected: %d, index: %ld, numAxes: %d, numButtons: %d, id: \"%s\", mapping: \"%s\"\n",
	eventType != 0 ? emscripten_event_type_to_string(eventType) : "Gamepad state", gamepadEvent->timestamp, gamepadEvent->connected, gamepadEvent->index, 
	gamepadEvent->numAxes, gamepadEvent->numButtons, gamepadEvent->id, gamepadEvent->mapping);
	return 0;
}

int ofxEmscriptenGamePad::gamepaddisconnected_cb(int eventType, const EmscriptenGamepadEvent *gamepadEvent, void *userData){
	printf("%s: timeStamp: %g, connected: %d, index: %ld, numAxes: %d, numButtons: %d, id: \"%s\", mapping: \"%s\"\n",
	eventType != 0 ? emscripten_event_type_to_string(eventType) : "Gamepad state", gamepadEvent->timestamp, gamepadEvent->connected, gamepadEvent->index, 
	gamepadEvent->numAxes, gamepadEvent->numButtons, gamepadEvent->id, gamepadEvent->mapping);
	return 0;
}

void ofxEmscriptenGamePad::gamePadData() {
	emscripten_sample_gamepad_data();
	int numGamepads = emscripten_get_num_gamepads();
	if (numGamepads != prevNumGamepads) {
		printf("Number of connected gamepads: %d\n", numGamepads);
		prevNumGamepads = numGamepads;
	}
	for(int i = 0; i < numGamepads && i < 32; ++i) {
		EmscriptenGamepadEvent ge;
		int ret = emscripten_get_gamepad_status(i, &ge);
		if (ret == EMSCRIPTEN_RESULT_SUCCESS) {
			int g = ge.index;
			for(int j = 0; j < ge.numAxes; ++j) {
				if (ge.axis[j] != prevState[g].axis[j])
					printf("Gamepad %d, axis %d: %g\n", g, j, ge.axis[j]);
				}
			for(int j = 0; j < ge.numButtons; ++j) {
				if (ge.analogButton[j] != prevState[g].analogButton[j] || ge.digitalButton[j] != prevState[g].digitalButton[j]) {
					printf("Gamepad %d, button %d: Digital: %d, Analog: %g\n", g, j, ge.digitalButton[j], ge.analogButton[j]);
				}
			}
			prevState[g] = ge;
		}
	}
}
