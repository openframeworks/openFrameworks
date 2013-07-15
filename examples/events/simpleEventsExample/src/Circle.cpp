//
//  circle.cpp
//  simpleEventsExample
//
//  Created by Roy Macdonald on 12/25/12.
//  Copyright (c) 2012 micasa. All rights reserved.
//

#include "Circle.h"

// the static event, or any static variable, must be initialized outside of the class definition.
ofEvent<ofVec2f> Circle::clickedInsideGlobal = ofEvent<ofVec2f>(); 

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
    ofCircle(x,y, radius);
    ofPopStyle();
}

void Circle::clear() {
    if(bRegisteredEvents) {
        ofUnregisterMouseEvents(this); // disable litening to mous events.
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
        ofVec2f mousePos = ofVec2f(args.x, args.y);
        ofNotifyEvent(clickedInside, mousePos, this); 
        ofNotifyEvent(clickedInsideGlobal, mousePos);
    }
}

//this function checks if the passed arguments are inside the circle.
bool Circle::inside(float _x, float _y ){
    return (ofVec2f(_x, _y).distance(ofVec2f(x, y)) <radius);
}
