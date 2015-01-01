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
    
    window = ofAppiOSWindow::getInstance();
    if(window == NULL) {
        ofLog(OF_LOG_FATAL_ERROR, "ofxiOSEAGLView::initWithFrame - window is NULL");
        return nil;
    }
    ESRendererVersion preferedRendererVersion = (ESRendererVersion)window->getSettings().glesVersion;
    
    self = [self initWithFrame:frame
           andPreferedRenderer:preferedRendererVersion
                      andDepth:window->isDepthBufferEnabled()
                         andAA:window->isAntiAliasingEnabled()
                 andNumSamples:window->getAntiAliasingSampleCount()
                     andRetina:window->isRetinaEnabled()
                andRetinaScale:window->getRetinaScale()];
    
    if(self) {
        
        _instanceRef = self;
        
        app = appPtr;
        activeTouches = [[NSMutableDictionary alloc] init];
                
        screenSize = new ofVec3f();
        windowSize = new ofVec3f();
        windowPos = new ofVec3f();
        [self updateDimensions];
        
        if(window->isProgrammableRenderer()){
            static_cast<ofGLProgrammableRenderer*>(window->renderer().get())->setup(window->getSettings().glesVersion, 0);
        } else{
            static_cast<ofGLRenderer*>(window->renderer().get())->setup();
        }
        
        if(app != ofGetAppPtr()) {              // check if already running.
            ofRunApp(shared_ptr<ofBaseApp>(app));    // this fallback only case occurs when app not created in main().
        }
        ofRegisterTouchEvents(app);
        ofxiOSAlerts.addListener(app);

        ofDisableTextureEdgeHack();
        
        bInit = YES;
    }
    
    return self;
}

- (void)setup {
    if(window != NULL){
        window->events().notifySetup();
        window->renderer()->clear();
    } else {
        ofLog(OF_LOG_FATAL_ERROR, "ofxiOSEAGLView setup. Failed setup. window is NULL");
    }
}

- (void)destroy {
    if(!bInit) {
        return;
    }
    
    window->events().notifyExit();
    
    [activeTouches release];
    
    delete screenSize;
    screenSize = NULL;
    delete windowSize;
    windowSize = NULL;
    delete windowPos;
    windowPos = NULL;
    
    ofBaseApp * baseAppPtr = ofGetAppPtr();
    ofRemoveListener(window->events().setup,          baseAppPtr, &ofBaseApp::setup,OF_EVENT_ORDER_APP);
    ofRemoveListener(window->events().update,         baseAppPtr, &ofBaseApp::update,OF_EVENT_ORDER_APP);
    ofRemoveListener(window->events().draw,           baseAppPtr, &ofBaseApp::draw,OF_EVENT_ORDER_APP);
    ofRemoveListener(window->events().exit,           baseAppPtr, &ofBaseApp::exit,OF_EVENT_ORDER_APP);
    ofRemoveListener(window->events().keyPressed,     baseAppPtr, &ofBaseApp::keyPressed,OF_EVENT_ORDER_APP);
    ofRemoveListener(window->events().keyReleased,    baseAppPtr, &ofBaseApp::keyReleased,OF_EVENT_ORDER_APP);
    ofRemoveListener(window->events().mouseMoved,     baseAppPtr, &ofBaseApp::mouseMoved,OF_EVENT_ORDER_APP);
    ofRemoveListener(window->events().mouseDragged,   baseAppPtr, &ofBaseApp::mouseDragged,OF_EVENT_ORDER_APP);
    ofRemoveListener(window->events().mousePressed,   baseAppPtr, &ofBaseApp::mousePressed,OF_EVENT_ORDER_APP);
    ofRemoveListener(window->events().mouseReleased,  baseAppPtr, &ofBaseApp::mouseReleased,OF_EVENT_ORDER_APP);
    ofRemoveListener(window->events().windowResized,  baseAppPtr, &ofBaseApp::windowResized,OF_EVENT_ORDER_APP);
    ofRemoveListener(window->events().windowEntered,  baseAppPtr, &ofBaseApp::windowEntry,OF_EVENT_ORDER_APP);
    ofRemoveListener(window->events().messageEvent,   baseAppPtr, &ofBaseApp::messageReceived,OF_EVENT_ORDER_APP);
    ofRemoveListener(window->events().fileDragEvent,  baseAppPtr, &ofBaseApp::dragged,OF_EVENT_ORDER_APP);
    
    ofUnregisterTouchEvents(app);
    ofxiOSAlerts.removeListener(app);
    ofSetAppPtr(shared_ptr<ofBaseApp>((app = NULL)));
    
    window = NULL;
    
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
    window->events().notifyWindowResized(ofGetWidth(), ofGetHeight());
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
    window->events().notifyUpdate();

    //------------------------------------------
    
    [self lockGL];
    [self startRender];
    
    window->renderer()->startRender();
    
    if(window->isSetupScreenEnabled()) {
        window->renderer()->setupScreen();
    }
    
    //------------------------------------------ draw.
    
    window->events().notifyDraw();
    
    //------------------------------------------
    
    window->renderer()->finishRender();
    
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
            window->events().notifyMousePressed(touchPoint.x, touchPoint.y, 0);
		}
		
		ofTouchEventArgs touchArgs;
        touchArgs.numTouches = [[event touchesForView:self] count];
		touchArgs.x = touchPoint.x;
		touchArgs.y = touchPoint.y;
		touchArgs.id = touchIndex;
        if([touch tapCount] == 2){
            ofNotifyEvent(window->events().touchDoubleTap,touchArgs);	// send doubletap
        }
		ofNotifyEvent(window->events().touchDown,touchArgs);	// but also send tap (upto app programmer to ignore this if doubletap came that frame)
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
            window->events().notifyMouseDragged(touchPoint.x, touchPoint.y, 0);
		}		
		ofTouchEventArgs touchArgs;
		touchArgs.numTouches = [[event touchesForView:self] count];
		touchArgs.x = touchPoint.x;
		touchArgs.y = touchPoint.y;
		touchArgs.id = touchIndex;
		ofNotifyEvent(window->events().touchMoved, touchArgs);
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
			window->events().notifyMouseReleased(touchPoint.x, touchPoint.y, 0);
		}
		
		ofTouchEventArgs touchArgs;
		touchArgs.numTouches = [[event touchesForView:self] count] - [touches count];
		touchArgs.x = touchPoint.x;
		touchArgs.y = touchPoint.y;
		touchArgs.id = touchIndex;
		ofNotifyEvent(window->events().touchUp, touchArgs);
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
		ofNotifyEvent(window->events().touchCancelled, touchArgs);
	}
	
	[self touchesEnded:touches withEvent:event];
}



@end
