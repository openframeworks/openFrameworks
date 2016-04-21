#pragma once

#include "ofxValuePlotter.h"

class ofxFpsPlotter : public ofxValuePlotter {
	public:
		ofxFpsPlotter();
		ofxFpsPlotter(const ofxValuePlotter::Config & config);
		virtual ~ofxFpsPlotter();

		ofxFpsPlotter & setup(string label = "", float minValue = Config().minValue, float maxValue = Config().maxValue, int plotSize = Config().plotSize, float width = defaultWidth, float height = defaultHeight);

		void update(ofEventArgs &);

	protected:

};
