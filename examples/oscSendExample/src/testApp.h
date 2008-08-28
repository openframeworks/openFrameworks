#ifndef _TEST_APP
#define _TEST_APP

#include "ofMain.h"

#define OF_ADDON_USING_OFXOSC

#include "ofAddons.h"

#define HOST "localhost"
#define PORT 12345

//--------------------------------------------------------
class testApp : public ofSimpleApp{
	
	public:
		
		void setup();
		void update();
		void draw();
		
		void keyPressed  (int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased();
		
		ofTrueTypeFont		font;

	private:
		
		ofxOscSender sender;
			
		
		
};

#endif	
