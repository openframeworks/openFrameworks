#pragma once

#include "ofxiOS.h"

#define GRID_WIDTH  16
#define GRID_HEIGHT 16
#define LENGTH		2 * 8

class ofApp : public ofxiOSApp{
	
    public:
        void setup();
        void update();
        void draw();
        void exit();
    
        void touchDown(ofTouchEventArgs & touch);
        void touchMoved(ofTouchEventArgs & touch);
        void touchUp(ofTouchEventArgs & touch);
        void touchDoubleTap(ofTouchEventArgs & touch);
        void touchCancelled(ofTouchEventArgs & touch);
	
        void lostFocus();
        void gotFocus();
        void gotMemoryWarning();
        void deviceOrientationChanged(int newOrientation);
	
        ofVbo vbo;
        glm::vec3 pos[GRID_WIDTH*GRID_HEIGHT*LENGTH];
        glm::vec3 center;

        float restLength;
        int total;
        int space;
        int count;
        bool bPause;
        int zoom;
        int zoomTarget;
};


