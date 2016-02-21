#pragma once

#include "ofMain.h"


// This openFrameworks example demonstrates how to use the OS X specific
// video grabber to record synced video and audio to disk.
//
// For more information regarding this example take a look at the README.md.


class ofApp : public ofBaseApp{
	
	public:
		
		void setup();
		void update();
		void draw();
		
		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);		
		
		ofVideoGrabber 			vidGrabber;
		ofPtr<ofQTKitGrabber>	vidRecorder;
    
    	ofVideoPlayer recordedVideoPlayback;
    
		void videoSaved(ofVideoSavedEventArgs& e);
	
    	vector<string> videoDevices;
	    vector<string> audioDevices;
    
        bool bLaunchInQuicktime;
};
