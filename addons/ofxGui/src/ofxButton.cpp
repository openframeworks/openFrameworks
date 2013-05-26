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
	currentFrame = 0;
	bGuiActive = false;
	value = false;
	checkboxRect.set(1, 1, b.height - 2, b.height - 2);

	ofRegisterMouseEvents(this);

	value.addListener(this,&ofxButton::valueChanged);

	return this;
}

void ofxButton::mouseReleased(ofMouseEventArgs & args){
	setValue(args.x, args.y, false);
	bGuiActive = false;
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
