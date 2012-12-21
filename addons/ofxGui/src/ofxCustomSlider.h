#pragma once

#include "ofxBaseGui.h"
#include "ofParameter.h"

template<typename Type>
class ofxCustomSlider : public ofxBaseGui{
	friend class ofPanel;
	
public:	
	ofxCustomSlider(){}
	ofxCustomSlider(string sliderName, int _val, int _min, int _max, float width = defaultWidth, float height = defaultHeight);
	ofxCustomSlider* setup(string sliderName, int _val, int _min, int _max, float width = defaultWidth, float height = defaultHeight);

	virtual void mouseMoved(ofMouseEventArgs & args);
	virtual void mousePressed(ofMouseEventArgs & args);
	virtual void mouseDragged(ofMouseEventArgs & args);
	virtual void mouseReleased(ofMouseEventArgs & args);
	
	virtual void saveToXml(ofxXmlSettings& xml);
	virtual void loadFromXml(ofxXmlSettings& xml);

	template<class ListenerClass>
	void addListener(ListenerClass * listener, void ( ListenerClass::*method )(Type&)){
		value.addListener(listener,method);
	}

	template<class ListenerClass>
	void removeListener(ListenerClass * listener, void ( ListenerClass::*method )(Type&)){
		value.removeListener(listener,method);
	}

	void setByIndex(int i);
	Type operator=(Type v);
	operator Type & ();

	void draw();

	ofxParameter<Type> value;

protected:
	virtual Type getValue(int i) const=0;
	virtual string getLabel(int i) const;
	virtual int getIndexOf(Type v) const;

protected:
	int index, min, max;
	
	void setValue(float mx, float my, bool bCheck);
};

template<typename Type>
ofxCustomSlider<Type>::ofxCustomSlider(string sliderName, int _val, int _min, int _max, float width, float height){
	setup(sliderName,_val,_min,_max,width,height);
}

template<typename Type>
ofxCustomSlider<Type>* ofxCustomSlider<Type>::setup(string sliderName, int _val, int _min, int _max, float width, float height){
	name = sliderName;
	index = _val;
	min = _min;
	max = _max;
	b.x = 0;
	b.y = 0;
	b.width = width;
	b.height = height;
	currentFrame = ofGetFrameNum();
	bGuiActive = false;

	ofRegisterMouseEvents(this);

	value = getValue(_val);
	return this;
}

template<typename Type>
void ofxCustomSlider<Type>::mouseMoved(ofMouseEventArgs & args){
}

template<typename Type>
void ofxCustomSlider<Type>::mousePressed(ofMouseEventArgs & args){
	setValue(args.x, args.y, true);
}

template<typename Type>
void ofxCustomSlider<Type>::mouseDragged(ofMouseEventArgs & args){
	setValue(args.x, args.y, false);
}

template<typename Type>
void ofxCustomSlider<Type>::mouseReleased(ofMouseEventArgs & args){
	bGuiActive = false;
}

template<typename Type>
void ofxCustomSlider<Type>::saveToXml(ofxXmlSettings& xml) {
	string xmlName = name;
	ofStringReplace(xmlName," ","_");
	ofStringReplace(xmlName,",","_");
	ofStringReplace(xmlName,"(","_");
	ofStringReplace(xmlName,")","_");
	ofStringReplace(xmlName,"/","_");
	xml.setValue(xmlName, value);
}

template<typename Type>
void ofxCustomSlider<Type>::loadFromXml(ofxXmlSettings& xml) {
	string xmlName = name;
	ofStringReplace(xmlName," ","_");
	ofStringReplace(xmlName,",","_");
	ofStringReplace(xmlName,"(","_");
	ofStringReplace(xmlName,")","_");
	ofStringReplace(xmlName,"/","_");
	value = xml.getValue(xmlName, value);
}

template<typename Type>
Type ofxCustomSlider<Type>::setByIndex(int i){
	index = i;
	value = getValue(i);
	return value;
}

template<typename Type>
Type ofxCustomSlider<Type>::operator=(Type v){
	index = getIndexOf(v);
	value = v;
	return v;
}

template<typename Type>
ofxCustomSlider<Type>::operator Type & (){
	return value;
}

template<typename Type>
void ofxCustomSlider<Type>::draw(){
	ofPushStyle();
	ofPushMatrix();

	currentFrame = ofGetFrameNum();
	ofFill();
	ofSetColor(backgroundColor);
	ofRect(b);

	ofTranslate(b.x, b.y);
	float valAsPct = ofMap( index, min, max, 0, b.width-2, true );
	ofEnableAlphaBlending();
	ofSetColor(fillColor);
	ofRect(1, 1, valAsPct, b.height-2);

	ofTranslate(0, b.height / 2 + 4);
	ofSetColor(textColor);
	ofDrawBitmapString(name, textPadding, 0);
	string valStr = getLabel(index);
	ofDrawBitmapString(valStr, b.width - textPadding - valStr.length() * 8, 0);

	ofPopMatrix();
	ofPopStyle();
}

template<typename Type>
void ofxCustomSlider<Type>::setValue(float mx, float my, bool bCheck){
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
		index = (int)ofMap(mx, b.x, b.x + b.width, min, max, true);
		value = getValue(index);
	}
}

template<typename Type>
string ofxCustomSlider<Type>::getLabel(int i) const{
	return ofToString(getValue(i));
}

template<typename Type>
int ofxCustomSlider<Type>::getIndexOf(Type v) const{
	for(int i = min; i <= max; ++i) {
		if(getValue(i) == v) {
			return i;
		}
	}
	return min-1;
}
