#pragma once

#include "ofxiOS.h"
#include "ofxGui.h"

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

        void circleResolutionChanged(int & circleResolution);
        void ringButtonPressed();

        bool bHide;

        ofParameter<float> radius;
        ofParameter<ofColor> color;
        ofParameter<glm::vec2> center;
        ofParameter<int> circleResolution;
        ofParameter<bool> filled;
        ofxButton twoCircles;
        ofxButton ringButton;
        ofParameter<string> screenSize;

        ofxPanel gui;

        ofSoundPlayer ring;
};

