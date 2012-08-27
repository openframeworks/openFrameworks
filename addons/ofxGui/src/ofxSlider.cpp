#include "ofxSlider.h"

template<typename Type>
ofxSlider<Type>::ofxSlider(string sliderName, ofParameter<Type> _val, float width, float height){
	setup(sliderName,_val,width,height);
}

template<typename Type>
ofxSlider<Type>::ofxSlider(string sliderName, Type _val, Type _min, Type _max, float width, float height){
	setup(sliderName,_val,_min,_max,width,height);
}

template<typename Type>
ofxSlider<Type>* ofxSlider<Type>::setup(string sliderName, ofParameter<Type> _val, float width, float height){
	name = sliderName;
	value = _val;
	b.x = 0;
	b.y = 0;
	b.width = width;
	b.height = height;
	currentFrame = ofGetFrameNum();
	bGuiActive = false;

	ofRegisterMouseEvents(this);
	return this;
}

template<typename Type>
ofxSlider<Type>* ofxSlider<Type>::setup(string sliderName, Type _val, Type _min, Type _max, float width, float height){
	ofParameter<Type> v(sliderName,_val,_min,_max);
	return setup(sliderName,v,width,height);
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
double ofxSlider<Type>::operator=(Type v){
	value = v;
	return v;
}

template<typename Type>
ofxSlider<Type>::operator const Type & (){
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
	float valAsPct = ofMap( value, value.getMin(), value.getMax(), 0, b.width-2, true );
	ofEnableAlphaBlending();
	ofSetColor(fillColor);
	ofRect(1, 1, valAsPct, b.height-2);

	ofTranslate(0, b.height / 2 + 4);
	ofSetColor(textColor);
	ofDrawBitmapString(name, textPadding, 0);
	string valStr = ofToString(value.get());
	ofDrawBitmapString(valStr, b.width - textPadding - valStr.length() * 8, 0);

	ofPopMatrix();
	ofPopStyle();
}


template<>
void ofxSlider<unsigned char>::draw(){
	ofPushStyle();
	ofPushMatrix();

	currentFrame = ofGetFrameNum();
	ofFill();
	ofSetColor(thisBackgroundColor);
	ofRect(b);

	ofTranslate(b.x, b.y);
	float valAsPct = ofMap( value, value.getMin(), value.getMax(), 0, b.width-2, true );
	ofEnableAlphaBlending();
	ofSetColor(thisFillColor);
	ofRect(1, 1, valAsPct, b.height-2);

	ofTranslate(0, b.height / 2 + 4);
	ofSetColor(thisTextColor);
	ofDrawBitmapString(name, textPadding, 0);
	string valStr = ofToString((int)value.get());
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
		value = ofMap(mx, b.x, b.x + b.width, value.getMin(), value.getMax(), true);
	}
}

template<typename Type>
ofAbstractParameter & ofxSlider<Type>::getParameter(){
	return value;
}


template class ofxSlider<int>;
template class ofxSlider<unsigned int>;
template class ofxSlider<float>;
template class ofxSlider<double>;
template class ofxSlider<signed char>;
template class ofxSlider<unsigned char>;
template class ofxSlider<unsigned short>;
template class ofxSlider<short>;
