#include "ofxSlider.h"
#include "ofGraphics.h"

template<typename Type>
ofxSlider<Type>::ofxSlider(){
	bUpdateOnReleaseOnly = false;
	bGuiActive = false;
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
	bUpdateOnReleaseOnly = false;
	value.makeReferenceTo(_val);
	b.x = 0;
	b.y = 0;
	b.width = width;
	b.height = height;
	bGuiActive = false;

	value.addListener(this,&ofxSlider::valueChanged);
	ofRegisterMouseEvents(this,OF_EVENT_ORDER_BEFORE_APP);
	generateDraw();
	return this;
}

template<typename Type>
ofxSlider<Type>* ofxSlider<Type>::setup(string sliderName, Type _val, Type _min, Type _max, float width, float height){
	value.set(sliderName,_val,_min,_max);
	return setup(value,width,height);
}

template<typename Type>
bool ofxSlider<Type>::mouseMoved(ofMouseEventArgs & args){
	if(isGuiDrawing() && b.inside(ofPoint(args.x,args.y))){
		return true;
	}else{
		return false;
	}
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

	float valAsPct = ofMap( value, value.getMin(), value.getMax(), 0, b.width-2, true );
	bar.setFillColor(thisFillColor);
	bar.setFilled(true);
	bar.rectangle(b.x+1, b.y+1, valAsPct, b.height-2);

	generateText();
}


template<typename Type>
void ofxSlider<Type>::generateText(){
	string valStr = ofToString(value);
	textMesh = getTextMesh(getName(), b.x + textPadding, b.y + b.height / 2 + 4);
	textMesh.append(getTextMesh(valStr, b.x + b.width - textPadding - getTextBoundingBox(valStr,0,0).width, b.y + b.height / 2 + 4));
}

template<>
void ofxSlider<unsigned char>::generateText(){
	string valStr = ofToString((int)value);
	textMesh = getTextMesh(getName(), b.x + textPadding, b.y + b.height / 2 + 4);
	textMesh.append(getTextMesh(valStr, b.x + b.width - textPadding - getTextBoundingBox(valStr,0,0).width, b.y + b.height / 2 + 4));
}

template<typename Type>
void ofxSlider<Type>::render(){
	ofColor c = ofGetStyle().color;

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
	generateDraw();
}

template class ofxSlider<int>;
template class ofxSlider<unsigned int>;
template class ofxSlider<float>;
template class ofxSlider<double>;
template class ofxSlider<signed char>;
template class ofxSlider<unsigned char>;
template class ofxSlider<unsigned short>;
template class ofxSlider<short>;
