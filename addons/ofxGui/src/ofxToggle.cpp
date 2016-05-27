#include "ofxToggle.h"
#include "ofxGuiGroup.h"
#include "ofGraphics.h"
using namespace std;

ofxToggle::ofxToggle():ofxBaseGui(){

	setup();

}

ofxToggle::ofxToggle(const string &toggleName):ofxToggle(){

	value.setName(toggleName);
	value.set(false);

}

ofxToggle::ofxToggle(const string &toggleName, const ofJson& config):ofxToggle(toggleName){

	_setConfig(config);

}

ofxToggle::ofxToggle(ofParameter<bool> &_bVal, const ofJson & config)
	:ofxBaseGui(){

	value.makeReferenceTo(_bVal);
	setup();
	_setConfig(config);

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

}

void ofxToggle::setup(){

	hasFocus = false;

	type.set("type", ofxToggleType::CHECKBOX);
	setType(type.get());
	value.addListener(this,&ofxToggle::valueChanged);

	registerMouseEvents();

}

void ofxToggle::_setConfig(const ofJson &config){

	ofxBaseGui::_setConfig(config);

	if (config.find(type.getName()) != config.end()) {
		std::string val = config[type.getName()];
		setType(val);
	}

}

void ofxToggle::setWidth(float width){
	float _width = ofxBaseGui::getTextWidth(getName(), getHeight())+2*textAlignment;
	Element::setWidth(max(_width,width));
}

bool ofxToggle::mousePressed(ofMouseEventArgs & args){
	ofxBaseGui::mousePressed(args);
	return setValue(args.x, args.y, true);
}

bool ofxToggle::mouseReleased(ofMouseEventArgs & args){
	ofxBaseGui::mouseReleased(args);
	bool hadFocus = hasFocus;
	hasFocus = false;
	return (hadFocus && isMouseOver());
}

void ofxToggle::generateDraw(){

	switch(type){
		default:
		case ofxToggleType::RADIO:
		case ofxToggleType::CHECKBOX: {
			checkboxRect.set(1, 1, getHeight() - 2, getHeight() - 2);
			break;
		}
		case ofxToggleType::FULLSIZE: {
			checkboxRect.set(1, 1, getWidth() - 2, getHeight() - 2);
			break;
		}
	}

	bg.clear();
	bg.setFillColor(backgroundColor);
	bg.setStrokeWidth(borderWidth);
	bg.setStrokeColor(borderColor);
	bg.rectangle(0,0,getWidth(),getHeight());

	fg.clear();
	fg.setFilled(true);
	if(value){
		fg.setFillColor(fillColor);
	}else{
		fg.setFillColor(ofColor(fillColor, 0.33*fillColor.get().a));
	}
	switch(type){
		default:
		case ofxToggleType::RADIO:{
			fg.arc(checkboxRect.getCenter(), checkboxRect.getHeight()/3, checkboxRect.getHeight()/3, 0, 360);
			break;
		}
		case ofxToggleType::CHECKBOX: {
			fg.rectangle(checkboxRect.getTopLeft()+ofPoint(checkboxRect.width/6,checkboxRect.height/6),
						 checkboxRect.width/3*2,checkboxRect.height/3*2);
			break;
		}
		case ofxToggleType::FULLSIZE: {
			fg.rectangle(checkboxRect.getTopLeft(),checkboxRect.width,checkboxRect.height);
			break;
		}
	}

	switch(type){
		default:
		case ofxToggleType::RADIO:
		case ofxToggleType::CHECKBOX: {

//			//create cross
//			cross.clear();
//			cross.setStrokeColor(textColor);
//			cross.setStrokeWidth(1);
//			cross.setFilled(false);
//			cross.moveTo(checkboxRect.getTopLeft());
//			cross.lineTo(checkboxRect.getBottomRight());
//			cross.moveTo(checkboxRect.getTopRight());
//			cross.lineTo(checkboxRect.getBottomLeft());

			// create label
			if(showName){
				textMesh = getTextMesh(getName(), textPadding + checkboxRect.width, getShape().getHeight() / 2 + 4);
			}
			break;
		}
		case ofxToggleType::FULLSIZE: {

			// create label
			if(showName){
				float textWidth = ofxBaseGui::getTextWidth(getName(), getShape().height);
				switch(textAlignment){
					default:
					case TextAlignment::CENTERED:
						if(getShape().getCenter().x - textWidth/2 > getShape().x+textPadding){
							textMesh = getTextMesh(getName(), getWidth()/2 - textWidth/2, getHeight()/ 2 + 4);
							break;
						}
					case TextAlignment::LEFT:
						textMesh = getTextMesh(getName(), textPadding, getShape().height / 2 + 4);
						break;
					case TextAlignment::RIGHT:
						textMesh = getTextMesh(getName(), getShape().getWidth() - textWidth - textPadding, getShape().height / 2 + 4);
						break;

				}
			}
			break;
		}
	}


}

void ofxToggle::render(){
	bg.draw();
	fg.draw();

//	if(value && (type == ofxToggle::Type::CHECKBOX
//			|| type == ofxToggle::Type::RADIO)){
//		cross.draw();
//	}

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

bool ofxToggle::setValue(float mx, float my, bool bCheck){

	if(isHidden()){
		hasFocus = false;
		return false;
	}
	if(bCheck){

		ofRectangle checkRect = checkboxRect;
		checkRect.x += getScreenPosition().x;
		checkRect.y += getScreenPosition().y;

		hasFocus = checkRect.inside(mx, my);
	}

	if(hasFocus){

		//if group has exclusive toggles and toggle is on, don't to anything
		ofxGuiGroup* parent = dynamic_cast<ofxGuiGroup*>(this->parent());
		if(parent){
			if(parent->getTogglesExclusive()){
				if(value.get()){
					return false;
				}
			}
		}

		value = !value;

		if(value.get()){
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

void ofxToggle::setType(const std::string& type){
	if(type == "checkbox"){
		setType(ofxToggleType::CHECKBOX);
	}
	else if(type == "radio"){
		setType(ofxToggleType::RADIO);
	}
	else if(type == "fullsize"){
		setType(ofxToggleType::FULLSIZE);
	}
}

void ofxToggle::setType(const ofxToggleType::Type &type){
	this->type.set(type);
	setNeedsRedraw();
}

ofxToggleType::Type ofxToggle::getType(){
	return type;
}
