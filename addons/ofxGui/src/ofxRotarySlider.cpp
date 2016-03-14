#include "ofxRotarySlider.h"
#include "ofGraphics.h"
#include "ofAppRunner.h"
using namespace std;

template <typename Type> ofxRotarySlider <Type>::ofxRotarySlider(const ofJson & config)
	:ofxSlider<Type>(config){
	this->registerPointerEvents();
}

template <typename Type> ofxRotarySlider <Type>::~ofxRotarySlider(){
	this->unregisterPointerEvents();
}

template <typename Type> ofxRotarySlider <Type>::ofxRotarySlider(ofParameter <Type> _val, const ofJson & config) :
	ofxRotarySlider <Type>(config){
	this->value.makeReferenceTo(_val);
}

template <typename Type>
void ofxRotarySlider <Type>::generateDraw(){

	float inner_r = min(this->getShape().width, this->getShape().height) / 6;
	float outer_r = min(this->getShape().width, this->getShape().height) / 2-1;

	this->bg.clear();
	this->bar.clear();
	this->border.clear();

	this->border.setStrokeColor(this->borderColor);
	this->border.setStrokeWidth(1);
	this->border.setFilled(false);
	arcStrip(this->border, ofPoint(this->getWidth()/2, this->getHeight()/2), outer_r, inner_r, 1);

	this->bg.setFillColor(this->backgroundColor);
	this->bg.setFilled(true);
	arcStrip(this->bg, ofPoint(this->getWidth()/2, this->getHeight()/2), outer_r-1, inner_r+1, 1);

	float val = ofMap(this->value, this->value.getMin(), this->value.getMax(), 0, 1);
	this->bar.setFillColor(this->fillColor);
	this->bar.setFilled(true);
	arcStrip(this->bar, ofPoint(this->getWidth()/2, this->getHeight()/2), outer_r - 1, inner_r + 1, val);

	generateText();
}


template <typename Type>
void ofxRotarySlider <Type>::generateText(){
	string valStr = ofToString(this->value.get(), this->precision);
	this->textMesh.clear();
	if(this->showName){
		this->textMesh.append(this->getTextMesh(this->getName(), this->textPadding, this->getShape().height - this->textPadding));
	}
	this->textMesh.append(this->getTextMesh(valStr, this->getShape().width - this->textPadding - this->getTextBoundingBox(valStr, 0, 0).width, this->getShape().height - this->textPadding));
}

template <>
void ofxRotarySlider <unsigned char>::generateText(){
	string valStr = ofToString(this->value.get(), precision);
	this->textMesh.clear();
	if(this->showName){
		this->textMesh.append(this->getTextMesh(this->getName(), this->textPadding, this->getShape().height - this->textPadding));
	}
	this->textMesh.append(this->getTextMesh(valStr, this->getShape().width - this->textPadding - this->getTextBoundingBox(valStr, 0, 0).width, this->getShape().height - this->textPadding));
}

template <typename Type>
void ofxRotarySlider <Type>::render(){
	ofColor c = ofGetStyle().color;

	this->bg.draw();
	this->bar.draw();
	this->border.draw();

	ofBlendMode blendMode = ofGetStyle().blendingMode;
	if(blendMode != OF_BLENDMODE_ALPHA){
		ofEnableAlphaBlending();
	}
	ofSetColor(this->textColor);

	this->bindFontTexture();
	this->textMesh.draw();
	this->unbindFontTexture();

	ofSetColor(c);
	if(blendMode != OF_BLENDMODE_ALPHA){
		ofEnableBlendMode(blendMode);
	}
}

/*
 * adapted from ofxUI by Reza Ali (www.syedrezaali.com || syed.reza.ali@gmail.com || @rezaali)
 *
 */
template <typename Type>
void ofxRotarySlider <Type>::arcStrip(ofPath & path, ofPoint center, float outer_radius, float inner_radius, float percent){
	float theta = ofMap(percent, 0, 1, 0, 360.0, true);

	{
		float x = sin(-ofDegToRad(0));
		float y = cos(-ofDegToRad(0));
		path.moveTo(center.x + outer_radius * x, center.y + outer_radius * y);
	}

	for(int i = 0; i <= theta; i += 10){
		float x = sin(-ofDegToRad(i));
		float y = cos(-ofDegToRad(i));

		path.lineTo(center.x + outer_radius * x, center.y + outer_radius * y);
	}

	{
		float x = sin(-ofDegToRad(theta));
		float y = cos(-ofDegToRad(theta));
		path.lineTo(center.x + outer_radius * x, center.y + outer_radius * y);
		path.lineTo(center.x + inner_radius * x, center.y + inner_radius * y);
	}

	for(int i = theta; i >= 0; i -= 10){
		float x = sin(-ofDegToRad(i));
		float y = cos(-ofDegToRad(i));

		path.lineTo(center.x + inner_radius * x, center.y + inner_radius * y);
	}

	{
		float x = sin(-ofDegToRad(0));
		float y = cos(-ofDegToRad(0));
		path.lineTo(center.x + inner_radius * x, center.y + inner_radius * y);
	}

	path.close();
}

template <typename Type>
void ofxRotarySlider <Type>::pointerPressed(PointerUIEventArgs & args){

	args.setCurrentTarget(this);
	Type firstClickVal = ofMap(args.localPosition().y, this->getShape().getHeight(), 0, 0, 1, true);
	Type lastVal = ofMap(this->value, this->value.getMin(), this->value.getMax(), 0, 1, true);
	_mouseOffset = (firstClickVal - lastVal) * this->getShape().height;
	ofxSlider<Type>::pointerPressed(args);

}

template <typename Type>
bool ofxRotarySlider <Type>::setValue(float mx, float my){

	ofPoint pos = this->screenToLocal(ofPoint(mx,my));

	Type res = ofMap(pos.y,
					 this->getHeight() - _mouseOffset,
					 - _mouseOffset,
					 this->value.getMin(),
					 this->value.getMax(),
					 true);
	this->value.set(res);
	this->setNeedsRedraw();
	return true;
}

template class ofxRotarySlider <int>;
template class ofxRotarySlider <unsigned int>;
template class ofxRotarySlider <float>;
template class ofxRotarySlider <double>;
template class ofxRotarySlider <signed char>;
template class ofxRotarySlider <unsigned char>;
template class ofxRotarySlider <unsigned short>;
template class ofxRotarySlider <short>;
