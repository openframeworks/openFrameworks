#include "ofxButton.h"
using namespace std;

ofxButton::ofxButton():ofxToggle(){}

ofxButton::ofxButton(ofParameter<void> _val, const Config & config)
:ofxToggle(value, config){
	//value.setSerializable(false);
	voidvalue.makeReferenceTo(_val);
	useVoidValue = true;
}

ofxButton::ofxButton(ofParameter<bool> _val, const Config & config)
:ofxToggle(_val, config){
}

ofxButton::~ofxButton(){
}

ofxButton & ofxButton::setup(ofParameter<void>& _val, const Config & config){
	ofxToggle::setup(value, config);
	voidvalue.makeReferenceTo(_val);
	useVoidValue = true;
	return *this;
}

ofxButton & ofxButton::setup(ofParameter<bool> &_val, const Config & config){
	ofxToggle::setup(_val, config);
	return *this;
}

ofxButton & ofxButton::setup(const std::string& buttonName, const Config & config){
	value.setName(buttonName);
	return setup(value, config);
}

ofxButton & ofxButton::setup(ofParameter<void>& _val, float width, float height){
	ofxToggle::setup(value, width, height);
	voidvalue.makeReferenceTo(_val);
	useVoidValue = true;
	return *this;
}

ofxButton & ofxButton::setup(ofParameter<bool>& _val, float width, float height){
	ofxToggle::setup(_val, width, height);
	return *this;
}

bool ofxButton::setValue(float mx, float my, bool bCheck){

	if( !isGuiDrawing() ){
		bGuiActive = false;
		return false;
	}
	if( bCheck ){
		ofRectangle checkRect = checkboxRect;
		checkRect.x += b.x;
		checkRect.y += b.y;

		if( checkRect.inside(mx, my) ){
			bGuiActive = true;
		}else{
			bGuiActive = false;

		}
	}
	if( bGuiActive ){
		value = !value;
		voidvalue.trigger();
		return true;
	}
	return false;
}

ofxButton & ofxButton::setup(const std::string& buttonName, float width, float height){
	value.setName(buttonName);
	return setup(value, width, height);
}

void ofxButton::generateDraw(){
	if(useVoidValue){
		value.setName(voidvalue.getName());
	}
	ofxToggle::generateDraw();
}

bool ofxButton::mouseReleased(ofMouseEventArgs & args){
	bool attended = setValue(args.x, args.y, false);
	bGuiActive = false;
	if(attended){
		return true;
	}else{
		return false;
	}
}

bool ofxButton::mouseMoved(ofMouseEventArgs & args){
	return ofxToggle::mouseMoved(args);
}

bool ofxButton::mousePressed(ofMouseEventArgs & args){
	return ofxToggle::mousePressed(args);
}

bool ofxButton::mouseDragged(ofMouseEventArgs & args){
	return ofxToggle::mouseDragged(args);
}

