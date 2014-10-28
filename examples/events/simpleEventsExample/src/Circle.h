//
//  circle.h
//  simpleEventsExample
//
//  Created by Roy Macdonald on 12/23/12.
//  Copyright (c) 2012 micasa. All rights reserved.
//

#ifndef simpleEventsExample_circle_h
#define simpleEventsExample_circle_h

#include "ofMain.h"

class Circle {
public:

    Circle();
    ~Circle();
    // to be able to broadcast events from a class we must declare the ofEvent object that will be broadcasted.
    // this object can be declared as class variable so events are broadcasted and listened for each specific class instance.
    // the data type of the event, what's declared between the < > can be whatever you want, event a custom class. 
    ofEvent<ofVec2f> clickedInside;

    //this is a shared event for all the instances of this class, so any instance of this class will broadcast to the same event,
    //this way you'll have to register only one listener to listen to any class instance broadcasting to this event.
    // "static" tells the compiler that all of this class instances will share a single variable
    static ofEvent<ofVec2f> clickedInsideGlobal;
   
    void setup(int radius, int x, int y, ofColor color);
    void draw();
    void clear();
    
    //We need to declare all this mouse events methods to be able to listen to mouse events.
    //All this must be declared even if we are just going to use only one of this methods.
    void mouseMoved(ofMouseEventArgs & args);
    void mouseDragged(ofMouseEventArgs & args);
    void mousePressed(ofMouseEventArgs & args);
    void mouseReleased(ofMouseEventArgs & args);
    void mouseScrolled(ofMouseEventArgs & args);
    
    //this function checks if the passed arguments are inside the circle.
    bool inside(float _x, float _y );
    int radius, x, y;
    ofColor color;

protected:
    bool bRegisteredEvents;
};

#endif
