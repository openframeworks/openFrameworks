#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	ofSetFrameRate(60); // run at 60 fps
	ofSetVerticalSync(true);

	// set bg to a nice grey!
	ofBackground(150);

	// load fonts to display stuff
	font.load("futura_book.otf", 12);
	titleFont.load("futura_book.otf", 20);

	// Enable some logging information
	ofSetLogLevel(OF_LOG_VERBOSE);

	//Server side
	//listen for incoming messages on a port; setup OSC receiver with usage:
	serverRecvPort = 9000;
	serverReceiver.setup(serverRecvPort);
	maxServerMessages = 38;

	//Client side
	clientDestination = "localhost";
	//	clientDestination	= "192.168.0.115"; // if you send to another instance enter IP here
	clientSendPort = 9000;
	clientSender.setup(clientDestination, clientSendPort);

	clientRecvPort = clientSendPort + 1;
	clientReceiver.setup(clientRecvPort);
}

//--------------------------------------------------------------
void ofApp::update(){

	// OSC receiver queues up new messages, so you need to iterate
	// through waiting messages to get each incoming message

	// check for waiting messages
	while(serverReceiver.hasWaitingMessages()){
		// get the next message
		ofxOscMessage m;
		serverReceiver.getNextMessage(&m);
		//Log received message for easier debugging of participants' messages:
		ofLogVerbose("Server recvd msg " + getOscMsgAsString(m) + " from " + m.getRemoteIp());

		// check the address of the incoming message
		if(m.getAddress() == "/typing"){
			//Identify host of incoming msg
			string incomingHost = m.getRemoteIp();
			//See if incoming host is a new one:
			if(std::find(knownClients.begin(), knownClients.end(), incomingHost)
			   == knownClients.end()){
				knownClients.push_back(incomingHost); //add new host to list
			}
			// get the first argument (we're only sending one) as a string
			if(m.getNumArgs() > 0){
				if(m.getArgType(0) == OFXOSC_TYPE_STRING){
					//reimplemented message display:
					//If vector has reached max size, delete the first/oldest element
					if(serverMessages.size() == maxServerMessages){
						serverMessages.erase(serverMessages.begin());
					}
					//Add message text at the end of the vector
					serverMessages.push_back(m.getArgAsString(0));

					//Broadcast message to other chat participants
					broadcastReceivedMessage(m.getArgAsString(0));
				}
			}
		}
		// handle getting random OSC messages here
		else{
			ofLogWarning("Server got weird message: " + m.getAddress());
		}
	}

	// Client side:

	// OSC receiver queues up new messages, so you need to iterate
	// through waiting messages to get each incoming message

	// check for waiting messages
	while(clientReceiver.hasWaitingMessages()){
		// get the next message
		ofxOscMessage m;
		clientReceiver.getNextMessage(&m);
		ofLogNotice("Client just received a message");
		// check the address of the incoming message
		if(m.getAddress() == "/chatlog"){
			// get the first argument (we're only sending one) as a string
			if(m.getNumArgs() > 0){
				if(m.getArgType(0) == OFXOSC_TYPE_STRING){
					string oldMessages = clientMessages;
					clientMessages = m.getArgAsString(0) + "\n" + oldMessages;
				}
			}
		}
	}

	//this is purely workaround for a mysterious OSCpack bug on 64bit linux
	// after startup, reinit the receivers
	// must be a timing problem, though - in debug, stepping through, it works.
	if(ofGetFrameNum() == 60){
		clientReceiver.setup(clientRecvPort);
		serverReceiver.setup(serverRecvPort);
	}
}

//--------------------------------------------------------------
void ofApp::draw(){

	//Display some information about the client on the screen with the font you loaded

	string debug = "Chat client\nSending messages to " + string(clientDestination) + ":"
				   + ofToString(clientSendPort);
	string instructions = "type to create a new message.\nhit RETURN to send!";

	titleFont.drawString(debug, 10, 37);
	titleFont.drawString(instructions, 10, 93);

	// what have we typed so far?
	titleFont.drawString(clientTyping, 10, 160);

	//received messages:
	font.drawString(clientMessages, 10, 180);

	//Display some information about the server
	string title = "Chat server";
	titleFont.drawString(title, 542, 37);
	title = "Listening for messages on port " + ofToString(serverRecvPort) + ".\nKnown chatters: "
			+ ofToString(knownClients.size());
	titleFont.drawString(title, 542, 65);

	//Display received messages:
	serverTyping = "";
	// Concatenate a nice multiline string to display
	for(unsigned int i = 0; i < serverMessages.size(); i++){
		string oldTyping = serverTyping;
		serverTyping = oldTyping + "\n" + serverMessages[i];
	}
	//Display the messages
	font.drawString(serverTyping, 542, 100);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	// if we didn't hit return, add the key to our string
	if(key != OF_KEY_RETURN){
		// some trickery: ignore the backspace key
		if(key != OF_KEY_BACKSPACE){
			clientTyping += key;
		}
		else{
			if(clientTyping.size() > 0){
				clientTyping.erase(clientTyping.end() - 1);
			}
		}
	}
	// hit Return, time to send the osc message
	else{
		// to send a string, create an ofxOscMessage object, give it an address
		// and add a string argument to the object
		ofxOscMessage m;
		m.setAddress("/typing");
		m.addStringArg(clientTyping);
		clientSender.sendMessage(m, false);

		// clear out "typing"
		clientTyping = "";
	}
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){

}

//--------------------------------------------------------------
string ofApp::getOscMsgAsString(ofxOscMessage m){
	string msg_string;
	msg_string = m.getAddress();
	msg_string += ":";
	for(int i = 0; i < m.getNumArgs(); i++){
		// get the argument type
		msg_string += " " + m.getArgTypeName(i);
		msg_string += ":";
		// display the argument - make sure we get the right type
		if(m.getArgType(i) == OFXOSC_TYPE_INT32){
			msg_string += ofToString(m.getArgAsInt32(i));
		}
		else if(m.getArgType(i) == OFXOSC_TYPE_FLOAT){
			msg_string += ofToString(m.getArgAsFloat(i));
		}
		else if(m.getArgType(i) == OFXOSC_TYPE_STRING){
			msg_string += m.getArgAsString(i);
		}
		else{
			msg_string += "unknown";
		}
	}
	return msg_string;
}

void ofApp::broadcastReceivedMessage(string chatmessage){

	//create a new OSC message
	ofxOscMessage m;
	m.setAddress("/chatlog");
	m.addStringArg(chatmessage);

	//Send message to all known hosts
	// use another port to avoid a localhost loop
	for(unsigned int i = 0; i < knownClients.size(); i++){
		serverSender.setup(knownClients[i], serverRecvPort + 1);
		m.setRemoteEndpoint(knownClients[i], serverRecvPort + 1);
		serverSender.sendMessage(m, false);
		ofLogVerbose("Server broadcast message " + m.getArgAsString(0) + " to " + m.getRemoteIp()
					 + ":" + ofToString(m.getRemotePort()));
	}
}
