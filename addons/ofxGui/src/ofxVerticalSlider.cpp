#include "ofxVerticalSlider.h"
#include "ofGraphics.h"
using namespace std;

template <typename Type> ofxVerticalSlider <Type>::ofxVerticalSlider(){
}

template <typename Type> ofxVerticalSlider <Type>::~ofxVerticalSlider(){
}

template <typename Type> ofxVerticalSlider <Type>::ofxVerticalSlider(ofParameter <Type> _val, const Config & config) :
	ofxSlider <Type>(_val, config){
}

template <typename Type>
void ofxVerticalSlider <Type>::generateDraw(){
	this->bg.clear();
	this->bar.clear();

	this->bg.setFillColor(this->thisBackgroundColor);
	this->bg.setFilled(true);
	this->bg.rectangle(this->b);

	float valAsPct = ofMap(this->value, this->value.getMin(), this->value.getMax(), 0, this->b.height - 2, true);
	this->bar.setFillColor(this->thisFillColor);
	this->bar.setFilled(true);
	this->bar.rectangle(this->b.x + 1, this->b.y + this->b.height - 1 - valAsPct, this->b.width - 2, valAsPct);

    generateText();
}

template <typename Type>
void ofxVerticalSlider <Type>::generateText(){
    this->textMesh.clear();
    if(this->bShowName){
        string nameStr = this->getName();
        while(this->getTextBoundingBox(nameStr, 0, 0).getWidth() + this->textPadding * 2 > this->b.getWidth() && nameStr.length() > 1){
            nameStr = nameStr.substr(0, nameStr.size() - 1);
        }
        this->textMesh.append(this->getTextMesh(nameStr, this->b.x + this->textPadding, this->b.y + this->textPadding + this->getTextBoundingBox(nameStr, 0, 0).height));
    }
    string valStr = ofToString(this->value);
	while(this->getTextBoundingBox(valStr, 0, 0).getWidth() + this->textPadding * 2 > this->b.getWidth() && valStr.length() > 1){
		valStr = valStr.substr(0, valStr.size() - 1);
    }
	this->textMesh.append(this->getTextMesh(valStr, this->b.x + this->textPadding, this->b.y + this->b.height - this->textPadding));
}

template <>
void ofxVerticalSlider <unsigned char>::generateText(){
    this->textMesh.clear();
    if(this->bShowName){
        string nameStr = this->getName();
        while(this->getTextBoundingBox(nameStr, 0, 0).getWidth() + this->textPadding * 2 > this->b.getWidth() && nameStr.length() > 1){
            nameStr = nameStr.substr(0, nameStr.size() - 1);
        }
        this->textMesh.append(this->getTextMesh(nameStr, this->b.x + this->textPadding, this->b.y + this->textPadding + this->getTextBoundingBox(nameStr, 0, 0).height));
    }
    string valStr = ofToString(this->value);
    while(this->getTextBoundingBox(valStr, 0, 0).getWidth() + this->textPadding * 2 > this->b.getWidth() && valStr.length() > 1){
        valStr = valStr.substr(0, valStr.size() - 1);
    }
    this->textMesh.append(this->getTextMesh(valStr, this->b.x + this->textPadding, this->b.y + this->b.height - this->textPadding));
}

template <typename Type>
void ofxVerticalSlider <Type>::render(){
	ofColor c = ofGetStyle().color;

	this->bg.draw();
	this->bar.draw();

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

template <typename Type>
bool ofxVerticalSlider <Type>::setValue(float mx, float my, bool bCheck){
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
		this->value = ofMap(my, this->b.y + this->b.height, this->b.y, this->value.getMin(), this->value.getMax(), true);
		return true;
	}
	return false;
}

template class ofxVerticalSlider <int>;
template class ofxVerticalSlider <unsigned int>;
template class ofxVerticalSlider <float>;
template class ofxVerticalSlider <double>;
template class ofxVerticalSlider <signed char>;
template class ofxVerticalSlider <unsigned char>;
template class ofxVerticalSlider <unsigned short>;
template class ofxVerticalSlider <short>;
