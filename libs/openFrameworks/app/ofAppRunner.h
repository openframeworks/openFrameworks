#pragma once

#include "ofConstants.h"
#include "ofPoint.h"
#include "ofRectangle.h"
#include "ofTypes.h"

class ofAppBaseWindow;
class ofBaseApp;
class ofBaseRenderer;

void 		ofSetupOpenGL(ofPtr<ofAppBaseWindow> windowPtr, int w, int h, int screenMode);	// sets up the opengl context!
void 		ofSetupOpenGL(int w, int h, int screenMode);	// sets up the opengl context!
OF_DEPRECATED_MSG("Use ofSetupOpenGL(int w, int h, int screenMode) instead.", void ofSetupOpenGL(ofAppBaseWindow * windowPtr, int w, int h, int screenMode));

void 		ofRunApp(ofPtr<ofBaseApp> OFSA);
OF_DEPRECATED_MSG("Use ofRunApp(ofPtr<ofBaseApp>) instead.", void ofRunApp(ofBaseApp * OFSA = NULL));


ofBaseApp * ofGetAppPtr();
void ofSetAppPtr(ofPtr<ofBaseApp> appPtr);

void		ofExit(int status=0);

//-------------------------- time
float 		ofGetFrameRate();
float 		ofGetTargetFrameRate();
int			ofGetFrameNum();
void 		ofSetFrameRate(int targetRate);
void		ofSleepMillis(int millis);
double		ofGetLastFrameTime();

void		ofSetOrientation(ofOrientation orientation, bool vFlip=true);
ofOrientation			ofGetOrientation();

//-------------------------- cursor
void 		ofHideCursor();
void 		ofShowCursor();
//-------------------------- window / screen
int 		ofGetWindowPositionX();
int 		ofGetWindowPositionY();
int 		ofGetScreenWidth();
int 		ofGetScreenHeight();
int			ofGetWindowMode();
int 		ofGetWidth();			// ofGetWidth is correct for orientation
int 		ofGetHeight();
int 		ofGetWindowWidth();			// ofGetWindowWidth is correct for actual window coordinates - so doesn't change with orientation. 
int 		ofGetWindowHeight();
bool		ofDoesHWOrientation();
ofPoint		ofGetWindowSize();
ofRectangle	ofGetWindowRect();
ofAppBaseWindow * ofGetWindowPtr();

void 		ofSetWindowPosition(int x, int y);
void 		ofSetWindowShape(int width, int height);
void 		ofSetWindowTitle(string title);
void		ofEnableSetupScreen();
void		ofDisableSetupScreen();
void		ofSetFullscreen(bool fullscreen);
void		ofToggleFullscreen();
//-------------------------- sync
void 		ofSetVerticalSync(bool bSync);

