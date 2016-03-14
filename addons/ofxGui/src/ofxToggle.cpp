#include "ofxToggle.h"
#include "ofxGuiGroup.h"
#include "ofGraphics.h"
using namespace std;

ofxToggle::ofxToggle(const string &toggleName, const ofJson & config):ofxBaseGui(config){

	setup(config);
	value.setName(toggleName);
	value.set(false);

}

ofxToggle::ofxToggle(ofParameter<bool> &_bVal, const ofJson & config)
	:ofxBaseGui(config){

	value.makeReferenceTo(_bVal);
	setup(config);

}

ofxToggle::ofxToggle(ofParameter<bool> &_bVal, float width, float height)
	:ofxToggle(_bVal){

	setSize(width, height);

}

ofxToggle::ofxToggle(const std::string& toggleName, bool _bVal, float width, float height)
	:ofxToggle(toggleName){

	value.set(_bVal);
	setSize(width, height);

}

ofxToggle::~ofxToggle(){

	value.removeListener(this,&ofxToggle::valueChanged);
	unregisterPointerEvents();

}

void ofxToggle::setup(const ofJson &config){

	checkboxRect.set(1, 1, getHeight() - 2, getHeight() - 2);
	value.addListener(this,&ofxToggle::valueChanged);
	registerPointerEvents();

}

void ofxToggle::pointerPressed(PointerUIEventArgs& e){
	setValue(e.screenPosition().x, e.screenPosition().y);
}

void ofxToggle::generateDraw(){
	bg.clear();
	bg.setFillColor(backgroundColor);
	bg.rectangle(0,0,getWidth(),getHeight());

	fg.clear();
	if(value){
		fg.setFilled(true);
		fg.setFillColor(fillColor);
	}else{
		fg.setFilled(false);
		fg.setStrokeWidth(1);
		fg.setStrokeColor(borderColor);
	}
	fg.rectangle(checkboxRect.getTopLeft(),checkboxRect.width,checkboxRect.height);

	cross.clear();
	cross.setStrokeColor(textColor);
	cross.setStrokeWidth(1);
	cross.setFilled(false);
	cross.moveTo(checkboxRect.getTopLeft());
	cross.lineTo(checkboxRect.getBottomRight());
	cross.moveTo(checkboxRect.getTopRight());
	cross.lineTo(checkboxRect.getBottomLeft());

	if(showName){
		textMesh = getTextMesh(getName(), textPadding + checkboxRect.width, getShape().getHeight() / 2 + 4);
	}
}

void ofxToggle::render(){
	bg.draw();
	fg.draw();

	if( value ){
		cross.draw();
	}

	if(showName){
		ofColor c = ofGetStyle().color;
		ofBlendMode blendMode = ofGetStyle().blendingMode;
		if(blendMode!=OF_BLENDMODE_ALPHA){
			ofEnableAlphaBlending();
		}
		ofSetColor(textColor);

		bindFontTexture();
		textMesh.draw();
		unbindFontTexture();

		ofSetColor(c);
		if(blendMode!=OF_BLENDMODE_ALPHA){
			ofEnableBlendMode(blendMode);
		}
	}
}

bool ofxToggle::operator=(bool v){
	value = v;
	return v;
}

ofxToggle::operator const bool & (){
	return value;
}

bool ofxToggle::setValue(float mx, float my){

	ofRectangle checkRect = checkboxRect;
	checkRect.x += getScreenPosition().x;
	checkRect.y += getScreenPosition().y;

	if( checkRect.inside(mx, my) ){
		value = !value;
		if(value.get()){
			ofxGuiGroup* parent = dynamic_cast<ofxGuiGroup*>(this->parent());
			if(parent){
				parent->deactivateAllOtherToggles(this);
			}
		}
		setNeedsRedraw();
		return true;
	}

	return false;
}

ofAbstractParameter & ofxToggle::getParameter(){
	return value;
}

void ofxToggle::valueChanged(bool & value){
	setNeedsRedraw();
}
