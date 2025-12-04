#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	
	ofSetVerticalSync(true);
	
	bSendSerialMessage = false;
	ofBackground(255);	
	ofSetLogLevel(OF_LOG_VERBOSE);
	
	fontSmall.load("DIN.otf", 12);
	font.load("DIN.otf", 32);
	
	serial.listDevices();
	deviceList = serial.getDeviceList();
	
	// set this string to the device you would like to connect to
	// the following example is for a pen plotter on osx 
	//serialDevicePathToConnectTo = "/dev/tty.usbserial-A10172HG";
	
	
	// this should be set to whatever com port your serial device is connected to.
	// (ie, COM4 on a pc, /dev/tty.... on linux, /dev/tty... on a mac)
	// arduino users check in arduino app....
	
	//serial.setup("COM4", baud); // windows example
	//serial.setup("/dev/tty.usbserial-A4001JEC", baud); // mac osx example
	//serial.setup("/dev/ttyUSB0", baud); //linux example
	int baud = 9600;
	// if the serialDevicePathToConnectTo variable is set, try connecting there first
	// the serial device will try to reconnect in the update app if no device is connected
	if( serialDevicePathToConnectTo != "" ) {
		serial.setup(serialDevicePathToConnectTo, baud);
	} else {
		serial.setup(0, baud); //open the first device
	}
	
	// we just tried to connect, so store the time
	timeLastTryConnect = ofGetElapsedTimef();
	readTime = 0;
}

//--------------------------------------------------------------
void ofApp::update(){
	
	if( serial.isInitialized() ) {
		if( bSendSerialMessage ) {
			
			// send out the message to send over serial
			serial.writeBytes( messageToSend.c_str(), (int)messageToSend.size() );
			// clear the message since it has already been sent.
			messageToSend = "";
			// we have sent the message, so clear the boolean flag
			bSendSerialMessage = false;
			
			// another way to construct a string for sending
//			string tstr;
//			tstr.resize( 3 );
//			tstr[0] = 27;
//			tstr[1] = '.';
//			tstr[2] = 'B';
//			serial.writeBytes( tstr.c_str(), (int)tstr.size() );
			
			// one way to send a single byte
//			serial.writeByte('a');
		}
		
		// the number of bytes that the serial has available for reading
		int numBytesToRead = serial.available();
		// cap the amount of bytes to read at 512
		if( numBytesToRead > 512 ) {
			numBytesToRead = 512;
		}
		if( numBytesToRead > 0 ) {
			serialReadBuffer.clear();
			// if there are bytes to read, then read it into the serialReadBuffer
			serial.readBytes(serialReadBuffer, numBytesToRead );
			// append the string with the new buffer of data that has arrived
			serialReadString += serialReadBuffer.getText();
			
			// create a temporary string that will populate from the read string
			string fullMessage = "";
			int eraseIndex = -1;
			// go through each character in the string to search for a return character
			for( int i = 0; i < serialReadString.length(); i++ ) {
				unsigned char character = serialReadString[i];
				// if we run into a return character, lets make this the end of the message
				// and we check if the fullMessage is larger than 512, we also create a new message
				if (character == '\n' || character == '\r' || character == '\t' || character == 13 || fullMessage.length() > 512) {
					if( fullMessage.length() > 0 ) {
						// store the new message in a vector
						receivedSerialMessages.push_back(fullMessage);
						// store the index of this character so that we can erase from the string
						eraseIndex = i;
					}
					fullMessage = "";
				}
				fullMessage += character;
			}
			if( eraseIndex > -1 ) {
				// a new message has been received and must be erased
				serialReadString = serialReadString.substr(eraseIndex);
			}
			
			readTime = ofGetElapsedTimef();
		}
	} else {
		// serial device is not initialized
		// lets try to connect again
		float etimef = ofGetElapsedTimef();
		if( etimef - timeLastTryConnect > 10.0 ) {
			// refresh device list
			deviceList = serial.getDeviceList();
			timeLastTryConnect = etimef;
			int baud = 9600;
			if( serialDevicePathToConnectTo != "" ) {
				ofLogNotice("Attempting to connect to serial device: ") << serialDevicePathToConnectTo;
				serial.setup(serialDevicePathToConnectTo, baud);
			} else {
				ofLogNotice("Attempting to connect to serial device: ") << 0;
				serial.setup(0, baud); //open the first device
			}
		}
	}
	
	// clear out the older serial messages //
	if( receivedSerialMessages.size() > 10 ) {
		receivedSerialMessages.erase( receivedSerialMessages.begin() );
	}
}

//--------------------------------------------------------------
void ofApp::draw(){
	ofSetColor( 40 );
	
	fontSmall.drawString("Serial connected: "+ofToString(serial.isInitialized()), 50, 40 );
	
	string deviceString = "Devices:\n";
	for( int i = 0; i < deviceList.size(); i++ ) {
		deviceString += ofToString(deviceList[i].getDeviceID())+": " +deviceList[i].getDevicePath() + " \n";
	}
	fontSmall.drawString( deviceString, 50, 80 );
	
	string msg = "Type send message\n";
	if( messageToSend.length() > 0 ) {
		msg += messageToSend;
	}
	font.drawString(msg, 50, 400);
	
	float posY = 60;
	font.drawString("Received messages", 550, posY);
	posY += 42;
	if( receivedSerialMessages.size() > 0 ) {
		
		for( int i = (int)receivedSerialMessages.size()-1; i >= 0; i-- ) {
			if (i == (int)receivedSerialMessages.size()-1 && ((ofGetElapsedTimef() - readTime) < 0.5f)){
				ofSetColor(40);
			} else {
				ofSetColor(120);
			}
			font.drawString(receivedSerialMessages[i], 550, posY);
			posY += 42;
		}
	}
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	// if return is pressed, then flag the message to send
	// it will be sent in the update() function
	if( key == OF_KEY_RETURN || key == 13 ) {
		if( messageToSend.length() > 0 ) {
			bSendSerialMessage = true;
		}
	} else if( key == OF_KEY_DEL || key == 27 || key == 8 ) {
		if( messageToSend.length() > 0 ) {
			messageToSend.pop_back();
		}
	} else if( key < 300 ) {
		messageToSend += (unsigned char)key;
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

