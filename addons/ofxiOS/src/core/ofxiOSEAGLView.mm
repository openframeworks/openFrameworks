//
//  ofxiOSEAGLView.m
//  iOS+OFLib
//
//  Created by lukasz karluk on 5/07/12.
//

#import "ofxiOSEAGLView.h"

#import "ofMain.h"
#import "ofAppiPhoneWindow.h"
#import "ofGLProgrammableRenderer.h"
#import "ofxiPhoneApp.h"
#import "ofxiOSExtensions.h"

static ofxiOSEAGLView * _instanceRef = nil;

@interface ofxiOSEAGLView() {
    BOOL bInit;
}
- (void)updateDimensions;
@end

@implementation ofxiOSEAGLView

@synthesize screenSize;
@synthesize windowSize;
@synthesize windowPos;

+ (ofxiOSEAGLView *) getInstance {
    return _instanceRef;
}

- (id)initWithFrame:(CGRect)frame andApp:(ofxiPhoneApp *)appPtr {
    
    ESRendererVersion preferedRendererVersion = ESRendererVersion_11;
    if(ofIsGLProgrammableRenderer()) {
        preferedRendererVersion = ESRendererVersion_20;
    }
    
    self = [self initWithFrame:frame
           andPreferedRenderer:preferedRendererVersion
                      andDepth:ofAppiPhoneWindow::getInstance()->isDepthBufferEnabled()
                         andAA:ofAppiPhoneWindow::getInstance()->isAntiAliasingEnabled()
                 andNumSamples:ofAppiPhoneWindow::getInstance()->getAntiAliasingSampleCount()
                     andRetina:ofAppiPhoneWindow::getInstance()->isRetinaEnabled()];
    
    if(self) {
        
        _instanceRef = self;
        
        if(rendererVersion == ESRendererVersion_20) {
            if(ofAppiPhoneWindow::getInstance()->isRendererES2() == false) {
                ofSetCurrentRenderer(ofPtr<ofBaseRenderer>(new ofGLProgrammableRenderer(false)));
            }
            ((ofGLProgrammableRenderer *)ofGetCurrentRenderer().get())->setup();
        } else if(rendererVersion == ESRendererVersion_11) {
            if(ofAppiPhoneWindow::getInstance()->isRendererES1() == false) {
                ofSetCurrentRenderer(ofPtr<ofBaseRenderer>(new ofGLRenderer(false)));
            }
        }
        
        app = appPtr;
        activeTouches = [[NSMutableDictionary alloc] init];
                
        screenSize = new ofVec3f();
        windowSize = new ofVec3f();
        windowPos = new ofVec3f();
        [self updateDimensions];
        
        if(app != ofGetAppPtr()) {              // check if already running.
            ofRunApp(ofPtr<ofBaseApp>(app));    // this case occurs when app is created in main().
        }
        ofRegisterTouchEvents(app);
        ofxiPhoneAlerts.addListener(app);

        ofDisableTextureEdgeHack();

        ofGLReadyCallback();
        ofReloadGLResources();
        
        bInit = YES;
    }
    
    return self;
}

- (void)setup {
    
    ofNotifySetup();
    
    glClearColor(ofBgColorPtr()[0], 
                 ofBgColorPtr()[1], 
                 ofBgColorPtr()[2], 
                 ofBgColorPtr()[3]); // clear background.
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

- (void)destroy {
    if(!bInit) {
        return;
    }
    
    ofNotifyExit();
    
    [activeTouches release];
    
    delete screenSize;
    screenSize = NULL;
    delete windowSize;
    windowSize = NULL;
    delete windowPos;
    windowPos = NULL;
    
    ofBaseApp * baseAppPtr = ofGetAppPtr();
    ofRemoveListener(ofEvents().setup,          baseAppPtr, &ofBaseApp::setup,OF_EVENT_ORDER_APP);
    ofRemoveListener(ofEvents().update,         baseAppPtr, &ofBaseApp::update,OF_EVENT_ORDER_APP);
    ofRemoveListener(ofEvents().draw,           baseAppPtr, &ofBaseApp::draw,OF_EVENT_ORDER_APP);
    ofRemoveListener(ofEvents().exit,           baseAppPtr, &ofBaseApp::exit,OF_EVENT_ORDER_APP);
    ofRemoveListener(ofEvents().keyPressed,     baseAppPtr, &ofBaseApp::keyPressed,OF_EVENT_ORDER_APP);
    ofRemoveListener(ofEvents().keyReleased,    baseAppPtr, &ofBaseApp::keyReleased,OF_EVENT_ORDER_APP);
    ofRemoveListener(ofEvents().mouseMoved,     baseAppPtr, &ofBaseApp::mouseMoved,OF_EVENT_ORDER_APP);
    ofRemoveListener(ofEvents().mouseDragged,   baseAppPtr, &ofBaseApp::mouseDragged,OF_EVENT_ORDER_APP);
    ofRemoveListener(ofEvents().mousePressed,   baseAppPtr, &ofBaseApp::mousePressed,OF_EVENT_ORDER_APP);
    ofRemoveListener(ofEvents().mouseReleased,  baseAppPtr, &ofBaseApp::mouseReleased,OF_EVENT_ORDER_APP);
    ofRemoveListener(ofEvents().windowResized,  baseAppPtr, &ofBaseApp::windowResized,OF_EVENT_ORDER_APP);
    ofRemoveListener(ofEvents().windowEntered,  baseAppPtr, &ofBaseApp::windowEntry,OF_EVENT_ORDER_APP);
    ofRemoveListener(ofEvents().messageEvent,   baseAppPtr, &ofBaseApp::messageReceived,OF_EVENT_ORDER_APP);
    ofRemoveListener(ofEvents().fileDragEvent,  baseAppPtr, &ofBaseApp::dragged,OF_EVENT_ORDER_APP);
    
    ofUnregisterTouchEvents(app);
    ofxiPhoneAlerts.removeListener(app);
    ofSetAppPtr(ofPtr<ofBaseApp>((app = NULL)));
    
    _instanceRef = nil;
    
    bInit = NO;
    
    [super destroy];
}

- (void)dealloc {
    [self destroy];
    [super dealloc];
}

- (void)layoutSubviews {
    [super layoutSubviews];
    [self updateDimensions];
    [super notifyResized];
}

- (void)updateDimensions {
    windowPos->set(self.frame.origin.x * scaleFactor, self.frame.origin.y * scaleFactor, 0);
    windowSize->set(self.frame.size.width * scaleFactor, self.frame.size.height * scaleFactor, 0);
    
    UIScreen * currentScreen = self.window.screen;  // current screen is the screen that GLView is attached to.
    if(!currentScreen) {                            // if GLView is not attached, assume to be main device screen.
        currentScreen = [UIScreen mainScreen];
    }
    screenSize->set(currentScreen.bounds.size.width * scaleFactor, currentScreen.bounds.size.height * scaleFactor, 0);
}

- (void)notifyResized {
    // blank this.
    // we want to notifyResized at the end of layoutSubviews.
}

- (void)drawView {
    
    ofNotifyUpdate();
    
    //------------------------------------------
    
    [self lockGL];
    [self startRender];
    
    ofGLProgrammableRenderer * es2Renderer = NULL;
    if(ofIsGLProgrammableRenderer()) {
        es2Renderer = (ofGLProgrammableRenderer *)(ofGetCurrentRenderer().get());
        es2Renderer->startRender();
    }

    ofViewport(ofRectangle(0, 0, ofGetWidth(), ofGetHeight()));
    
    float * bgPtr = ofBgColorPtr();
    bool bClearAuto = ofbClearBg();
    if(bClearAuto == true) {
        glClearColor(bgPtr[0], bgPtr[1], bgPtr[2], bgPtr[3]);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }
    
    if(ofAppiPhoneWindow::getInstance()->isSetupScreenEnabled()) {
        ofSetupScreen();
    }
    
    //------------------------------------------ draw.
    
    ofNotifyDraw();
    
    //------------------------------------------
    
    if(es2Renderer != NULL) {
        es2Renderer->finishRender();
    }
    
    [self finishRender];
    [self unlockGL];
    
    [super notifyDraw];   // alerts delegate that a new frame has been drawn.
}

- (void)notifyDraw {
    // blank this.
    // we want to notifyDraw at the end of drawView.
}

//------------------------------------------------------
- (void)touchesBegan:(NSSet *)touches 
           withEvent:(UIEvent *)event{
    
    if(!bInit) {
        // if the glView is destroyed which also includes the OF app,
        // we no longer need to pass on these touch events.
        return; 
    }
	
	for(UITouch *touch in touches) {
		int touchIndex = 0;
		while([[activeTouches allValues] containsObject:[NSNumber numberWithInt:touchIndex]]){
			touchIndex++;
		}
		
		[activeTouches setObject:[NSNumber numberWithInt:touchIndex] forKey:[NSValue valueWithPointer:touch]];
		
		CGPoint touchPoint = [touch locationInView:self];
		
		touchPoint.x *= scaleFactor; // this has to be done because retina still returns points in 320x240 but with high percision
		touchPoint.y *= scaleFactor;
		touchPoint = [self orientateTouchPoint:touchPoint];
		
		if( touchIndex==0 ){
			ofNotifyMousePressed(touchPoint.x, touchPoint.y, 0);
		}
		
		ofTouchEventArgs touchArgs;
        touchArgs.numTouches = [[event touchesForView:self] count];
		touchArgs.x = touchPoint.x;
		touchArgs.y = touchPoint.y;
		touchArgs.id = touchIndex;
		if([touch tapCount] == 2) ofNotifyEvent(ofEvents().touchDoubleTap,touchArgs);	// send doubletap
		ofNotifyEvent(ofEvents().touchDown,touchArgs);	// but also send tap (upto app programmer to ignore this if doubletap came that frame)
	}	
}

//------------------------------------------------------
- (void)touchesMoved:(NSSet *)touches 
           withEvent:(UIEvent *)event{
    
    if(!bInit) {
        // if the glView is destroyed which also includes the OF app,
        // we no longer need to pass on these touch events.
        return; 
    }
	
	for(UITouch *touch in touches){
		int touchIndex = [[activeTouches objectForKey:[NSValue valueWithPointer:touch]] intValue];
		
		CGPoint touchPoint = [touch locationInView:self];
		
		touchPoint.x *= scaleFactor; // this has to be done because retina still returns points in 320x240 but with high percision
		touchPoint.y *= scaleFactor;
		touchPoint = [self orientateTouchPoint:touchPoint];
		
		if( touchIndex==0 ){
			ofNotifyMouseDragged(touchPoint.x, touchPoint.y, 0);			
		}		
		ofTouchEventArgs touchArgs;
		touchArgs.numTouches = [[event touchesForView:self] count];
		touchArgs.x = touchPoint.x;
		touchArgs.y = touchPoint.y;
		touchArgs.id = touchIndex;
		ofNotifyEvent(ofEvents().touchMoved, touchArgs);
	}
}

//------------------------------------------------------
- (void)touchesEnded:(NSSet *)touches 
           withEvent:(UIEvent *)event{
    
    if(!bInit) {
        // if the glView is destroyed which also includes the OF app,
        // we no longer need to pass on these touch events.
        return; 
    }
    
	for(UITouch *touch in touches){
		int touchIndex = [[activeTouches objectForKey:[NSValue valueWithPointer:touch]] intValue];
		
		[activeTouches removeObjectForKey:[NSValue valueWithPointer:touch]];
		
		CGPoint touchPoint = [touch locationInView:self];
		
		touchPoint.x *= scaleFactor; // this has to be done because retina still returns points in 320x240 but with high percision
		touchPoint.y *= scaleFactor;
		touchPoint = [self orientateTouchPoint:touchPoint];
		
		if( touchIndex==0 ){
			ofNotifyMouseReleased(touchPoint.x, touchPoint.y, 0);						
		}
		
		ofTouchEventArgs touchArgs;
		touchArgs.numTouches = [[event touchesForView:self] count] - [touches count];
		touchArgs.x = touchPoint.x;
		touchArgs.y = touchPoint.y;
		touchArgs.id = touchIndex;
		ofNotifyEvent(ofEvents().touchUp, touchArgs);
	}
}

//------------------------------------------------------
- (void)touchesCancelled:(NSSet *)touches 
               withEvent:(UIEvent *)event{
    
    if(!bInit) {
        // if the glView is destroyed which also includes the OF app,
        // we no longer need to pass on these touch events.
        return; 
    }
	
	for(UITouch *touch in touches){
		int touchIndex = [[activeTouches objectForKey:[NSValue valueWithPointer:touch]] intValue];
		
		CGPoint touchPoint = [touch locationInView:self];
		
		touchPoint.x *= scaleFactor; // this has to be done because retina still returns points in 320x240 but with high percision
		touchPoint.y *= scaleFactor;
		touchPoint = [self orientateTouchPoint:touchPoint];
		
		ofTouchEventArgs touchArgs;
		touchArgs.numTouches = [[event touchesForView:self] count];
		touchArgs.x = touchPoint.x;
		touchArgs.y = touchPoint.y;
		touchArgs.id = touchIndex;
		ofNotifyEvent(ofEvents().touchCancelled, touchArgs);
	}
	
	[self touchesEnded:touches withEvent:event];
}

//------------------------------------------------------
- (CGPoint)orientateTouchPoint:(CGPoint)touchPoint {
    
    ofOrientation orientation = ofGetOrientation();
    CGPoint touchPointOriented = CGPointZero;
    
	switch(orientation) {
		case OF_ORIENTATION_180:
			touchPointOriented.x = ofGetWidth() - touchPoint.x;
			touchPointOriented.y = ofGetHeight() - touchPoint.y;
			break;
			
		case OF_ORIENTATION_90_LEFT:
			touchPointOriented.x = touchPoint.y;
			touchPointOriented.y = ofGetHeight() - touchPoint.x;
			break;
			
		case OF_ORIENTATION_90_RIGHT:
			touchPointOriented.x = ofGetWidth() - touchPoint.y;
			touchPointOriented.y = touchPoint.x;
			break;
			
		case OF_ORIENTATION_DEFAULT:
		default:
            touchPointOriented = touchPoint;
			break;
	}
    return touchPointOriented;
}

@end
