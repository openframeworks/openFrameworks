#include "ofxButton.h"
using namespace std;

ofxButton::ofxButton()
	:ofxToggle(){
	setup();
}

ofxButton::ofxButton(const string &buttonName)
	:ofxButton(){
	value.setName(buttonName);
}

ofxButton::ofxButton(const string &buttonName, const ofJson & config)
	:ofxButton(buttonName){
	_setConfig(config);
}

ofxButton::ofxButton(ofParameter<void> &_val, const ofJson & config)
	:ofxToggle(){
	voidvalue.makeReferenceTo(_val);
	useVoidValue = true;
	setup();
	_setConfig(config);
}

ofxButton::ofxButton(ofParameter<bool> &_val, const ofJson & config)
	:ofxToggle(_val){
	setup();
	_setConfig(config);
}

ofxButton::ofxButton(ofParameter<void>& _val, float width, float height)
	:ofxButton(_val){
	setSize(width, height);
}

ofxButton::ofxButton(ofParameter<bool>& _val, float width, float height)
	:ofxButton(_val){
	setSize(width, height);
}

ofxButton::ofxButton(const std::string& buttonName, float width, float height):
	ofxButton(buttonName){
	setSize(width, height);
}

ofxButton::~ofxButton(){
}

void ofxButton::setup(){

	value = false;
	value.setSerializable(false);
	voidvalue.setSerializable(false);
	registerMouseEvents();

}

bool ofxButton::setValue(float mx, float my, bool bCheck){

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
		value = !value;
		voidvalue.trigger();
		return true;
	}
	return false;
}

void ofxButton::generateDraw(){
	if(useVoidValue){
		value.setName(voidvalue.getName());
	}
	ofxToggle::generateDraw();
}

bool ofxButton::mouseReleased(ofMouseEventArgs & args){

	ofxBaseGui::mouseReleased(args);

	bool attended = setValue(args.x, args.y, false);
	hasFocus = false;
	return attended;

}
