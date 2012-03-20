//
//  ofxiPhoneViewController.m
//  Created by lukasz karluk on 12/12/11.
//

#import "ofxiPhoneViewController.h"
#import "ofxiPhoneExtras.h"

@implementation ofxiPhoneViewController

@synthesize glView;
@synthesize glLock;
@synthesize animTimer;
@synthesize animFrameInterval;
@synthesize animating;
@synthesize displayLinkSupported;
@synthesize displayLink;

/////////////////////////////////////////////////
//  INIT.
/////////////////////////////////////////////////

- (id) initWithFrame:(CGRect)frame 
                 app:(ofBaseApp*)app {
    if ((self=[super init])) {

        ofxiPhoneGetAppDelegate().glViewController = self;
        
        [ self initApp:app ];
        [ self initGLLock ];
        [ self initGLViewWithFrame:frame ];
        [ self initAnimationVars ];
        [ self setupApp ];
        [ self clearBuffers ];
        [ self reloadTextures ];
        [ self startAnimation ];
    }
    
    return self;
}

- (void) initApp:(ofBaseApp*)app {
    if (ofGetAppPtr()==app)  // app already running.
        return;
    
    ofPtr<ofBaseApp> appPtr;
    appPtr = ofPtr<ofBaseApp>(app);
    ofRunApp(appPtr);
}

- (void) initGLLock {
    self.glLock = [[[NSLock alloc] init] autorelease];
}

- (void) initGLViewWithFrame : (CGRect)frame {
    self.glView = [[[EAGLView alloc] initWithFrame:frame
                                          andDepth:iPhoneGetOFWindow()->isDepthEnabled()
                                             andAA:iPhoneGetOFWindow()->isAntiAliasingEnabled() 
                                     andNumSamples:iPhoneGetOFWindow()->getAntiAliasingSampleCount() 
                                         andRetina:iPhoneGetOFWindow()->isRetinaSupported()] autorelease];
    [self.view insertSubview:self.glView atIndex:0];
}

- (void) initAnimationVars {
    self.animating = NO;
    self.displayLinkSupported = NO;
    self.animFrameInterval = 1;
    self.displayLink = nil;
    self.animTimer = nil;
}

- (void) setupApp {
    ofRegisterTouchEvents((ofxiPhoneApp*)ofGetAppPtr());
    ofGetAppPtr()->setup();
    
#ifdef OF_USING_POCO
    static ofEventArgs voidEventArgs;
    ofNotifyEvent(ofEvents().setup, voidEventArgs);
    ofNotifyEvent(ofEvents().update, voidEventArgs);
#endif
}

- (void) clearBuffers {
    glClearColor(ofBgColorPtr()[0], ofBgColorPtr()[1], ofBgColorPtr()[2], ofBgColorPtr()[3]);   //-- clear background
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

- (void) reloadTextures {
    ofUpdateBitmapCharacterTexture();
}

/////////////////////////////////////////////////
//  DEALLOC.
/////////////////////////////////////////////////

- (void) dealloc {
    [self destroy];     //-- destroy in case it hasn't already been done.
    [super dealloc];
}

- (void) destroy {
    //------------------------------------------------------------------- remove controller reference.
    ofxiPhoneGetAppDelegate().glViewController = nil;
    
    //------------------------------------------------------------------- destroy app.
    ofBaseApp *app;
    app = ofGetAppPtr();
    
    if (app)
        app->exit();
    
    ofSetAppPtr(ofPtr<ofBaseApp>((app=NULL)));
    
    //------------------------------------------------------------------- stop animation.
    [self stopAnimation];
    
    //------------------------------------------------------------------- destroy glview.
    [self.glView removeFromSuperview];
    self.glView = nil;
    
    //------------------------------------------------------------------- the rest.
    self.glLock = nil;
}

/////////////////////////////////////////////////
//  UI VIEW CONTROLLER.
/////////////////////////////////////////////////

- (void) viewWillDisappear:(BOOL)animated {
    [self stopAnimation];
    ofxiPhoneGetAppDelegate().glViewController = nil;
    [super viewWillDisappear:animated];
}

/////////////////////////////////////////////////
//  LOCK / UNLOCK.
/////////////////////////////////////////////////

-(void)lockGL {
	[self.glLock lock];
}

-(void)unlockGL {
	[self.glLock unlock];
}

/////////////////////////////////////////////////
//  ANIMATION TIMER.
/////////////////////////////////////////////////

- (void) timerLoop {
    iPhoneGetOFWindow()->timerLoop();
}

- (void)startAnimation {
    if (!self.animating) {
        if (self.displayLinkSupported) {
            // CADisplayLink is API new to iPhone SDK 3.1. Compiling against earlier versions will result in a warning, but can be dismissed
            // if the system version runtime check for CADisplayLink exists in -initWithCoder:. The runtime check ensures this code will
            // not be called in system versions earlier than 3.1.
			
            self.displayLink = [NSClassFromString(@"CADisplayLink") displayLinkWithTarget:self selector:@selector(timerLoop)];
            [self.displayLink setFrameInterval:self.animFrameInterval];
            [self.displayLink addToRunLoop:[NSRunLoop currentRunLoop] forMode:NSDefaultRunLoopMode];
			ofLog(OF_LOG_VERBOSE, "CADisplayLink supported, running with interval: %i", self.animFrameInterval);
        }
        else {
			ofLog(OF_LOG_VERBOSE, "CADisplayLink not supported, running with interval: %i", self.animFrameInterval);
            self.animTimer = [NSTimer scheduledTimerWithTimeInterval:(NSTimeInterval)((1.0/60.0)*self.animFrameInterval)
                                                              target:self 
                                                            selector:@selector(timerLoop)
                                                            userInfo:nil 
                                                             repeats:TRUE ];
		}
		
        self.animating = TRUE;
    }
}

- (void)stopAnimation {
    if (self.animating) {
        if (self.displayLinkSupported) {
            [self.displayLink invalidate];
            self.displayLink = nil;
        }
        else {
            [self.animTimer invalidate];
            self.animTimer = nil;
		}
		
        self.animating = FALSE;
    }
}

- (void)setAnimationFrameInterval:(float)frameInterval {
    // Frame interval defines how many display frames must pass between each time the
    // display link fires. The display link will only fire 30 times a second when the
    // frame internal is two on a display that refreshes 60 times a second. The default
    // frame interval setting of one will fire 60 times a second when the display refreshes
    // at 60 times a second. A frame interval setting of less than one results in undefined
    // behavior.
    if (frameInterval >= 1) {
        self.animFrameInterval = frameInterval;
		
        if (self.animating) {
            [self stopAnimation];
            [self startAnimation];
        }
    }
}

-(void) setFrameRate:(float)rate {
	ofLog(OF_LOG_VERBOSE, "setFrameRate %.3f using NSTimer", rate);
	
	if(rate>0) [self setAnimationFrameInterval:60.0/rate];
	else [self stopAnimation];
}

@end
