#pragma once

#include "ofMain.h"
#include "ofxPd.h"
#include "ofxAppEmscriptenWindow.h"

#include "hSlider.h"
#include "label.h"

// a namespace for the Pd types
using namespace pd;

// inherit pd receivers to receive message and midi events
class ofApp : public ofBaseApp, public PdReceiver, public PdMidiReceiver {

	public:

		// main
		void setup();
		void update();
		void draw();
		void exit();
		
		// audio callbacks
		void audioReceived(float * input, int bufferSize, int nChannels);
		void audioRequested(float * output, int bufferSize, int nChannels);
		
		void hSlider_1onMousePressed(float & e);
		void hSlider_2onMousePressed(float & e);
		void hSlider_3onMousePressed(float & e);

		ofxPd pd;
		Patch patch;
		hSlider hSlider_1;
		hSlider hSlider_2;
		hSlider hSlider_3;
		label label_1;	
		label label_2;
		label label_3;
		label label_4;
		label label_5;
		label label_6;
};
