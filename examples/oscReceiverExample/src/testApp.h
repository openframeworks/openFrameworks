#ifndef _TEST_APP
#define _TEST_APP


#include "ofMain.h"

#define OF_ADDON_USING_OFXOSC

#include "ofAddons.h"

// listen on port 12345
#define PORT 12345
#define NUM_MSG_STRINGS 20


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
		ofxOscReceiver	receiver;

		int				current_msg_string;
		std::string		msg_strings[NUM_MSG_STRINGS];
		float			timers[NUM_MSG_STRINGS];
		
		int				mouseX, mouseY;
		char			mouseButtonState[128];
};

#endif	
