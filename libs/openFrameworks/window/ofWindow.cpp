/*
 *  ofWindow.cpp
 *  openFrameworksLib
 *
 *  Created by Philip Whitfield on 3/8/12.
 *  Copyright 2012 undef.ch. All rights reserved.
 *
 */

#include <ofGraphics.h>
#include "ofEvents.h"
#include "ofWindow.h"
#include "ofWindowManager.h"

int ofWindow::lastWindowID = 0;

ofWindow::ofWindow():isFocused(false){
	id = lastWindowID;
	lastWindowID++;
};

ofWindow::~ofWindow(){

};

void ofWindow::addListener(ofWindowListener* listener)
{
	listeners.push_back(listener);
}

void ofWindow::setup()
{
	glClearColor(.55, .55, .55, 0.0);
	glClear(GL_COLOR_BUFFER_BIT);

	ofAddListener(ofEvents().update, this, &ofWindow::update);
	ofAddListener(ofEvents().draw, this, &ofWindow::draw);
}

void ofWindow::update(ofEventArgs& e)
{
	update();
}

void ofWindow::update()
{
	ofGetWindowManager()->setActiveWindow(this);
	ofWindowListenerList::iterator it=listeners.begin();
	while(it!=listeners.end()) {
		(*it)->update(this);
		++it;
	}
}

void ofWindow::draw(ofEventArgs& e)
{
	draw();
}

void ofWindow::draw()
{
	enableContext();
	ofGetWindowManager()->setActiveWindow(this);

	float * bgPtr = ofBgColorPtr();
	ofViewport(0, 0, width, height, false);		// used to be glViewport( 0, 0, width, height );
   	ofClear(bgPtr[0]*255,bgPtr[1]*255,bgPtr[2]*255, bgPtr[3]*255);

	//ofGetCurrentRenderer()->setupScreenPerspective(800, 600);
	ofSetupScreenPerspective(width, height, OF_ORIENTATION_DEFAULT);

	ofWindowListenerList::iterator it=listeners.begin();
	while(it!=listeners.end()) {
		(*it)->draw(this);
		++it;
	}

	postDraw();
}

void ofWindow::moved(int _x, int _y){
	if(x == _x && y == _y)
		return;
	previousShape.x = x;
	previousShape.y = y;
	x = _x;
	y = _y;
};

void ofWindow::resized(int w, int h){
	if(width == w && height == h)
		return;
	previousShape.width = width;
	previousShape.height = height;
	width = w;
	height = h;
};

void ofWindow::focused(){
	isFocused = true;
};

void ofWindow::unfocused(){
	isFocused = false;
};

void ofWindow::closed(){};

ofPoint	ofWindow::getWindowPosition() {return ofPoint(x, y); }
ofPoint	ofWindow::getWindowSize(){return ofPoint(width, height); }

int ofWindow::getWidth(){return width;}
int ofWindow::getHeight(){return height;}

void ofWindow::setWindowPositionAndShape(int _x, int _y, int w, int h){
	if (!isInitialized) {
		x = _x;
		y = _y;
		width = w;
		height = h;
	}else{
		setWindowPosition(_x, _y);
		setWindowShape(_x, _y);
	}
}
