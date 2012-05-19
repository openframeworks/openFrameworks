/*
 *  ofWindowEvents.h
 *  openFrameworksLib
 *
 *  Created by Philip Whitfield on 5/19/12.
 *  Copyright 2012 undef.ch. All rights reserved.
 *
 */

#ifndef _CLASS_ofWindowEvents_
#define _CLASS_ofWindowEvents_

#include "ofEvents.h"

class ofWindow;

class ofWindowEventArgs: public ofEventArgs{
public:
	ofWindowEventArgs(ofWindow* win);
	ofWindow* window;
};

class ofWindowEvents
{
public:
	ofEvent<ofEventArgs> 		setup;
	ofEvent<ofEventArgs> 		update;
	ofEvent<ofEventArgs> 		draw;
	ofEvent<ofEventArgs> 		exit;
	ofEvent<ofResizeEventArgs> 	windowResized;
	ofEvent<ofMoveEventArgs> 	windowMoved;
	
	ofEvent<ofKeyEventArgs> 	keyPressed;
	ofEvent<ofKeyEventArgs> 	keyReleased;
	
	ofEvent<ofMouseEventArgs> 	mouseMoved;
	ofEvent<ofMouseEventArgs> 	mouseDragged;
	ofEvent<ofMouseEventArgs> 	mousePressed;
	ofEvent<ofMouseEventArgs> 	mouseReleased;
	
	ofEvent<ofTouchEventArgs>	touchDown;
	ofEvent<ofTouchEventArgs>	touchUp;
	ofEvent<ofTouchEventArgs>	touchMoved;
	ofEvent<ofTouchEventArgs>	touchDoubleTap;
	ofEvent<ofTouchEventArgs>	touchCancelled;
	
	ofEvent<ofMessage>			messageEvent;
	ofEvent<ofDragInfo>			fileDragEvent;
};

#endif