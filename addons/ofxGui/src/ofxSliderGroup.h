#pragma once

#include "ofxGuiGroup.h"


template<class VecType>
class ofxVecSlider_ : public ofxGuiGroup {
public:

	ofxVecSlider_();
	ofxVecSlider_(const ofJson & config);
	ofxVecSlider_(ofParameter<VecType> &value, const ofJson & config = ofJson());
	ofxVecSlider_(ofParameter<VecType> &value, float width, float height = defaultHeight);
	ofxVecSlider_(const std::string& controlName, const VecType & value, const VecType & min, const VecType & max, float width = defaultWidth, float height = defaultHeight);

	void setup();

	ofAbstractParameter & getParameter();

	VecType operator=(const VecType & v);
	operator const VecType & ();
	const VecType * operator->();
protected:
	void changeSlider(const void * parameter, float & value);
	void changeValue(VecType & value);
	ofParameter<VecType> value;
	bool sliderChanging;
	vector<std::string> names;
};

template<typename ColorType>
class ofxColorSlider_: public ofxGuiGroup{

public:

	ofxColorSlider_();
	ofxColorSlider_(const ofJson & config);
	ofxColorSlider_(ofParameter<ofColor_<ColorType>> &value, const ofJson & config = ofJson());
	ofxColorSlider_(ofParameter<ofColor_<ColorType>> &value, float width, float height = defaultHeight);
	ofxColorSlider_(const std::string& controlName, const ofColor_<ColorType> & value, const ofColor_<ColorType> & min, const ofColor_<ColorType> & max, float width = defaultWidth, float height = defaultHeight);

	void setup();

	ofAbstractParameter & getParameter();

	ofColor_<ColorType> operator=(const ofColor_<ColorType> & v);
	operator const ofColor_<ColorType> & ();
protected:
	void changeSlider(const void * parameter, ColorType & value);
	void changeValue(ofColor_<ColorType> & value);
	ofParameter<ofColor_<ColorType> > value;
	bool sliderChanging;
	vector<std::string> names;
};

