#include "ofxLabel.h"
#include "ofGraphics.h"
using namespace std;

template<typename Type>
ofxValueLabel<Type>::ofxValueLabel()
:ofxBaseGui(Config()){

}

template<typename Type>
ofxValueLabel<Type>::ofxValueLabel(ofParameter<Type> _label, const Config & config)
:ofxBaseGui(config){
	label.makeReferenceTo(_label);
	setNeedsRedraw();
	label.addListener(this,&ofxValueLabel::valueChanged);
}

template<typename Type>
ofxValueLabel<Type>::~ofxValueLabel(){
	label.removeListener(this,&ofxValueLabel::valueChanged);
}

template<typename Type>
ofxValueLabel<Type> & ofxValueLabel<Type>::setup(ofParameter<Type> _label, const Config & config){
	ofxBaseGui::setup(config);
	return setup(_label, config.shape.width, config.shape.height);
}

template<typename Type>
ofxValueLabel<Type> & ofxValueLabel<Type>::setup(ofParameter<Type> _label, float width, float height) {
	label.makeReferenceTo(_label);
	b.width  = width;
	b.height = height;
	setNeedsRedraw();
	label.addListener(this,&ofxValueLabel::valueChanged);
	return *this;
}

template<typename Type>
ofxValueLabel<Type> & ofxValueLabel<Type>::setup(const string& labelName, const Type& _label, const Config & config) {
	label.set(labelName,_label);
	return setup(label,config);
}

template<typename Type>
ofxValueLabel<Type> & ofxValueLabel<Type>::setup(const string& labelName, const Type& _label, float width, float height) {
	label.set(labelName,_label);
	return setup(label,width,height);
}

template<typename Type>
void ofxValueLabel<Type>::generateDraw(){
	bg.clear();

	bg.setFillColor(thisBackgroundColor);
	bg.setFilled(true);
	bg.rectangle(b);

	if(bShowName){
		string name;
		if(!getName().empty()){
			name = getName() + ": ";
		}
		textMesh = getTextMesh(name + label.toString(), b.x + textPadding, b.y + b.height / 2 + 4);
	}else {
		textMesh = getTextMesh(label.toString(), b.x + textPadding, b.y + b.height / 2 + 4);
	}
}

template<typename Type>
void ofxValueLabel<Type>::render() {
	ofColor c = ofGetStyle().color;

	bg.draw();

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
