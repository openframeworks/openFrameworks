//
//  ofxiOSEAGLView.m
//  iOS+OFLib
//
//  Created by lukasz karluk on 5/07/12.
//

#import "ofxiOSEAGLView.h"

#import "ofMain.h"
#import "ofAppiOSWindow.h"
#import "ofGLProgrammableRenderer.h"
#import "ofxiOSApp.h"
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

- (id)initWithFrame:(CGRect)frame andApp:(ofxiOSApp *)appPtr {
    
    ESRendererVersion preferedRendererVersion = ESRendererVersion_11;
    if(ofIsGLProgrammableRenderer()) {
        preferedRendererVersion = ESRendererVersion_20;
    }
    
    self = [self initWithFrame:frame
           andPreferedRenderer:preferedRendererVersion
                      andDepth:ofAppiOSWindow::getInstance()->isDepthBufferEnabled()
                         andAA:ofAppiOSWindow::getInstance()->isAntiAliasingEnabled()
                 andNumSamples:ofAppiOSWindow::getInstance()->getAntiAliasingSampleCount()
                     andRetina:ofAppiOSWindow::getInstance()->isRetinaEnabled()
                andRetinaScale:ofAppiOSWindow::getInstance()->getRetinaScale()];
    
    if(self) {
        
        _instanceRef = self;
        
        if(rendererVersion == ESRendererVersion_20) {
            if(ofAppiOSWindow::getInstance()->isRendererES2() == false) {
                ofSetCurrentRenderer(shared_ptr<ofBaseRenderer>(new ofGLProgrammableRenderer(false)));
            }
            ofGetGLProgrammableRenderer()->setup("120");
        } else if(rendererVersion == ESRendererVersion_11) {
            if(ofAppiOSWindow::getInstance()->isRendererES1() == false) {
                ofSetCurrentRenderer(shared_ptr<ofBaseRenderer>(new ofGLRenderer(false)));
            }
        }
        
        app = appPtr;
        activeTouches = [[NSMutableDictionary alloc] init];
                
        screenSize = new ofVec3f();
        windowSize = new ofVec3f();
        windowPos = new ofVec3f();
        [self updateDimensions];
        
        if(app != ofGetAppPtr()) {              // check if already running.
            ofRunApp(shared_ptr<ofBaseApp>(app));    // this case occurs when app is created in main().
        }
        ofRegisterTouchEvents(app);
        ofxiOSAlerts.addListener(app);

        ofDisableTextureEdgeHack();

        ofGLReadyCallback();
        
        bInit = YES;
    }
    
    return self;
}

- (void)setup {
    
    ofNotifySetup();
    ofGetCurrentRenderer()->clear();
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
    ofxiOSAlerts.removeListener(app);
    ofSetAppPtr(shared_ptr<ofBaseApp>((app = NULL)));
    
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
    ofNotifyWindowResized(ofGetWidth(), ofGetHeight());
}

- (void)updateDimensions {
    windowPos->set(self.frame.origin.x * scaleFactor, self.frame.origin.y * scaleFactor, 0);
    windowSize->set(self.bounds.size.width * scaleFactor, self.bounds.size.height * scaleFactor, 0);
    
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
    
    ofGetCurrentRenderer()->startRender();
    
    if(ofAppiOSWindow::getInstance()->isSetupScreenEnabled()) {
        ofSetupScreen();
    }
    
    //------------------------------------------ draw.
    
    ofNotifyDraw();
    
    //------------------------------------------
    
    ofGetCurrentRenderer()->finishRender();
    
    [self finishRender];
    [self unlockGL];
    
    [super notifyDraw];   // alerts delegate that a new frame has been drawn.
}

- (void)notifyDraw {
    // blank this.
    // we want to notifyDraw at the end of drawView.
}

//------------------------------------------------------
- (CGPoint)orientateTouchPoint:(CGPoint)touchPoint {
    
    if(ofAppiOSWindow::getInstance()->doesHWOrientation()) {
        return touchPoint;
    }
    
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



@end
