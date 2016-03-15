#pragma once

#include "ofxValuePlotter.h"

class ofxFpsPlotter : public ofxValuePlotter {
	public:
		ofxFpsPlotter();
		ofxFpsPlotter(const ofJson & config);
		ofxFpsPlotter(std::string label, float minValue, float maxValue, int plotSize = 100, float width = defaultWidth, float height = defaultHeight);

		virtual ~ofxFpsPlotter();

		void setup();
		void update(ofEventArgs &);

	protected:

};
