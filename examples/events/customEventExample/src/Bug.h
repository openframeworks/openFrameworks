#pragma once
#include "ofMain.h"

class Bug {
    
public:  
    
    ofVec2f          pos, vel;
    float            size;
    vector <ofVec2f> squashPts;
    bool             bSquashed;
    bool             bRemove;
    float            timeBugKilled;
    
    Bug() {        
        
        bRemove       = false;
        bSquashed     = false;
        size          = 10;
        timeBugKilled = 0;
        
        // make a squash shape
        int nPts = 10;
        for(int i=0; i<nPts; i++) {
            squashPts.push_back(ofVec2f(ofRandom(-4, 4), ofRandom(-4, 4)));
        }
    }
    
    void update() {
        if(!bSquashed) {
            pos += vel;
            float speed = 0.03;
            vel.x += ofRandom(-speed, speed);
            vel.y += ofRandom(-speed, speed);
        }
        else {
            float timeDead = ofGetElapsedTimef() - timeBugKilled;
            if(timeDead > 0.3) {
                bRemove = true;
            }
        }
    }
    
    void draw() {
        
        ofSetHexColor(0x688736);               

        // the bug
        if(!bSquashed) {
            ofPushMatrix();
            ofTranslate(pos);
            
            float angle = 90+ofRadToDeg(atan2(vel.y, vel.x));
            ofRotateZ(angle);
            
            ofDrawEllipse(0, 0, 5, 3);
            ofDrawLine(+1, 0, +3, -4);
            ofDrawLine(-1, 0, -3, -4);
            
            ofDrawEllipse(0, +3, 3, 4);
            ofDrawEllipse(0, +6, 3, 3);
            ofDrawEllipse(0, +8, 2, 3);
            
            ofDrawLine(0, +3, +4, +3);
            ofDrawLine(0, +5, +4, +5);
            ofDrawLine(0, +6, +4, +7);
            
            ofDrawLine(0, +3, -4, +3);
            ofDrawLine(0, +5, -4, +5);
            ofDrawLine(0, +6, -4, +7);
            ofPopMatrix();
        }
        
        // the squash
        else {
            ofSetHexColor(0xE32289);
            for(unsigned int i=0; i<squashPts.size(); i++) {
				ofDrawCircle(pos + squashPts[i], 2);
            }
        }
        
        
    }
};
