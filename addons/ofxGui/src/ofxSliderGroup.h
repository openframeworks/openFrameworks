#pragma once

#include "ofxGuiGroup.h"
#include "ofxSlider.h"

template<class VecType>
class ofxVecSlider : public ofxGuiGroup {
public:
    ofxVecSlider(string controlName, ofParameter<VecType> value, float width = defaultWidth, float height = defaultHeight);
    
    ofxVecSlider * setup(string controlName, ofParameter<VecType> value, float width = defaultWidth, float height = defaultHeight);

    ofAbstractParameter & getParameter();
protected:
    void changeSlider(const void * parameter, float & value);
    void changeValue(VecType & value);
    ofParameter<VecType> value;
    bool sliderChanging;
};


template<typename ColorType>
class ofxColorSlider: public ofxGuiGroup{

public:
	ofxColorSlider(string controlName, ofParameter<ofColor_<ColorType> > value, float width = defaultWidth, float height = defaultHeight);

	ofxColorSlider * setup(string controlName, ofParameter<ofColor_<ColorType> > value, float width = defaultWidth, float height = defaultHeight);

	ofAbstractParameter & getParameter();
protected:
    void changeSlider(const void * parameter, ColorType & value);
    void changeValue(ofColor_<ColorType> & value);
    ofParameter<ofColor_<ColorType> > value;
    bool sliderChanging;
};
