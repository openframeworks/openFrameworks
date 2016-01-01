#include "ofxValuePlotter.h"
#include "ofGraphics.h"
using namespace std;

ofxValuePlotter::ofxValuePlotter(){
}

ofxValuePlotter::ofxValuePlotter(ofParameter<float> value, const Config & config = Config()) :
	ofxBaseGui(config){
	setup(config);
	setName(value.getName());
}

ofxValuePlotter::~ofxValuePlotter(){
	value.removeListener(this,&ofxValuePlotter::valueChanged);
}

ofxValuePlotter & ofxValuePlotter::setup(const Config & config){
	return setup("", config.minValue, config.maxValue, config.plotSize, config.shape.width, config.shape.height);
}

ofxValuePlotter & ofxValuePlotter::setup(string label, float minValue, float maxValue, int plotSize, float width, float height){
	b.width = width;
	b.height = height;
	minVal = minValue;
	maxVal = maxValue;
	autoscale = minVal == maxVal;
	buffer.clear();
	this->plotSize = plotSize;
	value.addListener(this,&ofxValuePlotter::valueChanged);
	setName(label);
	setNeedsRedraw();
	return *this;
}

void ofxValuePlotter::setDecimalPlace(int place){
	decimalPlace = place;
}

void ofxValuePlotter::generateDraw(){

	bg.clear();

	bg.setFillColor(thisBackgroundColor);
	bg.setFilled(true);
	bg.rectangle(b);

	label = ofToString(value.get(), decimalPlace);
	if(bShowName){
		label += " " + this->getName();
	}

	textMesh = getTextMesh(label, b.x + textPadding, b.y + b.height / 2 + 4);

	if(plotSize > 0){

		plot.clear();
		if(minVal != maxVal && buffer.size() > 1){
			plot.moveTo(b.x, b.y + b.height);
			unsigned int i;
			for(i = 0; i < buffer.size(); i++){
				float x = ofMap(i, 0, buffer.size() - 1, b.x, b.x + b.width);
				float y = ofMap(buffer[i], minVal, maxVal, b.y + b.height, b.y);
				plot.lineTo(x, y);
			}
			plot.lineTo(b.x + b.width, b.y + b.height);
			plot.close();
			plot.setFilled(true);
			plot.setFillColor(thisFillColor);
		}
	}
}

void ofxValuePlotter::render(){
	ofColor c = ofGetStyle().color;

	bg.draw();
	if(plotSize > 0){
		plot.draw();
	}

	ofBlendMode blendMode = ofGetStyle().blendingMode;
	if(blendMode != OF_BLENDMODE_ALPHA){
		ofEnableAlphaBlending();
	}
	ofSetColor(textColor);

	bindFontTexture();
	textMesh.draw();
	unbindFontTexture();

	ofSetColor(c);
	if(blendMode != OF_BLENDMODE_ALPHA){
		ofEnableBlendMode(blendMode);
	}
}

ofAbstractParameter & ofxValuePlotter::getParameter(){
	return label;
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
