#pragma once

#include "ofxiOS.h"
#include "ofxAssimpModelLoader.h"

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
    
        void loadModel(int modelIndex);

        ofxAssimpModelLoader model;
        int modelIndex;
        int modelsTotal;
        ofLight	light;
};

