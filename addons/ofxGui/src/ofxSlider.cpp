#include "ofxSlider.h"
#include "ofGraphics.h"

using std::string;

namespace{
	template<typename Type>
	std::string toString(Type t){
		return ofToString(t);
	}

	template<>
	std::string toString(uint8_t t){
		return ofToString((int) t);
	}

	template<>
	std::string toString(int8_t t){
		return ofToString((int) t);
	}
}

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
ofxSlider<Type>::ofxSlider(ofParameter<Type> _val, float width, float height){
	setup(_val,width,height);
}

template<typename Type>
ofxSlider<Type>* ofxSlider<Type>::setup(ofParameter<Type> _val, float width, float height){
	listener = input.leftFocus.newListener([this]{
		state = Slider;
		if(!input.containsValidValue()){
			errorTime = ofGetElapsedTimef();
		}else{
			errorTime = 0;
		}
	});
	bUpdateOnReleaseOnly = false;
	value.makeReferenceTo(_val);
	b.x = 0;
	b.y = 0;
	b.width = width;
	b.height = height;
	bGuiActive = false;
	setNeedsRedraw();

	value.addListener(this,&ofxSlider::valueChanged);
	registerMouseEvents();

	input.setup(_val, width, height);
	return this;
}

template<typename Type>
ofxSlider<Type>* ofxSlider<Type>::setup(const std::string& sliderName, Type _val, Type _min, Type _max, float width, float height){
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
bool ofxSlider<Type>::mouseMoved(ofMouseEventArgs & mouse){
	bool mouseInside = isGuiDrawing() && b.inside(mouse);
	if(mouseInside != this->mouseInside && overlappingLabel){
		setNeedsRedraw();
	}
	this->mouseInside = mouseInside;
	return mouseInside;
}

template<typename Type>
bool ofxSlider<Type>::mousePressed(ofMouseEventArgs & mouse){
	if(state==Slider){
		if(mouse.button == OF_MOUSE_BUTTON_RIGHT){
			if(b.inside(mouse)){
				state = Input;
				auto mouseLeft = mouse;
				input.setShape(b);
				mouseLeft.button = OF_MOUSE_BUTTON_LEFT;
				input.mousePressed(mouseLeft);
				return true;
			}else{
				return false;
			}
		}else{
			if(bUpdateOnReleaseOnly){
				value.disableEvents();
			}
			if(setValue(mouse.x, mouse.y, true)){
				return true;
			}else{
				return false;
			}
		}
	}else{
		return isGuiDrawing() && input.mousePressed(mouse);
	}
}

template<typename Type>
bool ofxSlider<Type>::mouseDragged(ofMouseEventArgs & mouse){
	if(state==Slider){
		if(setValue(mouse.x, mouse.y, false)){
			return true;
		}else{
			return false;
		}
	}else{
		return isGuiDrawing() && input.mouseDragged(mouse);
	}
}

template<typename Type>
bool ofxSlider<Type>::mouseReleased(ofMouseEventArgs & mouse){
	if(state==Slider){
		if(bUpdateOnReleaseOnly){
			value.enableEvents();
		}
		bool attended = setValue(mouse.x, mouse.y, false);

		bGuiActive = false;
		if(attended){
			return true;
		}else{
			return false;
		}
	}else{
		return isGuiDrawing() && input.mouseReleased(mouse);
	}
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
bool ofxSlider<Type>::mouseScrolled(ofMouseEventArgs & args){
	if(isGuiDrawing()){
		if(state==Slider){
			if(mouseInside){
				if(args.scrollY>0 || args.scrollY<0){
					double range = getRange(value.getMin(),value.getMax(),b.width);
					Type newValue = value + ofMap(args.scrollY,-1,1,-range, range);
					newValue = ofClamp(newValue,value.getMin(),value.getMax());
					value = newValue;
				}
				return true;
			}else{
				return false;
			}
		}else{
			// the following will always return false as it is inside the slider.
//			return input.mouseScrolled(args);
		}
	}
	return false;
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
	bg.setExtents(b);

	float valAsPct = ofMap( value, value.getMin(), value.getMax(), 0, b.width-2, true );
	bar.setFillColor(thisFillColor);
	bar.setExtents(b.x+1, b.y+1, valAsPct, b.height-2);

	generateText();
	input.generateDraw();
}


template<typename Type>
void ofxSlider<Type>::generateText(){
	string valStr = toString(value.get());
	auto inputWidth = getTextBoundingBox(valStr,0,0).width;
	auto yPos = getTextVCenteredInRect(b);
	auto label = getTextBoundingBox(getName(), b.x + textPadding, yPos);
	auto value = getTextBoundingBox(valStr, b.x + b.width - textPadding - inputWidth, yPos);
	overlappingLabel = label.getMaxX() > value.x;

	textMesh.clear();
	if(!mouseInside || !overlappingLabel){
		std::string name;
		if(overlappingLabel){
			for(auto c: ofUTF8Iterator(getName())){
				auto next = name;
				ofUTF8Append(next, c);
				if(getTextBoundingBox(next,0,0).width > b.width-textPadding*2){
					break;
				}else{
					name = next;
				}
			}
		}else{
			name = getName();
		}
		textMesh.append(getTextMesh(name, b.x + textPadding, yPos));
	}
	if(!overlappingLabel || mouseInside){
		textMesh.append(getTextMesh(valStr, b.x + b.width - textPadding - getTextBoundingBox(valStr,0,0).width, yPos));
	}
}

template<typename Type>
void ofxSlider<Type>::render(){
	if(state==Slider){
		ofColor c = ofGetStyle().color;
		if(errorTime>0 && !input.containsValidValue()){
			auto now = ofGetElapsedTimef();
			auto pct = (now - errorTime) / 0.5;
			if(pct<1){
				bg.setFillColor(ofColor::darkRed.getLerped(thisBackgroundColor, pct));
				bar.setFillColor(ofColor::red.getLerped(thisFillColor, pct));
			}else{
				bg.setFillColor(thisBackgroundColor);
				bar.setFillColor(thisFillColor);
				errorTime = 0;
			}
		}

		bg.draw();
		bar.draw();

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
	}else{
		input.draw();
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
		value = ofMap(mx, b.x, b.x + b.width, value.getMin(), value.getMax(), true);
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

template<typename Type>
void ofxSlider<Type>::setPosition(const glm::vec3 & p){
	ofxBaseGui::setPosition(p);
	input.setPosition(p);
}

template<typename Type>
void ofxSlider<Type>::setPosition(float x, float y){
	ofxBaseGui::setPosition(x, y);
	input.setPosition(x, y);
}

template<typename Type>
void ofxSlider<Type>::setSize(float w, float h){
	ofxBaseGui::setSize(w, h);
	input.setSize(w, h);
}

template<typename Type>
void ofxSlider<Type>::setShape(ofRectangle r){
	ofxBaseGui::setShape(r);
	input.setShape(r);
}

template<typename Type>
void ofxSlider<Type>::setShape(float x, float y, float w, float h){
	ofxBaseGui::setShape(x, y, w, h);
	input.setShape(x, y, w, h);
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

//for some reason osx errors if this isn't defined 
#if defined TARGET_OSX || defined TARGET_EMSCRIPTEN
	template class ofxSlider<typename std::conditional<std::is_same<uint32_t, size_t>::value || std::is_same<uint64_t, size_t>::value, bool, size_t>::type>;
#endif

