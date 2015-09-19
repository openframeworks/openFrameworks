#include "ofxButton.h"
using namespace std;

ofxButton::ofxButton(){
	value.setSerializable(false);
}

ofxButton::~ofxButton(){
	//
}

ofxButton* ofxButton::setup(const std::string& toggleName, float width, float height){
	setName(toggleName);
	b.x = 0;
	b.y = 0;
	b.width = width;
	b.height = height;
	bGuiActive = false;
	value = false;
	checkboxRect.set(1, 1, b.height - 2, b.height - 2);

	registerMouseEvents();

	value.addListener(this,&ofxButton::valueChanged);

	return this;
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

void ofxButton::valueChanged(bool & v){
	if(!v){
		ofNotifyEvent(triggerEvent, this);
	}
}
