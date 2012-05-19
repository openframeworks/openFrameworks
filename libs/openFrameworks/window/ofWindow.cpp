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
	window = glfwOpenWindow( width, height, GLFW_WINDOWED, title.c_str(), NULL );
	isInitialized = true;
	setWindowPositionAndShape(x, y, width, height);
}
void ofWindow::addListener(ofWindowListener* listener) {
	listeners.push_back(listener);
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
	ofEventArgs e;
	ofNotifyEvent(events.update, e);
	ofWindowListenerList::iterator it=listeners.begin();
	while(it!=listeners.end()) {
		(*it)->update(this);
		++it;
	}
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
	ofEventArgs e;
	ofNotifyEvent(events.draw, e);
	ofWindowListenerList::iterator it=listeners.begin();
	while(it!=listeners.end()) {
		(*it)->draw(this);
		++it;
	}
	glfwSwapBuffers();
}
void ofWindow::destroy() {
	glfwCloseWindow(window);
}
//USER INTERACTION EVENTS
void ofWindow::mouseMoved(int mX, int mY) {
	mouseX = mX;
	mouseY = mY;
	ofNotifyMouseMoved(mouseX, mouseY);
	ofWindowListenerList::iterator it=listeners.begin();
	while(it!=listeners.end()) {
		(*it)->mouseMoved(mouseX, mouseY, this);
		++it;
	}
}
void ofWindow::mousePressed(int button) {
	mousePressed(mouseX, mouseY, button);
}
void ofWindow::mousePressed(int mX, int mY, int button) {
	mouseX = mX;
	mouseY = mY;
	ofNotifyMousePressed(mouseX, mouseY, button);
	ofWindowListenerList::iterator it=listeners.begin();
	while(it!=listeners.end()) {
		(*it)->mousePressed(mouseX, mouseY, button, this);
		++it;
	}
}
void ofWindow::mouseReleased(int button) {
	mouseReleased(mouseX, mouseY, button);
}
void ofWindow::mouseReleased(int mX, int mY, int button) {
	mouseX = mX;
	mouseY = mY;
	ofNotifyMouseReleased(mouseX, mouseY, button);
	ofWindowListenerList::iterator it=listeners.begin();
	while(it!=listeners.end()) {
		(*it)->mouseReleased(mouseX, mouseY, button, this);
		++it;
	}
}
void ofWindow::mouseDragged(int mouseX, int mouseY, int button) {
	ofNotifyMouseDragged(mouseX, mouseY, button);
	ofWindowListenerList::iterator it=listeners.begin();
	while(it!=listeners.end()) {
		(*it)->mouseDragged(mouseX, mouseY, button, this);
		++it;
	}
}

void ofWindow::scrolled(float deltaX, float deltaY){
	//ofNotifyMouseDragged(mouseX, mouseY, button);
	ofWindowListenerList::iterator it=listeners.begin();
	while(it!=listeners.end()) {
		(*it)->scrolled(deltaX, deltaY, this);
		++it;
	}
}

void ofWindow::keyPressed(int key){
	if(key<OF_MAX_NUM_KEYS)
		keyState[key] = true;
	ofNotifyKeyPressed(key);
	ofWindowListenerList::iterator it=listeners.begin();
	while(it!=listeners.end()) {
		(*it)->keyPressed(key, this);
		++it;
	}
}

void ofWindow::keyReleased(int key){
	if(key<OF_MAX_NUM_KEYS)
		keyState[key] = false;
	ofNotifyKeyReleased(key);
	ofWindowListenerList::iterator it=listeners.begin();
	while(it!=listeners.end()) {
		(*it)->keyReleased(key, this);
		++it;
	}
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
	ofLogNotice("WINDOW WAS CLOSED, TRIGGER SOME EVENTS AND THINGS!");
};
ofPoint	ofWindow::getWindowPosition() {
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
	return x;
}
int ofWindow::getY(){
	return y;
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
	ofResizeEventArgs e;
	e.width = width;
	e.height = height;
	ofNotifyEvent(events.windowResized, e);
	
	ofWindowListenerList::iterator it=listeners.begin();
	while(it!=listeners.end()) {
		(*it)->windowResized(width, height, this);
		++it;
	}
	
	draw();
}
void ofWindow::windowMoved(int _x, int _y) {
	if(x == _x && y == _y)
		return;
	previousShape.x = x;
	previousShape.y = y;
	x = _x;
	y = _y;
	
	ofMoveEventArgs e;
	e.x = x;
	e.y = y;
	ofNotifyEvent(events.windowMoved, e);
	
	ofWindowListenerList::iterator it=listeners.begin();
	while(it!=listeners.end()) {
		(*it)->windowMoved(x, y, this);
		++it;
	}
	
}
void ofWindow::setTitle(string t) {
	if (isInitialized)
		glfwSetWindowTitle(window, t.c_str());
	title = t;
}
string ofWindow::getTitle() {
	return title;
}
