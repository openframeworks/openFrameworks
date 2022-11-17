#pragma once
#include "ofMain.h"

class Bullet {

public:
	
	glm::vec2 pos, vel;
	bool    bRemove;
	
	void update() {
		pos += vel;
		vel *= 0.99; // slowdown in a long time
	}
	
	void draw() {
		ofSetColor(20);
		ofDrawCircle(pos, 2);
	}
	
};
