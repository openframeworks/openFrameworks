#pragma once
#include "ofMain.h"

class ImgAnalysisThread: public ofThread {
public:
	ImgAnalysisThread();
	~ImgAnalysisThread();
	void analize(ofPixels & pixels);
	void update();
	bool isFrameNew();
	ofPixels & getPixels();
	ofTexture & getTexture();
	void draw(float x, float y);
	void draw(float x, float y, float w, float h);

private:
	void threadedFunction();
	ofThreadChannel<ofPixels> toAnalize;
	ofThreadChannel<ofPixels> analized;
	ofPixels pixels;
	ofTexture texture;
	bool newFrame;
};
