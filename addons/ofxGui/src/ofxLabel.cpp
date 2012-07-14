#include "ofxLabel.h"

ofxLabel* ofxLabel::setup(string labelName, ofxParameter<string> _label, float width, float height) {
    name     = labelName;
    label    = _label;
    b.width  = width;
    b.height = height;
    return this;
}

ofxLabel* ofxLabel::setup(string labelName, string _label, float width, float height) {
    name     = labelName;
    label    = _label;
    b.width  = width;
    b.height = height;
    return this;
}

void ofxLabel::draw() {
    currentFrame = ofGetFrameNum();

    ofPushStyle();
    ofPushMatrix();

    ofSetColor(backgroundColor);
    ofRect(b);

    ofTranslate(b.x, b.y);
    ofSetColor(textColor);
    ofTranslate(0, b.height / 2 + 4);
    ofDrawBitmapString(label, textPadding, 0);

    ofPopMatrix();
    ofPopStyle();
}

ofAbstractParameter & ofxLabel::getParameter(){
	return label;
}
