#include "ofxLabel.h"
#include "ofGraphics.h"

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
    generateDraw();
    label.addListener(this,&ofxLabel::valueChanged);
    return this;
}

ofxLabel* ofxLabel::setup(string labelName, string _label, float width, float height) {
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

    textMesh = getTextMesh(name + (string)label, b.x + textPadding, b.y + b.height / 2 + 4);
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
#ifdef SUPPORT_FONTSTASH
	string name;
    if(!getName().empty()){
    	name = getName() + ": ";
    }
	unicodeFont.draw(name + (string)label,fontSize, b.x + textPadding, b.y + b.height / 2 + fontSize/2 - (2 * (fontSize/12)));
#else
	textMesh.draw();
#endif
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
	generateDraw();
}
