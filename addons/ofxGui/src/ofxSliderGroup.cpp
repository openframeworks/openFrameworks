#include "ofxSliderGroup.h"
using namespace std;

template<class VecType>
ofxVecSlider_<VecType>::ofxVecSlider_(ofParameter<VecType> value, float width, float height){
	sliderChanging = false;
    setup(value, width, height);
}

template<class VecType>
ofxVecSlider_<VecType> * ofxVecSlider_<VecType>::setup(ofParameter<VecType> value, float width, float height){
    ofxGuiGroup::setup(value.getName(), "", 0, 0);
    
    parameters.clear();
    
    const string names[4] = {"x", "y", "z", "w"};
    
    this->value.makeReferenceTo(value);
    this->value.addListener(this, & ofxVecSlider_::changeValue);

    VecType val = value;
    VecType min = value.getMin();
    VecType max = value.getMax();
    
	for (int i=0; i<dim(); i++) {
    	ofParameter<float> p(names[i], val[i], min[i], max[i]);
        add(new ofxSlider<float>(p, width, height));
        p.addListener(this, & ofxVecSlider_::changeSlider);
    }

    sliderChanging = false;
    return this;

}

template<class VecType>
ofxVecSlider_<VecType> * ofxVecSlider_<VecType>::setup(const std::string& controlName, const VecType & v, const VecType & min, const VecType & max, float width, float height){
	value.set(controlName,v,min,max);
	return setup(value,width,height);
}

template<class VecType>
void ofxVecSlider_<VecType>::changeSlider(const void * parameter, float & _value){
    sliderChanging = true;
    ofParameter<float> & param = *(ofParameter<float>*)parameter;
    int i = parameters.getPosition(param.getName());
    VecType data = value;
    data[i] = _value;
    value = data;
    sliderChanging = false;
}

template<class VecType>
void ofxVecSlider_<VecType>::changeValue(VecType & value){
    if (sliderChanging){
        return;
    }
	for (int i=0; i<dim(); i++){
        parameters[i].template cast<float>() = value[i];
    }
}

template<>
size_t ofxVecSlider_<glm::vec2>::dim(){
	return 2;
}

template<>
size_t ofxVecSlider_<glm::vec3>::dim(){
	return 3;
}

template<>
size_t ofxVecSlider_<glm::vec4>::dim(){
	return 4;
}

template<>
size_t ofxVecSlider_<ofVec2f>::dim(){
	return 2;
}

template<>
size_t ofxVecSlider_<ofVec3f>::dim(){
	return 3;
}

template<>
size_t ofxVecSlider_<ofVec4f>::dim(){
	return 4;
}

template<class VecType>
ofAbstractParameter & ofxVecSlider_<VecType>::getParameter(){
	return value;
}

template<class VecType>
VecType ofxVecSlider_<VecType>::operator=(const VecType & v){
	value = v;
	return value;
}

template<class VecType>
ofxVecSlider_<VecType>::operator const VecType & (){
	return value;
}

template<class VecType>
const VecType * ofxVecSlider_<VecType>::operator->(){
	return &value.get();
}

template class ofxVecSlider_<ofVec2f>;
template class ofxVecSlider_<ofVec3f>;
template class ofxVecSlider_<ofVec4f>;
template class ofxVecSlider_<glm::vec2>;
template class ofxVecSlider_<glm::vec3>;
template class ofxVecSlider_<glm::vec4>;


template<class ColorType>
ofxColorSlider_<ColorType>::ofxColorSlider_(ofParameter<ofColor_<ColorType> > value, float width, float height){
	sliderChanging = false;
    setup(value, width, height);
}

template<class ColorType>
ofxColorSlider_<ColorType> * ofxColorSlider_<ColorType>::setup(ofParameter<ofColor_<ColorType> > value, float width, float height){
    ofxGuiGroup::setup(value.getName(), "", 0, 0);
    parameters.clear();

	const string names[4] = {"r", "g", "b", "a"};

    ofColor_<ColorType> val = value;
    ofColor_<ColorType> min = value.getMin();
    ofColor_<ColorType> max = value.getMax();

	picker.setup(value, width, width * 9. / 11.);

    for (int i=0; i<4; i++) {
    	ofParameter<ColorType> p(names[i], val[i], min[i], max[i]);
        add(new ofxSlider<ColorType>(p, width, height));
        p.addListener(this, & ofxColorSlider_::changeSlider);
		collection[i]->setFillColor(value.get());
		collection[i]->setTextColor(p / p.getMax() > 0.75 ? ofFloatColor(0.) : ofFloatColor(1.));
    }
	add(&picker);
	picker.getParameter().template cast<ofColor_<ColorType>>().addListener(this, & ofxColorSlider_::changeValue);


	sliderChanging = false;
	minimize();
    return this;
}


template<class ColorType>
ofxColorSlider_<ColorType> * ofxColorSlider_<ColorType>::setup(const std::string& controlName, const ofColor_<ColorType> & v, const ofColor_<ColorType> & min, const ofColor_<ColorType> & max, float width, float height){
	ofParameter<ofColor_<ColorType>> color(controlName, v, min, max);
	return setup(color,width,height);
}

template<class ColorType>
void ofxColorSlider_<ColorType>::changeSlider(const void * parameter, ColorType & _value){
    sliderChanging = true;
    ofParameter<float> & param = *(ofParameter<float>*)parameter;
    int i = parameters.getPosition(param.getName());
	ofColor_<ColorType> data = picker.getParameter().template cast<ofColor_<ColorType>>();
    data[i] = _value;
	picker.getParameter().template cast<ofColor_<ColorType>>() = data;


    for (int i=0; i<4; i++){
		collection[i]->setFillColor(data);
		auto p = parameters[i].template cast<ColorType>();
		collection[i]->setTextColor(p / p.getMax() > 0.75 ? ofFloatColor(0.) : ofFloatColor(1.));
	}
    sliderChanging = false;
}

template<class ColorType>
void ofxColorSlider_<ColorType>::changeValue(ofColor_<ColorType> & value){
    if (sliderChanging){
        return;
    }
    for (int i=0; i<4; i++){
        parameters[i].template cast<ColorType>() = value[i];
    	collection[i]->setFillColor(value);
		auto p = parameters[i].template cast<ColorType>();
		collection[i]->setTextColor(p / p.getMax() > 0.75 ? ofFloatColor(0.) : ofFloatColor(1.));
	}
}

template<class ColorType>
ofAbstractParameter & ofxColorSlider_<ColorType>::getParameter(){
	return picker.getParameter();
}

template<class ColorType>
ofColor_<ColorType> ofxColorSlider_<ColorType>::operator=(const ofColor_<ColorType> & v){
	picker.getParameter().template cast<ofColor_<ColorType>>() = v;
	return picker.getParameter().template cast<ofColor_<ColorType>>();
}

template<class ColorType>
ofxColorSlider_<ColorType>::operator const ofColor_<ColorType> & (){
	return picker.getParameter().template cast<ofColor_<ColorType>>();
}

template<class ColorType>
void ofxColorSlider_<ColorType>::onMinimize(){
	originalHeaderBackground = thisHeaderBackgroundColor;
	originalHeaderText = thisTextColor;
	setHeaderBackgroundColor(picker.getParameter().template cast<ofColor_<ColorType>>().get());
	setTextColor(picker.getColorScale() > 0.5 ? ofFloatColor(0.) : ofFloatColor(1.));
	setNeedsRedraw();
}

template<class ColorType>
void ofxColorSlider_<ColorType>::onMaximize(){
	setHeaderBackgroundColor(originalHeaderBackground);
	setTextColor(originalHeaderText);
	setNeedsRedraw();
}

template class ofxColorSlider_<unsigned char>;
template class ofxColorSlider_<unsigned short>;
template class ofxColorSlider_<float>;
