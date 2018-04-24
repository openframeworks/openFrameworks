#pragma once

#include "ofMain.h"


class ofApp : public ofBaseApp{

	public:

		void setup();
		void update();
		void draw();

		void keyPressed  (int key);
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

		ofSoundPlayer 		beat;
		ofSoundPlayer		ow;
		ofSoundPlayer		dog;
		ofSoundPlayer		rooster;


		// we will bounce a circle using these variables:
		float px = 300;
		float py = 300;
		float vx = 0;
		float vy = 0;
		float prevx, prevy;

		static constexpr size_t nBandsToGet = 128;
		std::array<float, nBandsToGet> fftSmoothed{{0}};
};

