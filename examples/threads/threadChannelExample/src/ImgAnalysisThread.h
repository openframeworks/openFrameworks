#pragma once

#include "ofMain.h"

class ImgAnalysisThread: public ofThread {
public:
	ImgAnalysisThread();
	~ImgAnalysisThread();
	void analyze(ofPixels & pixels);
	void update();
	bool isFrameNew();
	ofPixels & getPixels();
	ofTexture & getTexture();
	void draw(float x, float y);
	void draw(float x, float y, float w, float h);

private:
	void threadedFunction();
	ofThreadChannel<ofPixels> toAnalyze;
	ofThreadChannel<ofPixels> analyzed;
	ofPixels pixels;
	ofTexture texture;
	bool newFrame;
};
