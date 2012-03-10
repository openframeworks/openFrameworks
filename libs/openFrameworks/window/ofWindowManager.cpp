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

#ifdef TARGET_LINUX
#include "ofWindowManagerX11.h"
#endif

#ifdef TARGET_OSX
#include "ofWindowManagerCocoa.h"
#endif

ofWindowManager* windowManager = NULL;

ofWindowManager* ofGetWindowManager() {
	if(windowManager == NULL) {
#ifdef TARGET_LINUX
		windowManager = new ofWindowManagerX11();
#endif

#ifdef TARGET_OSX
		windowManager = new ofWindowManagerCocoa();
#endif
		windowManager->init();
	}
	return windowManager;
}


/////////////////////////////////////////////////////////////////////////
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
};
ofWindowManager::~ofWindowManager() {
};
void ofWindowManager::addWindow(ofWindow* win) {
	windows.push_back(ofWindowPtr(win));
	win->initializeWindow();
	win->setup();
}
void ofWindowManager::setupOpenGL(int w, int h, int screenMode) {
	mainWindow = createWindow();
}

void ofWindowManager::runAppViaInfiniteLoop(ofBaseApp * appPtr) {
	//wrap the base app to a window listener
	mainWindow->addListener(new ofWindowToOfBaseApp(appPtr));
	appPtr->setup();

	//run the main loop
	while (true) {
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
	processEvents();
	ofNotifyUpdate();
}

void ofWindowManager::draw(){
	ofNotifyDraw();
}

ofWindow* ofWindowManager::getLastCreatedWindow()
{
	return windows.back().get();
}
