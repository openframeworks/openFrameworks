#pragma once

#include "ofMain.h"
#include "Car.h"

/* Refer to the README.md in the example's root folder for more information on usage */

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
		

        ofLight light;
        ofEasyCam cam;
        Car car;
        ofPlanePrimitive plane;
        ofMaterial roadMaterial;
};
