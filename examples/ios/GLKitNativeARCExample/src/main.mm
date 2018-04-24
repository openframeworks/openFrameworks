#include "ofMain.h"
#include "ofAppiOSWindow.h"

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
    settings.glesVersion = OFXIOS_RENDERER_ES2; // type of renderer to use, ES1, ES2, ES3
    settings.windowControllerType = ofxiOSWindowControllerType::GL_KIT; // Window Controller Type
    settings.colorType = ofxiOSRendererColorFormat::RGBA8888; // color format used default RGBA8888
    settings.depthType = ofxiOSRendererDepthFormat::DEPTH_NONE; // depth format (16/24) if depth enabled
    settings.stencilType = ofxiOSRendererStencilFormat::STENCIL_NONE; // stencil mode
    settings.windowMode = OF_FULLSCREEN;
    
    ofAppiOSWindow * window = (ofAppiOSWindow *)(ofCreateWindow(settings).get());
    window->startAppWithDelegate("MyAppDelegate");
    

    
}
