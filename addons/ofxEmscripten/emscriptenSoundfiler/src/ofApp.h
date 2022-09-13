#pragma once

#include "ofMain.h"
#include "ofxPd.h"

#include "hSlider.h"
#include "bang.h"
#include "toggle.h"
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
		
		// pd message receiver callbacks
		void print(const std::string &message);
		void receiveBang(const std::string &dest);
		void receiveFloat(const std::string &dest, float value);
		void receiveSymbol(const std::string &dest, const std::string &symbol);
		void receiveList(const std::string &dest, const pd::List &list);
		void receiveMessage(const std::string &dest, const std::string &msg, const pd::List &list);
		
		void hSlider_1onMousePressed(float & e);
		void hSlider_2onMousePressed(float & e);
		void hSlider_3onMousePressed(float & e);
		void toggle_1onMousePressed(bool & e);
		void bang_1onMousePressed(bool & e);
		void loadAudioToSoundfiler();

		ofxPd pd;
		Patch patch;
		toggle toggle_1;
		bang bang_1;
		hSlider hSlider_1;
		hSlider hSlider_2;
		hSlider hSlider_3;
		label label_1;	
		label label_2;
		label label_3;
		label label_4;
		label label_5;
		label label_7;
};
