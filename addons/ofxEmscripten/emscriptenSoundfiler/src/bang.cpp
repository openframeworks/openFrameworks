#include "bang.h"

bang::bang(){
    bRegisteredEvents = false;
}

bang::~bang() {
    clear();
}

void bang::setup(float sendXpos, float sendYpos, float sendSize){
    if(!bRegisteredEvents) {
        ofRegisterMouseEvents(this);
        ofRegisterTouchEvents(this);
        bRegisteredEvents = true;
    }
	xpos = sendXpos;
	ypos = sendYpos;
	size = sendSize;
	isTouch = false;
}

void bang::update(){

}

void bang::draw(){
	ofPushMatrix();
	ofPushStyle();
	ofTranslate(xpos, ypos);
	ofSetCircleResolution(100);
	ofSetColor(0);
	ofDrawRectangle(0, 0, size, size);
	ofSetColor(255);
	ofDrawRectangle(size / 40, size / 40, size - size / 20, size - size / 20);
	ofSetColor(0);
	ofDrawCircle(size / 2, size / 2, size / 2.5);
	if (now < ofGetElapsedTimeMillis()){
		ofSetColor(255);
	}
	ofDrawCircle(size / 2, size / 2, size / 2.7);
	ofPopMatrix();
	ofPopStyle();
}

void bang::clear() {
    if(bRegisteredEvents) {
        ofUnregisterMouseEvents(this);
        ofUnregisterTouchEvents(this);
        bRegisteredEvents = false;
    }
}

void bang::mouseMoved(ofMouseEventArgs & args){

}

void bang::mouseDragged(ofMouseEventArgs & args){

}

void bang::mousePressed(ofMouseEventArgs & args){
	if (ofDist(args.x, args.y, xpos + size / 2, ypos + size / 2) <= size / 2.5 && !isTouch){
		now = ofGetElapsedTimeMillis() + 200;
		value = true;
		ofNotifyEvent(onMousePressed, value, this);
	}
}

void bang::mouseReleased(ofMouseEventArgs & args){

}

void bang::mouseScrolled(ofMouseEventArgs & args){

}

void bang::mouseEntered(ofMouseEventArgs & args){

}

void bang::mouseExited(ofMouseEventArgs & args){

}

void bang::touchDown(ofTouchEventArgs & args){
	if (ofDist(args.x, args.y, xpos + size / 2, ypos + size / 2) <= size / 2.5){
		isTouch = true;
		now = ofGetElapsedTimeMillis() + 200;
		value = true;
		ofNotifyEvent(onMousePressed, value, this);
	}
}

void bang::touchMoved(ofTouchEventArgs & args){

}

void bang::touchUp(ofTouchEventArgs & args){

}

void bang::touchDoubleTap(ofTouchEventArgs & args){

}

void bang::touchCancelled(ofTouchEventArgs & args){

}
