#include "ofxFpsPlotter.h"
#include "ofAppRunner.h"

ofxFpsPlotter::ofxFpsPlotter()
:ofxValuePlotter(){

	setName("FPS");
	setup();

}

ofxFpsPlotter::ofxFpsPlotter(const ofJson & config)
:ofxFpsPlotter(){

	_setConfig(config);

}

ofxFpsPlotter::ofxFpsPlotter(string label, float minValue, float maxValue, int plotSize, float width, float height)
	:ofxValuePlotter(label, minValue, maxValue, plotSize, width, height){

	setup();

}

ofxFpsPlotter::~ofxFpsPlotter(){
	ofRemoveListener(ofEvents().update,this,&ofxFpsPlotter::update);
}

void ofxFpsPlotter::setup(){
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

