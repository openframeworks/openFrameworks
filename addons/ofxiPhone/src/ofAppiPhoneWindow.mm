/***********************************************************************
 
 Copyright (c) 2008, 2009, Memo Akten, www.memo.tv
 *** The Mega Super Awesome Visuals Company ***
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of MSA Visuals nor the names of its contributors 
 *       may be used to endorse or promote products derived from this software
 *       without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" 
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, 
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS 
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 * OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE 
 * OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
 * OF THE POSSIBILITY OF SUCH DAMAGE. 
 *
 * ***********************************************************************/ 


#import "ofMain.h"

#import "ofAppiPhoneWindow.h"
#import "ofxiPhoneExtras.h"



// use for checking if stuff has been initialized
#define NOT_INITIALIZED			-1000000

static ofAppiPhoneWindow *_instance = NULL;

ofAppiPhoneWindow* ofAppiPhoneWindow::getInstance() {
	return _instance;
}


/******** Constructor ************/

ofAppiPhoneWindow::ofAppiPhoneWindow() {
	ofLog(OF_LOG_VERBOSE, "ofAppiPhoneWindow::ofAppiPhoneWindow()");
	if(_instance == NULL) _instance = this;
	else ofLog(OF_LOG_ERROR, "Instanciating ofAppiPhoneWindow more than once! how come?");
	nFrameCount = 0;
	bEnableSetupScreen = true;
	
	windowPos.set(NOT_INITIALIZED, NOT_INITIALIZED);
	windowSize.set(NOT_INITIALIZED, NOT_INITIALIZED);
	screenSize.set(NOT_INITIALIZED, NOT_INITIALIZED);
}

/******** Initialization methods ************/

void ofAppiPhoneWindow::setupOpenGL(int w, int h, int screenMode) {
	ofLog(OF_LOG_VERBOSE, "ofAppiPhoneWindow::setupOpenGL()");
	
	windowMode = screenMode;	// use this as flag for displaying status bar or not

	ofDisableTextureEdgeHack();
	
	
	// w & h are ignored, currently Apple don't allow windows which aren't fullscreen
	// DO NOTHING ELSE, opengl will be setup by the app which creates an opengl view
}


void ofAppiPhoneWindow::initializeWindow() {
	ofLog(OF_LOG_VERBOSE, "ofAppiPhoneWindow::initializeWindow()");
	// DO NOTHING, window will be created when the app is launched
}


void  ofAppiPhoneWindow::runAppViaInfiniteLoop(ofBaseApp * appPtr) {
	ofLog(OF_LOG_VERBOSE, "ofAppiPhoneWindow::runAppViaInfiniteLoop()");
	
	NSAutoreleasePool * pool = [[NSAutoreleasePool alloc] init];
	UIApplicationMain(nil, nil, nil, @"ofxiPhoneAppDelegate");		// this will run the infinite loop checking all events
	[pool release];	
}




/******** Set Window properties ************/

void setWindowPosition(int x, int y) {
	// DO NOTHING, you're not really allowed to play with window for now
}

void setWindowShape(int w, int h) {
	// DO NOTHING, you're not really allowed to play with window for now
}



/******** Get Window/Screen properties ************/

// return cached pos, read if nessecary
ofPoint	ofAppiPhoneWindow::getWindowPosition() {
	if(windowPos.x == NOT_INITIALIZED) {
		CGPoint p = [[[UIApplication sharedApplication] keyWindow] bounds].origin;
		windowPos.set(p.x, p.y, 0);
	}
	return windowPos;
}


// return cached size, read if nessecary
ofPoint	ofAppiPhoneWindow::getWindowSize() {
	if(windowSize.x == NOT_INITIALIZED) {
		CGSize s = [[[UIApplication sharedApplication] keyWindow] bounds].size;
		if(orientation == OFXIPHONE_ORIENTATION_PORTRAIT || orientation == OFXIPHONE_ORIENTATION_UPSIDEDOWN)
		{
			windowSize.set(s.width, s.height, 0);
		}
		else //if(orientation == OFXIPHONE_ORIENTATION_LANDSCAPE)
		{
			windowSize.set(s.height, s.width, 0);
		}
	}
	return windowSize;
}


// return cached size, read if nessecary
ofPoint	ofAppiPhoneWindow::getScreenSize() {
	if(screenSize.x == NOT_INITIALIZED) {
		CGSize s = [[UIScreen mainScreen] bounds].size;
		if(orientation == OFXIPHONE_ORIENTATION_PORTRAIT || orientation == OFXIPHONE_ORIENTATION_UPSIDEDOWN)
		{
			screenSize.set(s.width, s.height, 0);
		}
		else //if(orientation == OFXIPHONE_ORIENTATION_LANDSCAPE)
		{
			screenSize.set(s.height, s.width, 0);
		}
	}
	return screenSize;
}

int	ofAppiPhoneWindow::getWindowMode() {
	return windowMode;
}

float ofAppiPhoneWindow::getFrameRate() {
	return frameRate;
}

/******** Other stuff ************/
void ofAppiPhoneWindow::setFrameRate(float targetRate) {
	[ofxiPhoneGetAppDelegate() setFrameRate:targetRate];
}

int	ofAppiPhoneWindow::getFrameNum() {
	return nFrameCount;
}

void ofAppiPhoneWindow::setWindowTitle(string title) {
}


void ofAppiPhoneWindow::setFullscreen(bool fullscreen) {
	[[UIApplication sharedApplication] setStatusBarHidden:fullscreen animated:YES];
	
	if(fullscreen) windowMode		= OF_FULLSCREEN;
	else windowMode					= OF_WINDOW;
}

void ofAppiPhoneWindow::toggleFullscreen() {
	if(windowMode == OF_FULLSCREEN) setFullscreen(false);
	else setFullscreen(true);
}


void ofAppiPhoneWindow::enableSetupScreen(){
	bEnableSetupScreen = true;
};

void ofAppiPhoneWindow::disableSetupScreen(){
	bEnableSetupScreen = false;
};

void ofAppiPhoneWindow::setOrientation(int orientation) {
	ofLog(OF_LOG_VERBOSE, "ofAppiPhoneWindow::setOrientation: " + ofToString(orientation));
	switch (orientation) {
		case OFXIPHONE_ORIENTATION_PORTRAIT:
			[[UIApplication sharedApplication] setStatusBarOrientation: UIInterfaceOrientationPortrait];
			break;
		case OFXIPHONE_ORIENTATION_UPSIDEDOWN:
			[[UIApplication sharedApplication] setStatusBarOrientation: UIInterfaceOrientationPortraitUpsideDown];
			break;
		case OFXIPHONE_ORIENTATION_LANDSCAPE_RIGHT:
			[[UIApplication sharedApplication] setStatusBarOrientation: UIInterfaceOrientationLandscapeRight];
			break;
		case OFXIPHONE_ORIENTATION_LANDSCAPE_LEFT:
			[[UIApplication sharedApplication] setStatusBarOrientation: UIInterfaceOrientationLandscapeLeft];			break;
			
		default:
			break;
	}
	
	this->orientation = orientation;
	windowSize.x = NOT_INITIALIZED;
	screenSize.x = NOT_INITIALIZED;
	getScreenSize();
	getWindowSize();
}


int ofAppiPhoneWindow::getOrientation() {
	return orientation;
}

void ofAppiPhoneWindow::rotateXY(float &x, float &y) {
	float savedX;
	switch(orientation) {
		case OFXIPHONE_ORIENTATION_UPSIDEDOWN:
			x = windowSize.x - x;
			y = windowSize.y - y;
			break;
			
		case OFXIPHONE_ORIENTATION_LANDSCAPE_RIGHT:
			savedX = x;
			x = y;
			y = windowSize.y - savedX;
			break;
			
		case OFXIPHONE_ORIENTATION_LANDSCAPE_LEFT:
			savedX = x;
			x = windowSize.x - y;
			y = savedX;
			break;
			
		case OFXIPHONE_ORIENTATION_PORTRAIT:
		default:
			break;
			
	}
}


void ofAppiPhoneWindow::timerLoop() {
	static ofEventArgs voidEventArgs;
	
	ofGetAppPtr()->update();
		
	#ifdef OF_USING_POCO
		ofNotifyEvent( ofEvents.update, voidEventArgs);
	#endif
	
	[ofxiPhoneGetAppDelegate() lockGL];

	[ofxiPhoneGetGLView() startRender];

	// this could be taken out and included in ofAppBaseWIndow
	if(orientation == OFXIPHONE_ORIENTATION_PORTRAIT || orientation == OFXIPHONE_ORIENTATION_UPSIDEDOWN)
		glViewport( 0, 0, ofGetWidth(), ofGetHeight() );
	else
		glViewport( 0, 0, ofGetHeight(), ofGetWidth() );
	
	float * bgPtr = ofBgColorPtr();
	bool bClearAuto = ofbClearBg();
	if ( bClearAuto == true){
		glClearColor(bgPtr[0],bgPtr[1],bgPtr[2], bgPtr[3]);
		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}
	
	
	if(bEnableSetupScreen) {
		int w, h;
		
		switch(orientation) {
			case OFXIPHONE_ORIENTATION_LANDSCAPE_RIGHT:
					h = ofGetWidth();
					w = ofGetHeight();
				break;
				
			case OFXIPHONE_ORIENTATION_LANDSCAPE_LEFT:
					h = ofGetWidth();
					w = ofGetHeight();
				break;
	
			default:
				w = ofGetWidth();
				h = ofGetHeight();
			break;
		}
		
		float halfFov, theTan, screenFov, aspect;
		screenFov 		= 60.0f;
		
		float eyeX 		= (float)w / 2.0;
		float eyeY 		= (float)h / 2.0;
		halfFov 		= PI * screenFov / 360.0;
		theTan 			= tanf(halfFov);
		float dist 		= eyeY / theTan;
		float nearDist 	= dist / 10.0;	// near / far clip plane
		float farDist 	= dist * 10.0;
		aspect 			= (float)w/(float)h;
		
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective(screenFov, aspect, nearDist, farDist);
		
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		gluLookAt(eyeX, eyeY, dist, eyeX, eyeY, 0.0, 0.0, 1.0, 0.0);
		
		// adjust so that top left is always 0,0
		switch(orientation) {
			case OFXIPHONE_ORIENTATION_UPSIDEDOWN:
				glRotatef(180, 0, 0, 1);
				//				glTranslatef(0, -w, 0);
				break;
				
			case OFXIPHONE_ORIENTATION_LANDSCAPE_RIGHT:
				glRotatef(-90, 0, 0, 1);
				glScalef(1, -1, 1);        
				glTranslatef(-h, -w, 0);    
				break;
				
			case OFXIPHONE_ORIENTATION_LANDSCAPE_LEFT:
				glRotatef(90, 0, 0, 1);
				glScalef(1, -1, 1);        
				break;
				
			case OFXIPHONE_ORIENTATION_PORTRAIT:
			default:
				glScalef(1, -1, 1);        
				glTranslatef(0, -h, 0);    
				break;
		}
		
		
	}
	
	
	ofGetAppPtr()->draw();
	#ifdef OF_USING_POCO
		ofNotifyEvent( ofEvents.draw, voidEventArgs );
	#endif
	
	[ofxiPhoneGetGLView() finishRender];
	
	[ofxiPhoneGetAppDelegate() unlockGL];

	
	
	
	// -------------- fps calculation:
	timeNow = ofGetElapsedTimef();
    double diff = timeNow-timeThen;
	if( diff  > 0.0f ) {
		fps = 1.0 / diff;
		frameRate *= 0.9f;
        frameRate += 0.1f*fps;
	}
	timeThen = timeNow;
  	// --------------
	
	nFrameCount++;		// increase the overall frame count
}
