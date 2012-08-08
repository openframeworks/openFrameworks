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
ofWindow::ofWindow():mouseX(0),mouseY(0),oldMouseX(0),oldMouseY(0),isFocused(false),title("ofTestApp"),isInitialized(false) {
	id = lastWindowID;
	lastWindowID++;
	width = 800;
	height = 600;
	window = NULL;
	for(unsigned int i=0;i<OF_MAX_NUM_KEYS;i++){
		keyState[i] = false;
	}
};
ofWindow::~ofWindow() {
	ofRemoveListener(ofEvents().update, this, &ofWindow::update);
	ofRemoveListener(ofEvents().draw, this, &ofWindow::draw);
};
void ofWindow::initializeWindow() {
	ofLogNotice("CREATING WINDOW AT "+ofToString(x)+"/"+ofToString(y)+" SIZE "+ofToString(width)+" x "+ofToString(height));
	window = glfwCreateWindow( width, height, GLFW_WINDOWED, title.c_str(), NULL );
	isInitialized = true;
	setWindowPositionAndShape(x, y, width, height);
}
void ofWindow::addListener(ofWindowListener* listener) {
#ifdef OF_USING_POCO
	ofAddListener(events.update, listener, &ofWindowListener::update);
	ofAddListener(events.draw, listener, &ofWindowListener::draw);
	
	ofAddListener(events.mouseMoved, listener, &ofWindowListener::mouseMoved);
	ofAddListener(events.mousePressed, listener, &ofWindowListener::mousePressed);
	ofAddListener(events.mouseDragged, listener, &ofWindowListener::mouseDragged);
	ofAddListener(events.mouseReleased, listener, &ofWindowListener::mouseReleased);
	
	ofAddListener(events.keyPressed, listener, &ofWindowListener::keyPressed);
	ofAddListener(events.keyReleased, listener, &ofWindowListener::keyReleased);

	ofAddListener(events.windowResized, listener, &ofWindowListener::windowResized);
	ofAddListener(events.windowMoved, listener, &ofWindowListener::windowMoved);
	ofAddListener(events.windowClosed, listener, &ofWindowListener::windowClosed);
#endif
#ifndef OF_USING_POCO
	listeners.push_back(listener);
#endif
}
void ofWindow::addListener(ofBaseApp* app) {
	addListener(new ofWindowToOfBaseApp(app));
}
GLFWwindow ofWindow::getGlfwWindow() {
	return window;
}
void ofWindow::enableContext() {
	glfwMakeContextCurrent(window);
}
void ofWindow::setup() {
	glClearColor(.55, .55, .55, 0.0);
	glClear(GL_COLOR_BUFFER_BIT);
	ofAddListener(ofEvents().update, this, &ofWindow::update);
	ofAddListener(ofEvents().draw, this, &ofWindow::draw);
}
void ofWindow::update(ofEventArgs& e) {
	update();
}
void ofWindow::update() {
	ofGetWindowManager()->setActiveWindow(this);
#ifdef OF_USING_POCO
	ofWindowEventArgs e;
	e.window = this;
	ofNotifyEvent(events.update, e);
#endif
#ifndef OF_USING_POCO
	ofWindowListenerList::iterator it=listeners.begin();
	while(it!=listeners.end()) {
		(*it)->update(this);
		++it;
	}
#endif
}
void ofWindow::draw(ofEventArgs& e) {
	draw();
}
void ofWindow::draw() {
	ofGetWindowManager()->setActiveWindow(this);
	float * bgPtr = ofBgColorPtr();
	ofViewport(0, 0, width, height, false);		// used to be glViewport( 0, 0, width, height );
	ofClear(bgPtr[0]*255,bgPtr[1]*255,bgPtr[2]*255, bgPtr[3]*255);
	//ofGetCurrentRenderer()->setupScreenPerspective(800, 600);
	ofSetupScreenPerspective(width, height, OF_ORIENTATION_DEFAULT);

#ifdef OF_USING_POCO
	ofWindowEventArgs e;
	e.window = this;
	ofNotifyEvent(events.draw, e);
#endif
#ifndef OF_USING_POCO
	ofWindowListenerList::iterator it=listeners.begin();
	while(it!=listeners.end()) {
		(*it)->draw(this);
		++it;
	}
#endif
	glfwSwapBuffers(window);
}
void ofWindow::destroy() {
	ofRemoveListener(ofEvents().update, this, &ofWindow::update);
	ofRemoveListener(ofEvents().draw, this, &ofWindow::draw);
	//getWindowPosition();
	glfwDestroyWindow(window);
	window = NULL;
}
//USER INTERACTION EVENTS
void ofWindow::mouseMoved(int mX, int mY) {
	mouseX = mX;
	mouseY = mY;
	ofNotifyMouseMoved(mouseX, mouseY);
#ifdef OF_USING_POCO	
	ofMouseEventArgs e;
	e.x = mX;
	e.y = mY;
	e.window = this;
	ofNotifyEvent(events.mouseMoved, e);
#endif
#ifndef OF_USING_POCO
	ofWindowListenerList::iterator it=listeners.begin();
	while(it!=listeners.end()) {
		(*it)->mouseMoved(mouseX, mouseY, this);
		++it;
	}
#endif
}
void ofWindow::mousePressed(int button) {
	mousePressed(mouseX, mouseY, button);
}
void ofWindow::mousePressed(int mX, int mY, int button) {
	mouseX = mX;
	mouseY = mY;
	ofNotifyMousePressed(mouseX, mouseY, button);

#ifdef OF_USING_POCO
	ofMouseEventArgs e;
	e.x = mX;
	e.y = mY;
	e.button = button;
	e.window = this;
	ofNotifyEvent(events.mousePressed, e);
#endif
#ifndef OF_USING_POCO
	ofWindowListenerList::iterator it=listeners.begin();
	while(it!=listeners.end()) {
		(*it)->mousePressed(mouseX, mouseY, button, this);
		++it;
	}
#endif
}
void ofWindow::mouseReleased(int button) {
	mouseReleased(mouseX, mouseY, button);
}
void ofWindow::mouseReleased(int mX, int mY, int button) {
	mouseX = mX;
	mouseY = mY;
	ofNotifyMouseReleased(mouseX, mouseY, button);

#ifdef OF_USING_POCO
	ofMouseEventArgs e;
	e.x = mX;
	e.y = mY;
	e.button = button;
	e.window = this;
	ofNotifyEvent(events.mouseReleased, e);
#endif
#ifndef OF_USING_POCO
	ofWindowListenerList::iterator it=listeners.begin();
	while(it!=listeners.end()) {
		(*it)->mouseReleased(mouseX, mouseY, button, this);
		++it;
	}
#endif
}
void ofWindow::mouseDragged(int mX, int mY, int button) {
	mouseX = mX;
	mouseY = mY;
	
	ofNotifyMouseDragged(mouseX, mouseY, button);
	
#ifdef OF_USING_POCO
	ofMouseEventArgs e;
	e.x = mouseX;
	e.y = mouseY;
	e.button = button;
	e.window = this;
	ofNotifyEvent(events.mouseDragged, e);
#endif
#ifndef OF_USING_POCO
	ofWindowListenerList::iterator it=listeners.begin();
	while(it!=listeners.end()) {
		(*it)->mouseDragged(mouseX, mouseY, button, this);
		++it;
	}
#endif
}

void ofWindow::scrolled(float deltaX, float deltaY){
	//ofNotifyMouseDragged(mouseX, mouseY, button);
	/*ofWindowListenerList::iterator it=listeners.begin();
	while(it!=listeners.end()) {
		(*it)->scrolled(deltaX, deltaY, this);
		++it;
	}*/
}

void ofWindow::keyPressed(int key){
	if(key<OF_MAX_NUM_KEYS)
		keyState[key] = true;
	ofNotifyKeyPressed(key);

#ifdef OF_USING_POCO
	ofKeyEventArgs e;
	e.key = key;
	e.window = this;
	ofNotifyEvent(events.keyPressed, e);
#endif
#ifndef OF_USING_POCO
	ofWindowListenerList::iterator it=listeners.begin();
	while(it!=listeners.end()) {
		(*it)->keyPressed(key, this);
		++it;
	}
#endif
}

void ofWindow::keyReleased(int key){
	if(key<OF_MAX_NUM_KEYS)
		keyState[key] = false;
	ofNotifyKeyReleased(key);
	
#ifdef OF_USING_POCO
	ofKeyEventArgs e;
	e.key = key;
	e.window = this;
	ofNotifyEvent(events.keyReleased, e);
#endif
#ifndef OF_USING_POCO
	ofWindowListenerList::iterator it=listeners.begin();
	while(it!=listeners.end()) {
		(*it)->keyReleased(key, this);
		++it;
	}
#endif
}

bool ofWindow::isKeyPressed(int key){
	return keyState[key];
}


//WINDOW SPECIFIC EVENT
void ofWindow::windowFocused() {
	isFocused = true;
};
void ofWindow::windowUnfocused() {
	isFocused = false;
};
void ofWindow::windowClosed() {
	window = NULL;
#ifdef OF_USING_POCO
	ofWindowEventArgs e;
	e.window = this;
	ofNotifyEvent(events.windowClosed, e);
#endif
#ifndef OF_USING_POCO
	ofWindowListenerList::iterator it=listeners.begin();
	while(it!=listeners.end()) {
		(*it)->windowClosed(this);
		++it;
	}
#endif
};
ofPoint	ofWindow::getWindowPosition() {
	if(window != NULL)
		glfwGetWindowPos(window, &x, &y);
	return ofPoint(x, y);
}
ofPoint	ofWindow::getWindowSize() {
	return ofPoint(width, height);
}
int ofWindow::getWidth() {
	return width;
}
int ofWindow::getHeight() {
	return height;
}
int ofWindow::getX(){
	return getWindowPosition().x;
}
int ofWindow::getY(){
	return getWindowPosition().y;
}

int ofWindow::setX(int _x){
	setWindowPosition(_x, y);
}
int ofWindow::setY(int _y){
	setWindowPosition(x, _y);
}

void ofWindow::setWindowPositionAndShape(int _x, int _y, int w, int h) {
	if (!isInitialized) {
		x = _x;
		y = _y;
		width = w;
		height = h;
	} else {
		setWindowPosition(_x, _y);
		setWindowShape(w, h);
	}
}
void ofWindow::setWindowPosition(int x, int y) {
	glfwSetWindowPos(window, x, y);
}
void ofWindow::setWindowPosition(ofPoint pos) {
	setWindowPosition(pos.x, pos.y);
}
void ofWindow::setWindowShape(int w, int h) {
	glfwSetWindowSize(window, w, h);
}

void ofWindow::setWidth(int w){
	glfwSetWindowSize(window, w, height);
}
void ofWindow::setHeight(int h){
	glfwSetWindowSize(window, width, h);
}
void ofWindow::windowResized(int w, int h) {
	if(width == w && height == h)
		return;
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
#ifndef OF_USING_POCO
	ofWindowListenerList::iterator it=listeners.begin();
	while(it!=listeners.end()) {
		(*it)->windowResized(width, height, this);
		++it;
	}
#endif
	draw();
}
void ofWindow::windowMoved(int _x, int _y) {
	if(x == _x && y == _y)
		return;
	previousShape.x = x;
	previousShape.y = y;
	x = _x;
	y = _y;

#ifdef OF_USING_POCO
	ofMoveEventArgs e;
	e.x = x;
	e.y = y;
	ofNotifyEvent(events.windowMoved, e);
#endif
#ifndef OF_USING_POCO
	
	ofWindowListenerList::iterator it=listeners.begin();
	while(it!=listeners.end()) {
		(*it)->windowMoved(x, y, this);
		++it;
	}
#endif
}
void ofWindow::setTitle(string t) {
	if (isInitialized)
		glfwSetWindowTitle(window, t.c_str());
	title = t;
}
string ofWindow::getTitle() {
	return title;
}

void ofWindow::close(){
	ofGetWindowManager()->deleteWindow(this);
}

bool ofWindow::isClosed(){
	return window == NULL;
}