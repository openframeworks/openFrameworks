#include "ofxSlider.h"
#include "ofGraphics.h"
using namespace std;

template<typename Type>
ofxSlider<Type>::ofxSlider(){
    bUpdateOnReleaseOnly = false;
    bGuiActive = false;
    mouseInside = false;
}

template<typename Type>
ofxSlider<Type>::~ofxSlider(){
	value.removeListener(this,&ofxSlider::valueChanged);
}

template<typename Type>
ofxSlider<Type>::ofxSlider(ofParameter<Type> _val, const Config & config)
:ofxBaseGui(config)
,bUpdateOnReleaseOnly(config.updateOnReleaseOnly)
,bGuiActive(false)
,mouseInside(false){
    if(layout == ofxBaseGui::Vertical){
        float _w = b.width;
        float _h = b.height;
        b.height = _w;
        b.width = _h;
    }
	value.makeReferenceTo(_val);
	value.addListener(this,&ofxSlider::valueChanged);
	setNeedsRedraw();
	registerMouseEvents();
}

template<typename Type>
ofxSlider<Type> & ofxSlider<Type>::setup(const std::string& sliderName, Type _val, Type _min, Type _max, float width, float height){
	value.set(sliderName,_val,_min,_max);
	return setup(value,width,height);
}

template<typename Type>
void ofxSlider<Type>::setMin(Type min){
    value.setMin(min);
}

template<typename Type>
Type ofxSlider<Type>::getMin(){
    return value.getMin();
}

template<typename Type>
void ofxSlider<Type>::setMax(Type max){
    value.setMax(max);
}

template<typename Type>
Type ofxSlider<Type>::getMax(){
    return value.getMax();
}

template<typename Type>
bool ofxSlider<Type>::mouseMoved(ofMouseEventArgs & args){
	mouseInside = isGuiDrawing() && b.inside(ofPoint(args.x,args.y));
	return mouseInside;
}

template<typename Type>
bool ofxSlider<Type>::mousePressed(ofMouseEventArgs & args){
	if(bUpdateOnReleaseOnly){
		value.disableEvents();
	}
	if(setValue(args.x, args.y, true)){
		return true;
	}else{
		return false;
	}
}

template<typename Type>
bool ofxSlider<Type>::mouseDragged(ofMouseEventArgs & args){
	if(setValue(args.x, args.y, false)){
		return true;
	}else{
		return false;
	}
}

template<typename Type>
bool ofxSlider<Type>::mouseReleased(ofMouseEventArgs & args){
	if(bUpdateOnReleaseOnly){
		value.enableEvents();
	}
	bool attended = setValue(args.x, args.y, false);

	bGuiActive = false;
	if(attended){
		return true;
	}else{
		return false;
	}
}

template<typename Type>
bool ofxSlider<Type>::mouseScrolled(ofMouseEventArgs & args){
	if(mouseInside){
		if(args.y>0 || args.y<0){
			double range = value.getMax() - value.getMin();
			range /= b.width*4;
			Type newValue = value + ofMap(args.y,-1,1,-range, range);
			newValue = ofClamp(newValue,value.getMin(),value.getMax());
			value = newValue;
		}
		return true;
	}else{
		return false;
	}
}

template<>
bool ofxSlider<int>::mouseScrolled(ofMouseEventArgs & args){
	if(mouseInside){
		if(args.y>0 || args.y<0){
			double range = value.getMax() - value.getMin();
			range /= b.width*4;
			range = max(range, 1.0);
			int newValue = value + ofMap(args.y,-1,1,-range, range);
			newValue = ofClamp(newValue,value.getMin(),value.getMax());
			value = newValue;
		}
		return true;
	}else{
		return false;
	}
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
void ofxSlider<Type>::generateDraw(){
	bg.clear();
	bar.clear();

	bg.setFillColor(thisBackgroundColor);
	bg.setFilled(true);
	bg.rectangle(b);

    float valAsPct;
    if(layout == ofxBaseGui::Horizontal){
        valAsPct = ofMap( value, value.getMin(), value.getMax(), 0, b.width-2, true );
    }else{
        valAsPct = ofMap( value, value.getMin(), value.getMax(), 0, b.height-2, true );
    }
	bar.setFillColor(thisFillColor);
	bar.setFilled(true);
    if(layout == ofxBaseGui::Horizontal){
        bar.rectangle(b.x+1, b.y+1, valAsPct, b.height-2);
    }else{
        bar.rectangle(b.x + 1, b.y + b.height - 1 - valAsPct, b.width - 2, valAsPct);
    }

    if(bShowName){
        generateText();
    }
}


template<typename Type>
void ofxSlider<Type>::generateText(){
    if(layout == ofxBaseGui::Horizontal){
        string valStr = ofToString(value);
        textMesh = getTextMesh(getName(), b.x + textPadding, b.y + b.height / 2 + 4);
        textMesh.append(getTextMesh(valStr, b.x + b.width - textPadding - getTextBoundingBox(valStr,0,0).width, b.y + b.height / 2 + 4));
    }else{
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
}

template<>
void ofxSlider<unsigned char>::generateText(){
    if(layout == ofxBaseGui::Horizontal){
        string valStr = ofToString(value);
        textMesh = getTextMesh(getName(), b.x + textPadding, b.y + b.height / 2 + 4);
        textMesh.append(getTextMesh(valStr, b.x + b.width - textPadding - getTextBoundingBox(valStr,0,0).width, b.y + b.height / 2 + 4));
    }else{
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
}

template<typename Type>
void ofxSlider<Type>::render(){
	ofColor c = ofGetStyle().color;

	bg.draw();
	bar.draw();

    if(bShowName){
        ofBlendMode blendMode = ofGetStyle().blendingMode;
        if(blendMode!=OF_BLENDMODE_ALPHA){
            ofEnableAlphaBlending();
        }
        ofSetColor(thisTextColor);

        bindFontTexture();
        textMesh.draw();
        unbindFontTexture();

        ofSetColor(c);
        if(blendMode!=OF_BLENDMODE_ALPHA){
            ofEnableBlendMode(blendMode);
        }
    }
}


template<typename Type>
bool ofxSlider<Type>::setValue(float mx, float my, bool bCheck){
	if( !isGuiDrawing() ){
		bGuiActive = false;
		return false;
	}
	if( bCheck ){
		if( b.inside(mx, my) ){
			bGuiActive = true;
		}else{
			bGuiActive = false;
		}
	}
	if( bGuiActive ){
        if(layout == ofxBaseGui::Horizontal){
            value = ofMap(mx, b.x, b.x + b.width, value.getMin(), value.getMax(), true);
        }else{
            value = ofMap(my, b.y + b.height, b.y, value.getMin(), value.getMax(), true);
        }
		return true;
	}
	return false;
}

template<typename Type>
ofAbstractParameter & ofxSlider<Type>::getParameter(){
	return value;
}


template<typename Type>
void ofxSlider<Type>::setUpdateOnReleaseOnly(bool _bUpdateOnReleaseOnly){
	bUpdateOnReleaseOnly = _bUpdateOnReleaseOnly;
}

template<typename Type>
void ofxSlider<Type>::valueChanged(Type & value){
    setNeedsRedraw();
}

template class ofxSlider<int>;
template class ofxSlider<unsigned int>;
template class ofxSlider<float>;
template class ofxSlider<double>;
template class ofxSlider<signed char>;
template class ofxSlider<unsigned char>;
template class ofxSlider<unsigned short>;
template class ofxSlider<short>;
