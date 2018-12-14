#include "ofxLabel.h"
#include "ofGraphics.h"
using namespace std;

ofxLabel::ofxLabel(ofParameter<string> _label, float width, float height){
	setup(_label,width,height);
}

ofxLabel::~ofxLabel(){
    label.removeListener(this,&ofxLabel::valueChanged);
}

ofxLabel* ofxLabel::setup(ofParameter<string> _label, float width, float height) {
    label.makeReferenceTo(_label);
    b.width  = width;
    b.height = height;
    setNeedsRedraw();
	_label.addListener(this,&ofxLabel::valueChanged);
    return this;
}

ofxLabel* ofxLabel::setup(const std::string& labelName, string _label, float width, float height) {
    label.set(labelName,_label);
    return setup(label,width,height);
}

void ofxLabel::generateDraw(){
	bg.clear();

	bg.setFillColor(thisBackgroundColor);
	bg.setFilled(true);
	bg.rectangle(b);

    string name;
    if(!getName().empty()){
    	name = getName() + ": ";
    }
    name += (string)label;    
    
    // resize the string inside the max width
    if(font.isLoaded()){ // using font
        while( font.stringWidth(name) > (getWidth() - textPadding*2.0f) ){
            name.resize(name.size()-1);
        }  
    }else{ // using bitmap font
        int max = (getWidth() - textPadding*2.0f) / 8.0f;
        name.resize(max);
    } 
    
    textMesh = getTextMesh(name, b.x + textPadding, getTextVCenteredInRect(b));
}

void ofxLabel::render() {
	ofColor c = ofGetStyle().color;

	bg.draw();

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

ofAbstractParameter & ofxLabel::getParameter(){
	return label;
}

void ofxLabel::valueChanged(string & value){
    setNeedsRedraw();
}
