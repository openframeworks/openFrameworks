#include "ofxLabel.h"
#include "ofGraphics.h"
using namespace std;

template<typename Type>
ofxValueLabel<Type>::ofxValueLabel(const ofJson & config)
:ofxBaseGui(config){

	setup(config);

}

template<typename Type>
ofxValueLabel<Type>::ofxValueLabel(ofParameter<Type> _label, const ofJson & config)
:ofxBaseGui(config){

	label.makeReferenceTo(_label);
	setup(config);

}

template<>
ofxValueLabel<std::string>::ofxValueLabel(const string& labelName, const ofJson & config)
:ofxValueLabel<std::string>(config){

	label.set(labelName);

}


template<typename Type>
ofxValueLabel<Type>::ofxValueLabel(ofParameter<Type> _label, float width, float height)
	:ofxValueLabel<Type>(_label){

	setSize(width, height);

}

template<typename Type>
ofxValueLabel<Type>::ofxValueLabel(const string& labelName, const Type& _label, const ofJson & config)
	:ofxValueLabel<Type>(config){

	label.set(labelName,_label);

}

template<typename Type>
ofxValueLabel<Type>::ofxValueLabel(const string& labelName, const Type& _label, float width, float height)
	:ofxValueLabel<Type>(){

	label.set(labelName,_label);
	setSize(width, height);

}

template<typename Type>
ofxValueLabel<Type>::~ofxValueLabel(){

	label.removeListener(this,&ofxValueLabel::valueChanged);

}

template<typename Type>
void ofxValueLabel<Type>::setup(const ofJson&){

	label.addListener(this,&ofxValueLabel::valueChanged);

}

template<typename Type>
void ofxValueLabel<Type>::generateDraw(){

	ofxBaseGui::generateDraw();

	if(showName){
		string name;
		if(!getName().empty()){
			name = getName() + ": ";
		}
		textMesh = getTextMesh(name + label.toString(), textPadding, getShape().height / 2 + 4);
	}else {
		textMesh = getTextMesh(label.toString(), textPadding, getShape().height / 2 + 4);
	}
}

template<typename Type>
void ofxValueLabel<Type>::render() {
	ofColor c = ofGetStyle().color;

	ofxBaseGui::render();

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

template<typename Type>
ofAbstractParameter & ofxValueLabel<Type>::getParameter(){
	return label;
}

template<typename Type>
void ofxValueLabel<Type>::valueChanged(Type & value){
	setNeedsRedraw();
}

template class ofxValueLabel<std::string>;
template class ofxValueLabel<int>;
template class ofxValueLabel<float>;
template class ofxValueLabel<bool>;
