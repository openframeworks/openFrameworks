#pragma once

#include "ofMain.h"

class Car{
public:
    Car();
    void setup();
    void draw();
    void update();
    void brake();
    void accelerate();
    void steer(float dir);
	const ofNode & getNode() const;
    
private:
    float acceleration;
    float vel;
    ofBoxPrimitive geometry;
    ofMaterial material;
    ofLight lightR, lightL;
};
