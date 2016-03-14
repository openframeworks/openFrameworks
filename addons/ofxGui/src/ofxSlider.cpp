#include "ofxSlider.h"
#include "JsonConfigParser.h"
#include "ofGraphics.h"
using namespace std;
using namespace ofx;
using namespace ofx::DOM;

template<typename Type>
ofxSlider<Type>::ofxSlider(const ofJson &config)
	:ofxBaseGui(config){

	setup(config);

}

template<typename Type>
ofxSlider<Type>::ofxSlider(ofParameter<Type> _val, const ofJson &config)
:ofxSlider(config){

	value.makeReferenceTo(_val);
	value.addListener(this,&ofxSlider::valueChanged);

}

template<typename Type>
ofxSlider<Type>::ofxSlider(ofParameter<Type> _val, float width, float height)
	:ofxSlider(){

	value.makeReferenceTo(_val);
	setSize(width, height);
	value.addListener(this,&ofxSlider::valueChanged);

}

template<typename Type>
ofxSlider<Type>::ofxSlider(const std::string& sliderName, Type _val, Type _min, Type _max, float width, float height)
	:ofxSlider(){

	value.set(sliderName,_val,_min,_max);
	value.addListener(this,&ofxSlider::valueChanged);
	setSize(width,height);

}

template<typename Type>
ofxSlider<Type>::~ofxSlider(){

	value.removeListener(this,&ofxSlider::valueChanged);
	ofRemoveListener(resize, this, &ofxSlider<Type>::resized);

}

template<typename Type>
void ofxSlider<Type>::setup(const ofJson &config){

	updateOnReleaseOnly.set("update-on-release-only", false);
	precision.set("precision", 6);
	horizontal = getWidth() > getHeight();
	ofAddListener(resize, this, &ofxSlider<Type>::resized);
	registerPointerEvents();
	processConfig(config);

}

template<typename Type>
void ofxSlider<Type>::processConfig(const ofJson &config){

	JsonConfigParser::parse(config, updateOnReleaseOnly);
	JsonConfigParser::parse(config, precision);

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
void ofxSlider<Type>::resized(ResizeEventArgs &){
	horizontal = getWidth() > getHeight();
}

template<typename Type>
void ofxSlider<Type>::setPrecision(int precision){
	this->precision = precision;
}

template<typename Type>
void ofxSlider<Type>::pointerPressed(PointerUIEventArgs& e){
	if(updateOnReleaseOnly){
		value.disableEvents();
	}
	this->setValue(e.screenPosition().x, e.screenPosition().y);
}

template<typename Type>
void ofxSlider<Type>::pointerReleased(PointerUIEventArgs& e){
	if(updateOnReleaseOnly){
		value.enableEvents();
	}
	this->setValue(e.screenPosition().x, e.screenPosition().y);
}

template<typename Type>
void ofxSlider<Type>::pointerDragged(PointerUIEventArgs& e){
	this->setValue(e.screenPosition().x, e.screenPosition().y);
}

template<typename Type>
void ofxSlider<Type>::pointerScrolled(PointerUIEventArgs& e){
	// TODO set value based on scrolling
//	if(args.scrollY>0 || args.scrollY<0){
//		double range = getRange(value.getMin(),value.getMax(),b.width);
//		Type newValue = value + ofMap(args.scrollY,-1,1,-range, range);
//		newValue = ofClamp(newValue,value.getMin(),value.getMax());
//		value = newValue;
//	}
}


template<typename Type>
typename std::enable_if<std::is_integral<Type>::value, Type>::type
getRange(Type min, Type max, float width){
	double range = max - min;
	range /= width*4;
	return std::max(range,1.0);
}

template<typename Type>
typename std::enable_if<std::is_floating_point<Type>::value, Type>::type
getRange(Type min, Type max, float width){
	double range = max - min;
	range /= width*4;
	return range;
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

	ofxBaseGui::generateDraw();

	bar.clear();

	float valAsPct;
	if(horizontal){
		valAsPct = ofMap(value, value.getMin(), value.getMax(), 0, getWidth(), true);
	}else{
		valAsPct = ofMap(value, value.getMin(), value.getMax(), 0, getHeight(), true);
	}
	bar.setFillColor(fillColor);
	bar.setFilled(true);
	if(horizontal){
		bar.rectangle(0,0, valAsPct, getHeight());
	}else{
		bar.rectangle(0, getHeight() - valAsPct, getWidth(), valAsPct);
	}

	if(showName){
		generateText();
	}
}


template<typename Type>
void ofxSlider<Type>::generateText(){

	string valStr = ofToString(value.get(), precision);
	_generateText(valStr);
}

template<>
void ofxSlider<unsigned char>::generateText(){

	string valStr = ofToString((int)value, precision);
	_generateText(valStr);
}

template<typename Type>
void ofxSlider<Type>::_generateText(std::string valStr){
	if(horizontal){
		textMesh = getTextMesh(getName(), ofPoint(textPadding, getHeight() / 2 + 4));
		textMesh.append(getTextMesh(valStr, getShape().getWidth() - textPadding - getTextBoundingBox(valStr,0,0).width, getHeight() / 2 + 4));
	}else{
		textMesh.clear();
		if(showName){
			string nameStr = getName();
			while(getTextBoundingBox(nameStr, 0, 0).getWidth() + textPadding * 2 > getWidth() && nameStr.length() > 1){
				nameStr = nameStr.substr(0, nameStr.size() - 1);
			}
			textMesh.append(getTextMesh(nameStr, textPadding, textPadding + getTextBoundingBox(nameStr, 0, 0).height));
		}
		while(getTextBoundingBox(valStr, 0, 0).getWidth() + textPadding * 2 > getWidth() && valStr.length() > 1){
			valStr = valStr.substr(0, valStr.size() - 1);
		}
		textMesh.append(getTextMesh(valStr, textPadding, getHeight() - textPadding));
	}
}

template<typename Type>
void ofxSlider<Type>::render(){
	ofColor c = ofGetStyle().color;

	ofxBaseGui::render();

	bar.draw();

	if(showName){
		ofBlendMode blendMode = ofGetStyle().blendingMode;
		if(blendMode!=OF_BLENDMODE_ALPHA){
			ofEnableAlphaBlending();
		}
		ofSetColor(textColor);

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
bool ofxSlider<Type>::setValue(float mx, float my){
	ofPoint topleft = localToScreen(ofPoint(0, 0));
	ofPoint bottomright = localToScreen(ofPoint(getWidth(), getHeight()));
	if(horizontal){
		value = ofMap(mx, topleft.x, bottomright.x, value.getMin(), value.getMax(), true);
	}else{
		value = ofMap(my, bottomright.y, topleft.y, value.getMin(), value.getMax(), true);
	}
	return true;
}

template<typename Type>
ofAbstractParameter & ofxSlider<Type>::getParameter(){
	return value;
}

template<typename Type>
void ofxSlider<Type>::valueChanged(Type & value){
	setNeedsRedraw();
}

template class ofxSlider<int8_t>;
template class ofxSlider<uint8_t>;
template class ofxSlider<int16_t>;
template class ofxSlider<uint16_t>;
template class ofxSlider<int32_t>;
template class ofxSlider<uint32_t>;
template class ofxSlider<int64_t>;
template class ofxSlider<uint64_t>;
template class ofxSlider<float>;
template class ofxSlider<double>;
