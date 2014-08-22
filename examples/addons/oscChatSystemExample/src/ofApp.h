#pragma once

#include "ofMain.h"
#include "ofxOsc.h"

/***************************************************************

 oscChatSystemExample

 - this app demonstrates using the ofxOsc addon to create a
 simple chat room
 - it sends chat messages to a server running in the same app
 - optionally, it can talk to another instance of this app on another IP.
 - get the IP address of the second computer
 - in ofApp.cpp inside the setup() function, change the value of
 clientDestination to the other IP as shown in the comment.

 ***************************************************************/

class ofApp : public ofBaseApp {

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y);
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);

		ofTrueTypeFont font; // standard font
		ofTrueTypeFont titleFont; // font for some info in the title line

		//----------------------------------------
		// Client side:

		ofxOscSender clientSender; // all-important ofxOscSender object
		string clientDestination; // IP address we're sending to
		int clientSendPort; // port we're sending to
		string clientTyping; // what we're going to send: some stuff you typed

		ofxOscReceiver clientReceiver; // OSC receiver
		int clientRecvPort; // port where we listen for messages
		string clientMessages; // string containing the received messages for display

		//----------------------------------------
		// Server side:
		ofxOscReceiver serverReceiver; // OSC receiver
		int serverRecvPort; // port we're listening on: must match port from sender!
		string serverTyping; //messages you've received from the clientes

		// Message display variables
		vector<string>serverMessages; //vector containing the received messages for display
		unsigned int maxServerMessages; //nr of messages fitting on the screen

		vector<string>knownClients; //collected IP's of chat participants
		ofxOscSender serverSender;

		//Distribute a received message among the known hosts
		void broadcastReceivedMessage(string chatmessage);

		// Parse an OscMessage into a string for easy logging
		string getOscMsgAsString(ofxOscMessage m);

};
