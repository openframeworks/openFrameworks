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
#include <TargetConditionals.h>
#include "ofAppiOSWindow.h"
#include "ofGLRenderer.h"
#include "ofGLProgrammableRenderer.h"
#if TARGET_OS_IOS || (TARGET_OS_IPHONE && !TARGET_OS_TV)
    #include "ofxiOSAppDelegate.h"
    #include "ofxiOSViewController.h"
    const std::string appDelegateName = "ofxiOSAppDelegate";
#elif TARGET_OS_TV
    #include "ofxtvOSAppDelegate.h"
    #include "ofxtvOSViewController.h"
    const std::string appDelegateName = "ofxtvOSAppDelegate";
#endif
#include "ofxiOSGLKView.h"
#include "ofxiOSEAGLView.h"

//----------------------------------------------------------------------------------- instance.
static ofAppiOSWindow * _instance = NULL;
ofAppiOSWindow * ofAppiOSWindow::getInstance() {
	return _instance;
}

//----------------------------------------------------------------------------------- constructor / destructor.
ofAppiOSWindow::ofAppiOSWindow() : hasExited(false) {
	if(_instance == NULL) {
        _instance = this;
    } else {
        ofLog(OF_LOG_ERROR, "ofAppiOSWindow instantiated more than once");
    }
    bRetinaSupportedOnDevice = false;
    bRetinaSupportedOnDeviceChecked = false;
}

ofAppiOSWindow::~ofAppiOSWindow() {
    close();
	_instance = NULL;
}

void ofAppiOSWindow::close() {
    if(hasExited == false){
        hasExited = true;
        events().notifyExit();
        events().disable();
    }
}

void ofAppiOSWindow::setup(const ofWindowSettings & _settings) {
    const ofiOSWindowSettings * iosSettings = dynamic_cast<const ofiOSWindowSettings*>(&_settings);
    if(iosSettings){
        setup(*iosSettings);
    } else{
        setup(ofiOSWindowSettings(_settings));
    }
}

void ofAppiOSWindow::setup(const ofGLESWindowSettings & _settings) {
    const ofiOSWindowSettings * iosSettings = dynamic_cast<const ofiOSWindowSettings*>(&_settings);
    if(iosSettings){
        setup(*iosSettings);
    } else{
        setup(ofiOSWindowSettings(_settings));
    }
}

void ofAppiOSWindow::setup(const ofiOSWindowSettings & _settings) {
    settings = _settings;
	setup();
}

void ofAppiOSWindow::setup() {
	
	
	if(settings.setupOrientation == OF_ORIENTATION_UNKNOWN) {
		settings.setupOrientation = OF_ORIENTATION_DEFAULT;
	}
	setOrientation(settings.setupOrientation);
	if(settings.glesVersion >= ESRendererVersion_20) {
		currentRenderer = std::shared_ptr<ofBaseRenderer>(new ofGLProgrammableRenderer(this));
	} else {
		currentRenderer = std::shared_ptr<ofBaseRenderer>(new ofGLRenderer(this));
	}
	
	hasExited = false;
}

//----------------------------------------------------------------------------------- opengl setup.
void ofAppiOSWindow::setupOpenGL(int w, int h, ofWindowMode screenMode) {
	settings.windowMode = screenMode; // use this as flag for displaying status bar or not
    setup(settings);
}

void ofAppiOSWindow::loop() {
    startAppWithDelegate(appDelegateName);
}

void ofAppiOSWindow::run(ofBaseApp * appPtr){
    startAppWithDelegate(appDelegateName);
}

void ofAppiOSWindow::startAppWithDelegate(std::string appDelegateClassName) {
    static bool bAppCreated = false;
    if(bAppCreated == true) {
        return;
    }
    bAppCreated = true;
    
    @autoreleasepool {
        UIApplicationMain(0, nil, nil, [NSString stringWithUTF8String:appDelegateClassName.c_str()]);
    }
}

void ofAppiOSWindow::update() {
    
}
void ofAppiOSWindow::draw() {
    
}


//----------------------------------------------------------------------------------- cursor.
void ofAppiOSWindow::hideCursor() {
    // not supported on iOS.
}

void ofAppiOSWindow::showCursor() {
    // not supported on iOS.
}

//----------------------------------------------------------------------------------- window / screen properties.
void ofAppiOSWindow::setWindowPosition(int x, int y) {
	// not supported on iOS.
}

void ofAppiOSWindow::setWindowShape(int w, int h) {
	// not supported on iOS.
}

glm::vec2	ofAppiOSWindow::getWindowPosition() {
	if(settings.windowControllerType == METAL_KIT || settings.windowControllerType == GL_KIT)
		return *[[ofxiOSGLKView getInstance] getWindowPosition];
	else
		return *[[ofxiOSEAGLView getInstance] getWindowPosition];
}

glm::vec2	ofAppiOSWindow::getWindowSize() {
	if(settings.windowControllerType == METAL_KIT || settings.windowControllerType == GL_KIT)
		return *[[ofxiOSGLKView getInstance] getWindowSize];
	else
		return *[[ofxiOSEAGLView getInstance] getWindowSize];
}

glm::vec2	ofAppiOSWindow::getScreenSize() {
	if(settings.windowControllerType == METAL_KIT || settings.windowControllerType == GL_KIT)
		return *[[ofxiOSGLKView getInstance] getScreenSize];
	else
		return *[[ofxiOSEAGLView getInstance] getScreenSize];
}

int ofAppiOSWindow::getWidth(){
	if(settings.enableHardwareOrientation == true ||
       orientation == OF_ORIENTATION_DEFAULT ||
       orientation == OF_ORIENTATION_180) {
		return (int)getWindowSize().x;
	}
	return (int)getWindowSize().y;
}

int ofAppiOSWindow::getHeight(){
	if(settings.enableHardwareOrientation == true ||
       orientation == OF_ORIENTATION_DEFAULT ||
       orientation == OF_ORIENTATION_180) {
		return (int)getWindowSize().y;
	}
	return (int)getWindowSize().x;
}

ofWindowMode ofAppiOSWindow::getWindowMode() {
	return settings.windowMode;
}

#if TARGET_OS_IOS || (TARGET_OS_IPHONE && !TARGET_OS_TV)
//----------------------------------------------------------------------------------- orientation.
void ofAppiOSWindow::setOrientation(ofOrientation toOrientation) {
    if(orientation == toOrientation) {
        return;
    }
    bool bOrientationPortraitOne = (orientation == OF_ORIENTATION_DEFAULT) || (orientation == OF_ORIENTATION_180);
    bool bOrientationPortraitTwo = (toOrientation == OF_ORIENTATION_DEFAULT) || (toOrientation == OF_ORIENTATION_180);
    bool bResized = bOrientationPortraitOne != bOrientationPortraitTwo;

    orientation = toOrientation;
#if TARGET_OS_IOS || (TARGET_OS_IPHONE && !TARGET_OS_TV)
    UIInterfaceOrientation interfaceOrientation = UIInterfaceOrientationPortrait;
    switch (orientation) {
        case OF_ORIENTATION_DEFAULT:
            interfaceOrientation = UIInterfaceOrientationPortrait;
            break;
        case OF_ORIENTATION_180:
            interfaceOrientation = UIInterfaceOrientationPortraitUpsideDown;
            break;
        case OF_ORIENTATION_90_RIGHT:
            interfaceOrientation = UIInterfaceOrientationLandscapeLeft;
            break;
        case OF_ORIENTATION_90_LEFT:
            interfaceOrientation = UIInterfaceOrientationLandscapeRight;
            break;
    }


    id<UIApplicationDelegate> appDelegate = [UIApplication sharedApplication].delegate;
    if([appDelegate respondsToSelector:@selector(glViewController)] == NO) {
        // check app delegate has glViewController,
        // otherwise calling glViewController will cause a crash.
        return;
    }
    UIViewController * uiViewController = ((ofxiOSAppDelegate *)appDelegate).uiViewController;
	if([uiViewController isKindOfClass:[ofxiOSViewController class]] == YES) {
		ofxiOSViewController * glViewController = (ofxiOSViewController*)uiViewController;
		if(glViewController) {
			ofxiOSEAGLView * glView = glViewController.glView;
			if(settings.enableHardwareOrientation == true) {
				[glViewController rotateToInterfaceOrientation:interfaceOrientation animated:settings.enableHardwareOrientationAnimation];
			} else {
				[[UIApplication sharedApplication] setStatusBarOrientation:interfaceOrientation animated:settings.enableHardwareOrientationAnimation];
				if(bResized == true) {
					[glView layoutSubviews]; // calling layoutSubviews so window resize notification is fired.
				}
			}
		}
	}
#endif
}

ofOrientation ofAppiOSWindow::getOrientation() {
	return orientation;
}

bool ofAppiOSWindow::doesHWOrientation() {
    return settings.enableHardwareOrientation;
}

//-----------------------------------------------------------------------------------
bool ofAppiOSWindow::enableHardwareOrientation() {
    return (settings.enableHardwareOrientation = true);
}

bool ofAppiOSWindow::disableHardwareOrientation() {
    return (settings.enableHardwareOrientation = false);
}

bool ofAppiOSWindow::enableOrientationAnimation() {
    return (settings.enableHardwareOrientationAnimation = true);
}

bool ofAppiOSWindow::disableOrientationAnimation() {
    return (settings.enableHardwareOrientationAnimation = false);
}

#endif

//-----------------------------------------------------------------------------------
void ofAppiOSWindow::setWindowTitle(std::string title) {
    // not supported on iOS.
}

void ofAppiOSWindow::setFullscreen(bool fullscreen) {
#if TARGET_OS_IOS || (TARGET_OS_IPHONE && !TARGET_OS_TV)
    [[UIApplication sharedApplication] setStatusBarHidden:fullscreen withAnimation:UIStatusBarAnimationSlide];
#endif
    if(fullscreen) {
        settings.windowMode = OF_FULLSCREEN;
    } else {
        settings.windowMode = OF_WINDOW;
    }
}

void ofAppiOSWindow::toggleFullscreen() {
    if(settings.windowMode == OF_FULLSCREEN) {
        setFullscreen(false);
    } else {
        setFullscreen(true);
    }
}



//-----------------------------------------------------------------------------------
bool ofAppiOSWindow::enableRendererES2() {
    if(isRendererES2() == true) {
        return false;
    }
    std::shared_ptr<ofBaseRenderer>renderer (new ofGLProgrammableRenderer(this));
    ofSetCurrentRenderer(renderer);
    return true;
}

bool ofAppiOSWindow::enableRendererES1() {
    if(isRendererES1() == true) {
        return false;
    }
    std::shared_ptr<ofBaseRenderer> renderer(new ofGLRenderer(this));
    ofSetCurrentRenderer(renderer);
    return true;
}


bool ofAppiOSWindow::isProgrammableRenderer() {
    return (currentRenderer && currentRenderer->getType()==ofGLProgrammableRenderer::TYPE);
}

ofxiOSRendererType ofAppiOSWindow::getGLESVersion() {
    return (ofxiOSRendererType)settings.glesVersion;
}

bool ofAppiOSWindow::isRendererES2() {
    return (isProgrammableRenderer() && settings.glesVersion == 2);
}

bool ofAppiOSWindow::isRendererES1() {
    return !isProgrammableRenderer();
}

//-----------------------------------------------------------------------------------
void ofAppiOSWindow::enableSetupScreen() {
	settings.enableSetupScreen = true;
};

void ofAppiOSWindow::disableSetupScreen() {
	settings.enableSetupScreen = false;
};

bool ofAppiOSWindow::isSetupScreenEnabled() {
    return settings.enableSetupScreen;
}

void ofAppiOSWindow::setVerticalSync(bool enabled) {
    // not supported on iOS.
}

//----------------------------------------------------------------------------------- retina.
bool ofAppiOSWindow::enableRetina(float retinaScale) {
    if(isRetinaSupportedOnDevice()) {
        settings.enableRetina = true;
        settings.retinaScale = retinaScale;
    }
    return settings.enableRetina;
}

bool ofAppiOSWindow::disableRetina() {
    return (settings.enableRetina = false);
}

bool ofAppiOSWindow::isRetinaEnabled() {
    return settings.enableRetina;
}

bool ofAppiOSWindow::isRetinaSupportedOnDevice() {
    if(bRetinaSupportedOnDeviceChecked) {
        return bRetinaSupportedOnDevice;
    }
    
    bRetinaSupportedOnDeviceChecked = true;
    
    @autoreleasepool {
        if([[UIScreen mainScreen] respondsToSelector:@selector(scale)]){
            if ([[UIScreen mainScreen] scale] > 1){
                bRetinaSupportedOnDevice = true;
            }
        }
    }
    
    return bRetinaSupportedOnDevice;
}

float ofAppiOSWindow::getRetinaScale() {
    return settings.retinaScale;
}

//----------------------------------------------------------------------------------- depth buffer.
bool ofAppiOSWindow::enableDepthBuffer() {
	settings.depthType = ofxiOSRendererDepthFormat::DEPTH_24;
    return (settings.enableDepth = true);
}

bool ofAppiOSWindow::disableDepthBuffer() {
	settings.depthType = ofxiOSRendererDepthFormat::DEPTH_NONE;
    return (settings.enableDepth = false);
}

bool ofAppiOSWindow::isDepthBufferEnabled() {
    return settings.enableDepth;
}

//----------------------------------------------------------------------------------- anti aliasing.
bool ofAppiOSWindow::enableAntiAliasing(int samples) {
	settings.numOfAntiAliasingSamples = samples;
    return (settings.enableAntiAliasing = true);
}

void ofAppiOSWindow::enableMultiTouch(bool isOn) {
	settings.enableMultiTouch = isOn;
#if TARGET_OS_IOS || (TARGET_OS_IPHONE && !TARGET_OS_TV)
	if(settings.windowControllerType == METAL_KIT || settings.windowControllerType == GL_KIT) {
		if([ofxiOSGLKView getInstance]) {
			[[ofxiOSGLKView getInstance] setMultipleTouchEnabled:isOn];
		}
	} else {
		if([ofxiOSEAGLView getInstance])
			[[ofxiOSEAGLView getInstance] setMultipleTouchEnabled:isOn];
	}
#endif
}

bool ofAppiOSWindow::isMultiTouch() {
	return settings.enableMultiTouch;
}

bool ofAppiOSWindow::disableAntiAliasing() {
    return (settings.enableAntiAliasing = false);
}

bool ofAppiOSWindow::isAntiAliasingEnabled() {
    return settings.enableAntiAliasing;
}

int	ofAppiOSWindow::getAntiAliasingSampleCount() {
    return settings.numOfAntiAliasingSamples;
}

ofxiOSWindowControllerType ofAppiOSWindow::getWindowControllerType() {
	return settings.windowControllerType;
}

ofxiOSRendererColorFormat ofAppiOSWindow::getRendererColorType() {
	return settings.colorType;
}

ofxiOSRendererDepthFormat ofAppiOSWindow::getRendererDepthType() {
	return settings.depthType;
}

ofxiOSRendererStencilFormat ofAppiOSWindow::getRendererStencilType() {
	return settings.stencilType;
}

//-----------------------------------------------------------------------------------
ofCoreEvents & ofAppiOSWindow::events(){
    return coreEvents;
}

//-----------------------------------------------------------------------------------
std::shared_ptr<ofBaseRenderer> & ofAppiOSWindow::renderer(){
    return currentRenderer;
}

ofiOSWindowSettings & ofAppiOSWindow::getSettings() {
    return settings;
}
