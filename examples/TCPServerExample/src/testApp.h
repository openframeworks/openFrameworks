#ifndef _TEST_APP
#define _TEST_APP

#define OF_ADDON_USING_OFXNETWORK

#include "ofMain.h"
#include "ofAddons.h"

class testApp : public ofSimpleApp{
	
	public:
		
		void setup();
		void update();
		void draw();
		
		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased();
		
		ofxTCPServer TCP;
		
		ofTrueTypeFont  mono;
		ofTrueTypeFont  monosm;
		
		vector <string> storeText;
};

#endif	

