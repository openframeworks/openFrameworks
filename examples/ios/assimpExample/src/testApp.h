#pragma once

#include "ofMain.h"
#include "ofxiPhone.h"
#include "ofxiPhoneExtras.h"
#include "ofMain.h"

#include "ofxAssimpModelLoader.h"
#include "ofVboMesh.h"

class testApp : public ofxiPhoneApp{

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

