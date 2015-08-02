#include "ofxFpsPlotter.h"
#include "ofAppRunner.h"

ofxFpsPlotter::ofxFpsPlotter(){
}

ofxFpsPlotter::ofxFpsPlotter(const Config & config){
	setup(config);
}

ofxFpsPlotter::ofxFpsPlotter(float minValue, float maxValue, int plotSize, float width, float height){
	setup(minValue, maxValue, plotSize, width, height);
}

ofxFpsPlotter::~ofxFpsPlotter(){
}

ofxFpsPlotter & ofxFpsPlotter::setup(const Config & config){
	return setup(config.minValue, config.maxValue, config.plotSize);
}

ofxFpsPlotter & ofxFpsPlotter::setup(float minValue, float maxValue, int plotSize, float width, float height){
	if(minValue == maxValue){
		if(ofGetTargetFrameRate() > 0){
			minValue = 0;
			maxValue = ofGetTargetFrameRate();
		}
	}
	ofxValuePlotter::setup("FPS", minValue, maxValue, plotSize, width, height);
	setDecimalPlace(0);
	return *this;
}

void ofxFpsPlotter::update(){
	ofxValuePlotter::update(ofGetFrameRate());
}

