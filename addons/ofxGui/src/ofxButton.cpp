#include "ofxButton.h"

ofxButton::ofxButton(){
	value.setSerializable(false);
}

ofxButton* ofxButton::setup(string toggleName, float width, float height){
	setName(toggleName);
	b.x = 0;
	b.y = 0;
	b.width = width;
	b.height = height;
	bGuiActive = false;
	value = false;
	checkboxRect.set(1, 1, b.height - 2, b.height - 2);

	ofRegisterMouseEvents(this,OF_EVENT_ORDER_BEFORE_APP);

	value.addListener(this,&ofxButton::valueChanged);

	return this;
}

void ofxButton::mouseReleased(ofMouseEventArgs & args){
	bool attended = setValue(args.x, args.y, false);
	bGuiActive = false;
	if(attended){
		ofEventMarkAttended();
	}
}

void ofxButton::mouseMoved(ofMouseEventArgs & args){
	ofxToggle::mouseMoved(args);
}

void ofxButton::mousePressed(ofMouseEventArgs & args){
	ofxToggle::mousePressed(args);
}

void ofxButton::mouseDragged(ofMouseEventArgs & args){
	ofxToggle::mouseDragged(args);
}

void ofxButton::valueChanged(bool & v){
	if(!v){
		ofNotifyEvent(triggerEvent);
	}
}
