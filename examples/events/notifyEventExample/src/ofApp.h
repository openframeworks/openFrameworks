#pragma once

#include "ofMain.h"

// note: this example, inspired by simpleEvents, is kept voluntarily
// as small as possible to give a sense of the bare minimum requirements

class ofApp : public ofBaseApp {

	ofParameter<bool> animate { "animate", false };
	ofParameter<void> refresh { "refresh" };
	ofParameter<float> size { "size", 1.0f, 0.0f, 100.0f };
	ofParameterGroup params { "myCallbacksAndSettings", animate, refresh, size };

	bool animating { false };
	ofColor color { ofColor::white };

public:
	void update() override;
	void draw() override;
	void keyPressed(int key) override;
};
