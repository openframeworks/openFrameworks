#pragma once

#include "ofxiOS.h"

class ofApp : public ofxiOSApp {
	
    public:
        void setup() override;
        void update() override;
        void draw() override;
        void exit() override;
	
        void touchDown(ofTouchEventArgs & touch) override;
        void touchMoved(ofTouchEventArgs & touch) override;
        void touchUp(ofTouchEventArgs & touch) override;
        void touchDoubleTap(ofTouchEventArgs & touch) override;
        void touchCancelled(ofTouchEventArgs & touch) override;

        void lostFocus() override;
        void gotFocus() override;
        void gotMemoryWarning() override;
        void deviceOrientationChanged(int newOrientation) override;
        void launchedWithURL(std::string url) override;

};
