#pragma once

#define BOUNCE_FACTOR			0.5
#define ACCELEROMETER_FORCE		0.2
#define RADIUS					30


class Ball {
public:
	ofPoint pos;
	ofPoint vel;
	ofColor col;
	
	void init() {
		pos.set(ofRandomWidth(), ofRandomHeight(), 0);
		vel.set(ofRandomf(), ofRandomf(), 0);
		
		col.r = ofRandom(0, 255);
		col.g = ofRandom(0, 255);
		col.b = ofRandom(0, 255);
		col.a = ofRandom(0, 255);
	}
	

    void update() {
        vel.x += ACCELEROMETER_FORCE * ofxAccelerometer.getForce().x * ofRandomuf();
        vel.y += -ACCELEROMETER_FORCE * ofxAccelerometer.getForce().y * ofRandomuf();        // this one is subtracted cos world Y is opposite to opengl Y
		
        // add vel to pos
        pos += vel;
		
        // check boundaries
        if(pos.x < RADIUS) {
            pos.x = RADIUS;
            vel.x *= -BOUNCE_FACTOR;
        } else if(pos.x >= ofGetWidth() - RADIUS) {
            pos.x = ofGetWidth() - RADIUS;
			vel.x *= -BOUNCE_FACTOR;
		}
		
		if(pos.y < RADIUS) {
			pos.y = RADIUS;
			vel.y *= -BOUNCE_FACTOR;
		} else if(pos.y >= ofGetHeight() - RADIUS) {
			pos.y = ofGetHeight() - RADIUS;
			vel.y *= -BOUNCE_FACTOR; 
		}
	}
	
	void draw() {
		ofSetColor(col.r, col.g, col.b, col.a);
		ofCircle(pos.x, pos.y, 30);
	}
	
	void moveTo(int x, int y) {
		pos.set(x, y, 0);
		vel.set(0, 0, 0);
	}
};
