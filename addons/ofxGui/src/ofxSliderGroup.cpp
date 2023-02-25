#include "ofxSliderGroup.h"

template<class VecType>
ofxVecSlider_<VecType>::ofxVecSlider_(ofParameter<VecType> value, float width, float height){
	sliderChanging = false;
    setup(value, width, height);
}

template<class VecType>
ofxVecSlider_<VecType> * ofxVecSlider_<VecType>::setup(ofParameter<VecType> value, float width, float height){
    ofxGuiGroup::setup(value.getName(), "", 0, 0);
    
    parameters.clear();
	listeners.unsubscribeAll();
    const std::string names[4] = {"x", "y", "z", "w"};
    
    this->value.makeReferenceTo(value);
	listeners.push(this->value.newListener(this, & ofxVecSlider_::changeValue));

    VecType val = value;
    VecType min = value.getMin();
    VecType max = value.getMax();

	for (size_t i = 0; i < dim(); i++) {
    	ofParameter<float> p(names[i], val[i], min[i], max[i]);
		add(createGuiElement<ofxSlider<float>>(p, width, height));
        listeners.push(p.newListener(this, & ofxVecSlider_::changeSlider));
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
	for (size_t i = 0; i < dim(); i++){
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

	const std::string names[4] = {"r", "g", "b", "a"};

    ofColor_<ColorType> val = value;
    ofColor_<ColorType> min = value.getMin();
    ofColor_<ColorType> max = value.getMax();

	picker.setup(value, width, width * 9. / 11.);
	
	listeners.unsubscribeAll();
    for (int i=0; i<4; i++) {
    	ofParameter<ColorType> p(names[i], val[i], min[i], max[i]);
        add(createGuiElement<ofxSlider<ColorType>>(p, width, height));
		listeners.push(p.newListener(this, & ofxColorSlider_::changeSlider));
		collection[i]->setFillColor(value.get());
        float range = p.getMax()-p.getMin();
        if(range == 0){
            collection[i]->setTextColor( ofFloatColor(0.));
        }else{
            collection[i]->setTextColor( p/range > 0.75 ? ofFloatColor(0.) : ofFloatColor(1.));
        }
    }
	add(&picker);
	listeners.push(picker.getParameter().template cast<ofColor_<ColorType>>().newListener(this, & ofxColorSlider_::changeValue));


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
        float range = p.getMax()-p.getMin();
        if(range == 0){
            collection[i]->setTextColor( ofFloatColor(0.));
        }else{
            collection[i]->setTextColor( p/range > 0.75 ? ofFloatColor(0.) : ofFloatColor(1.));
        }
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
        float range = p.getMax()-p.getMin();
        if(range == 0){
            collection[i]->setTextColor( ofFloatColor(0.));
        }else{
            collection[i]->setTextColor( p/range > 0.75 ? ofFloatColor(0.) : ofFloatColor(1.));
        }
	}
    if(isMinimized()){
        setHeaderBackgroundColor(value);
        float b = value.getBrightness() / value.limit();
        setTextColor(b > 0.75 ? ofFloatColor(0.0) : ofFloatColor(1.0));
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


//--------------------
ofxRectangleSlider::ofxRectangleSlider(ofParameter<ofRectangle> value, float width, float height){
	sliderChanging = false;
    setup(value, width, height);
}


ofxRectangleSlider * ofxRectangleSlider::setup(ofParameter<ofRectangle> value, float width, float height){
    ofxGuiGroup::setup(value.getName(), "", 0, 0);
    
    parameters.clear();
    
//    const string names[4] = {"x", "y", "width", "height"};
	
	listeners.unsubscribeAll();
    this->value.makeReferenceTo(value);
    listeners.push(this->value.newListener(this, & ofxRectangleSlider::changeValue));

    ofRectangle val = value;
    ofRectangle min = value.getMin();
    ofRectangle max = value.getMax();
    
	// adding a [] operator to ofRectangle would be handy for this situation
//	for (size_t i = 0; i < 4; i++) {
//    	ofParameter<float> p(names[i], val[i], min[i], max[i]);
//        add(createGuiElement<ofxSlider<float>>(p, width, height));
//        p.addListener(this, & ofxRectangleSlider::changeSlider);
//    }
    	ofParameter<float> x("x", val.x, min.x, max.x);
        add(createGuiElement< ofxSlider<float> >(x, width, height));
        listeners.push(x.newListener(this, & ofxRectangleSlider::changeSlider));
	
		ofParameter<float> y("y", val.y, min.y, max.y);
        add(createGuiElement<ofxSlider<float>>(y, width, height));
        listeners.push(y.newListener(this, & ofxRectangleSlider::changeSlider));

    	ofParameter<float> w("width", val.width, min.width, max.width);
        add(createGuiElement<ofxSlider<float>>(w, width, height));
        listeners.push(w.newListener(this, & ofxRectangleSlider::changeSlider));

		ofParameter<float> h("height", val.height, min.height, max.height);
        add(createGuiElement<ofxSlider<float>>(h, width, height));
        listeners.push(h.newListener(this, & ofxRectangleSlider::changeSlider));


    sliderChanging = false;
    return this;

}

ofxRectangleSlider * ofxRectangleSlider::setup(const std::string& controlName, const ofRectangle & v, const ofRectangle & min, const ofRectangle & max, float width, float height){
	value.set(controlName,v,min,max);
	return setup(value,width,height);
}


void ofxRectangleSlider::changeSlider(const void * parameter, float & _value){
    sliderChanging = true;
    ofParameter<float> & param = *(ofParameter<float>*)parameter;
	
    ofRectangle data = value;
	auto _name = param.getName();
	if(_name == "x"){data.x = _value;}
	else if(_name == "y"){data.y = _value;}
	else if(_name == "width"){data.width = _value;}
	else if(_name == "height"){data.height = _value;}
    value = data;
    sliderChanging = false;
}


void ofxRectangleSlider::changeValue(ofRectangle & value){
    if (sliderChanging){
        return;
    }
	parameters[0].template cast<float>() = value.x;
	parameters[1].template cast<float>() = value.y;
	parameters[2].template cast<float>() = value.width;
	parameters[3].template cast<float>() = value.height;
}

ofAbstractParameter & ofxRectangleSlider::getParameter(){
	return value;
}


ofRectangle ofxRectangleSlider::operator=(const ofRectangle & v){
	value = v;
	return value;
}


ofxRectangleSlider::operator const ofRectangle & (){
	return value;
}


const ofRectangle * ofxRectangleSlider::operator->(){
	return &value.get();
}

