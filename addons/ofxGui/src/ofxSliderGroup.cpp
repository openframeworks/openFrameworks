#include "ofxSliderGroup.h"
using namespace std;

template<class VecType>
ofxVecSlider_<VecType>::ofxVecSlider_(const ofJson &config)
// TODO the following line doesn't work (segfault) no clue why
//	:ofxGuiGroup(getName(), config){
	:ofxGuiGroup("", config){

	setup(config);

}

template<class VecType>
ofxVecSlider_<VecType>::ofxVecSlider_(ofParameter<VecType> &value, const ofJson & config)
:ofxVecSlider_(config){

	setName(value.getName());

	const string names[4] = {"x", "y", "z", "w"};

	this->value.makeReferenceTo(value);
	this->value.addListener(this, & ofxVecSlider_::changeValue);

	VecType val = value;
	VecType min = value.getMin();
	VecType max = value.getMax();
	for (int i=0; i<VecType::DIM; i++) {
		ofParameter<float> p(names[i], val[i], min[i], max[i]);
		add(p);
		p.addListener(this, & ofxVecSlider_::changeSlider);
	}

}

template<class VecType>
ofxVecSlider_<VecType>::ofxVecSlider_(ofParameter<VecType> &value, float width, float height)
	:ofxVecSlider_(value){

	setSize(width, height);

}

template<class VecType>
ofxVecSlider_<VecType>::ofxVecSlider_(const std::string& controlName, const VecType & v, const VecType & min, const VecType & max, float width, float height)
	:ofxVecSlider_(){

	value.set(controlName,v,min,max);

	const string names[4] = {"x", "y", "z", "w"};

	this->value.addListener(this, & ofxVecSlider_::changeValue);

	VecType val = value;
	for (int i=0; i<VecType::DIM; i++) {
		ofParameter<float> p(names[i], val[i], min[i], max[i]);
		add(p);
		p.addListener(this, & ofxVecSlider_::changeSlider);
	}

	setSize(width, height);

}

template<class VecType>
void ofxVecSlider_<VecType>::setup(const ofJson &config){

	sliderChanging = false;

}

template<class VecType>
void ofxVecSlider_<VecType>::changeSlider(const void * parameter, float & _value){
	sliderChanging = true;
	ofParameter<float> & param = *(ofParameter<float>*)parameter;
	int i = getControlIndex(param.getName());
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
	for (int i=0; i<VecType::DIM; i++){
		getControl(i)->getParameter().template cast<float>() = value[i];
	}
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


template<class ColorType>
ofxColorSlider_<ColorType>::ofxColorSlider_(const ofJson &config)
// TODO why is this not working?
//	:ofxGuiGroup(getName(), config){
	:ofxGuiGroup("", config){

	setup(config);

}

template<class ColorType>
ofxColorSlider_<ColorType>::ofxColorSlider_(ofParameter<ofColor_<ColorType> > &value, const ofJson & config)
	:ofxColorSlider_(config){

	setName(value.getName());

	const string names[4] = {"r", "g", "b", "a"};

	this->value.makeReferenceTo(value);
	this->value.addListener(this, & ofxColorSlider_::changeValue);

	ofColor_<ColorType> val = value;
	ofColor_<ColorType> min = value.getMin();
	ofColor_<ColorType> max = value.getMax();

	for (int i=0; i<4; i++) {
		ofParameter<ColorType> p(names[i], val[i], min[i], max[i]);
		add<ofxSlider<ColorType>>(p);
		p.addListener(this, & ofxColorSlider_::changeSlider);
		getControl(i)->setFillColor(value.get());
	}

	sliderChanging = false;

}

template<class ColorType>
ofxColorSlider_<ColorType>::ofxColorSlider_(ofParameter<ofColor_<ColorType> > &value, float width, float height)
	:ofxColorSlider_(value){

	setSize(width, height);

}

template<class ColorType>
ofxColorSlider_<ColorType>::ofxColorSlider_(const std::string& controlName, const ofColor_<ColorType> & v, const ofColor_<ColorType> & min, const ofColor_<ColorType> & max, float width, float height)
	:ofxColorSlider_(){

	value.set(controlName,v,min,max);

	const string names[4] = {"r", "g", "b", "a"};

	this->value.addListener(this, & ofxColorSlider_::changeValue);

	ofColor_<ColorType> val = value;

	for (int i=0; i<4; i++) {
		ofParameter<ColorType> p(names[i], val[i], min[i], max[i]);
		add<ofxSlider<ColorType>>(p);
		p.addListener(this, & ofxColorSlider_::changeSlider);
		getControl(i)->setFillColor(value.get());
	}

	sliderChanging = false;

	setSize(width, height);

}

template<class ColorType>
void ofxColorSlider_<ColorType>::setup(const ofJson &config){

	sliderChanging = false;

}

template<class ColorType>
void ofxColorSlider_<ColorType>::changeSlider(const void * parameter, ColorType & _value){
	sliderChanging = true;
	ofParameter<float> & param = *(ofParameter<float>*)parameter;
	int i = getControlIndex(param.getName());
	ofColor_<ColorType> data = value;
	data[i] = _value;
	value = data;


	for (int i=0; i<4; i++){
		getControl(i)->setFillColor(value.get());
	}
	sliderChanging = false;
}

template<class ColorType>
void ofxColorSlider_<ColorType>::changeValue(ofColor_<ColorType> & value){
	if (sliderChanging){
		return;
	}
	for (int i=0; i<4; i++){
		getControl(i)->getParameter().template cast<ColorType>() = value[i];
		getControl(i)->setFillColor(value);
	}
}

template<class ColorType>
ofAbstractParameter & ofxColorSlider_<ColorType>::getParameter(){
	return value;
}

template<class ColorType>
ofColor_<ColorType> ofxColorSlider_<ColorType>::operator=(const ofColor_<ColorType> & v){
	value = v;
	return value;
}

template<class ColorType>
ofxColorSlider_<ColorType>::operator const ofColor_<ColorType> & (){
	return value;
}

template class ofxColorSlider_<unsigned char>;
template class ofxColorSlider_<unsigned short>;
template class ofxColorSlider_<float>;
