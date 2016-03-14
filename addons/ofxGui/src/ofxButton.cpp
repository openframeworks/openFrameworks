#include "ofxButton.h"
using namespace std;

ofxButton::ofxButton(const string &buttonName, const ofJson & config)
	:ofxToggle(buttonName, config){
	setup(config);
}

ofxButton::ofxButton(ofParameter<void> &_val, const ofJson & config)
	:ofxToggle(value, config){
	//value.setSerializable(false);
	voidvalue.makeReferenceTo(_val);
	useVoidValue = true;
	setup(config);
}

ofxButton::ofxButton(ofParameter<bool> &_val, const ofJson & config)
	:ofxToggle(_val, config){
	value.makeReferenceTo(_val);
	setup(config);
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

void ofxButton::setup(const ofJson &config){
	processConfig(config);
}

bool ofxButton::setValue(float mx, float my){

	ofRectangle checkRect = checkboxRect;
	checkRect.x += getScreenPosition().x;
	checkRect.y += getScreenPosition().y;

	if( checkRect.inside(mx, my) ){
		value = !value;
		voidvalue.trigger();
		setNeedsRedraw();
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

void ofxButton::pointerReleased(PointerUIEventArgs& e){
	setValue(e.screenPosition().x, e.screenPosition().y);
}


