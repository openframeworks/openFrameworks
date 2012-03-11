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

#import "ofxiPhoneAppDelegate.h"
#import "ofxiPhoneExtras.h"

@implementation ofxiPhoneAppDelegate

@synthesize window;
@synthesize glViewController;

-(void) applicationDidFinishLaunching:(UIApplication *)application {    
	self.window = [[[UIWindow alloc] initWithFrame: [[UIScreen mainScreen] bounds]] autorelease];
	[self.window makeKeyAndVisible];
    
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
	
	iPhoneSetOrientation(OFXIPHONE_ORIENTATION_PORTRAIT);
	
	// show or hide status bar depending on OF_WINDOW or OF_FULLSCREEN
	[[UIApplication sharedApplication] setStatusBarHidden:(iPhoneGetOFWindow()->windowMode == OF_FULLSCREEN) animated:YES];
	
    // Listen to did rotate event
    [[UIDevice currentDevice] beginGeneratingDeviceOrientationNotifications];
    [[NSNotificationCenter defaultCenter] addObserver: self 
                                             selector: @selector(receivedRotate:) 
                                                 name: UIDeviceOrientationDidChangeNotification 
                                               object: nil];
    
    /**
     *  check if app delegate is being extended.
     *  if not, create a new view controller.
     */
    
    NSString *appDelegateClassName;
    appDelegateClassName = [[self class] description];
    if ([appDelegateClassName isEqualToString:@"ofxiPhoneAppDelegate"]) { // app delegate is not being extended. 
        ofxiPhoneViewController *viewController;
        viewController = [[[ofxiPhoneViewController alloc] initWithFrame:[[UIScreen mainScreen] bounds] 
                                                                     app:ofGetAppPtr()] autorelease];
        [self.window insertSubview:viewController.view atIndex:0];  // make sure it is the bottom most view.
    }
}


-(void) receivedRotate:(NSNotification*)notification {
	UIDeviceOrientation interfaceOrientation = [[UIDevice currentDevice] orientation];
    ofLog(OF_LOG_NOTICE, "Device orientation changed to %i", interfaceOrientation);
	
	if(interfaceOrientation != UIDeviceOrientationUnknown)
        ofxiPhoneAlerts.deviceOrientationChanged(interfaceOrientation);
}

-(void) dealloc {
    self.glViewController = nil;
    self.window = nil;
    
    [super dealloc];
}

-(void) applicationWillResignActive:(UIApplication *)application {
    [ofxiPhoneGetViewController() stopAnimation];
	
	ofxiPhoneAlerts.lostFocus();
}

-(void) applicationDidBecomeActive:(UIApplication *)application {
    [ofxiPhoneGetViewController() startAnimation];
	
	ofxiPhoneAlerts.gotFocus();
}


-(void) applicationWillTerminate:(UIApplication *)application {
    [ofxiPhoneGetViewController() stopAnimation];
	
    // stop listening for orientation change notifications
    [[NSNotificationCenter defaultCenter] removeObserver: self];
    [[UIDevice currentDevice] endGeneratingDeviceOrientationNotifications];
}

-(void) applicationDidReceiveMemoryWarning:(UIApplication *)application {
	ofxiPhoneAlerts.gotMemoryWarning();
}

- (BOOL)application:(UIApplication *)application handleOpenURL:(NSURL *)url {
	NSString *urlData = [url absoluteString];
	const char * response = [urlData UTF8String];
	ofxiPhoneAlerts.launchedWithURL(response);
	return YES;
}

@end