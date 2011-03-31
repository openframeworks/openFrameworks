#pragma once

#include "ofxCv.h"
#include "ofTexture.h"
#include "ofBaseTypes.h"
#include "ofGraphics.h"

namespace ofxCv {
	
	using namespace cv;
	
	// single channel, floating point image
	class FloatImage : public ofBaseDraws {
	public:
		void loadImage(string filename);
		void set(ofImage& img);
		void normalizeToSum();
		void normalizeToMax();
		void allocate(int width, int height);
		void update();
		void draw();
		void draw(float x,float y);	
		void draw(float x,float y,float w, float h);	
		float getWidth();	
		float getHeight();	
		Mat& getMat();
	private:
		Mat pixels;
		ofTexture texture;
	};
	
}