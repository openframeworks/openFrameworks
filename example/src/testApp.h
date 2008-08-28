#ifndef _TEST_APP
#define _TEST_APP

#define OF_ADDON_USING_OFXXMLSETTINGS

#include "ofMain.h"
#include "ofAddons.h"

#define NUM_PTS 800

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
		
		ofxXmlSettings XML;
		ofTrueTypeFont TTF;

		string xmlStructure;
		string message;
		
		ofPoint dragPts[NUM_PTS];
				
		int pointCount;
		int lineCount;
		int lastTagNumber;
		
		float red;
		float green;
		float blue;
};

#endif	

