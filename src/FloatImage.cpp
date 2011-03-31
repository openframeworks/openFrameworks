#include "ofxCv.h"

namespace ofxCv {
	
	using namespace cv;
	
	void FloatImage::loadImage(string filename) {
		ofImage img;
		img.loadImage(filename);
		img.setImageType(OF_IMAGE_GRAYSCALE);
		set(img);
		update();
	}
	
	void FloatImage::set(ofImage& img) {
		imitate(*this, img);
		Mat imgMat = ofxCv::getMat(img);
		imgMat.convertTo(pixels, CV_32F, 1 / 255.);
	}
	
	void FloatImage::normalizeToSum() {
		pixels.convertTo(pixels, CV_32F, 1. / sum(pixels)[0]);
	}
	
	void FloatImage::normalizeToMax() {
		double minVal, maxVal;
		minMaxLoc(pixels, &minVal, &maxVal);
		pixels.convertTo(pixels, CV_32F, 1. / maxVal);
	}
	
	void FloatImage::allocate(int width, int height) {
		pixels = Mat(height, width, CV_32FC1);
		texture.allocate(width, height, GL_LUMINANCE32F_ARB);
		
		// just for testing, allocate with noise initially
		randu(pixels, 0, 1);
	}
	
	void FloatImage::update() {
		texture.loadData((float*) pixels.data, getWidth(), getHeight(), GL_LUMINANCE);
	}
	
	void FloatImage::draw() {
		texture.draw(0, 0);
	}
	
	void FloatImage::draw(float x,float y) {
		draw(x, y, getWidth(), getHeight());
	}
	
	void FloatImage::draw(float x,float y,float w, float h) {
		ofPushMatrix();
		ofTranslate(x, y);
		ofScale(w / getWidth(), h / getHeight());
		draw();
		ofPopMatrix();
	}
	
	float FloatImage::getWidth() {
		return pixels.cols;
	}
	
	float FloatImage::getHeight() {
		return pixels.rows;
	}
	
	Mat& FloatImage::getMat() {
		return pixels;
	}
	
}