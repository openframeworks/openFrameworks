#pragma once

#include "ofMain.h"
#include "ofxOsc.h"

// demonstrates the optional pattern as well as constructors and method chaining

class ofApp : public ofBaseApp{
	
	const int port { 12345 } ; // listening port
	ofxOscReceiver osc_receiver { port };
	ofxOscSender osc_sender { "127.0.0.1", port };
	
public:
	
	void setup() override;
	void update() override;
	void draw() override;
	
	void keyPressed(int key) override;
	
	// ofBaseApp interface boilerplate: //
	
	//	void keyReleased(int key) override;
	//	void mouseMoved(int x, int y) override;
	//	void mouseDragged(int x, int y, int button) override;
	//	void mousePressed(int x, int y, int button) override;
	//	void mouseReleased(int x, int y, int button) override;
	//	void mouseScrolled(int x, int y, float scrollX, float scrollY) override;
	//	void mouseEntered(int x, int y) override;
	//	void mouseExited(int x, int y) override;
	//	void windowResized(int w, int h) override;
	//	void dragEvent(ofDragInfo dragInfo) override;
	//	void gotMessage(ofMessage msg) override;
};
