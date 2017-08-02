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

#include "ofxiOSAppDelegate.h"

#if TARGET_OS_IOS || (TARGET_OS_IPHONE && !TARGET_OS_TV)

#include "ofxiOSViewController.h"
#include "ofxiOSExternalDisplay.h"
#include "ofxiOSExtras.h"
#include "ofxiOSAlerts.h"
#include "ofxiOSEAGLView.h"
#include "ofAppiOSWindow.h"
#include "ofAppRunner.h"

@implementation ofxiOSAppDelegate

@synthesize window;
@synthesize externalWindow;
@synthesize glViewController;
@synthesize currentScreenIndex;

- (void)dealloc {
    self.window = nil;
    self.externalWindow = nil;
    self.glViewController = nil;
    [super dealloc];
}

- (void)applicationDidFinishLaunching:(UIApplication *)application {
	
    self.window = [[[UIWindow alloc] initWithFrame: [[UIScreen mainScreen] bounds]] autorelease];
	[self.window makeKeyAndVisible];
    
    currentScreenIndex = 0;
    
    // set the root application path
    ofSetDataPathRoot([[NSString stringWithFormat:@"%@/", [[NSBundle mainBundle] resourcePath]] cStringUsingEncoding:NSUTF8StringEncoding]);
    
	// show or hide status bar depending on OF_WINDOW or OF_FULLSCREEN
    [[UIApplication sharedApplication] setStatusBarHidden:(ofxiOSGetOFWindow()->getWindowMode() == OF_FULLSCREEN)];
	
    // Listen to did rotate event
    [[UIDevice currentDevice] beginGeneratingDeviceOrientationNotifications];
    
    NSNotificationCenter * center = [NSNotificationCenter defaultCenter];
    
    [center addObserver:self
               selector:@selector(receivedRotate:)
                   name:UIDeviceOrientationDidChangeNotification
                 object:nil];
    
    [center addObserver:self
               selector:@selector(handleScreenConnectNotification:)
                   name:UIScreenDidConnectNotification object:nil];
    
    [center addObserver:self
               selector:@selector(handleScreenDisconnectNotification:)
                   name:UIScreenDidDisconnectNotification object:nil];
    
    [center addObserver:self
               selector:@selector(handleScreenModeDidChangeNotification:)
                   name:UIScreenModeDidChangeNotification object:nil];
    
    
    bool bDoesHWOrientation = ofxiOSGetOFWindow()->doesHWOrientation();
    
    UIInterfaceOrientation iOrient  = [[UIApplication sharedApplication] statusBarOrientation];
    // is the os version less than 6.0? 
    if( [[[UIDevice currentDevice] systemVersion] compare:@"6.0" options:NSNumericSearch] == NSOrderedAscending ) {
		iOrient = UIInterfaceOrientationPortrait;
		
        UIDeviceOrientation deviceOrientation = [[UIDevice currentDevice] orientation];
        switch (deviceOrientation) {
            case UIDeviceOrientationPortrait:
                iOrient = UIInterfaceOrientationPortrait;
                break;
            case UIDeviceOrientationPortraitUpsideDown:
                iOrient = UIInterfaceOrientationPortraitUpsideDown;
                break;
            case UIDeviceOrientationLandscapeLeft:
                iOrient = UIInterfaceOrientationLandscapeRight;
                break;
            case UIDeviceOrientationLandscapeRight:
                iOrient = UIInterfaceOrientationLandscapeLeft;
                break;
            default:
                iOrient = UIInterfaceOrientationPortrait;
                break;
        }
    }
    
    BOOL bIsPortrait = UIInterfaceOrientationIsPortrait( iOrient );
	
	CGRect frame = [[UIScreen mainScreen] bounds];
	
	if( (!bIsPortrait && bDoesHWOrientation)) {
		float tWidth    = frame.size.width;
		float tHeight   = frame.size.height;
		frame.size.width    = tHeight;
		frame.size.height   = tWidth;
	}
	
	ofOrientation defaultOrient = ofGetOrientation();
	
    // check if app delegate is being extended.
    // if not, create a new view controller.
    NSString * appDelegateClassName = [[self class] description];
    if ([appDelegateClassName isEqualToString:@"ofxiOSAppDelegate"]) { // app delegate is not being extended.
		
        self.glViewController = [[[ofxiOSViewController alloc] initWithFrame:frame app:(ofxiOSApp *)ofGetAppPtr()] autorelease];
        self.window.rootViewController = self.glViewController;
        
        ofOrientation requested = ofGetOrientation();
		UIInterfaceOrientation interfaceOrientation = UIInterfaceOrientationPortrait;
		switch (requested) {
			case OF_ORIENTATION_DEFAULT:
				interfaceOrientation = UIInterfaceOrientationPortrait;
				break;
			case OF_ORIENTATION_180:
				interfaceOrientation = UIInterfaceOrientationPortraitUpsideDown;
				break;
			case OF_ORIENTATION_90_RIGHT:
				interfaceOrientation = UIInterfaceOrientationLandscapeLeft;
				break;
			case OF_ORIENTATION_90_LEFT:
				interfaceOrientation = UIInterfaceOrientationLandscapeRight;
				break;
		}
		
        if(!bDoesHWOrientation) {
            [self.glViewController rotateToInterfaceOrientation:UIInterfaceOrientationPortrait animated:false];
		} else {
            [[UIApplication sharedApplication] setStatusBarOrientation:interfaceOrientation animated:NO];
            [self.glViewController rotateToInterfaceOrientation:interfaceOrientation animated:false];
            ofSetOrientation(requested);
        }
        
    }
}

//------------------------------------------------------------------------------------------- application delegate callbacks.
- (void)applicationWillResignActive:(UIApplication *)application {
    [ofxiOSGetGLView() stopAnimation];
	
	ofxiOSAlerts.lostFocus();
	glFinish();
}

- (void)applicationDidEnterBackground:(UIApplication *)application {
	glFinish();
}

- (void)applicationDidBecomeActive:(UIApplication *)application {
    [ofxiOSGetGLView() startAnimation];
	
	ofxiOSAlerts.gotFocus();
}

- (void)applicationWillTerminate:(UIApplication *)application {
    [ofxiOSGetGLView() stopAnimation];
	
    // stop listening for orientation change notifications
    [[NSNotificationCenter defaultCenter] removeObserver: self];
    [[UIDevice currentDevice] endGeneratingDeviceOrientationNotifications];
}

- (void)applicationDidReceiveMemoryWarning:(UIApplication *)application {
	ofxiOSAlerts.gotMemoryWarning();
}

- (BOOL)application:(UIApplication *)application handleOpenURL:(NSURL *)url {
	NSString *urlData = [url absoluteString];
	const char * response = [urlData UTF8String];
	ofxiOSAlerts.launchedWithURL(response);
	return YES;
}

//-------------------------------------------------------------------------------------------
#ifdef __IPHONE_6_0
-(NSUInteger)application:(UIApplication *)application supportedInterfaceOrientationsForWindow:(UIWindow *)window {
    return UIInterfaceOrientationMaskAll;
}
#endif

//------------------------------------------------------------------------------------------- device rotation callback.
- (void)receivedRotate:(NSNotification*)notification {
	UIDeviceOrientation deviceOrientation = [[UIDevice currentDevice] orientation];
    ofLogVerbose("ofxiOSAppDelegate") << "device orientation changed to " << deviceOrientation;
	if( [[[UIDevice currentDevice] systemVersion] compare:@"8.0" options:NSNumericSearch] == NSOrderedAscending ) {
		//iOS7-
		if(deviceOrientation != UIDeviceOrientationUnknown && deviceOrientation != UIDeviceOrientationFaceUp && deviceOrientation != UIDeviceOrientationFaceDown ) {
			if([self.glViewController isReadyToRotate]) {
            ofxiOSAlerts.deviceOrientationChanged( deviceOrientation );
			}
		}
	}else {
        ofxiOSAlerts.deviceOrientationChanged( deviceOrientation );
    }
}

//------------------------------------------------------------------------------------------- external display.

/**
 *
 *  Below are methods that manage the external display.
 *  The code is based on Apple programming guidelines from the link below,
 *  http://developer.apple.com/library/ios/#documentation/WindowsViews/Conceptual/ViewPG_iPhoneOS/CreatingWindows/CreatingWindows.html
 *
 */

- (void)handleScreenConnectNotification:(NSNotification*)aNotification {
    ofxiOSExternalDisplay::alertExternalDisplayConnected(); // alert any OF apps listening for a new external device.
}

- (void)handleScreenDisconnectNotification:(NSNotification*)aNotification {
    [self destroyExternalWindow];
    ofxiOSExternalDisplay::alertExternalDisplayDisconnected();
}

- (void)handleScreenModeDidChangeNotification:(NSNotification*)aNotification {
	ofxiOSExternalDisplay::alertExternalDisplayChanged();
}

//-------------------------------------------------------------------------------------------
- (BOOL)createExternalWindowWithPreferredMode {
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
- (BOOL)createExternalWindowWithScreenModeIndex:(NSInteger)screenModeIndex {
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
- (BOOL)destroyExternalWindow {
    if(!self.externalWindow){
        return NO;
    }
    
    self.externalWindow = nil;
    [self displayOnScreenWithIndex:0 andScreenModeIndex:0];
    
    return YES;
}

//-------------------------------------------------------------------------------------------
- (BOOL)displayOnScreenWithIndex:(NSInteger)screenIndex 
              andScreenModeIndex:(NSInteger)screenModeIndex {
    
    if(screenIndex < 0 || screenIndex > [[UIScreen screens] count]-1){
        return NO; // invalid screen index.
    }
    
    if(currentScreenIndex != screenIndex){
        currentScreenIndex = screenIndex;
    } else {
        return NO; // already displaying on this screen.
    }
    
    ofxiOSEAGLView * glView = ofxiOSGetGLView();
    
    if(screenIndex > 0){ // display on external screen.
        
        [self createExternalWindowWithScreenModeIndex:screenModeIndex];

        glView.frame = self.externalWindow.screen.bounds;
        [self.externalWindow insertSubview:glView atIndex:0];
        [self.externalWindow makeKeyAndVisible];
        
    } else { // display back on device screen.

        if(self.glViewController != nil) {
            glView.frame = [UIScreen mainScreen].bounds;
            [self.glViewController.view insertSubview:glView atIndex:0];
            [self.window makeKeyAndVisible];
        }
    }
    
    ofxiOSExternalDisplay::alertExternalDisplayChanged();
    
    return YES;
}


@end

#endif
