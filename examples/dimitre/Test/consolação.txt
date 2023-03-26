#pragma once

#include "ofMain.h"
//#include "ofBaseApp.h"  // for ofBaseApp

// #include "ofxMicroUI.h"

// #include "feature.h"
// #include "featureAll.h"
// #include "featureBpm.h"
// #include "featureFFT.h"

//#include "ofxScenes.h"
//#include "scenesMuti.h"

class ofApp : public ofBaseApp{
	public:
		void setup();
		void update();
		void draw();
		void keyPressed(int key);

//	ofxMicroUI u = { "u.txt" };
//	ofxMicroUISoftware soft = { &u, "Coca01" };
//	ofxMicroUI * ui = &u.uis["ui"];
//	ofFbo * fbo = &soft.fbo;
	
//	ofxScenes scenes =
//	{ "scene", fbo, { &u, ui, &u.uis["scene"], &u.uis["colors"] }}; //, &u.uis["draw"]
		

};
