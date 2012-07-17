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

#import "ofMain.h"
#import "ofxiPhoneAppDelegate.h"
#import "ofxiPhoneExtras.h"
#import "ofxiPhoneExternalDisplay.h"

@implementation ofxiPhoneAppDelegate

@synthesize window;
@synthesize externalWindow;
@synthesize glViewController;
@synthesize currentScreenIndex;

-(void) applicationDidFinishLaunching:(UIApplication *)application {    
	
    self.window = [[[UIWindow alloc] initWithFrame: [[UIScreen mainScreen] bounds]] autorelease];
	[self.window makeKeyAndVisible];
    
    currentScreenIndex = 0;
    
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
	
	// show or hide status bar depending on OF_WINDOW or OF_FULLSCREEN
	[[UIApplication sharedApplication] setStatusBarHidden:(iPhoneGetOFWindow()->windowMode == OF_FULLSCREEN) animated:YES];
	
    // Listen to did rotate event
    [[UIDevice currentDevice] beginGeneratingDeviceOrientationNotifications];

    NSNotificationCenter* center = [NSNotificationCenter defaultCenter];
    
    [center addObserver: self 
               selector: @selector(receivedRotate:) 
                   name: UIDeviceOrientationDidChangeNotification 
                 object: nil];
    
    [center addObserver:self 
               selector:@selector(handleScreenConnectNotification:)
                   name:UIScreenDidConnectNotification object:nil];
    
    [center addObserver:self 
               selector:@selector(handleScreenDisconnectNotification:)
                   name:UIScreenDidDisconnectNotification object:nil];

    [center addObserver:self 
               selector:@selector(handleScreenModeDidChangeNotification:)
                   name:UIScreenDidDisconnectNotification object:nil];
    
    /**
     *  check if app delegate is being extended.
     *  if not, create a new view controller.
     */
    
    NSString *appDelegateClassName;
    appDelegateClassName = [[self class] description];
    if ([appDelegateClassName isEqualToString:@"ofxiPhoneAppDelegate"]) { // app delegate is not being extended. 
        self.glViewController = [[[ofxiPhoneViewController alloc] initWithFrame:[[UIScreen mainScreen] bounds] 
                                                                            app:ofGetAppPtr()] autorelease];
        self.window.rootViewController = self.glViewController;
    }
    
	#ifdef __IPHONE_4_3

    /**
     *  check if external display is connected.
     *  if so, create an external window for it.
     */
    
    if([[UIScreen screens] count] > 1){
        [self createExternalWindowWithPreferredMode]; // create external window as soon as external screen is connected to prevent unwanted mirroring.
        ofxiPhoneExternalDisplay::alertExternalDisplayConnected(); // alert any OF apps listening for a new external device.
    }
	
	#endif
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
    [ofxiPhoneGetGLView() stopAnimation];
	
	ofxiPhoneAlerts.lostFocus();
}

-(void) applicationDidBecomeActive:(UIApplication *)application {
    [ofxiPhoneGetGLView() startAnimation];
	
	ofxiPhoneAlerts.gotFocus();
}


-(void) applicationWillTerminate:(UIApplication *)application {
    [ofxiPhoneGetGLView() stopAnimation];
	
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

#ifdef __IPHONE_4_3

/**
 *
 *  Below are methods that manage the external display.
 *  The code is based on Apple programming guidelines from the link below,
 *  http://developer.apple.com/library/ios/#documentation/WindowsViews/Conceptual/ViewPG_iPhoneOS/CreatingWindows/CreatingWindows.html
 *
 */

- (void)handleScreenConnectNotification:(NSNotification*)aNotification {
    [self createExternalWindowWithPreferredMode]; // create external window as soon as external screen is connected to prevent unwanted mirroring.
    ofxiPhoneExternalDisplay::alertExternalDisplayConnected(); // alert any OF apps listening for a new external device.
}

- (void)handleScreenDisconnectNotification:(NSNotification*)aNotification {
    [self destroyExternalWindow];
    ofxiPhoneExternalDisplay::alertExternalDisplayDisconnected();
}

- (void)handleScreenModeDidChangeNotification:(NSNotification*)aNotification {
    //
}

//-------------------------------------------------------------------------------------------
-(BOOL) createExternalWindowWithPreferredMode {
    if([[UIScreen screens] count] == 1){
        return NO;
    }
    
    UIScreen * externalScreen;
    externalScreen = [[UIScreen screens] objectAtIndex:1];
    
    externalScreen.currentMode = externalScreen.preferredMode;
    
    NSInteger w = externalScreen.currentMode.size.width;
    NSInteger h = externalScreen.currentMode.size.height;
    
    CGRect externalScreenFrame;
    externalScreenFrame = CGRectZero;
    externalScreenFrame.size = CGSizeMake(w, h);
    
    self.externalWindow = [[[UIWindow alloc] initWithFrame:externalScreenFrame] autorelease];
    self.externalWindow.screen = externalScreen;
    self.externalWindow.clipsToBounds = YES;
    self.externalWindow.hidden = NO;
    
    return YES;
}


//-------------------------------------------------------------------------------------------
-(BOOL) createExternalWindowWithScreenModeIndex:(NSInteger)screenModeIndex {
    if([[UIScreen screens] count] == 1){
        return NO;
    }
    
    UIScreen * externalScreen;
    externalScreen = [[UIScreen screens] objectAtIndex:1];
    
    if(screenModeIndex < 0){
        screenModeIndex = 0;
    }
    
    if(screenModeIndex > [[externalScreen availableModes] count] - 1) {
        screenModeIndex = [[externalScreen availableModes] count] - 1;
    }
    
    externalScreen.currentMode = [[externalScreen availableModes] objectAtIndex:screenModeIndex];
    
    NSInteger w = externalScreen.currentMode.size.width;
    NSInteger h = externalScreen.currentMode.size.height;
    
    CGRect externalScreenFrame;
    externalScreenFrame = CGRectZero;
    externalScreenFrame.size = CGSizeMake(w, h);
    
    self.externalWindow = [[[UIWindow alloc] initWithFrame:externalScreenFrame] autorelease];
    self.externalWindow.screen = externalScreen;
    self.externalWindow.clipsToBounds = YES;
    self.externalWindow.hidden = NO;
    
    return YES;
}

//-------------------------------------------------------------------------------------------
-(BOOL) destroyExternalWindow {
    if(!self.externalWindow){
        return NO;
    }
    
    self.externalWindow = nil;
    [self displayOnScreenWithIndex:0 andScreenModeIndex:0];
    
    return YES;
}

//-------------------------------------------------------------------------------------------
-(BOOL) displayOnScreenWithIndex:(NSInteger)screenIndex 
              andScreenModeIndex:(NSInteger)screenModeIndex {
    
    if(screenIndex < 0 || screenIndex > [[UIScreen screens] count]-1){
        return NO; // invalid screen index.
    }
    
    if(currentScreenIndex != screenIndex){
        currentScreenIndex = screenIndex;
    } else {
        return NO; // already displaying on this screen.
    }
    
    if(screenIndex > 0){ // display on external screen.
        
        [self createExternalWindowWithScreenModeIndex:screenModeIndex];

        self.glViewController.glView.frame = self.externalWindow.screen.bounds;
        [self.externalWindow insertSubview:self.glViewController.view atIndex:0];
        [self.externalWindow makeKeyAndVisible];
        
    } else { // display back on device screen.

        self.glViewController.glView.frame = [UIScreen mainScreen].bounds;
        [self.window insertSubview:self.glViewController.view atIndex:0];
        [self.window makeKeyAndVisible];
    }
    
    ofxiPhoneGetOFWindow()->resetDimensions();
    
    ofxiPhoneExternalDisplay::alertExternalDisplayChanged();
    
    return YES;
}

#endif

@end