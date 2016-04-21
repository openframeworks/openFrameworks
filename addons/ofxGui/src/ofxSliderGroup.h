#pragma once

#include "ofxGuiGroup.h"


template<class VecType>
class ofxVecSlider_ : public ofxGuiGroup {
public:
	struct Config: public ofxGuiGroup::Config{
		Config(){}
		Config(const ofxGuiGroup::Config & c)
		:ofxGuiGroup::Config(c){}
		Config(const ofxBaseGui::Config & c)
		:ofxGuiGroup::Config(c){}
	};
	ofxVecSlider_();
	ofxVecSlider_(ofParameter<VecType> value, const Config & config = Config());
	ofxVecSlider_(ofParameter<VecType> value, float width, float height = defaultHeight);

	ofxVecSlider_ & setup(ofParameter<VecType> value, float width = defaultWidth, float height = defaultHeight);
	ofxVecSlider_ & setup(ofParameter<VecType> value, const Config & config = Config());
	ofxVecSlider_ & setup(const std::string& controlName, const VecType & value, const VecType & min, const VecType & max, float width = defaultWidth, float height = defaultHeight);

	ofAbstractParameter & getParameter();

	VecType operator=(const VecType & v);
	operator const VecType & ();
	const VecType * operator->();
protected:
	void changeSlider(const void * parameter, float & value);
	void changeValue(VecType & value);
	ofParameter<VecType> value;
	bool sliderChanging;
};

typedef ofxVecSlider_<ofVec3f> ofxVec3Slider;
typedef ofxVecSlider_<ofVec2f> ofxVec2Slider;
typedef ofxVecSlider_<ofVec4f> ofxVec4Slider;
typedef ofxVecSlider_<ofVec3f> ofxPointSlider;

template<typename ColorType>
class ofxColorSlider_: public ofxGuiGroup{

public:
	struct Config: public ofxGuiGroup::Config{
		Config(){}
		Config(const ofxGuiGroup::Config & c)
		:ofxGuiGroup::Config(c){}
		Config(const ofxBaseGui::Config & c)
		:ofxGuiGroup::Config(c){}
	};

	ofxColorSlider_();
	ofxColorSlider_(ofParameter<ofColor_<ColorType>> value, const Config & config = Config());
	ofxColorSlider_(ofParameter<ofColor_<ColorType> > value, float width = defaultWidth, float height = defaultHeight);

	ofxColorSlider_ & setup(ofParameter<ofColor_<ColorType> > value, float width = defaultWidth, float height = defaultHeight);
	ofxColorSlider_ & setup(ofParameter<ofColor_<ColorType> > value, const Config & config = Config());
	ofxColorSlider_ & setup(const std::string& controlName, const ofColor_<ColorType> & value, const ofColor_<ColorType> & min, const ofColor_<ColorType> & max, float width = defaultWidth, float height = defaultHeight);

	ofAbstractParameter & getParameter();

	ofColor_<ColorType> operator=(const ofColor_<ColorType> & v);
	operator const ofColor_<ColorType> & ();
protected:
	void changeSlider(const void * parameter, ColorType & value);
	void changeValue(ofColor_<ColorType> & value);
	ofParameter<ofColor_<ColorType> > value;
	bool sliderChanging;
};

typedef ofxColorSlider_<unsigned char> ofxColorSlider;
typedef ofxColorSlider_<unsigned short> ofxShortColorSlider;
typedef ofxColorSlider_<float> ofxFloatColorSlider;

template<class C>
void ofxGuiGroup::add(ofParameter <ofVec2f> & parameter, const C & config){
	add<ofxVec2Slider>(parameter, config);
}

template<class C>
void ofxGuiGroup::add(ofParameter <ofVec3f> & parameter, const C & config){
	add<ofxVec3Slider>(parameter, config);
}

template<class C>
void ofxGuiGroup::add(ofParameter <ofVec4f> & parameter, const C & config){
	add<ofxVec4Slider>(parameter, config);
}

template<class C>
void ofxGuiGroup::add(ofParameter <ofColor> & parameter, const C & config){
	add<ofxColorSlider>(parameter, config);
}

template<class C>
void ofxGuiGroup::add(ofParameter <ofShortColor> & parameter, const C & config){
	add<ofxShortColorSlider>(parameter, config);
}

template<class C>
void ofxGuiGroup::add(ofParameter <ofFloatColor> & parameter, const C & config){
	add<ofxFloatColorSlider>(parameter, config);
}
