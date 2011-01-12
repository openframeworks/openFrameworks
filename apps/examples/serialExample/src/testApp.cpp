#include "testApp.h"


//--------------------------------------------------------------
void testApp::setup(){	 

	ofSetVerticalSync(true);

	bSendSerialMessage = false;
	ofBackground(255,255,255);	
	
	
	serial.enumerateDevices();
			
	
	//----------------------------------- note:
	// < this should be set
	// to whatever com port
	// your serial device is
	// connected to.
	// (ie, COM4 on a pc, /dev/tty.... on linux, /dev/tty... on a mac)
	// arduino users check in arduino app....

	//serial.setup("COM4");  						  // windows example
	serial.setup("/dev/tty.usbserial-A4001JEC",9600); // mac osx example
	//serial.setup("/dev/ttyUSB0", 9600);			  //linux example

	//----------------------------------- 
	font.loadFont("DIN.otf",64);
	nTimesRead = 0;
	nBytesRead = 0;
	readTime = 0;
	memset(bytesReadString, 0, 4);
}

//--------------------------------------------------------------
void testApp::update(){
	
	if (bSendSerialMessage){
		
		// (1) write the letter "a" to serial:
		serial.writeByte('a');
		
		// (2) read
		// now we try to read 3 bytes
		// since we might not get them all the time 3 - but sometimes 0, 6, or something else,
		// we will try to read three bytes, as much as we can
		// otherwise, we may have a "lag" if we don't read fast enough
		// or just read three every time. now, we will be sure to 
		// read as much as we can in groups of three...

		nTimesRead = 0;
		nBytesRead = 0;
		int nRead  = 0;  // a temp variable to keep count per read
		
		unsigned char bytesReturned[3];

		memset(bytesReadString, 0, 4);
		memset(bytesReturned, 0, 3);
		
		while( (nRead = serial.readBytes( bytesReturned, 3)) > 0){
			nTimesRead++;	
			nBytesRead = nRead;
		};

		memcpy(bytesReadString, bytesReturned, 3);
		
		bSendSerialMessage = false;
		readTime = ofGetElapsedTimef();
	}
}

//--------------------------------------------------------------
void testApp::draw(){

	char tempStr[1024];
	sprintf(tempStr, "click to test serial:\nnBytes read %i\nnTimes read %i\nread: %s\n(at time %0.3f)", nBytesRead, nTimesRead, bytesReadString, readTime);
	
	if (nBytesRead > 0 && ((ofGetElapsedTimef() - readTime) < 0.5f)){
		ofSetHexColor(0x000000);
	} else {
		ofSetHexColor(0xdddddd);
	}
	font.drawString(tempStr, 50,100);

}


//--------------------------------------------------------------
void testApp::keyPressed  (int key){ 
	
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){ 
	
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){
	
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){
	
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
	bSendSerialMessage = true;
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

