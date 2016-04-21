#include "ofxFpsPlotter.h"
#include "ofAppRunner.h"

ofxFpsPlotter::ofxFpsPlotter(){
}

ofxFpsPlotter::ofxFpsPlotter(const ofxValuePlotter::Config & config)
:ofxValuePlotter(ofParameter<float>("fps",0),config){
	if(minVal == maxVal) {
		if(ofGetTargetFrameRate() > 0) {
			minVal = 0;
			maxVal = ofGetTargetFrameRate();
		}
	}
	setDecimalPlace(0);
	ofAddListener(ofEvents().update,this,&ofxFpsPlotter::update);
}

ofxFpsPlotter::~ofxFpsPlotter(){
	ofRemoveListener(ofEvents().update,this,&ofxFpsPlotter::update);
}

ofxFpsPlotter & ofxFpsPlotter::setup(string label, float minValue, float maxValue, int plotSize, float width, float height){
	ofxValuePlotter::setup(label, minValue, maxValue, plotSize, width, height);
	if(minVal == maxVal) {
		if(ofGetTargetFrameRate() > 0) {
			minVal = 0;
			maxVal = ofGetTargetFrameRate();
		}
	}
	setDecimalPlace(0);
	ofAddListener(ofEvents().update,this,&ofxFpsPlotter::update);
	return *this;
}

void ofxFpsPlotter::update(ofEventArgs &){
	value = ofGetFrameRate();
	setNeedsRedraw();
}

