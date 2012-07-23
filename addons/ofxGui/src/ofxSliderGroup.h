#pragma once

#include "ofParameter.h"
#include "ofxSlider.h"

class ofxSliderGroup<class VecType> {
public:
	ofxSliderGroup() : count(VecType.DIM) {
		sliders.resize(count);
		for (int i=0; i<count; i++) {
			sliders[i];
		}
	}
protected:
	vector<ofxSlider> sliders;
	 const int count;
};