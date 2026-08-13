#include "ofApp.h"

#include <cstdlib>
#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

namespace {
// std::exit() alone doesn't stop Emscripten's requestAnimationFrame loop
// (the runtime stays alive for pending async work), so draw() keeps getting
// called forever after "finishing" unless we guard against it below, and the
// process itself needs emscripten_force_exit() to actually stop.
void terminateProcess(int code) {
#ifdef __EMSCRIPTEN__
	emscripten_force_exit(code);
#else
	std::exit(code);
#endif
}
}

void ofApp::setup() {
	ofSetLogLevel(OF_LOG_NOTICE);
	ofSetFrameRate(60);
	ofBackground(0);
	ofLogNotice("glRenderSmokeTest") << "renderer=" << glGetString(GL_RENDERER)
		<< " version=" << glGetString(GL_VERSION);
	core.allocate();
}

void ofApp::draw() {
	if (core.finished()) {
		return;
	}

	core.drawFrame();

	if (core.finished()) {
		ofLogNotice("glRenderSmokeTest") << core.resultLine();
		terminateProcess(core.passed() ? 0 : 1);
	}
}
