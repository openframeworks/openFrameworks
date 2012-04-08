#pragma once

#define BOUNCE_FACTOR			0.7
#define ACCELEROMETER_FORCE		0.2
#define RADIUS					20


class Ball{

    public:
        ofPoint pos;
        ofPoint vel;
        ofColor col;
        ofColor touchCol;
        bool bDragged;
	
        //----------------------------------------------------------------	
        void init(int id) {
            pos.set(ofRandomWidth(), ofRandomHeight(), 0);
            vel.set(ofRandomf(), ofRandomf(), 0);
		
            float val = ofRandom( 30, 100 );
            col.set( val, val, val, 120 );
		
            if( id % 3 == 0  ){
                touchCol.setHex(0x809d00);
            }else if(  id % 3 == 1){
                touchCol.setHex(0x009d88);
            }else{
                touchCol.setHex(0xf7941d);
            }
		
            bDragged = false;
        }
	
        //----------------------------------------------------------------	
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
	
        //----------------------------------------------------------------
        void draw() {
            if( bDragged ){
                ofSetColor(touchCol);
                ofCircle(pos.x, pos.y, 80);
            }else{
                ofSetColor(col);		
                ofCircle(pos.x, pos.y, RADIUS);
            }
        }
	
        //----------------------------------------------------------------	
        void moveTo(int x, int y) {
            pos.set(x, y, 0);
            vel.set(0, 0, 0);
        }
};
