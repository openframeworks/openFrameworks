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
	
	void FloatImage::loadRaw(string filename, unsigned int width, unsigned int height) {
		ifstream file;
		file.open(ofToDataPath(filename).c_str(), ios::in | ios::binary);
		if (file.is_open()) {
			allocate(width, height);
			unsigned int length = width * height;
			file.read(reinterpret_cast<char*>(getPixels()), sizeof(float) * length);
		}
		file.close();
		update();
	}
	
	void FloatImage::saveRaw(string filename) {
		ofstream file;
		file.open(ofToDataPath(filename).c_str(), ios::out | ios::binary);
		if (file.is_open()) {
			file.write(reinterpret_cast<char*>(getPixels()), sizeof(float) * getWidth() * getHeight());
		}
		file.close();
	}
	
	void FloatImage::set(ofImage& img) {
		imitate(*this, img);
		Mat imgMat = ofxCv::toCv(img);
		imgMat.convertTo(pixels, CV_32F, 1 / 255.);
	}
	
	void FloatImage::set(float* img) {
		Mat imgMat = Mat(pixels.rows, pixels.cols, CV_32FC1, img);
		imgMat.copyTo(pixels);
	}
	
	void FloatImage::set(int x, int y, float value) {
		pixels.at<float>(y, x) = value;
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
#ifdef ZERO_STEP
		data.resize(width * height);
		pixels = Mat(height, width, CV_32FC1, &data[0]);
#else
		pixels = Mat(height, width, CV_32FC1);
#endif
		texture.allocate(width, height, GL_LUMINANCE32F_ARB);
	}
	
	void FloatImage::update() {
		texture.loadData((float*) pixels.data, getWidth(), getHeight(), GL_LUMINANCE32F_ARB);
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
	
	float FloatImage::get(int x, int y) {
		return pixels.at<float>(y, x);
	}
	
	cv::Size FloatImage::getSize() {
		return pixels.size();
	}
	
	float* FloatImage::getPixels() {
		return (float*) pixels.ptr();
	}
	
	Mat& FloatImage::toCv() {
		return pixels;
	}
	
}