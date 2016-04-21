#include "ofxRotarySlider.h"
#include "ofGraphics.h"
#include "ofAppRunner.h"
using namespace std;

template <typename Type> ofxRotarySlider <Type>::ofxRotarySlider(){
	this->setSize(this->getWidth(), Config().shape.height);
}

template <typename Type> ofxRotarySlider <Type>::~ofxRotarySlider(){
}

template <typename Type> ofxRotarySlider <Type>::ofxRotarySlider(ofParameter <Type> _val, const Config & config) :
	ofxSlider <Type>(_val, config){
}

template <typename Type>
void ofxRotarySlider <Type>::generateDraw(){

	float inner_r = min(this->b.width, this->b.height) / 6;
	float outer_r = min(this->b.width, this->b.height) / 2-1;

	this->bg.clear();
	this->bar.clear();
	this->border.clear();

	this->border.setStrokeColor(this->thisBorderColor);
	this->border.setStrokeWidth(1);
	this->border.setFilled(false);
	arcStrip(this->border, this->b.getCenter(), outer_r, inner_r, 1);

	this->bg.setFillColor(this->thisBackgroundColor);
	this->bg.setFilled(true);
	arcStrip(this->bg, this->b.getCenter(), outer_r-1, inner_r+1, 1);

	float val = ofMap(this->value, this->value.getMin(), this->value.getMax(), 0, 1);
	this->bar.setFillColor(this->thisFillColor);
	this->bar.setFilled(true);
	arcStrip(this->bar, this->b.getCenter(), outer_r - 1, inner_r + 1, val);

	generateText();
}


template <typename Type>
void ofxRotarySlider <Type>::generateText(){
	string valStr = ofToString(this->value.get(), this->precision);
	this->textMesh.clear();
	if(this->bShowName){
		this->textMesh.append(this->getTextMesh(this->getName(), this->b.x + this->textPadding, this->b.y + this->b.height - this->textPadding));
	}
	this->textMesh.append(this->getTextMesh(valStr, this->b.x + this->b.width - this->textPadding - this->getTextBoundingBox(valStr, 0, 0).width, this->b.y + this->b.height - this->textPadding));
}

template <>
void ofxRotarySlider <unsigned char>::generateText(){
	string valStr = ofToString(this->value.get(), precision);
	this->textMesh.clear();
	if(this->bShowName){
		this->textMesh.append(this->getTextMesh(this->getName(), this->b.x + this->textPadding, this->b.y + this->b.height - this->textPadding));
	}
	this->textMesh.append(this->getTextMesh(valStr, this->b.x + this->b.width - this->textPadding - this->getTextBoundingBox(valStr, 0, 0).width, this->b.y + this->b.height - this->textPadding));
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
	ofSetColor(this->thisTextColor);

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
bool ofxRotarySlider <Type>::mousePressed(ofMouseEventArgs & args){
	Type firstClickVal = ofMap(args.y, this->b.getBottom(), this->b.getTop(), 0, 1, true);
	Type lastVal = ofMap(this->value, this->value.getMin(), this->value.getMax(), 0, 1, true);
	_mouseOffset = (firstClickVal - lastVal) * this->b.height;
	return ofxSlider <Type>::mousePressed(args);
}

template <typename Type>
bool ofxRotarySlider <Type>::setValue(float mx, float my, bool bCheck){
	if(!this->isGuiDrawing()){
		this->bGuiActive = false;
		return false;
	}
	if(bCheck){
		if(this->b.inside(mx, my)){
			this->bGuiActive = true;
		}else{
			this->bGuiActive = false;
		}
	}
	if(this->bGuiActive){
		this->value = ofMap(my, this->b.getBottom() - _mouseOffset, this->b.getTop() - _mouseOffset, this->value.getMin(), this->value.getMax(), true);
		return true;
	}
	return false;
}

template class ofxRotarySlider <int>;
template class ofxRotarySlider <unsigned int>;
template class ofxRotarySlider <float>;
template class ofxRotarySlider <double>;
template class ofxRotarySlider <signed char>;
template class ofxRotarySlider <unsigned char>;
template class ofxRotarySlider <unsigned short>;
template class ofxRotarySlider <short>;
