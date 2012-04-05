/*
 *  ofWindowManager.cpp
 *  openFrameworksLib
 *
 *  Created by Philip Whitfield on 3/8/12.
 *  Copyright 2012 undef.ch. All rights reserved.
 *
 */
#include <ofEvents.h>
#include "ofWindowManager.h"
#include "ofConstants.h"
#include "ofAppRunner.h"


/********** GLFW CALLBACKS **********************************************************/

void glfwErrorCallback(int type, const char* err){
	ofLog(OF_LOG_ERROR, err);
}

void glfwWindowSizeCallback(GLFWwindow glfwWin, int w, int h){
	ofGetWindowManager()->glfwWindowSize(glfwWin, w, h);
}

int glfwWindowCloseCallback(GLFWwindow glfwWin){
	return ofGetWindowManager()->glfwWindowClose(glfwWin);
}

void glfwWindowRefreshCallback(GLFWwindow glfwWin){
	ofGetWindowManager()->glfwWindowRefresh(glfwWin);
}

void glfwWindowFocusCallback(GLFWwindow glfwWin, int action){
	ofGetWindowManager()->glfwWindowFocus(glfwWin, action);
}

void glfwWindowIconifyCallback(GLFWwindow glfwWin, int action){
	ofGetWindowManager()->glfwWindowIconify(glfwWin, action);
}

void glfwMouseButtonCallback(GLFWwindow glfwWin, int button, int action){
	ofGetWindowManager()->glfwMouseButton(glfwWin, button, action);
}

void glfwMousePosCallback(GLFWwindow glfwWin, int x, int y){
	ofGetWindowManager()->glfwMousePos(glfwWin, x, y);
}

void glfwCursorEnterCallback(GLFWwindow glfwWin, int action){
	ofGetWindowManager()->glfwCursorEnter(glfwWin, action);
}

void glfwScrollCallback(GLFWwindow glfwWin, double deltaX, double deltaY){
	ofGetWindowManager()->glfwScroll(glfwWin, deltaX, deltaY);
}

void glfwKeyCallback(GLFWwindow glfwWin, int key, int action){
	ofGetWindowManager()->glfwKey(glfwWin, key, action);
}

void glfwCharCallback(GLFWwindow glfwWin, int character){
	ofGetWindowManager()->glfwChar(glfwWin, character);
}



/********* C STYLE FUNCTION DEFINTIONS ************************************************/

ofWindowManager* windowManager = NULL;

ofWindowManager* ofGetWindowManager() {
	if(windowManager == NULL) {
		windowManager = new ofWindowManager();
	}
	return windowManager;
}

ofWindow* ofCreateWindow(int x, int y, int width, int height){
	return ofGetWindowManager()->createWindow(x, y, width, height);
}

ofWindow* ofCreateWindow(int width, int height){
	return ofCreateWindow(0, 0, width, height);
}

ofWindow* ofGetMainWindow(){
	return ofGetWindowManager()->getMainWindow();
}

ofWindow* ofGetLastCreatedWindow(){
	return ofGetWindowManager()->getLastCreatedWindow();
}



/********** ofWindowManager DEFINITIONS ******************************************/

static float		timeNow, timeThen, fps;
static int			nFramesForFPS;
static int			nFrameCount;
static bool			bFrameRateSet;
static int 			millisForFrame;
static int 			prevMillis;
static int 			diffMillis;
static float 		frameRate;
static double		lastFrameTime;

ofWindowManager::ofWindowManager() {
	timeNow				= 0;
	timeThen			= 0;
	fps					= 60.0; //give a realistic starting value - win32 issues
	frameRate			= 60.0;
	millisForFrame		= 0;
	prevMillis			= 0;
	diffMillis			= 0;
	setFrameRate(60);
};

ofWindowManager::~ofWindowManager() {
	glfwTerminate();
};

ofWindow* ofWindowManager::createWindow(int w, int h){
	return createWindow(0, 0, w, h);
}

ofWindow* ofWindowManager::createWindow(int x, int y, int width, int height)
{
	ofWindow* win = new ofWindow();
	win->setWindowPositionAndShape(x, y, width, height);
	addWindow(win);
	win->initializeWindow();
	win->setup();
	return win;
}

void ofWindowManager::addWindow(ofWindow* win) {
	windows.push_back(ofWindowPtr(win));
}

void ofWindowManager::deleteWindow(ofWindow* win){
	win->destroy();
}

void ofWindowManager::removeWindow(ofWindow* win){
	ofWindowList::iterator it=windows.begin();
	while(it!=windows.end()) {
		if((*it).get() == win) {
			windows.erase(it);
			break;
		}
		++it;
	}
	if(windows.size()==0)
		ofExit();
}

void ofWindowManager::deleteWindow(int id){
	deleteWindow(getWindowById(id));
}

ofWindow* ofWindowManager::getWindowById(int id){
	ofWindowList::iterator it = windows.begin();
	while (it != windows.end()) {
		if((*it)->id == id)
			return (*it).get();
		++it;
	}
	ofLogWarning("ofWindowManager::getWindowById - ID was not found, returning mainWindow instead");
	return mainWindow;
}


ofWindow* ofWindowManager::getWindowByGlfw(GLFWwindow win){
	ofWindowList::iterator it = windows.begin();
	while (it != windows.end()) {
		if((*it)->getGlfwWindow() == win)
			return (*it).get();
		++it;
	}
	ofLogWarning("ofWindowManager::getWindowById - GLFWwindow was not found, returning mainWindow instead");
	return mainWindow;
}

void ofWindowManager::initializeGLFW(){
	if( !glfwInit() )
    {
        ofLogError("Failed to initialize GLFW");
        ofExit(0);
    }
}

void ofWindowManager::setupOpenGL(int w, int h, int screenMode) {
	initializeGLFW();
	
	glfwOpenWindowHint(GLFW_DEPTH_BITS, 16);
	
	mainWindow = createWindow(w, h);
	activeWindow = mainWindow;
}

void ofWindowManager::initializeWindow()
{
	//define all callbacks now, so there won't happen anything before OF is fully initialized
	glfwSetErrorCallback(&glfwErrorCallback);
	glfwSetWindowSizeCallback(&glfwWindowSizeCallback);
	glfwSetWindowCloseCallback(&glfwWindowCloseCallback);
	glfwSetWindowRefreshCallback(&glfwWindowRefreshCallback);
	glfwSetWindowFocusCallback(&glfwWindowFocusCallback);
	glfwSetWindowIconifyCallback(&glfwWindowIconifyCallback);
	glfwSetMouseButtonCallback(&glfwMouseButtonCallback);
	glfwSetMousePosCallback(&glfwMousePosCallback);
	glfwSetCursorEnterCallback(&glfwCursorEnterCallback);
	glfwSetScrollCallback(&glfwScrollCallback);
	glfwSetKeyCallback(&glfwKeyCallback);
	glfwSetCharCallback(&glfwCharCallback);
	
	mainWindow->setup();
}

void ofWindowManager::runAppViaInfiniteLoop(ofBaseApp * appPtr) {
	mainWindow->addListener(appPtr);
	appPtr->setup();
	
	//run the main loop
	while (true) {
		glfwPollEvents();
		update();
		draw();
	}
}

void ofWindowManager::update() {
	//	thanks to jorge for the fix:
	//	http://www.openframeworks.cc/forum/viewtopic.php?t=515&highlight=frame+rate
	if (nFrameCount != 0 && bFrameRateSet == true) {
		diffMillis = ofGetElapsedTimeMillis() - prevMillis;
		if (diffMillis > millisForFrame) {
			; // we do nothing, we are already slower than target frame
		} else {
			int waitMillis = millisForFrame - diffMillis;
#ifdef TARGET_WIN32
			Sleep(waitMillis);         //windows sleep in milliseconds
#else
			usleep(waitMillis * 1000);   //mac sleep in microseconds - cooler :)
#endif
		}
	}
	prevMillis = ofGetElapsedTimeMillis(); // you have to measure here
										   // -------------- fps calculation:
										   // theo - now moved from display to idle_cb
										   // discuss here: http://github.com/openframeworks/openFrameworks/issues/labels/0062#issue/187
	//
	//
	// theo - please don't mess with this without letting me know.
	// there was some very strange issues with doing ( timeNow-timeThen ) producing different values to: double diff = timeNow-timeThen;
	// http://www.openframeworks.cc/forum/viewtopic.php?f=7&t=1892&p=11166#p11166
	timeNow = ofGetElapsedTimef();
	double diff = timeNow-timeThen;
	if( diff  > 0.00001 ) {
		fps			= 1.0 / diff;
		frameRate	*= 0.9f;
		frameRate	+= 0.1f*fps;
	}
	lastFrameTime	= diff;
	timeThen		= timeNow;
	
	// --------------
	ofNotifyUpdate();
}

void ofWindowManager::draw(){
	ofNotifyDraw();
}

ofWindow* ofWindowManager::getLastCreatedWindow()
{
	return windows.back().get();
}


ofWindow* ofWindowManager::getMainWindow()
{
	return mainWindow;
}

void ofWindowManager::setFrameRate(float targetRate){
	// given this FPS, what is the amount of millis per frame
	// that should elapse?
	
	// --- > f / s
	
	if (targetRate == 0){
		bFrameRateSet = false;
		return;
	}
	
	bFrameRateSet 			= true;
	float durationOfFrame 	= 1.0f / (float)targetRate;
	millisForFrame 			= (int)(1000.0f * durationOfFrame);
	
	frameRate				= targetRate;
	
}

void ofWindowManager::setActiveWindow(ofWindow* win)
{
	activeWindow = win;
	activeWindow->enableContext();
}

ofPoint	ofWindowManager::getWindowPosition() {return activeWindow->getWindowPosition(); }
ofPoint	ofWindowManager::getWindowSize(){return activeWindow->getWindowSize(); }

//this is used by ofGetWidth and now determines the window width based on orientation
int	ofWindowManager::getWidth(){ return activeWindow->getWidth(); }
int	ofWindowManager::getHeight(){ return activeWindow->getHeight(); }

////GLFW CALLBACKS
void ofWindowManager::glfwWindowFocus(GLFWwindow glfwWin, int action){
	ofWindow* win = getWindowByGlfw(glfwWin);
}
void ofWindowManager::glfwWindowSize(GLFWwindow glfwWin, int w, int h){
	ofWindow* win = getWindowByGlfw(glfwWin);
	win->windowResized(w, h);
}
int ofWindowManager::glfwWindowClose(GLFWwindow glfwWin){
	ofWindow* win = getWindowByGlfw(glfwWin);
	removeWindow(win);
	return 1;
}
void ofWindowManager::glfwWindowRefresh(GLFWwindow glfwWin){
	ofWindow* win = getWindowByGlfw(glfwWin);
}	
void ofWindowManager::glfwWindowIconify(GLFWwindow glfwWin, int action){
	ofWindow* win = getWindowByGlfw(glfwWin);
}
void ofWindowManager::glfwMouseButton(GLFWwindow glfwWin, int button, int action){
	ofWindow* win = getWindowByGlfw(glfwWin);
}
void ofWindowManager::glfwMousePos(GLFWwindow glfwWin, int mouseX, int mouseY){
	ofWindow* win = getWindowByGlfw(glfwWin);
}
void ofWindowManager::glfwCursorEnter(GLFWwindow glfwWin, int action){
	ofWindow* win = getWindowByGlfw(glfwWin);
}
void ofWindowManager::glfwScroll(GLFWwindow glfwWin, double deltaX, double deltaY){
	ofWindow* win = getWindowByGlfw(glfwWin);
}
void ofWindowManager::glfwKey(GLFWwindow glfwWin, int key, int action){
	ofWindow* win = getWindowByGlfw(glfwWin);
}
void ofWindowManager::glfwChar(GLFWwindow glfwWin, int key){
	ofWindow* win = getWindowByGlfw(glfwWin);
}
