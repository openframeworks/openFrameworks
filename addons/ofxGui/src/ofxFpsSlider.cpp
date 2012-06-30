#include "ofxFpsSlider.h"

ofxFpsSlider* ofxFpsSlider::setup(string _name, float _max, float width, float height) {
    ofxFloatSlider::setup(_name, 0.0, 0.0, _max, width, height);
    setAutoMax(true);
    // Clicking the fps slider should do nothing.
    ofUnregisterMouseEvents((ofxFloatSlider*)this);
    return this;
}

void ofxFpsSlider::draw() {
    value = ofGetFrameRate();
    ofxFloatSlider::draw();
}
