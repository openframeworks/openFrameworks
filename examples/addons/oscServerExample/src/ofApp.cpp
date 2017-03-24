#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){

	// listen on the given port
	cout << "server listening for OSC messages on port " << SERVER_PORT << "\n";
	receiver.setup(SERVER_PORT);

	ofBackground(30, 30, 130);
	ofSetVerticalSync(true);
	ofSetFrameRate(60);
}

//--------------------------------------------------------------
void ofApp::update(){

	log.update();

	expireOldClients();

	// check for waiting messages
	while(receiver.hasWaitingMessages()){
		// get the next message
		ofxOscMessage m;
		receiver.getNextMessage(m);

		ClientInfo& currentClient = getClientFromMessage(m);
		currentClient.resetExpiration(10.f);

		// check for mouse moved message
		if("/mouse/position" == m.getAddress()){
			// both the arguments are int32's
			currentClient.mousePos.x = m.getArgAsInt32(0);
			currentClient.mousePos.y = m.getArgAsInt32(1);
		}
		// check for mouse button message
		else if("/mouse/button" == m.getAddress()){
			// the arguments are button id and a string
			currentClient.mouseButtonState = m.getArgAsString(1);

			if("down" == currentClient.mouseButtonState){
				ofxOscMessage response;
				response.setAddress("/calculate/sum");
				response.addInt32Arg(currentClient.mousePos.x + currentClient.mousePos.y);
				currentClient.sender->sendMessage(response, false);
			}
		}
		else{
			// unrecognized message: display on the bottom of the screen
			// and return an error back to the client
			log.append("[" + ofToString(m.getRemotePort()) +"] Unsupported: " + m.getAddress(), 5.0f);

			string msg_string;
			msg_string = m.getAddress();
			msg_string += ": ";
			for(int i = 0; i < m.getNumArgs(); i++){
				// get the argument type
				msg_string += m.getArgTypeName(i);
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
			ofxOscMessage response;
			response.setAddress("/error");
			response.addStringArg("Server doesn't recognize '" + msg_string + "'");
			currentClient.sender->sendMessage(response, false);
		}

	}
}


//--------------------------------------------------------------
void ofApp::draw(){

	string buf;
	buf = "OSC server listening for messages on port " + ofToString(SERVER_PORT);
	ofDrawBitmapString(buf, 10, 20);

	int offsetY = 40;
	drawClients(300, offsetY);

	log.draw(10, 100);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

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
void ofApp::expireOldClients(){

	for(auto it = oscClients.cbegin(); it != oscClients.cend(); /* no increment */){
		ClientInfo const& client = it->second;
		if(client.hasExpired()){
			string clientAddress = it->first;
			oscClients.erase(it++);
			ofLog(OF_LOG_NOTICE, "Client: " + clientAddress + " expired (now have " + ofToString(oscClients.size()) + ")");
		}
		else{
			++it;
		}
	}
}

//--------------------------------------------------------------
ClientInfo& ofApp::getClientFromMessage(ofxOscMessage const& m){

	string clientAddress = m.getRemoteIp() + ":" + ofToString(m.getRemotePort());
	if(oscClients.find(clientAddress) == oscClients.end()){
		// client not found: add a new one to the map of active clients
		ClientInfo newClient;
		newClient.setup(m.getRemoteIp(), m.getRemotePort());
		oscClients[clientAddress] = newClient;
		ofLog(OF_LOG_NOTICE, "New client: " + clientAddress + " (now have " + ofToString(oscClients.size()) + ")");
	}
	return oscClients[clientAddress];
}

void ofApp::drawClients(int x, int y){

	string buf;
	int clientId = 0;

	for(auto const& entry: oscClients){
		ClientInfo const& client = entry.second;
		// draw mouse state
		buf = "[" + entry.first + ofToString(client.timeToLive()+0.5, 0, 3, ' ') + "s] xy: " + ofToString(client.mousePos, 0);
		ofDrawBitmapString(buf, x, y);
		ofDrawBitmapString(client.mouseButtonState, x + 300, y);
		++clientId;
		y += 15;
	}
}
