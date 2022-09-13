#include "hSlider.h"

hSlider::hSlider(){
    bRegisteredEvents = false;
}

hSlider::~hSlider() {
    clear();
}

void hSlider::setup(float sendXpos, float sendYpos, float sendWidth, float sendHeight, float sendMinVal, float sendMaxVal){
    if(!bRegisteredEvents) {
        ofRegisterMouseEvents(this);
        ofRegisterTouchEvents(this);
        bRegisteredEvents = true;
    }
	xpos = sendXpos;
	ypos = sendYpos;
	width = sendWidth;
	height = sendHeight;
	minVal = sendMinVal;
	maxVal = sendMaxVal;
	isTouch = false;
	mouseState = false;
}

void hSlider::update(){

}

void hSlider::draw(){
	ofPushMatrix();
	ofPushStyle();
	ofTranslate(xpos, ypos);
	ofSetColor(0);
	ofDrawRectangle(0, 0, width, height);
	ofSetColor(255);
	ofDrawRectangle(height / 10, height / 10, width - height / 5, height - height / 5);
	ofSetColor(100, 200, 255);
	ofDrawRectangle(height / 10, height / 10, slider * (width - height / 5), height - height / 5);
	ofPopMatrix();
	ofPopStyle();
}

void hSlider::clear() {
    if(bRegisteredEvents) {
        ofUnregisterMouseEvents(this);
        ofUnregisterTouchEvents(this);
        bRegisteredEvents = false;
    }
}

void hSlider::mouseMoved(ofMouseEventArgs & args){

}

void hSlider::mouseDragged(ofMouseEventArgs & args){
	if (mouseState && !isTouch) {
		slider = ofNormalize((args.x - xpos - height / 10) * (maxVal - minVal) / (width - height / 5) + minVal, minVal, maxVal);
		value = ofLerp(minVal, maxVal, slider);
		ofNotifyEvent(onMousePressed, value, this);
	}
}

void hSlider::mousePressed(ofMouseEventArgs & args){
	if (args.x >= xpos + height / 10 && args.x <= xpos + width - height / 10 && args.y >= ypos + height / 10 && args.y <= ypos + height - height / 10 && !isTouch){
		mouseState = true;
		slider = ofNormalize((args.x - xpos - height / 10) * (maxVal - minVal) / (width - height / 5) + minVal, minVal, maxVal);
		value = ofLerp(minVal, maxVal, slider);
		ofNotifyEvent(onMousePressed, value, this);
	}
}

void hSlider::mouseReleased(ofMouseEventArgs & args){
	if (!isTouch){
		mouseState = false;
	}
}

void hSlider::mouseScrolled(ofMouseEventArgs & args){

}

void hSlider::mouseEntered(ofMouseEventArgs & args){

}

void hSlider::mouseExited(ofMouseEventArgs & args){

}

void hSlider::touchDown(ofTouchEventArgs & args){
	if (args.x >= xpos + height / 10 && args.x <= xpos + width - height / 10 && args.y >= ypos + height / 10 && args.y <= ypos + height - height / 10) {
		isTouch = true;
		mouseState = true;
		slider = ofNormalize((args.x - xpos - height / 10) * (maxVal - minVal) / (width - height / 5) + minVal, minVal, maxVal);
		value = ofLerp(minVal, maxVal, slider);
		ofNotifyEvent(onMousePressed, value, this);
	}
}

void hSlider::touchMoved(ofTouchEventArgs & args){
	if (mouseState) {
		slider = ofNormalize((args.x - xpos - height / 10) * (maxVal - minVal) / (width - height / 5) + minVal, minVal, maxVal);
		value = ofLerp(minVal, maxVal, slider);
		ofNotifyEvent(onMousePressed, value, this);
	}
}

void hSlider::touchUp(ofTouchEventArgs & args){
	mouseState = false;
}

void hSlider::touchDoubleTap(ofTouchEventArgs & args){

}

void hSlider::touchCancelled(ofTouchEventArgs & args){

}
