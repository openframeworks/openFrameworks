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

#import <mach/mach_time.h>

#import "ofxiPhoneAppDelegate.h"
#import "ofMain.h"
#import "ofxiPhone.h"
#import "ofxiPhoneExtras.h"

@implementation ofxiPhoneAppDelegate




-(void) timerLoop {
//	NSLog(@"ofxiPhoneAppDelegate::timerLoop");
	
	// create autorelease pool in case anything needs it
	NSAutoreleasePool * pool = [[NSAutoreleasePool alloc] init];
	
	iPhoneGetOFWindow()->timerLoop();
		
	// release pool
	[pool release];
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


// based on Robert Carlsen's code from:		// THREAD MOD
// http://www.openframeworks.cc/forum/viewtopic.php?f=25&t=2288&p=13128
// also discussion at http://www.memo.tv/looping_via_nsthread_vs_nstimer
-(void) timerLoopThreaded:(id*)sender {
	
	// create autorelease pool in case anything needs it
	NSAutoreleasePool * pool = [[NSAutoreleasePool alloc] init];
	
	// SET CURRENT GL CONTEXT FOR THREAD
	[EAGLContext setCurrentContext:glView.context];
	
	
	// <MEMO>: trying out mach timing
	mach_timebase_info_data_t info;
	mach_timebase_info(&info); 
	while(!killThread) {
//		NSLog(@"ofxiPhoneAppDelegate::timerLoopThreaded");

		uint64_t startTime = mach_absolute_time() * info.numer / info.denom;
		iPhoneGetOFWindow()->timerLoop();
		
		uint64_t endTime = mach_absolute_time() * info.numer / info.denom;
//		uint64_t diffTime = endTime - startTime;
		
		uint64_t targetStart = startTime + frameLength;
		
		if(targetStart > endTime) [NSThread sleepForTimeInterval:(targetStart - endTime) * 1e-9];
	}
	
	// / <MEMO>
	
	
	/*	
	 double begin_time; // RC: testing alternative loop method
	 while(!killThread)
	 {
	 iPhoneGetOFWindow()->timerLoop();
	 
	 // throttle the loop for desired targetFrameRate
	 double sleep = (1.0/targetFrameRate) - (((double)CFAbsoluteTimeGetCurrent()) - begin_time);
	 if(sleep>0.0) [ NSThread sleepForTimeInterval:sleep];
	 double end_time = (double)CFAbsoluteTimeGetCurrent();     
	 //float fps = (1.0 / (end_time-begin_time));     
	 begin_time = end_time;
	 }
	 */
	
	// release pool
	[pool release];
}



-(void) enableLoopInThread {
	ofLog(OF_LOG_VERBOSE, "ofxiPhoneAppDelegate::enableLoopInThread");
	loopInThreadIsEnabled = YES;
}


-(void) applicationDidFinishLaunching:(UIApplication *)application {    
	static ofEventArgs voidEventArgs;
	ofLog(OF_LOG_VERBOSE, "applicationDidFinishLaunching() start");
	
	// create an NSLock for GL Context locking
	glLock = [[NSLock alloc] init];
	
	// create fullscreen window
	UIWindow *window = [[UIWindow alloc] initWithFrame:[[UIScreen mainScreen] bounds]];
	
	// create the OpenGL view and add it to the window
	glView = [[EAGLView alloc] initWithFrame:[[UIScreen mainScreen] bounds] pixelFormat:GL_RGB565_OES depthFormat:GL_DEPTH_COMPONENT16_OES preserveBackbuffer:NO];
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
	

	// zero targetFrameRate (to see if user setes it in setup()
	targetFrameRate = -1;
	
	iPhoneSetOrientation(OFXIPHONE_ORIENTATION_PORTRAIT);
	
	// call testApp::setup()
	ofGetAppPtr()->setup();
	#ifdef OF_USING_POCO
		ofNotifyEvent( ofEvents.setup, voidEventArgs );
		ofNotifyEvent( ofEvents.update, voidEventArgs );
	#endif
	
	// if user didn't set framerate in setup(), default to 60
	if(targetFrameRate == -1) [self setFrameRate:60];

	
	// show or hide status bar depending on OF_WINDOW or OF_FULLSCREEN
	[[UIApplication sharedApplication] setStatusBarHidden:(iPhoneGetOFWindow()->windowMode == OF_FULLSCREEN) animated:YES];
	
	// clear background
	glClearColor(ofBgColorPtr()[0], ofBgColorPtr()[1], ofBgColorPtr()[2], ofBgColorPtr()[3]);
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	// From Robert Carlsen		// THREAD MOD
	if(loopInThreadIsEnabled) {
		[NSThread detachNewThreadSelector:@selector(timerLoopThreaded:) toTarget:self withObject:nil];
	}
    
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


-(void) setFrameRate:(float)rate {
	// THREAD MOD
	targetFrameRate = rate;
	frameLength = targetFrameRate ? 1e9/targetFrameRate : 0;
	
	if(!loopInThreadIsEnabled) {
		ofLog(OF_LOG_VERBOSE, "setFrameRate %.3f using NSTimer", rate);
		
		[timer invalidate];
		if(rate) timer = [[NSTimer scheduledTimerWithTimeInterval:(1.0f / rate) target:self selector:@selector(timerLoop) userInfo:nil repeats:YES] retain];
	} else {
		ofLog(OF_LOG_VERBOSE, "setFrameRate %.3f using NSThread", rate);
		
	}
	
}

-(void) dealloc {
	// THREAD MOD	
	if(!loopInThreadIsEnabled) {
		[timer release];		// THREAD MOD
	}
    [ofxiPhoneGetUIWindow() release];
	[glLock release];
    [super dealloc];
}




-(void) applicationWillResignActive:(UIApplication *)application {
	ofxiPhoneAlerts.lostFocus();
	
	//just extend ofxiPhoneAlertsListener with testApp and implement these methods to use them,
	//behaves just like ofxMultiTouchListener
}


-(void) applicationDidBecomeActive:(UIApplication *)application {
	ofxiPhoneAlerts.gotFocus();
}


-(void) applicationWillTerminate:(UIApplication *)application {
	// THREAD MOD
	if(!loopInThreadIsEnabled) {
		killThread = true;
	} else {
		[timer invalidate];
	}
	
    // stop listening for orientation change notifications
    [[NSNotificationCenter defaultCenter] removeObserver: self];
    [[UIDevice currentDevice] endGeneratingDeviceOrientationNotifications];
    
	[glView release];
	
}


-(void) applicationDidReceiveMemoryWarning:(UIApplication *)application {
	ofxiPhoneAlerts.gotMemoryWarning();
}


@end
