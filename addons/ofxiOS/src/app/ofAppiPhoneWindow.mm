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
#import "ofGLProgrammableRenderer.h"
#import "ofAppiPhoneWindow.h"
#import "ofxiOSEAGLView.h"

//----------------------------------------------------------------------------------- instance.
static ofAppiPhoneWindow * _instance = NULL;
ofAppiPhoneWindow * ofAppiPhoneWindow::getInstance() {
	return _instance;
}

//----------------------------------------------------------------------------------- constructor / destructor.
ofAppiPhoneWindow::ofAppiPhoneWindow() {
	if(_instance == NULL) {
        _instance = this;
    } else {
        ofLog(OF_LOG_ERROR, "ofAppiPhoneWindow::ofAppiPhoneWindow() - instanciated more than once.");
    }

    windowMode = OF_FULLSCREEN;
    
	bEnableSetupScreen = true;
    orientation = OF_ORIENTATION_DEFAULT;

	bRetinaEnabled = false;
    bRetinaSupportedOnDevice = false;
    bRetinaSupportedOnDeviceChecked = false;
	bDepthEnabled = false;
	bAntiAliasingEnabled = false;
    antiAliasingSamples = 0;
}

ofAppiPhoneWindow::~ofAppiPhoneWindow() {
    //
}

//----------------------------------------------------------------------------------- opengl setup.
void ofAppiPhoneWindow::setupOpenGL(int w, int h, int screenMode) {
	windowMode = screenMode; // use this as flag for displaying status bar or not
}

void ofAppiPhoneWindow::initializeWindow() {
    //
}

void ofAppiPhoneWindow::runAppViaInfiniteLoop(ofBaseApp * appPtr) {
    startAppWithDelegate("ofxiPhoneAppDelegate");
}

void ofAppiPhoneWindow::startAppWithDelegate(string appDelegateClassName) {
    static bool bAppCreated = false;
    if(bAppCreated == true) {
        return;
    }
    bAppCreated = true;
    
    NSAutoreleasePool * pool = [[NSAutoreleasePool alloc] init];
    UIApplicationMain(nil, nil, nil, [NSString stringWithUTF8String:appDelegateClassName.c_str()]);
    [pool release];
}


//----------------------------------------------------------------------------------- cursor.
void ofAppiPhoneWindow::hideCursor() {
    // not supported on iOS.
}

void ofAppiPhoneWindow::showCursor() {
    // not supported on iOS.
}

//----------------------------------------------------------------------------------- window / screen properties.
void ofAppiPhoneWindow::setWindowPosition(int x, int y) {
	// not supported on iOS.
}

void ofAppiPhoneWindow::setWindowShape(int w, int h) {
	// not supported on iOS.
}

ofPoint	ofAppiPhoneWindow::getWindowPosition() {
	return *[[ofxiOSEAGLView getInstance] getWindowPosition];
}

ofPoint	ofAppiPhoneWindow::getWindowSize() {
	return *[[ofxiOSEAGLView getInstance] getWindowSize];
}

ofPoint	ofAppiPhoneWindow::getScreenSize() {
	return *[[ofxiOSEAGLView getInstance] getScreenSize];
}

int ofAppiPhoneWindow::getWidth(){
	if( orientation == OF_ORIENTATION_DEFAULT || orientation == OF_ORIENTATION_180 ){
		return (int)getWindowSize().x;
	}
	return (int)getWindowSize().y;
}

int ofAppiPhoneWindow::getHeight(){
	if( orientation == OF_ORIENTATION_DEFAULT || orientation == OF_ORIENTATION_180 ){
		return (int)getWindowSize().y;
	}
	return (int)getWindowSize().x;
}

int	ofAppiPhoneWindow::getWindowMode() {
	return windowMode;
}

//----------------------------------------------------------------------------------- orientation.
void ofAppiPhoneWindow::setOrientation(ofOrientation orientation) {
	switch (orientation) {
		case OF_ORIENTATION_DEFAULT:
			[[UIApplication sharedApplication] setStatusBarOrientation: UIInterfaceOrientationPortrait];
			break;
		case OF_ORIENTATION_180:
			[[UIApplication sharedApplication] setStatusBarOrientation: UIInterfaceOrientationPortraitUpsideDown];
			break;
		case OF_ORIENTATION_90_RIGHT:
			[[UIApplication sharedApplication] setStatusBarOrientation: UIInterfaceOrientationLandscapeLeft];
			break;
		case OF_ORIENTATION_90_LEFT:
			[[UIApplication sharedApplication] setStatusBarOrientation: UIInterfaceOrientationLandscapeRight];
            break;
	}
	
	this->orientation = orientation;
}

ofOrientation ofAppiPhoneWindow::getOrientation() {
	return orientation;
}

bool ofAppiPhoneWindow::doesHWOrientation() {
    return false;
}

//-----------------------------------------------------------------------------------
void ofAppiPhoneWindow::setWindowTitle(string title) {
    // not supported on iOS.
}

void ofAppiPhoneWindow::setFullscreen(bool fullscreen) {
    [[UIApplication sharedApplication] setStatusBarHidden:fullscreen withAnimation:UIStatusBarAnimationSlide];
	if(fullscreen) {
        windowMode = OF_FULLSCREEN;
    } else {
        windowMode = OF_WINDOW;
    }
}

void ofAppiPhoneWindow::toggleFullscreen() {
	if(windowMode == OF_FULLSCREEN) {
        setFullscreen(false);
    } else {
        setFullscreen(true);
    }
}

//-----------------------------------------------------------------------------------
bool ofAppiPhoneWindow::enableRendererES2() {
    if(isRendererES2() == true) {
        return false;
    }
    ofSetCurrentRenderer(ofPtr<ofBaseRenderer>(new ofGLProgrammableRenderer(false)));
    return true;
}

bool ofAppiPhoneWindow::enableRendererES1() {
    if(isRendererES1() == true) {
        return false;
    }
    ofSetCurrentRenderer(ofPtr<ofBaseRenderer>(new ofGLRenderer(false)));
    return true;
}

bool ofAppiPhoneWindow::isRendererES2() {
    return (ofGetCurrentRenderer() && ofGetCurrentRenderer()->getType()==ofGLProgrammableRenderer::TYPE);
}

bool ofAppiPhoneWindow::isRendererES1() {
    return (ofGetCurrentRenderer() && ofGetCurrentRenderer()->getType()==ofGLRenderer::TYPE);
}

//-----------------------------------------------------------------------------------
void ofAppiPhoneWindow::enableSetupScreen() {
	bEnableSetupScreen = true;
};

void ofAppiPhoneWindow::disableSetupScreen() {
	bEnableSetupScreen = false;
};

bool ofAppiPhoneWindow::isSetupScreenEnabled() {
    return bEnableSetupScreen;
}

void ofAppiPhoneWindow::setVerticalSync(bool enabled) {
    // not supported on iOS.
}

//----------------------------------------------------------------------------------- retina.
bool ofAppiPhoneWindow::enableRetina() {
    if(isRetinaSupportedOnDevice()) {
        bRetinaEnabled = true;
    }
    return bRetinaEnabled;
}

bool ofAppiPhoneWindow::disableRetina() {
    return (bRetinaEnabled = false);
}

bool ofAppiPhoneWindow::isRetinaEnabled() {
    return bRetinaEnabled;
}

bool ofAppiPhoneWindow::isRetinaSupportedOnDevice() {
    if(bRetinaSupportedOnDeviceChecked) {
        return bRetinaSupportedOnDevice;
    }
    
    bRetinaSupportedOnDeviceChecked = true;
    
    NSAutoreleasePool * pool = [[NSAutoreleasePool alloc] init];
    if([[UIScreen mainScreen] respondsToSelector:@selector(scale)]){
        if ([[UIScreen mainScreen] scale] > 1){
            bRetinaSupportedOnDevice = true;
        }
    }
    [pool release];
    
    return bRetinaSupportedOnDevice;
}

//----------------------------------------------------------------------------------- depth buffer.
bool ofAppiPhoneWindow::enableDepthBuffer() {
    return (bDepthEnabled = true);
}

bool ofAppiPhoneWindow::disableDepthBuffer() {
    return (bDepthEnabled = false);
}

bool ofAppiPhoneWindow::isDepthBufferEnabled() {
    return bDepthEnabled;
}

//----------------------------------------------------------------------------------- anti aliasing.
bool ofAppiPhoneWindow::enableAntiAliasing(int samples) {
	antiAliasingSamples = samples;
    return (bAntiAliasingEnabled = true);
}

bool ofAppiPhoneWindow::disableAntiAliasing() {
    return (bAntiAliasingEnabled = false);
}

bool ofAppiPhoneWindow::isAntiAliasingEnabled() {
    return bAntiAliasingEnabled;
}

int	ofAppiPhoneWindow::getAntiAliasingSampleCount() {
    return antiAliasingSamples;
}
