#include "ofMain.h"
#include "ofAppiOSWindow.h"
#include "SquareApp.h"

int main(){

    //  here are the most commonly used iOS window settings.
    //------------------------------------------------------
    ofiOSWindowSettings settings;
    settings.enableRetina = true; // enables retina resolution if the device supports it.
    settings.enableDepth = false; // enables depth buffer for 3d drawing.
    settings.enableAntiAliasing = false; // enables anti-aliasing which smooths out graphics on the screen.
    settings.numOfAntiAliasingSamples = 0; // number of samples used for anti-aliasing.
    settings.enableHardwareOrientation = false; // enables native view orientation.
    settings.enableHardwareOrientationAnimation = false; // enables native orientation changes to be animated.
    settings.glesVersion = OFXIOS_RENDERER_ES1; // type of renderer to use, ES1, ES2, etc.
    
    shared_ptr<ofAppBaseWindow> windowBase = ofCreateWindow(settings);
    ofAppiOSWindow * window = (ofAppiOSWindow *)(windowBase.get());
    
    bool bUseNative = true;
    if (bUseNative){
        /**
         *
         *  Below is how you start using a native ios setup.
         *
         *  First a ofAppiOSWindow is created and added to ofSetupOpenGL()
         *  Notice that no app is being sent to ofRunApp() - this happens later when we actually need the app.
         *
         *  One last thing that needs to be done is telling ofAppiOSWindow which AppDelegate to use.
         *  This is a custom AppDelegate and inside it you can start coding your native iOS application.
         *  The AppDelegate must extend ofxiOSAppDelegate.
         *
         **/
        
        window->startAppWithDelegate("MyAppDelegate");
    }
    else {
        /**
         *
         *  This is the normal way of running an app using ofxiOS.
         *  This code has been left in this example to show that ofxiOS still works
         *
         **/
        
        return ofRunApp(new SquareApp());
    }

}