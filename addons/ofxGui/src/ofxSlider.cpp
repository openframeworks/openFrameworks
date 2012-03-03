#include "ofxSlider.h"

template<typename Type>
ofxSlider<Type>::ofxSlider(string sliderName, ofxParameter<Type> _val, Type _min, Type _max, float width, float height){
	setup(sliderName,_val,_min,_max,width,height);
}

template<typename Type>
ofxSlider<Type>* ofxSlider<Type>::setup(string sliderName, ofxParameter<Type> _val, Type _min, Type _max, float width, float height){
	name = sliderName;
	value = _val;
	min = _min;
	max = _max;
	b.x = 0;
	b.y = 0;
	b.width = width;
	b.height = height;
	currentFrame = 0;
	bGuiActive = false;

	ofRegisterMouseEvents(this);
	return this;
}

template<typename Type>
void ofxSlider<Type>::mouseMoved(ofMouseEventArgs & args){
}

template<typename Type>
void ofxSlider<Type>::mousePressed(ofMouseEventArgs & args){
	setValue(args.x, args.y, true);
}

template<typename Type>
void ofxSlider<Type>::mouseDragged(ofMouseEventArgs & args){
	setValue(args.x, args.y, false);
}

template<typename Type>
void ofxSlider<Type>::mouseReleased(ofMouseEventArgs & args){
	bGuiActive = false;
}

template<typename Type>
void ofxSlider<Type>::saveToXml(ofxXmlSettings& xml) {
	string xmlName = name;
	ofStringReplace(xmlName," ","_");
	xml.setValue(xmlName, value);
}

template<typename Type>
void ofxSlider<Type>::loadFromXml(ofxXmlSettings& xml) {
	string xmlName = name;
	ofStringReplace(xmlName," ","_");
	value = xml.getValue(xmlName, value);
}

template<typename Type>
double ofxSlider<Type>::operator=(Type v){
	value = v;
	return v;
}

template<typename Type>
ofxSlider<Type>::operator Type & (){
	return value;
}

template<typename Type>
void ofxSlider<Type>::draw(){
	ofPushStyle();
	ofPushMatrix();

	currentFrame = ofGetFrameNum();
	ofFill();
	ofSetColor(backgroundColor);
	ofRect(b);

	ofTranslate(b.x, b.y);
	float valAsPct = ofMap( value, min, max, 0, b.width-2, true );
	ofEnableAlphaBlending();
	ofSetColor(fillColor);
	ofRect(1, 1, valAsPct, b.height-2);

	ofTranslate(0, b.height / 2 + 4);
	ofSetColor(textColor);
	ofDrawBitmapString(name, textPadding, 0);
	string valStr = ofToString(value.getValue());
	ofDrawBitmapString(valStr, b.width - textPadding - valStr.length() * 8, 0);

	ofPopMatrix();
	ofPopStyle();
}

template<typename Type>
void ofxSlider<Type>::setValue(float mx, float my, bool bCheck){
	if( ofGetFrameNum() - currentFrame > 1 ){
		bGuiActive = false;
		return;
	}
	if( bCheck ){
		if( b.inside(mx, my) ){
			bGuiActive = true;
		}else{
			bGuiActive = false;
		}
	}
	if( bGuiActive ){
		value = ofMap(mx, b.x, b.x + b.width, min, max, true);
	}
}


template class ofxSlider<int>;
template class ofxSlider<float>;
