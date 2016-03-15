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

	type.set("type", ofxToggle::Type::CHECKBOX);
	setType(type.get());
	value.addListener(this,&ofxToggle::valueChanged);

	registerMouseEvents();

}

void ofxToggle::_setConfig(const ofJson &config){

	ofxBaseGui::_setConfig(config);

	/// Since there is no parse method for ofxToggle::Type in JsonConfigParser,
	/// we call a function that sets the type according to the string provided by the config

	if (config.find(type.getName()) != config.end()) {
		std::string val = config[type.getName()];
		setType(val);
	}

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

	bg.clear();
	bg.setFillColor(backgroundColor);
	bg.rectangle(0,0,getWidth(),getHeight());

	fg.clear();
	fg.setStrokeWidth(borderWidth);
	fg.setStrokeColor(borderColor);
	if(value){
		fg.setFilled(true);
		fg.setFillColor(fillColor);
	}else{
		fg.setFilled(false);
	}
	switch(type){
		default:
		case ofxToggle::Type::RADIO:{
			fg.arc(checkboxRect.getCenter(), checkboxRect.getHeight()/3, checkboxRect.getHeight()/3, 0, 360);
			break;
		}
		case ofxToggle::Type::CHECKBOX:
		case ofxToggle::Type::FULLSIZE: {
			fg.rectangle(checkboxRect.getTopLeft(),checkboxRect.width,checkboxRect.height);
			break;
		}
	}

	switch(type){
		default:
		case ofxToggle::Type::RADIO:
		case ofxToggle::Type::CHECKBOX: {

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
		case ofxToggle::Type::FULLSIZE: {

			// create label
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
		setType(ofxToggle::Type::CHECKBOX);
	}
	else if(type == "radio"){
		setType(ofxToggle::Type::RADIO);
	}
	else if(type == "fullsize"){
		setType(ofxToggle::Type::FULLSIZE);
	}
}

void ofxToggle::setType(const ofxToggle::Type& type){
	this->type.set(type);
	switch(type){
		default:
		case ofxToggle::Type::RADIO:
		case ofxToggle::Type::CHECKBOX: {
			checkboxRect.set(1, 1, getHeight() - 2, getHeight() - 2);
			break;
		}
		case ofxToggle::Type::FULLSIZE: {
			checkboxRect.set(1, 1, getWidth() - 2, getHeight() - 2);
			break;
		}
	}
	setNeedsRedraw();
}

ofxToggle::Type ofxToggle::getType(){
	return type;
}
