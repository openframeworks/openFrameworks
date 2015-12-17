#include "ofAppRunner.h"
#include "ofApp.h"

int main(){
    //  Here are the most commonly used iOS window settings.
    //------------------------------------------------------
    ofiOSWindowSettings settings;
    settings.enableRetina = false; // Enables retina resolution if the device supports it.
    settings.enableDepth = false; // Enables depth buffer for 3d drawing.
    settings.enableAntiAliasing = false; // Enables anti-aliasing which smooths out graphics on the screen.
    settings.numOfAntiAliasingSamples = 0; // The number of samples used for anti-aliasing.
    settings.enableHardwareOrientation = false; // Enables native view orientation.
    settings.enableHardwareOrientationAnimation = false; // Enables native orientation changes to be animated.
    settings.glesVersion = OFXIOS_RENDERER_ES1; // The type of renderer to use, ES1, ES2, ES3.
    settings.windowMode = OF_FULLSCREEN;

    auto window = ofCreateWindow(settings);
	auto app = std::make_shared<ofApp>();

    ofRunApp(window,app);

	return ofRunMainLoop();
}
