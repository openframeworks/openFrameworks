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


/********** ofWindowDefinitions *****************/

int ofWindow::lastWindowID = 0;

ofWindow::ofWindow():isFocused(false),isInitialized(false),title("ofTestApp"){
	id = lastWindowID;
	lastWindowID++;
	width = 800;
	height = 600;
};

ofWindow::~ofWindow(){

};

void ofWindow::initializeWindow(){
	window = glfwOpenWindow( width, height, GLFW_WINDOWED, title.c_str(), NULL );
	glfwSwapInterval( 1 );
	
	isInitialized = true;
	setWindowPositionAndShape(x, y, width, height);
}

void ofWindow::addListener(ofWindowListener* listener)
{
	listeners.push_back(listener);
}
void ofWindow::addListener(ofBaseApp* app){
	addListener(new ofWindowToOfBaseApp(app));
}	

GLFWwindow ofWindow::getGlfwWindow(){
	return window;
}

void ofWindow::enableContext(){
	glfwMakeContextCurrent(window);	
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
	ofGetWindowManager()->setActiveWindow(this);

	float * bgPtr = ofBgColorPtr();
	ofViewport(0, 0, width, height, false);		// used to be glViewport( 0, 0, width, height );
   	ofClear(bgPtr[0]*255,bgPtr[1]*255,bgPtr[2]*255, bgPtr[3]*255);

	//ofGetCurrentRenderer()->setupScreenPerspective(800, 600);
	ofSetupScreenPerspective(width, height, OF_ORIENTATION_DEFAULT);

#ifdef OF_USING_POCO
	ofEventArgs e;
	ofNotifyEvent(events.draw, e);
#endif
	
	ofWindowListenerList::iterator it=listeners.begin();
	while(it!=listeners.end()) {
		(*it)->draw(this);
		++it;
	}

}

void ofWindow::windowFocused(){
	isFocused = true;
};

void ofWindow::windowUnfocused(){
	isFocused = false;
};

void ofWindow::windowClosed(){};

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

void ofWindow::setWindowPosition(int x, int y){
	glfwSetWindowPos(window, x, y);
}

void ofWindow::setWindowPosition(ofPoint pos){
	setWindowPosition(pos.x, pos.y);
}

void ofWindow::setWindowShape(int w, int h){
	glfwSetWindowSize(window, w, h);
}

void ofWindow::windowResized(int w, int h){
	if(width == w && height == h)
		return;
	if (w<=0) w = 1;
	if (h<=0) h = 1;
	previousShape.width = width;
	previousShape.height = height;
	width = w;
	height = h;
	
	ofNotifyWindowResized(width, height);
	
	#ifdef OF_USING_POCO
	ofResizeEventArgs e;
	e.width = width;
	e.height = height;
	ofNotifyEvent(events.windowResized, e);
	#endif
	
	draw();
}

void ofWindow::windowMoved(int _x, int _y){
	if(x == _x && y == _y)
		return;
	previousShape.x = x;
	previousShape.y = y;
	x = _x;
	y = _y;
	
	/*
	#ifdef OF_USING_POCO
	e.width = width;
	e.height = height;
	ofNotifyEvent(events.windowResized, e);
	#endif
	*/
}

void ofWindow::setTitle(string t){
	if (isInitialized)
		glfwSetWindowTitle(window, t.c_str());
	title = t;
}

string ofWindow::getTitle(){
	return title;
}