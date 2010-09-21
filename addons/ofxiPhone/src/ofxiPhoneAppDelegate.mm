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

//#import <mach/mach_time.h>

#import "ofxiPhoneAppDelegate.h"
#import "ofMain.h"
#import "ofxiPhone.h"
#import "ofxiPhoneExtras.h"

@implementation ofxiPhoneAppDelegate




-(void) timerLoop {
//	NSLog(@"ofxiPhoneAppDelegate::timerLoop");
	
	// create autorelease pool in case anything needs it
//	NSAutoreleasePool * pool = [[NSAutoreleasePool alloc] init];
	
	iPhoneGetOFWindow()->timerLoop();
		
	// release pool
//	[pool release];
}

-(EAGLView*) getGLView {
	return glView;
}

-(void)lockGL {
	[glLock lock];
}

-(void)unlockGL {
	[glLock unlock];
}



-(void) applicationDidFinishLaunching:(UIApplication *)application {    
	static ofEventArgs voidEventArgs;
	ofLog(OF_LOG_VERBOSE, "applicationDidFinishLaunching() start");
	
	// create an NSLock for GL Context locking
	glLock = [[NSLock alloc] init];
	
	// get screen bounds
	CGRect screenBounds = [[UIScreen mainScreen] bounds];
	
	// create fullscreen window
	UIWindow *window = [[UIWindow alloc] initWithFrame:screenBounds];
	
	// create the OpenGL view and add it to the window
	glView = [[EAGLView alloc] initWithFrame:screenBounds];// pixelFormat:GL_RGB565_OES depthFormat:GL_DEPTH_COMPONENT16_OES preserveBackbuffer:NO];
	[window addSubview:glView];
	//	[glView release];	// do not release, incase app wants to removeFromSuper and add later
	
	// make window active
	[window makeKeyAndVisible];
	
	//----- DAMIAN
	// set data path root for ofToDataPath()
	// path on iPhone will be ~/Applications/{application GUID}/openFrameworks.app/data
	// get the resource path for the bundle (ie '~/Applications/{application GUID}/openFrameworks.app')
	NSString *bundle_path_ns = [[NSBundle mainBundle] resourcePath];
	// convert to UTF8 STL string
	string path = [bundle_path_ns UTF8String];
	// append data
	//path.append( "/data/" ); // ZACH
	path.append( "/" ); // ZACH
	ofLog(OF_LOG_VERBOSE, "setting data path root to " + path);
	ofSetDataPathRoot( path );
	//-----
	

	animating = FALSE;
	displayLinkSupported = FALSE;
	animationFrameInterval = 1;
	displayLink = nil;
	animationTimer = nil;
	
	// A system version of 3.1 or greater is required to use CADisplayLink. The NSTimer
	// class is used as fallback when it isn't available.
	NSString *reqSysVer = @"3.1";
	NSString *currSysVer = [[UIDevice currentDevice] systemVersion];
//	if ([currSysVer compare:reqSysVer options:NSNumericSearch] != NSOrderedAscending) displayLinkSupported = TRUE;

	
	
	
	iPhoneSetOrientation(OFXIPHONE_ORIENTATION_PORTRAIT);
	

	// call testApp::setup()
	ofGetAppPtr()->setup();

	#ifdef OF_USING_POCO
		ofNotifyEvent( ofEvents.setup, voidEventArgs );
		ofNotifyEvent( ofEvents.update, voidEventArgs );
	#endif
	
	
	// show or hide status bar depending on OF_WINDOW or OF_FULLSCREEN
	[[UIApplication sharedApplication] setStatusBarHidden:(iPhoneGetOFWindow()->windowMode == OF_FULLSCREEN) animated:YES];
	
	// clear background
	glClearColor(ofBgColorPtr()[0], ofBgColorPtr()[1], ofBgColorPtr()[2], ofBgColorPtr()[3]);
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
    // Listen to did rotate event
    [[UIDevice currentDevice] beginGeneratingDeviceOrientationNotifications];
    [[NSNotificationCenter defaultCenter] addObserver: self 
                                             selector: @selector(receivedRotate:) 
                                                 name: UIDeviceOrientationDidChangeNotification 
                                               object: nil];  
}


-(void) receivedRotate:(NSNotification*)notification {
	UIDeviceOrientation interfaceOrientation = [[UIDevice currentDevice] orientation];
    ofLog(OF_LOG_NOTICE, "Device orientation changed to %i", interfaceOrientation);
	
	if(interfaceOrientation != UIDeviceOrientationUnknown)
        ofxiPhoneAlerts.deviceOrientationChanged(interfaceOrientation);
}


- (void)startAnimation
{
    if (!animating)
    {
        if (displayLinkSupported)
        {
            // CADisplayLink is API new to iPhone SDK 3.1. Compiling against earlier versions will result in a warning, but can be dismissed
            // if the system version runtime check for CADisplayLink exists in -initWithCoder:. The runtime check ensures this code will
            // not be called in system versions earlier than 3.1.
	
            displayLink = [NSClassFromString(@"CADisplayLink") displayLinkWithTarget:self selector:@selector(timerLoop)];
            [displayLink setFrameInterval:animationFrameInterval];
            [displayLink addToRunLoop:[NSRunLoop currentRunLoop] forMode:NSDefaultRunLoopMode];
			ofLog(OF_LOG_VERBOSE, "CADisplayLink supported, running with interval: %i", animationFrameInterval);
        }
        else
			ofLog(OF_LOG_VERBOSE, "CADisplayLink not supported, running with interval: %i", animationFrameInterval);
		
            animationTimer = [NSTimer scheduledTimerWithTimeInterval:(NSTimeInterval)((1.0 / 60.0) * animationFrameInterval) target:self selector:@selector(timerLoop) userInfo:nil repeats:TRUE];
		
        animating = TRUE;
    }
}

- (void)stopAnimation
{
    if (animating)
    {
        if (displayLinkSupported)
        {
            [displayLink invalidate];
            displayLink = nil;
        }
        else
        {
            [animationTimer invalidate];
            animationTimer = nil;
	}
	
        animating = FALSE;
    }
}


- (void)setAnimationFrameInterval:(NSInteger)frameInterval
{
    // Frame interval defines how many display frames must pass between each time the
    // display link fires. The display link will only fire 30 times a second when the
    // frame internal is two on a display that refreshes 60 times a second. The default
    // frame interval setting of one will fire 60 times a second when the display refreshes
    // at 60 times a second. A frame interval setting of less than one results in undefined
    // behavior.
    if (frameInterval >= 1)
    {
        animationFrameInterval = frameInterval;
		
        if (animating)
        {
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



-(void) dealloc {
    [ofxiPhoneGetUIWindow() release];
	[glLock release];
    [super dealloc];
}




-(void) applicationWillResignActive:(UIApplication *)application {
	[self stopAnimation];

	ofxiPhoneAlerts.lostFocus();
	
	//just extend ofxiPhoneAlertsListener with testApp and implement these methods to use them,
	//behaves just like ofxMultiTouchListener
}


-(void) applicationDidBecomeActive:(UIApplication *)application {
	[self startAnimation];
	
	ofxiPhoneAlerts.gotFocus();
}


-(void) applicationWillTerminate:(UIApplication *)application {
	[self stopAnimation];
	
    // stop listening for orientation change notifications
    [[NSNotificationCenter defaultCenter] removeObserver: self];
    [[UIDevice currentDevice] endGeneratingDeviceOrientationNotifications];
    
	[glView release];
	
}


-(void) applicationDidReceiveMemoryWarning:(UIApplication *)application {
	ofxiPhoneAlerts.gotMemoryWarning();
}


- (BOOL)application:(UIApplication *)application handleOpenURL:(NSURL *)url {
	
	
	cout<<"uhh"<<endl;
	
	NSString *urlData = [url absoluteString];
	
	char response[ [urlData length]+1 ];
	[urlData getCString:response];
	
	
	ofxiPhoneAlerts.launchedWithURL(response);
}

@end
