#ifndef _TEST_APP
#define _TEST_APP

#include "ofMain.h"
#include "ofxAssimpModelLoader.h"
#include "ofVboMesh.h"

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();
	
		void loadModel(int aindex);
        void loadModel(string filename);

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

		ofxAssimpModelLoader model;

		bool bHelpText;
		bool bAnimate;
		bool bAnimateMouse;
		float animationPosition;
        int animationIndex = 0;
        
		ofMesh mesh;
		ofLight	light;
        ofEasyCam cam;
        
        bool bUseCamera = false;
	
		bool bEnableAutoSwitch = false;
		float mTimeModelLoaded = 0.f;
		int modelIndex = 1;
	
};

#endif
