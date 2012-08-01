#include "ofxSliderGroup.h"

template<class VecType>
ofxVecSlider<VecType>::ofxVecSlider(string controlName, ofParameter<VecType> value, float width, float height){
	sliderChanging = false;
    setup(controlName, value);
}

template<class VecType>
ofxVecSlider<VecType> * ofxVecSlider<VecType>::setup(string controlName, ofParameter<VecType> value, float width, float height){
    ofxGuiGroup::setup(value.getName(), "", 0, 0);
    
    parameters.clear();
    
    const string names[4] = {"x", "y", "z", "w"};
    
    this->value = value;
    this->value.addListener(this, & ofxVecSlider::changeValue);

    VecType val = value;
    VecType min = value.getMin();
    VecType max = value.getMax();
    
    for (int i=0; i<VecType::DIM; i++) {
    	ofParameter<float> p(names[i], val[i], min[i], max[i]);
        add(new ofxSlider<float>(names[i], p));
        p.addListener(this, & ofxVecSlider::changeSlider);
    }

    sliderChanging = false;
    return this;
}

template<class VecType>
void ofxVecSlider<VecType>::changeSlider(const void * parameter, float & _value){
    sliderChanging = true;
    ofParameter<float> & param = *(ofParameter<float>*)parameter;
    int i = parameters.getPosition(param.getName());
    VecType data = value;
    data[i] = _value;
    value = data;
    sliderChanging = false;
}

template<class VecType>
void ofxVecSlider<VecType>::changeValue(VecType & value){
    if (sliderChanging){
        return;
    }
    for (int i=0; i<VecType::DIM; i++){
        parameters[i].cast<float>() = value[i];
    }
}

template<class VecType>
ofAbstractParameter & ofxVecSlider<VecType>::getParameter(){
	return value;
}

template class ofxVecSlider<ofVec2f>;
template class ofxVecSlider<ofVec3f>;
template class ofxVecSlider<ofVec4f>;


template<class ColorType>
ofxColorSlider<ColorType>::ofxColorSlider(string controlName, ofParameter<ofColor_<ColorType> > value, float width, float height){
	sliderChanging = false;
    setup(controlName, value);
}

template<class ColorType>
ofxColorSlider<ColorType> * ofxColorSlider<ColorType>::setup(string controlName, ofParameter<ofColor_<ColorType> > value, float width, float height){
    ofxGuiGroup::setup(value.getName(), "", 0, 0);

    parameters.clear();

    const string names[4] = {"r", "g", "b", "a"};

    this->value = value;
    this->value.addListener(this, & ofxColorSlider::changeValue);

    ofColor_<ColorType> val = value;
    ofColor_<ColorType> min = value.getMin();
    ofColor_<ColorType> max = value.getMax();

    for (int i=0; i<4; i++) {
    	ofParameter<ColorType> p(names[i], val[i], min[i], max[i]);
        add(new ofxSlider<ColorType>(names[i], p));
        p.addListener(this, & ofxColorSlider::changeSlider);
    }

    sliderChanging = false;
    return this;
}

template<class ColorType>
void ofxColorSlider<ColorType>::changeSlider(const void * parameter, ColorType & _value){
    sliderChanging = true;
    ofParameter<float> & param = *(ofParameter<float>*)parameter;
    int i = parameters.getPosition(param.getName());
    ofColor_<ColorType> data = value;
    data[i] = _value;
    value = data;


    for (int i=0; i<4; i++){
    	collection[i]->setFillColor(value.get());
    }
    sliderChanging = false;
}

template<class ColorType>
void ofxColorSlider<ColorType>::changeValue(ofColor_<ColorType> & value){
    if (sliderChanging){
        return;
    }
    for (int i=0; i<4; i++){
        parameters[i].cast<ColorType>() = value[i];
    	collection[i]->setFillColor(value);
    }
}

template<class ColorType>
ofAbstractParameter & ofxColorSlider<ColorType>::getParameter(){
	return value;
}

template class ofxColorSlider<unsigned char>;
template class ofxColorSlider<unsigned short>;
template class ofxColorSlider<float>;
