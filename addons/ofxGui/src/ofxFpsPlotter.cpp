#include "ofxFpsPlotter.h"
#include "ofAppRunner.h"

ofxFpsPlotter::ofxFpsPlotter(const ofJson & config)
:ofxValuePlotter(ofParameter<float>("fps",0),config){

	setup(config);
}

ofxFpsPlotter::ofxFpsPlotter(string label, float minValue, float maxValue, int plotSize, float width, float height)
	:ofxValuePlotter(label, minValue, maxValue, plotSize, width, height){

	setup();

}

ofxFpsPlotter::~ofxFpsPlotter(){
	ofRemoveListener(ofEvents().update,this,&ofxFpsPlotter::update);
}

void ofxFpsPlotter::setup(const ofJson&){
	if(minVal.get() == maxVal.get()) {
		if(ofGetTargetFrameRate() > 0) {
			minVal = 0;
			maxVal = ofGetTargetFrameRate();
		}
	}
	setDecimalPlace(0);
	ofAddListener(ofEvents().update,this,&ofxFpsPlotter::update);
}

void ofxFpsPlotter::update(ofEventArgs &){
	value = ofGetFrameRate();
	setNeedsRedraw();
}

