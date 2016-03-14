#include "ofxMinimalToggle.h"
#include "ofGraphics.h"
using namespace std;

ofxMinimalToggle::ofxMinimalToggle(const std::string & toggleName, const ofJson &config) :
	ofxToggle(toggleName, config){

	setup(config);

}

ofxMinimalToggle::ofxMinimalToggle(ofParameter <bool> &val, const ofJson & config)
:ofxToggle(val, config){

	value.makeReferenceTo(val);

}

ofxMinimalToggle::ofxMinimalToggle(ofParameter<bool> &val, float width, float height)
	:ofxMinimalToggle(val){

	if(width == 0){
		width = ofxBaseGui::getTextWidth(val.getName(), height);
	}
	setSize(width, height);

}

ofxMinimalToggle::ofxMinimalToggle(const string & toggleName, bool val, float width, float height)
	:ofxMinimalToggle(toggleName){

	value.set(toggleName, val);
	if(width == 0){
		width = ofxBaseGui::getTextWidth(toggleName, height);
	}
	setSize(width, height);

}

ofxMinimalToggle::~ofxMinimalToggle(){
}

void ofxMinimalToggle::setup(const ofJson &){

	borderColor = fillColor;
	if(getWidth() == 0) {
		setSize(ofxBaseGui::getTextWidth(value.getName(), getHeight()), getHeight());
	}

}

void ofxMinimalToggle::generateDraw(){
	checkboxRect.set(1, 1, getWidth() - 2, getHeight() - 2);

	bg.clear();
	bg.setFillColor(backgroundColor);
	bg.rectangle(checkboxRect.getTopLeft()+ofPoint(1,1), checkboxRect.width-2, checkboxRect.height-2);

	fg.clear();
	fg.setFilled(true);
	fg.setFillColor(fillColor);
	fg.rectangle(checkboxRect.getTopLeft()+ofPoint(1,1), checkboxRect.width-2, checkboxRect.height-2);

	border.clear();
	border.setFilled(false);
	border.setStrokeWidth(1);
	border.setStrokeColor(borderColor);
	border.rectangle(checkboxRect.getTopLeft(), checkboxRect.width, checkboxRect.height);

	if(showName){
		float textWidth = ofxBaseGui::getTextWidth(getName(), getShape().height);
		switch(textAlignment){
			default:
			case TextAlignment::Centered:
				if(getShape().getCenter().x - textWidth/2 > getShape().x+textPadding){
					textMesh = getTextMesh(getName(), getWidth()/2 - textWidth/2, getHeight()/ 2 + 4);
					break;
				}
			case TextAlignment::Left:
				textMesh = getTextMesh(getName(), textPadding, getShape().height / 2 + 4);
				break;
			case TextAlignment::Right:
				textMesh = getTextMesh(getName(), getShape().getWidth() - textWidth - textPadding, getShape().height / 2 + 4);
				break;

		}

	}
}

void ofxMinimalToggle::render(){

	bg.draw();
	if(value){
		fg.draw();
	}
	border.draw();

	if(showName){
		ofColor c = ofGetStyle().color;
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
}


