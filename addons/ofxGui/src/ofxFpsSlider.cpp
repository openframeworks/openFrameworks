#include "ofxFpsSlider.h"

ofxFpsSlider* ofxFpsSlider::setup(string _name, float _max, float width, float height) {
    ofxFloatSlider::setup(_name, 0.0, 0.0, _max, width, height);
    setAutoMax(true);
    return this;

// TODO: Using this class generates segfaults on exit. Tried this bit no different.
//    name = _name;
//    value = 0.0;
//    min = 0.0;
//    max = _max;
//    b.x = 0;
//    b.y = 0;
//    b.width = width;
//    b.height = height;
//    currentFrame = ofGetFrameNum();
//    bGuiActive = false;
//
//    //ofRegisterMouseEvents(this);
//    return this;
}

void ofxFpsSlider::draw() {
    value = ofGetFrameRate();
    ofxFloatSlider::draw();
}
