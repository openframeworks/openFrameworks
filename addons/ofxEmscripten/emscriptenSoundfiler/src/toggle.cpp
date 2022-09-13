#include "toggle.h"

toggle::toggle(){
    bRegisteredEvents = false;
}

toggle::~toggle() {
    clear();
}

void toggle::setup(float sendXpos, float sendYpos, float sendSize){
    if(!bRegisteredEvents) {
        ofRegisterMouseEvents(this);
        ofRegisterTouchEvents(this);
        bRegisteredEvents = true;
    }
	xpos = sendXpos;
	ypos = sendYpos;
	size = sendSize;
	isTouch = false;
	value = false;
}

void toggle::update(){

}

void toggle::draw(){
	ofPushMatrix();
	ofPushStyle();
	ofTranslate(xpos, ypos);
	ofSetColor(0);
	ofDrawRectangle(0, 0, size, size);
	ofSetColor(255);
	ofDrawRectangle(size / 40, size / 40, size - size / 20, size - size / 20);
	ofSetColor(0);
	if (value) {
		ofSetLineWidth(size / 40);
		ofDrawLine(size / 40, size / 40, size - size / 40, size - size / 40);
		ofDrawLine(size - size / 40, size / 40, size / 40, size - size / 40);
	}
	ofPopMatrix();
	ofPopStyle();
}

void toggle::clear() {
    if(bRegisteredEvents) {
        ofUnregisterMouseEvents(this);
        ofUnregisterTouchEvents(this);
        bRegisteredEvents = false;
    }
}

void toggle::mouseMoved(ofMouseEventArgs & args){

}

void toggle::mouseDragged(ofMouseEventArgs & args){

}

void toggle::mousePressed(ofMouseEventArgs & args){
	if (args.x > xpos && args.x < xpos + size && args.y > ypos && args.y < ypos + size && !isTouch){
		value = !value;
		ofNotifyEvent(onMousePressed, value, this); 
	}
}

void toggle::mouseReleased(ofMouseEventArgs & args){

}

void toggle::mouseScrolled(ofMouseEventArgs & args){

}

void toggle::mouseEntered(ofMouseEventArgs & args){

}

void toggle::mouseExited(ofMouseEventArgs & args){

}

void toggle::touchDown(ofTouchEventArgs & args){
	if (args.x > xpos && args.x < xpos + size && args.y > ypos && args.y < ypos + size){
		isTouch = true;
		value = !value;
		ofNotifyEvent(onMousePressed, value, this); 
	}
}

void toggle::touchMoved(ofTouchEventArgs & args){

}

void toggle::touchUp(ofTouchEventArgs & args){

}

void toggle::touchDoubleTap(ofTouchEventArgs & args){

}

void toggle::touchCancelled(ofTouchEventArgs & args){

}
