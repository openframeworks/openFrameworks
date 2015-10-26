//
//  ofxtvOSAppDelegate.mm
//  tvOS+OFLib
//
//  Created by Daniel Rosser on 26/10/2015.
//

#include "ofxtvOSAppDelegate.h"

#include "ofxtvOSViewController.h"
#include "ofxiOSExtras.h"
#include "ofxiOSAlerts.h"
#include "ofxiOSEAGLView.h"
#include "ofAppiOSWindow.h"
#include "ofAppRunner.h"

@implementation ofxtvOSAppDelegate

@synthesize window;
@synthesize glViewController;

- (void)dealloc {
    self.window = nil;
    self.glViewController = nil;
    [super dealloc];
}

- (void)applicationDidFinishLaunching:(UIApplication *)application {
    
    self.window = [[[UIWindow alloc] initWithFrame: [[UIScreen mainScreen] bounds]] autorelease];
    [self.window makeKeyAndVisible];

    // set the root application path
    ofSetDataPathRoot([[NSString stringWithFormat:@"%@/", [[NSBundle mainBundle] resourcePath]] cStringUsingEncoding:NSUTF8StringEncoding]);
    

    //NSNotificationCenter * center = [NSNotificationCenter defaultCenter];
    CGRect frame = [[UIScreen mainScreen] bounds];
    NSString * appDelegateClassName = [[self class] description];
    if ([appDelegateClassName isEqualToString:@"ofxtvOSAppDelegate"]) { // app delegate is not being extended.
        
        self.glViewController = [[[ofxtvOSViewController alloc] initWithFrame:frame app:(ofxiOSApp *)ofGetAppPtr()] autorelease];
        self.window.rootViewController = self.glViewController;
    }
    
}

//------------------------------------------------------------------------------------------- application delegate callbacks.
- (void)applicationWillResignActive:(UIApplication *)application {
    [ofxiOSGetGLView() stopAnimation];
    
    ofxiOSAlerts.lostFocus();
}

- (void)applicationDidBecomeActive:(UIApplication *)application {
    [ofxiOSGetGLView() startAnimation];
    
    ofxiOSAlerts.gotFocus();
}

- (void)applicationWillTerminate:(UIApplication *)application {
    [ofxiOSGetGLView() stopAnimation];
    
    // stop listening for orientation change notifications
    //[[NSNotificationCenter defaultCenter] removeObserver: self];

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


@end

