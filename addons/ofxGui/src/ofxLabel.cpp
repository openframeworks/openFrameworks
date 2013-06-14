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
	bg.moveTo(b.x, b.y);
	bg.lineTo(b.x+b.width,b.y);
	bg.lineTo(b.x+b.width,b.y+b.height);
	bg.lineTo(b.x,b.y+b.height);
	bg.close();

    string name;
    if(!getName().empty()){
    	name = getName() + ": ";
    }
	textMesh = font.getStringMesh(name + (string)label, b.x + textPadding, b.y + b.height / 2 + 4);
}

void ofxLabel::draw() {
    currentFrame = ofGetFrameNum();

	ofColor c = ofGetStyle().color;

	bg.draw();

	ofBlendMode blendMode = ofGetStyle().blendingMode;
	if(blendMode!=OF_BLENDMODE_ALPHA){
		ofEnableAlphaBlending();
	}
    ofSetColor(textColor);
    font.getFontTexture().bind();
    textMesh.draw();
    font.getFontTexture().unbind();
    //font.drawString(name + (string)label, b.x + textPadding, b.y + b.height / 2 + 4);

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
