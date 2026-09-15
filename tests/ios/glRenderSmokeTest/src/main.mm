#include "ofApp.h"

int main() {
	
	//  here are the most commonly used iOS window settings.
	//------------------------------------------------------
	ofiOSWindowSettings settings;
	settings.enableRetina = true; // enables retina resolution if the device supports it.
	settings.enableDepth = false; // enables depth buffer for 3d drawing.
	settings.enableAntiAliasing = false;
	settings.enableHardwareOrientation = false; // enables native view orientation.
	settings.enableHardwareOrientationAnimation = false; // enables native orientation changes to be animated.
	settings.glesVersion = OFXIOS_RENDERER_ES3; // ES1 / ES2 / ES3 — core adapts if you change this.
	
	ofCreateWindow(settings);
	
	return ofRunApp(new ofApp);
}
