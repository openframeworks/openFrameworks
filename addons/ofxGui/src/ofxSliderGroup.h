#pragma once

#include "ofxGuiGroup.h"
#include "ofxSlider.h"
#include "ofxColorPicker.h"

template<class VecType>
class ofxVecSlider_ : public ofxGuiGroup {
public:
    ofxVecSlider_(){
        sliderChanging = false;
    };
    ofxVecSlider_(ofParameter<VecType> value, float width = defaultWidth, float height = defaultHeight);

    ofxVecSlider_ * setup(ofParameter<VecType> value, float width = defaultWidth, float height = defaultHeight);
    ofxVecSlider_ * setup(const std::string& controlName, const VecType & value, const VecType & min, const VecType & max, float width = defaultWidth, float height = defaultHeight);

    ofAbstractParameter & getParameter();

    VecType operator=(const VecType & v);
	operator const VecType & ();
	const VecType * operator->();
protected:
    void changeSlider(const void * parameter, float & value);
    void changeValue(VecType & value);
	static size_t dim();
    ofParameter<VecType> value;
    bool sliderChanging;
	ofEventListeners listeners;
};

typedef ofxVecSlider_<ofDefaultVec3> ofxVec3Slider;
typedef ofxVecSlider_<ofDefaultVec2> ofxVec2Slider;
typedef ofxVecSlider_<ofDefaultVec4> ofxVec4Slider;
typedef ofxVecSlider_<glm::vec3> ofxPointSlider;

template<typename ColorType>
class ofxColorSlider_: public ofxGuiGroup{

public:
	ofxColorSlider_(){
	    sliderChanging = false;
	};
	ofxColorSlider_(ofParameter<ofColor_<ColorType> > value, float width = defaultWidth, float height = defaultHeight);

	ofxColorSlider_ * setup(ofParameter<ofColor_<ColorType> > value, float width = defaultWidth, float height = defaultHeight);
	ofxColorSlider_ * setup(const std::string& controlName, const ofColor_<ColorType> & value, const ofColor_<ColorType> & min, const ofColor_<ColorType> & max, float width = defaultWidth, float height = defaultHeight);

	ofAbstractParameter & getParameter();

	ofColor_<ColorType> operator=(const ofColor_<ColorType> & v);
	operator const ofColor_<ColorType> & ();
protected:
	void onMinimize();
	void onMaximize();
    void changeSlider(const void * parameter, ColorType & value);
	void changeValue(ofColor_<ColorType> & value);
    bool sliderChanging;
	ofColor originalHeaderBackground;
	ofColor originalHeaderText;
	ofxColorPicker_<ColorType> picker;
	ofEventListeners listeners;
};
typedef ofxColorSlider_<unsigned char> ofxColorSlider;
typedef ofxColorSlider_<unsigned short> ofxShortColorSlider;
typedef ofxColorSlider_<float> ofxFloatColorSlider;

class ofxRectangleSlider: public ofxGuiGroup{

public:
	ofxRectangleSlider(){
	    sliderChanging = false;
	};
	ofxRectangleSlider(ofParameter<ofRectangle> value, float width = defaultWidth, float height = defaultHeight);

	ofxRectangleSlider * setup(ofParameter<ofRectangle>, float width = defaultWidth, float height = defaultHeight);
	ofxRectangleSlider * setup(const std::string& controlName, const ofRectangle & value, const ofRectangle & min, const ofRectangle & max, float width = defaultWidth, float height = defaultHeight);

	ofAbstractParameter & getParameter();

	ofRectangle operator=(const ofRectangle & v);
	operator const ofRectangle & ();
	const ofRectangle * operator->();
protected:
    ofParameter<ofRectangle> value;
    bool sliderChanging;
    void changeSlider(const void * parameter, float & value);
	void changeValue(ofRectangle & value);
	ofEventListeners listeners;
};
