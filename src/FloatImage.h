#pragma once

#include "ofxCv.h"
#include "ofTexture.h"
#include "ofBaseTypes.h"
#include "ofGraphics.h"

// if there is some issue with cv allocating non-zero step sizes,
// you can compile with ZERO_STEP to force zero step size.

namespace ofxCv {
	
	using namespace cv;
	
	// single channel, floating point image
	class FloatImage : public ofBaseDraws {
	public:
		void loadImage(string filename);
		void loadRaw(string filename, unsigned int width, unsigned int height);
		void saveRaw(string filename);
		
		void set(ofImage& img);
		void set(float* img);
		void set(int x, int y, float value);
		void normalizeToSum();
		void normalizeToMax();
		void allocate(int width, int height);
		void update();
		void draw();
		void draw(float x,float y);	
		void draw(float x,float y,float w, float h);	
		float getWidth();	
		float getHeight();
		cv::Size getSize();
		float* getPixels();
		float get(int x, int y);
		Mat& toCv();
	private:		
		Mat pixels;
		ofTexture texture;
		
#ifdef ZERO_STEP
		vector<float> data;
#endif
	};
	
}