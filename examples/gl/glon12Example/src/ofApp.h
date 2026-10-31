#pragma once

#include "ofMain.h"

class ofApp : public ofBaseApp {
public:
	void setup() override;
	void update() override;
	void draw() override;
	void keyPressed(int key) override;

	bool warpRequested = false;

private:
	void restart(bool warp);

	std::string renderer;
	std::string version;
	std::string mode;
	ofEasyCam cam;
	ofVboMesh box;
	ofShader shader;
};
