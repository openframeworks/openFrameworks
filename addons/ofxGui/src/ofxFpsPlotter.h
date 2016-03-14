#pragma once

#include "ofxValuePlotter.h"

class ofxFpsPlotter : public ofxValuePlotter {
	public:
		ofxFpsPlotter(const ofJson & config = ofJson());
		ofxFpsPlotter(std::string label, float minValue, float maxValue, int plotSize = 100, float width = defaultWidth, float height = defaultHeight);

		virtual ~ofxFpsPlotter();

		void setup(const ofJson &config = ofJson());
		void update(ofEventArgs &);

	protected:

};
