#pragma once

#include "ofMain.h"

class testApp : public ofBaseApp{
	public:
		void setup();
		void update();
		void draw();
		
		void drawFboTest(); // draws some test graphics into the two fbos
		
		void keyPressed  (int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
		
		//8 bits red, 8 bits green, 8 bits blue, from 0 to 255 in 256 steps
		ofFbo rgbaFbo; // with alpha
		
		//32 bits red, 32 bits green, 32 bits blue, from 0 to 1 in 'infinite' steps	
		ofFbo rgbaFboFloat; // with alpha
		
		int fadeAmnt;
};
