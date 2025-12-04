#pragma once

#include "ofMain.h"
#if (_MSC_VER)
	#define strcasecmp _stricmp
#endif

typedef struct {
	string name;
	ofColor color;
} colorNameMapping;

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
	
		map < string, ofColor > colorNameMap;
	
		vector < colorNameMapping > colorNames;
	
		glm::vec3 mouseSmoothed;

		int sortedType;             // keep track of which sort we've done
	
};
