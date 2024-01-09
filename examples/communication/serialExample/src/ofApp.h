#pragma once

#include "ofMain.h"

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
		
		ofTrueTypeFont		font;
		ofTrueTypeFont		fontSmall;
		
		// a flag for sending serial
		bool bSendSerialMessage;
		// when did we last read?
		float readTime;
		
		ofSerial serial;
		ofBuffer serialReadBuffer;
		// store all of the received messages
		vector<string> receivedSerialMessages;
		// store a message to send
		string messageToSend;
		// if the device is not connected, when is the last time we tried to connect to it
		float timeLastTryConnect = 0.;
		// store the device list so we can print it out in the draw function
		vector <ofSerialDeviceInfo> deviceList;
		// set this to the device path that you would like to connect to
		// the default will be the first device in the list
		string serialDevicePathToConnectTo = "";
		// keep a history of the bytes read from the serial device so we can split it based on a character
		string serialReadString = "";
};

