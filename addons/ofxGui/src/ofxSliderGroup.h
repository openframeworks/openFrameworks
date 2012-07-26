#pragma once

#include "ofxBaseGroup.h"
#include "ofxSlider.h"

template<typename Type>
class ofxSliderGroup : public ofxBaseGroup {
public:
	ofxSliderGroup() : ofxBaseGroup("", "", 0, 0) {}
    
protected:
    vector<ofParameter<Type> > subParameters;
};

template<class VecType>
class ofxVecSlider : public ofxSliderGroup<float> {
public:
    ofxVecSlider(string controlName, ofParameter<VecType> value, float width = defaultWidth, float height = defaultHeight);
    
    ofxVecSlider * setup(string controlName, ofParameter<VecType> value, float width = defaultWidth, float height = defaultHeight);
protected:
    void changeSlider(const void * parameter, float & value);
    void changeValue(VecType & value);
    ofParameter<VecType> value;
};