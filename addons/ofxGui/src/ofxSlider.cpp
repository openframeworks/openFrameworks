#include "ofxSlider.h"
#include "ofGraphics.h"

template<typename Type>
ofxSlider<Type>::ofxSlider(){
	bUpdateOnReleaseOnly = false;
	currentFrame = ofGetFrameNum();
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
	currentFrame = ofGetFrameNum();
	bGuiActive = false;

	value.addListener(this,&ofxSlider::valueChanged);
	ofRegisterMouseEvents(this);
	generateDraw();
	return this;
}

template<typename Type>
ofxSlider<Type>* ofxSlider<Type>::setup(string sliderName, Type _val, Type _min, Type _max, float width, float height){
	value.set(sliderName,_val,_min,_max);
	return setup(value,width,height);
}

template<typename Type>
void ofxSlider<Type>::mouseMoved(ofMouseEventArgs & args){
}

template<typename Type>
void ofxSlider<Type>::mousePressed(ofMouseEventArgs & args){
	if(bUpdateOnReleaseOnly){
		value.disableEvents();
	}
	setValue(args.x, args.y, true);
}

template<typename Type>
void ofxSlider<Type>::mouseDragged(ofMouseEventArgs & args){
	setValue(args.x, args.y, false);
}

template<typename Type>
void ofxSlider<Type>::mouseReleased(ofMouseEventArgs & args){
	if(bUpdateOnReleaseOnly){
		value.enableEvents();
	}
	setValue(args.x, args.y, false);
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
void ofxSlider<Type>::generateDraw(){
	bg.clear();
	bar.clear();

	bg.setFillColor(thisBackgroundColor);
	bg.setFilled(true);
	bg.moveTo(b.x, b.y);
	bg.lineTo(b.x+b.width,b.y);
	bg.lineTo(b.x+b.width,b.y+b.height);
	bg.lineTo(b.x,b.y+b.height);
	bg.close();

	float valAsPct = ofMap( value, value.getMin(), value.getMax(), 0, b.width-2, true );
	bar.setFillColor(thisFillColor);
	bar.setFilled(true);
	bar.moveTo(b.x+1, b.y+1);
	bar.lineTo(b.x+1+valAsPct,b.y+1);
	bar.lineTo(b.x+1+valAsPct,b.y+b.height-1);
	bar.lineTo(b.x+1,b.y+b.height-1);
	bar.close();

	generateText();
}


template<typename Type>
void ofxSlider<Type>::generateText(){
	string valStr = ofToString(value);
	textMesh = font.getStringMesh(getName(), b.x + textPadding, b.y + b.height / 2 + 4);
	textMesh.append(font.getStringMesh(valStr, b.x + b.width - textPadding - valStr.length() * 8, b.y + b.height / 2 + 4));
}

template<>
void ofxSlider<unsigned char>::generateText(){
	string valStr = ofToString((int)value);
	textMesh = font.getStringMesh(getName(), b.x + textPadding, b.y + b.height / 2 + 4);
	textMesh.append(font.getStringMesh(valStr, b.x + b.width - textPadding - valStr.length() * 8, b.y + b.height / 2 + 4));
}

template<typename Type>
void ofxSlider<Type>::draw(){
	ofColor c = ofGetStyle().color;

	currentFrame = ofGetFrameNum();

	bg.draw();
	bar.draw();

	ofBlendMode blendMode = ofGetStyle().blendingMode;
	if(blendMode!=OF_BLENDMODE_ALPHA){
		ofEnableAlphaBlending();
	}
	ofSetColor(thisTextColor);
	/*font.bind();
	font.drawString(getName(), b.x + textPadding, b.y + b.height / 2 + 4);
	string valStr = ofToString(value);
	font.drawString(valStr, b.x + b.width - textPadding - valStr.length() * 8, b.y + b.height / 2 + 4);
	font.unbind();*/
	font.getFontTexture().bind();
	textMesh.draw();
	font.getFontTexture().unbind();

	ofSetColor(c);
	if(blendMode!=OF_BLENDMODE_ALPHA){
		ofEnableBlendMode(blendMode);
	}
}


/*template<>
void ofxSlider<unsigned char>::draw(){
	bool currentFill = ofGetStyle().bFill;
	ofColor c = ofGetStyle().color;

	currentFrame = ofGetFrameNum();

	bg.draw();
	bar.draw();

	font.getFontTexture().bind();
	textMesh.draw();
	font.getFontTexture().unbind();

	ofSetColor(c);
	if(!currentFill) ofNoFill();
}*/

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
