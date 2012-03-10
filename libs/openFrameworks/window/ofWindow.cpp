/*
 *  ofWindow.cpp
 *  openFrameworksLib
 *
 *  Created by Philip Whitfield on 3/8/12.
 *  Copyright 2012 undef.ch. All rights reserved.
 *
 */

#include "ofEvents.h"
#include "ofWindow.h"


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
	ofWindowListenerList::iterator it=listeners.begin();
	while(it!=listeners.end()) {
		(*it)->draw(this);
		++it;
	}
}


