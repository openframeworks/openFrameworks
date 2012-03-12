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


ofWindow::ofWindow(){

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
	ofViewport(0, 0, 800, 600, false);		// used to be glViewport( 0, 0, width, height );
   	ofClear(bgPtr[0]*255,bgPtr[1]*255,bgPtr[2]*255, bgPtr[3]*255);

	//ofGetCurrentRenderer()->setupScreenPerspective(800, 600);
	ofSetupScreenPerspective(800, 600, OF_ORIENTATION_DEFAULT);

	ofWindowListenerList::iterator it=listeners.begin();
	while(it!=listeners.end()) {
		(*it)->draw(this);
		++it;
	}

	postDraw();
}


