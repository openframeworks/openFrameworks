#ifndef _TEST_APP
#define _TEST_APP


#define OF_ADDON_USING_OFXOPENCV
#define OF_ADDON_USING_OFXOBJLOADER
#define OF_ADDON_USING_OFXDIRLIST
#define OF_ADDON_USING_OFXVECTORMATH
#define OF_ADDON_USING_OFXNETWORK
#define OF_ADDON_USING_OFXVECTORGRAPHICS
#define OF_ADDON_USING_OFXOSC
#define OF_ADDON_USING_OFXTHREAD
#define OF_ADDON_USING_OFXXMLSETTINGS


#include "ofMain.h"
#include "ofAddons.h"

class testApp : public ofSimpleApp{
	
	public:
		
		void setup();
		void update();
		void draw();
		
		void keyPressed  (int key);
		void keyReleased (int key);
		
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased();
		
		// we don't actually use these
		// just checking to see if they 
		// all work in the same place :)
		
		ofxCvGrayscaleImage cvGray;
		ofxObjLoader obj;
		ofxDirList dirList;
		ofxVec2f p;
		ofxTCPClient client;
		ofxTCPServer server;
		ofxOscSender osc_sender;
		ofxThread thread;
		ofxXmlSettings settings;
};

#endif
	
