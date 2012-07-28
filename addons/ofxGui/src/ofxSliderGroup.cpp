#include "ofxSliderGroup.h"

#pragma mark ofxSliderGroup

#pragma mark ofxVecSlider
template<class VecType>
ofxVecSlider<VecType>::ofxVecSlider(string controlName, ofParameter<VecType> value, float width, float height){
    this->setup(controlName, value);
}

template<class VecType>
ofxVecSlider<VecType> * ofxVecSlider<VecType>::setup(string controlName, ofParameter<VecType> value, float width, float height){
    ofxBaseGroup::setup(value.getName(), "", 0, 0);
    
    subParameters.clear();
    
    const string names[4] = {"x", "y", "z", "w"};
    
    this->value = value;
    this->value.addListener(this, & ofxVecSlider::changeValue);

    VecType val = value;
    VecType min = value.getMin();
    VecType max = value.getMax();
    
    for (int i=0; i<VecType::DIM; i++) {
        subParameters.push_back(ofParameter<float>(names[i], val[i], min[i], max[i]));
        this->add(new ofxSlider<float>(names[i], subParameters.back()));
        subParameters.back().addListener(this, & ofxVecSlider::changeSlider);
    }
    
    this->sliderChanging = false;
}

template<class VecType>
void ofxVecSlider<VecType>::changeSlider(const void * parameter, float & value){
    if (this->sliderChanging){
        cout << "feedback loop" << endl;
        return;
    }
    
    this->sliderChanging = true;
    
    cout << "{" << endl;
    VecType data = this->value;
    for (int i=0; i<VecType::DIM; i++){
        if (parameter == & this->subParameters[i]) {
            data[i] = value;
            cout << "match found at" << i << endl;
            break;
        }
    }
    cout << this->value << "->" << data << endl;
    this->value.setValue(data);
    cout << "..." << this->value << " [" << data << ", " << value << "]" << endl;
    cout << "}" << endl;
    
    this->sliderChanging = false;
}

template<class VecType>
void ofxVecSlider<VecType>::changeValue(VecType & value){
    for (int i=0; i<VecType::DIM; i++){
        this->subParameters[i] = value[i];
    }
}

template class ofxVecSlider<ofVec2f>;
template class ofxVecSlider<ofVec3f>;
template class ofxVecSlider<ofVec4f>;