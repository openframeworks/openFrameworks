#pragma once

#include "ofMain.h"
#include "GuiApp.h"

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void radiusChanged(float & radius);

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
		
		shared_ptr<GuiApp> gui;
		shared_ptr<ofAppBaseWindow> window;
		shared_ptr<ofBaseGLRenderer> gl;
		ofSpherePrimitive sphere;
		ofEasyCam camera;
};
