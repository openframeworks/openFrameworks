#pragma once

#include "ofxSlider.h"

class ofxFpsSlider : public ofxFloatSlider {
public:
    ofxFpsSlider() : ofxFloatSlider() {}
    virtual ~ofxFpsSlider() {}

    ofxFpsSlider* setup(string _name, float _max, float width = defaultWidth, float height = defaultHeight);

    void draw();
};
