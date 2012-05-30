#pragma once
#include "ofMain.h"

class testApp : public ofBaseApp {

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y);
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);

		OF_DEPRECATED_MSG("This is an old function, don't use!", void deprecatedFunction(void));
		OF_DEPRECATED_MSG("Obsolete type, use float instead!", typedef int T1);
		OF_DEPRECATED_MSG("This is an old variable, don't use!", int oldvar3);
};

OF_DEPRECATED_MSG("old class!", class oldClass {

					  public:
						  oldClass(){
						  };
						  ~oldClass(){}
				  });
