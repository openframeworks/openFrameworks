#pragma once

#include "ofMain.h"

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();
	
		void audioOut(ofSoundBuffer &outBuffer);

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
	
		double wavePhase;
		double pulsePhase;
	
		std::mutex audioMutex;
		ofSoundStream soundStream;
		ofSoundBuffer lastBuffer;
		ofPolyline waveform;
		float rms;
};
