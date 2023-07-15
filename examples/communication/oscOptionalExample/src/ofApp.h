#pragma once

#include "ofMain.h"
#include "ofxOsc.h"

// demonstrates the optional pattern as well as constructors and method chaining

// listening port
#define PORT 12345

class ofApp : public ofBaseApp{
	
	ofxOscReceiver osc_receiver { PORT };
	ofxOscSender osc_sender { "127.0.0.1", PORT };
	
public:
	
	void setup() override;
	void update() override;
	void draw() override;
	
	void keyPressed(int key) override;
	
/* boilerplate:
	 void keyReleased(int key);
	 void mouseMoved(int x, int y);
	 void mouseDragged(int x, int y, int button);
	 void mousePressed(int x, int y, int button);
	 void mouseReleased(int x, int y, int button);
	 void mouseEntered(int x, int y);
	 void mouseExited(int x, int y);
	 void windowResized(int w, int h);
	 void dragEvent(ofDragInfo dragInfo);
	 void gotMessage(ofMessage msg);
*/
	
};
