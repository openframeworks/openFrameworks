#include "ofMain.h"
#include "ofAppiOSWindow.h"

/*
Andreas Borg
http://crea.tion.to
Sep 2015

This examples shows how you can use ARC with oF even though the core is
not yet ARC compatible.

Just enable Objective-C Automatic Reference Counting in build settings in your app
and you no longer need autorelease etc. The destructors will be called automatically
when you set your objects to 0.

eg.

@interface ArcTest : NSObject

- (void) dealloc;

@end

@implementation ArcTest

- (void) dealloc {

    NSLog(@"ArcTest dealloc");

}

@end


int main(){
    ArcTest * arc = [[ArcTest alloc] init];
    arc = 0;
}

*/

int main(){


    //  here are the most commonly used iOS window settings.
    //------------------------------------------------------
    ofiOSWindowSettings settings;
    settings.enableRetina = true; // enables retina resolution if the device supports it.
    settings.enableDepth = false; // enables depth buffer for 3d drawing.
    settings.enableAntiAliasing = true; // enables anti-aliasing which smooths out graphics on the screen.
    settings.numOfAntiAliasingSamples = 4; // number of samples used for anti-aliasing.
    settings.enableHardwareOrientation = false; // enables native view orientation.
    settings.enableHardwareOrientationAnimation = true; // enables native orientation changes to be animated.
    settings.glesVersion = OFXIOS_RENDERER_ES2; // type of renderer to use, ES1, ES2, etc.
    
    ofAppiOSWindow * window = (ofAppiOSWindow *)(ofCreateWindow(settings).get());
    window->startAppWithDelegate("MyAppDelegate");
    

    
}