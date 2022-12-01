//
//  ofxtvOSAppDelegate.mm
//  tvOS+OFLib
//
//  Created by Daniel Rosser on 26/10/2015.
//

#include "ofxtvOSAppDelegate.h"

#include "ofxtvOSViewController.h"
#include "ofxtvOSGLKViewController.h"
#include "ofxiOSExtras.h"
#include "ofxiOSAlerts.h"
#include "ofxiOSEAGLView.h"
#include "ofAppiOSWindow.h"
#include "ofAppRunner.h"
#include "ofUtils.h"
#include "ofFileUtils.h"

@implementation ofxtvOSAppDelegate

@synthesize window;
@synthesize uiViewController;

- (void)dealloc {
    self.window = nil;
    self.uiViewController = nil;
}

- (void)applicationDidFinishLaunching:(UIApplication *)application {

    self.window = [[UIWindow alloc] initWithFrame: [[UIScreen mainScreen] bounds]];
    [self.window makeKeyAndVisible];

    // set the root application path
    ofSetDataPathRoot([[NSString stringWithFormat:@"%@/", [[NSBundle mainBundle] resourcePath]] cStringUsingEncoding:NSUTF8StringEncoding]);


    //NSNotificationCenter * center = [NSNotificationCenter defaultCenter];
    CGRect frame = [[UIScreen mainScreen] bounds];
    NSString * appDelegateClassName = [[self class] description];
    if ([appDelegateClassName isEqualToString:@"ofxtvOSAppDelegate"]) { // app delegate is not being extended.
        
        switch(ofxiOSGetOFWindow()->getWindowControllerType()) {
            case METAL_KIT:
                NSLog(@"No MetalKit yet supported for openFrameworks: Falling back to GLKit");
            case GL_KIT:
                self.uiViewController = (UIViewController *)[[ofxtvOSGLKViewController alloc] initWithFrame:frame app:(ofxiOSApp *)ofGetAppPtr() sharegroup:nil];
                break;
            case CORE_ANIMATION:
            default:
                self.uiViewController = [[ofxtvOSViewController alloc] initWithFrame:frame app:(ofxiOSApp *)ofGetAppPtr() sharegroup:nil];
                break;
        }
        self.window.rootViewController = self.uiViewController;
    }

}

//------------------------------------------------------------------------------------------- application delegate callbacks.
- (void)applicationWillResignActive:(UIApplication *)application {
    if(ofxiOSGetOFWindow()->getWindowControllerType() == CORE_ANIMATION)
        [ofxiOSGetGLView() stopAnimation];

    ofxiOSAlerts.lostFocus();
}

- (void)applicationDidEnterBackground:(UIApplication *)application {
    glFinish();
}

- (void)applicationDidBecomeActive:(UIApplication *)application {
    if(ofxiOSGetOFWindow()->getWindowControllerType() == CORE_ANIMATION)
        [ofxiOSGetGLView() startAnimation];

    ofxiOSAlerts.gotFocus();
}

- (void)applicationWillTerminate:(UIApplication *)application {
    if(ofxiOSGetOFWindow()->getWindowControllerType() == CORE_ANIMATION)
        [ofxiOSGetGLView() stopAnimation];

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
