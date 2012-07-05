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

@implementation ofxiOSEAGLView

@synthesize lastFrameTime;
@synthesize nFrameCount;
@synthesize frameRate;

+ (ofxiOSEAGLView *) getInstance {
    return _instanceRef;
}

- (id)initWithFrame:(CGRect)frame andApp:(ofBaseApp *)appPtr {
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
        
        if(app != ofGetAppPtr()) {              // check if already running.
            ofRunApp(ofPtr<ofBaseApp>(app));    // this case occurs when app is created in main().
        }
        ofRegisterTouchEvents((ofxiPhoneApp *)app);
        ofxiPhoneAlerts.addListener((ofxiPhoneApp *)app);
        
        ofUpdateBitmapCharacterTexture();
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

- (void)dealloc {
    [activeTouches release];
    
    ofUnregisterTouchEvents((ofxiPhoneApp *)app);
    ofxiPhoneAlerts.removeListener((ofxiPhoneApp *)app);
    app->exit();
    ofSetAppPtr(ofPtr<ofBaseApp>((app=NULL)));
    
    _instanceRef = nil;
    
    [super dealloc];
}

- (void) drawView {
    
    ofNotifyUpdate();
    
    //------------------------------------------
    
    [self lockGL];
    [self startRender];
    
    //we do this as ofGetWidth() now accounts for rotation 
    //so we just make our viewport across the whole screen
    glViewport(0, 0, self.frame.size.width, self.frame.size.height);
    
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
}

//------------------------------------------------------
- (void)touchesBegan:(NSSet *)touches 
           withEvent:(UIEvent *)event{
	
	for(UITouch *touch in touches) {
		int touchIndex = 0;
		while([[activeTouches allValues] containsObject:[NSNumber numberWithInt:touchIndex]]){
			touchIndex++;
		}
		
		[activeTouches setObject:[NSNumber numberWithInt:touchIndex] forKey:[NSValue valueWithPointer:touch]];
		
		CGPoint touchPoint = [touch locationInView:self];
		
		touchPoint.x*=touchScaleFactor; // this has to be done because retina still returns points in 320x240 but with high percision
		touchPoint.y*=touchScaleFactor;
		
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
	
	for(UITouch *touch in touches){
		int touchIndex = [[activeTouches objectForKey:[NSValue valueWithPointer:touch]] intValue];
		
		CGPoint touchPoint = [touch locationInView:self];
		
		touchPoint.x*=touchScaleFactor; // this has to be done because retina still returns points in 320x240 but with high percision
		touchPoint.y*=touchScaleFactor;
		
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
    
	for(UITouch *touch in touches){
		int touchIndex = [[activeTouches objectForKey:[NSValue valueWithPointer:touch]] intValue];
		
		[activeTouches removeObjectForKey:[NSValue valueWithPointer:touch]];
		
		CGPoint touchPoint = [touch locationInView:self];
		
		touchPoint.x*=touchScaleFactor; // this has to be done because retina still returns points in 320x240 but with high percision
		touchPoint.y*=touchScaleFactor;
		
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
	
	for(UITouch *touch in touches){
		int touchIndex = [[activeTouches objectForKey:[NSValue valueWithPointer:touch]] intValue];
		
		CGPoint touchPoint = [touch locationInView:self];
		
		touchPoint.x*=touchScaleFactor; // this has to be done because retina still returns points in 320x240 but with high percision
		touchPoint.y*=touchScaleFactor;
		
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
