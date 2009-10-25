#include "testApp.h"


//--------------------------------------------------------------
void testApp::setup(){

	ofSetVerticalSync(true);
	ofSetFrameRate(60);

	ofBackground(255,0,130);

	bgImage.loadImage("firmata.png");
	font.loadFont("franklinGothic.otf", 20);

	ard.connect("/dev/ttyUSB0", 115200);

	bSetupArduino	= false;							// flag so we setup arduino when its ready, you don't need to touch this :)
}

//--------------------------------------------------------------
void testApp::update(){


	if ( ard.isArduinoReady()){

		// 1st: setup the arduino if haven't already:
		if (bSetupArduino == false){
			setupArduino();
			bSetupArduino = true;	// only do this once
		}
		// 2nd do the update of the arduino
		updateArduino();
	}

}

//--------------------------------------------------------------
void testApp::setupArduino(){

	// this is where you setup all the pins and pin modes, etc
	for (int i = 0; i < 13; i++){
		ard.sendDigitalPinMode(i, ARD_OUTPUT);
	}

	ard.sendDigitalPinMode(13, ARD_OUTPUT);
	ard.sendAnalogPinReporting(0, ARD_ANALOG);	// AB: report data
	ard.sendDigitalPinMode(11, ARD_PWM);		// on diecimelia: 11 pwm?*/
}

//--------------------------------------------------------------
void testApp::updateArduino(){

	// update the arduino, get any data or messages:
	ard.update();
	ard.sendPwm(11, (int)(128 + 128 * sin(ofGetElapsedTimef())));   // pwm...

}


//--------------------------------------------------------------
void testApp::draw(){
	bgImage.draw(0,0);

	if (!ard.isArduinoReady()){
		font.drawString("arduino not ready\n", 545, 40);
	} else {
		font.drawString("analog pin 0: " + ofToString(ard.getAnalog(0)) +
						"\nsending pwm: " + ofToString((int)(128 + 128 * sin(ofGetElapsedTimef()))), 545, 40);

	}
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
	ard.sendDigital(13, ARD_HIGH);
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){
	ard.sendDigital(13, ARD_LOW);
}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}
