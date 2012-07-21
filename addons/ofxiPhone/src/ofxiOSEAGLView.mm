//
//  ofxiOSEAGLView.m
//  iOS+OFLib
//
//  Created by lukasz karluk on 5/07/12.
//

#import "ofxiOSEAGLView.h"

#import "ofMain.h"
#import "ofAppiPhoneWindow.h"
#import "ofxiPhoneApp.h"
#import "ofxiOSExtensions.h"

static ofxiOSEAGLView * _instanceRef = nil;

@interface ofxiOSEAGLView() {
    BOOL bInit;
}
- (void)updateDimensions;
@end

@implementation ofxiOSEAGLView

@synthesize lastFrameTime;
@synthesize nFrameCount;
@synthesize frameRate;
@synthesize screenSize;
@synthesize windowSize;
@synthesize windowPos;

+ (ofxiOSEAGLView *) getInstance {
    return _instanceRef;
}

- (id)initWithFrame:(CGRect)frame andApp:(ofxiPhoneApp *)appPtr {
    self = [self initWithFrame:frame 
                      andDepth:ofAppiPhoneWindow::getInstance()->isDepthEnabled()
                         andAA:ofAppiPhoneWindow::getInstance()->isAntiAliasingEnabled()
                 andNumSamples:ofAppiPhoneWindow::getInstance()->getAntiAliasingSampleCount()
                     andRetina:ofAppiPhoneWindow::getInstance()->isRetinaSupported()];
    
    if(self) {
        
        _instanceRef = self;
        
        app = appPtr;
        activeTouches = [[NSMutableDictionary alloc] init];
        
        nFrameCount = 0;
        lastFrameTime = 0;
        fps = frameRate = 60.0f;
        timeNow = 0.0;
        timeThen = 0.0;
        
        screenSize = new ofVec3f();
        windowSize = new ofVec3f();
        windowPos = new ofVec3f();
        [self updateDimensions];
        
        if(app != ofGetAppPtr()) {              // check if already running.
            ofRunApp(ofPtr<ofBaseApp>(app));    // this case occurs when app is created in main().
        }
        ofRegisterTouchEvents(app);
        ofxiPhoneAlerts.addListener(app);
        
        ofUpdateBitmapCharacterTexture();
        
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
    
    [activeTouches release];
    
    delete screenSize;
    screenSize = NULL;
    delete windowSize;
    windowSize = NULL;
    delete windowPos;
    windowPos = NULL;
    
    ofUnregisterTouchEvents(app);
    ofxiPhoneAlerts.removeListener(app);
    app->exit();
    ofSetAppPtr(ofPtr<ofBaseApp>((app=NULL)));
    
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

- (void) drawView {
    
    ofNotifyUpdate();
    
    //------------------------------------------
    
    [self lockGL];
    [self startRender];

    glViewport(0, 0, windowSize->x, windowSize->y);
    
    float * bgPtr = ofBgColorPtr();
    bool bClearAuto = ofbClearBg();
    if ( bClearAuto == true){
        glClearColor(bgPtr[0],bgPtr[1],bgPtr[2], bgPtr[3]);
        glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }
    
    if(ofAppiPhoneWindow::getInstance()->isSetupScreenEnabled()) {
        ofSetupScreen();
    }
    
    //------------------------------------------ draw.
    
    ofNotifyDraw();
    
    //------------------------------------------
    
    [self finishRender];
    [self unlockGL];
    
    //------------------------------------------
    
    timeNow = ofGetElapsedTimef();
    double diff = timeNow-timeThen;
    if( diff  > 0.00001 ){
        fps			= 1.0 / diff;
        frameRate	*= 0.9f;
        frameRate	+= 0.1f*fps;
    }
    lastFrameTime	= diff;
    timeThen		= timeNow;
    
    nFrameCount++;
    
    //------------------------------------------ 
    
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
		
		ofAppiPhoneWindow::getInstance()->rotateXY(touchPoint.x, touchPoint.y);
		
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
		
		ofAppiPhoneWindow::getInstance()->rotateXY(touchPoint.x, touchPoint.y);
		
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
		
		ofAppiPhoneWindow::getInstance()->rotateXY(touchPoint.x, touchPoint.y);
		
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
		
		ofAppiPhoneWindow::getInstance()->rotateXY(touchPoint.x, touchPoint.y);
		
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
