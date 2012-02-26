#ifndef _TEST_APP
#define _TEST_APP



#include "ofMain.h"
#include "ofEvents.h"
#include "eventsObject.h"



class testApp : public ofBaseApp{

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
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
	

		//method that will receive events from ofxEventsAddon
		void newFloat(float & f);
		void newInt(int & i);


		eventsObject evObject;

		string floatMsg;
		string intMsg;
		string expireMsg;

		bool enabled;

};

#endif

