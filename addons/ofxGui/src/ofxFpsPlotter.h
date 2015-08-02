#pragma once

#include "ofxValuePlotter.h"

class ofxFpsPlotter : public ofxValuePlotter {
	public:
		struct Config : public ofxValuePlotter::Config {
			Config(){
			}
			Config(const ofxValuePlotter::Config & c) : ofxValuePlotter::Config(c){
			}
		};
		ofxFpsPlotter();
		ofxFpsPlotter(const Config & config);
		ofxFpsPlotter(float minValue, float maxValue, int plotSize = Config().plotSize, float width = defaultWidth, float height = defaultHeight);
		virtual ~ofxFpsPlotter();

		void update();

		ofxFpsPlotter & setup(const Config & config);
		ofxFpsPlotter & setup(float minValue = Config().minValue, float maxValue = Config().maxValue, int plotSize = Config().plotSize, float width = defaultWidth, float height = defaultHeight);

	protected:

};
