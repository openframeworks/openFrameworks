#include "ofxMinimalToggle.h"
#include "ofGraphics.h"
using namespace std;

ofxMinimalToggle::ofxMinimalToggle() :
	ofxToggle(){
	thisBorderColor = thisFillColor;
}

ofxMinimalToggle::ofxMinimalToggle(ofParameter <bool> &val, const Config & config)
:ofxToggle(val,config){
	if(b.width == 0) {
		b.width = ofxBaseGui::getTextWidth(val.getName(), config.shape.height);
	}
}

ofxMinimalToggle::~ofxMinimalToggle(){
}

ofxMinimalToggle & ofxMinimalToggle::setup(ofParameter <bool> &val, const Config & config){
	ofxToggle::setup(val, config);
	thisBorderColor = thisFillColor;
	if(b.width == 0) {
		b.width = ofxBaseGui::getTextWidth(val.getName(), config.shape.height);
	}
	return *this;
}

ofxMinimalToggle & ofxMinimalToggle::setup(ofParameter<bool> &val, float width, float height){
	ofxToggle::setup(val,width,height);
	if(b.width == 0){
		b.width = ofxBaseGui::getTextWidth(val.getName(), height);
	}

	return *this;

}

ofxMinimalToggle & ofxMinimalToggle::setup(const string & toggleName, bool val, float width, float height){
	value.set(toggleName, val);
	return setup(value, width, height);
}

void ofxMinimalToggle::generateDraw(){
	checkboxRect.set(1, 1, b.width - 2, b.height - 2);

	bg.clear();
	bg.setFillColor(thisBackgroundColor);
	bg.rectangle(b.getPosition() + checkboxRect.getTopLeft()+ofPoint(1,1), checkboxRect.width-2, checkboxRect.height-2);

	fg.clear();
	fg.setFilled(true);
	fg.setFillColor(thisFillColor);
	fg.rectangle(b.getPosition() + checkboxRect.getTopLeft()+ofPoint(1,1), checkboxRect.width-2, checkboxRect.height-2);

	border.clear();
	border.setFilled(false);
	border.setStrokeWidth(1);
	border.setStrokeColor(thisBorderColor);
	border.rectangle(b.getPosition() + checkboxRect.getTopLeft(), checkboxRect.width, checkboxRect.height);

	if(bShowName){
		float textWidth = ofxBaseGui::getTextWidth(getName(), b.height);
		switch(textLayout){
			default:
			case ofxBaseGui::Centered:
				if(b.getCenter().x - textWidth/2 > b.x+textPadding){
					textMesh = getTextMesh(getName(), b.getCenter().x - textWidth/2, b.y + b.height / 2 + 4);
					break;
				}
			case ofxBaseGui::Left:
				textMesh = getTextMesh(getName(), b.x + textPadding, b.y + b.height / 2 + 4);
				break;
			case ofxBaseGui::Right:
				textMesh = getTextMesh(getName(), b.getRight() - textWidth - textPadding, b.y + b.height / 2 + 4);
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

	if(bShowName){
		ofColor c = ofGetStyle().color;
		ofBlendMode blendMode = ofGetStyle().blendingMode;
		if(blendMode != OF_BLENDMODE_ALPHA){
			ofEnableAlphaBlending();
		}
		ofSetColor(thisTextColor);

		bindFontTexture();
		textMesh.draw();
		unbindFontTexture();

		ofSetColor(c);
		if(blendMode != OF_BLENDMODE_ALPHA){
			ofEnableBlendMode(blendMode);
		}
	}
}


