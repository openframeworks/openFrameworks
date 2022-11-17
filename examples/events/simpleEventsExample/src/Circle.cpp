//
//  circle.cpp
//  simpleEventsExample
//
//  Created by Roy Macdonald on 12/25/12.
//  Copyright (c) 2012 micasa. All rights reserved.
//

#include "Circle.h"

// the static event, or any static variable, must be initialized outside of the class definition.
ofEvent<glm::vec2> Circle::clickedInsideGlobal = ofEvent<glm::vec2>(); 

Circle::Circle() {
	bRegisteredEvents = false;
}

Circle::~Circle() {
	clear();
}

void Circle::setup(int radius, int x, int y, ofColor color){
	this->radius = radius;
	this->x = x;
	this->y = y;
	this->color = color;
	if(!bRegisteredEvents) {
		ofRegisterMouseEvents(this); // this will enable our circle class to listen to the mouse events.
		bRegisteredEvents = true;
	}
}
void Circle::draw(){
	ofPushStyle();
	ofSetColor(color);
	ofDrawCircle(x,y, radius);
	ofPopStyle();
}

void Circle::clear() {
	if(bRegisteredEvents) {
		ofUnregisterMouseEvents(this); // disable listening to mouse events.
		bRegisteredEvents = false;
	}
}

void Circle::mouseMoved(ofMouseEventArgs & args){}
void Circle::mouseDragged(ofMouseEventArgs & args){}
void Circle::mousePressed(ofMouseEventArgs & args){}
void Circle::mouseReleased(ofMouseEventArgs & args){
	if (inside(args.x, args.y)) {
		// if the mouse is pressed over the circle an event will be notified (broadcasted) 
		// the circleEvent object will contain the mouse position, so this values are accesible to any class that is listening.
		glm::vec2 mousePos = glm::vec2(args.x, args.y);
		ofNotifyEvent(clickedInside, mousePos, this); 
		ofNotifyEvent(clickedInsideGlobal, mousePos);
	}
}
void Circle::mouseScrolled(ofMouseEventArgs & args){}
void Circle::mouseEntered(ofMouseEventArgs & args){}
void Circle::mouseExited(ofMouseEventArgs & args){}

//this function checks if the passed arguments are inside the circle.
bool Circle::inside(float _x, float _y ){
	return (glm::distance(glm::vec2(_x, _y), glm::vec2(x, y)) < radius);
}
