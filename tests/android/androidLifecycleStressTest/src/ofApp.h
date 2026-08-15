#pragma once

#include "ofMain.h"
#include "ofxAndroid.h"

class ofApp : public ofxAndroidApp {
public:
	void setup() override;
	void update() override;
	void draw() override;
	void exit() override;

	void pause() override;
	void resume() override;
	void stop() override;
	void unloadGL() override;
	void reloadGL() override;

	bool backPressed() override;

private:
	void allocateGLResources();
	void releaseGLResources();
	void logLifecycle(const std::string & event);

	ofVboMesh indexedMesh;
	ofFbo fbo;
	ofPath tessellatedPath;
	ofShader shader;

	uint64_t renderedFrames = 0;
	uint64_t lifecycleSequence = 0;
	int pauseCount = 0;
	int resumeCount = 0;
	int unloadCount = 0;
	int reloadCount = 0;
	bool resourcesAllocated = false;
};
