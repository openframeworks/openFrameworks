#include "ofApp.h"

int main() {
	
	//  here are the most commonly used iOS window settings.
	//------------------------------------------------------
	ofiOSWindowSettings settings;
	settings.enableRetina = true; // enables retina resolution if the device supports it.
	settings.enableDepth = false; // enables depth buffer for 3d drawing.
	settings.enableAntiAliasing = true; // enables anti-aliasing which smooths out graphics on the screen.
	settings.numOfAntiAliasingSamples = 4; // number of samples used for anti-aliasing.
	settings.enableHardwareOrientation = false; // enables native view orientation.
	settings.enableHardwareOrientationAnimation = false; // enables native orientation changes to be animated.
	settings.glesVersion = OFXIOS_RENDERER_ES3; // type of renderer to use, ES1, ES2, ES3.
	
	ofCreateWindow(settings);
	
	return ofRunApp(new ofApp);
}
