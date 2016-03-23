#include "ofxValuePlotter.h"
#include "ofGraphics.h"
using namespace std;

ofxValuePlotter::ofxValuePlotter()
	:ofxBaseGui(){

	setup();

}

ofxValuePlotter::ofxValuePlotter(const ofJson & config)
	:ofxBaseGui(){

	setup();
	_setConfig(config);

}


ofxValuePlotter::ofxValuePlotter(ofParameter<float> value, const ofJson & config) :
	ofxBaseGui(){

	this->value.makeReferenceTo(value);
	setup();
	_setConfig(config);
}

ofxValuePlotter::ofxValuePlotter(string label, float minValue, float maxValue, int plotSize, float width, float height)
	:ofxValuePlotter(){

	setSize(width, height);
	minVal = minValue;
	maxVal = maxValue;
	this->plotSize = plotSize;
	setName(label);

}

ofxValuePlotter::~ofxValuePlotter(){
	value.removeListener(this,&ofxValuePlotter::valueChanged);
}

void ofxValuePlotter::setup(){

	decimalPlace.set("precision", 3);
	minVal.set("min", 0);
	maxVal.set("max", 0);
	plotSize.set("plotsize", 100);

	autoscale = minVal == maxVal;
	buffer.clear();
	value.addListener(this,&ofxValuePlotter::valueChanged);

}

void ofxValuePlotter::_setConfig(const ofJson & config){
	ofxBaseGui::_setConfig(config);
	// TODO
}

void ofxValuePlotter::setDecimalPlace(int place){
	decimalPlace = place;
}

void ofxValuePlotter::generateDraw(){

	ofxBaseGui::generateDraw();

	label = ofToString(value.get(), decimalPlace);
	if(showName){
		label += " " + this->getName();
	}

	textMesh = getTextMesh(label, textPadding, getShape().getHeight() / 2 + 4);

	if(plotSize > 0){

		plot.clear();
		if(minVal != maxVal && buffer.size() > 1){
			plot.moveTo(0, getShape().getHeight());
			unsigned int i;
			for(i = 0; i < buffer.size(); i++){
				float x = ofMap(i, 0, buffer.size() - 1, 0, getShape().getWidth());
				float y = ofMap(buffer[i], minVal, maxVal, getShape().getHeight(), 0);
				plot.lineTo(x, y);
			}
			plot.lineTo(getShape().getWidth(), getShape().getHeight());
			plot.close();
			plot.setFilled(true);
			plot.setFillColor(fillColor);
		}
	}
}

void ofxValuePlotter::render(){
	ofColor c = ofGetStyle().color;

	ofxBaseGui::render();

	if(plotSize > 0){
		plot.draw();
	}

	ofBlendMode blendMode = ofGetStyle().blendingMode;
	if(blendMode != OF_BLENDMODE_ALPHA){
		ofEnableAlphaBlending();
	}
	ofSetColor(defaultTextColor);

	bindFontTexture();
	textMesh.draw();
	unbindFontTexture();

	ofSetColor(c);
	if(blendMode != OF_BLENDMODE_ALPHA){
		ofEnableBlendMode(blendMode);
	}
}

ofAbstractParameter & ofxValuePlotter::getParameter(){
	return value;
}

void ofxValuePlotter::valueChanged(float & value){
	if(plotSize > 0){
		buffer.push_back(value);

		if((int)buffer.size() > plotSize){
			buffer.erase(buffer.begin(), buffer.begin() + 1);
		}
		if(autoscale){
			if(value < minVal){
				minVal = value;
			}
			if(value > maxVal){
				maxVal = value;
			}
		}
	}
	setNeedsRedraw();
}
