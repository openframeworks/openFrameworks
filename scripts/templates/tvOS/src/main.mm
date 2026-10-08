#include "ofApp.h"

#ifndef OF_GLES_VERSION
#define OF_GLES_VERSION OFXIOS_RENDERER_ES2
#endif

int main() {
    
    //  here are the most commonly used iOS window settings.
    //------------------------------------------------------
    ofiOSWindowSettings settings;
    settings.enableRetina = false; // enables retina resolution if the device supports it.
    settings.enableDepth = false; // enables depth buffer for 3d drawing.
    settings.enableAntiAliasing = false; // enables anti-aliasing which smooths out graphics on the screen.
    settings.numOfAntiAliasingSamples = 0; // number of samples used for anti-aliasing.
    settings.enableHardwareOrientation = false; // enables native view orientation.
    settings.enableHardwareOrientationAnimation = false; // enables native orientation changes to be animated.
    settings.glesVersion = OF_GLES_VERSION; // type of renderer to use, ES1, ES2, ES3 - default ES2, set via OF_GLES_VERSION define
    settings.windowControllerType = ofxiOSWindowControllerType::GL_KIT; // Window Controller Type
    settings.colorType = ofxiOSRendererColorFormat::RGBA8888; // color format used default RGBA8888
    settings.depthType = ofxiOSRendererDepthFormat::DEPTH_NONE; // depth format (16/24) if depth enabled
    settings.stencilType = ofxiOSRendererStencilFormat::STENCIL_NONE; // stencil mode
    settings.windowMode = OF_FULLSCREEN;
    ofCreateWindow(settings);
    
    return ofRunApp(new ofApp);
}
