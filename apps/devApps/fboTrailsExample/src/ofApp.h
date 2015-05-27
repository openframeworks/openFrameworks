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
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
		
		ofFbo nonFloatingPointFbo_GL_RGB; // without alpha, 8 bits red, 8 bits green, 8 bits blue, from 0 to 255 in 256 steps
		ofFbo nonFloatingPointFbo_GL_RGBA; // with alpha, 32 bits red, 32 bits green, 32 bits blue, from 0 to 1 in 'infinite' steps
		
		ofFbo floatingPointFbo_GL_RGB32F_ARB; // without alpha
		ofFbo floatingPointFbo_GL_RGBA32F_ARB; // with alpha
};
