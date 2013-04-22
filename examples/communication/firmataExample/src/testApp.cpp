/*
 * This is a simple example use of ofArduino
 *
 * ofArduino currently only supports the standard Arduino boards
 * (UNO, Duemilanove, Diecimila, NG, and other boards based on the
 * ATMega168 or ATMega328 microcontrollers
 * The Arduio FIO and Arduino Mini should also work.
 * The Arduino MEGA and other variants based on microcontrollers
 * other than the ATMega168 and ATMega328 are not currently supported.
 * 
 * To use this example, open Arduino (preferably Arduino 1.0) and 
 * navigate to File -> Examples -> Firmata and open StandardFirmata.
 * Compile and upload StandardFirmata for your board, then close
 * the Arduino application and run this application.
 *
 * If you have a servo attached, press the left arrow key to rotate
 * the servo head counterclockwise and press the right arrow key to
 * rotate the servo head clockwise.
 *
 * Clicking the mouse over any part of the application will turn the
 * on-board LED on and off.
 *
 */

#include "testApp.h"


//--------------------------------------------------------------
void testApp::setup(){

	ofSetVerticalSync(true);
	ofSetFrameRate(60);

	ofBackground(255,0,130);
    
    buttonState = "digital pin:";
    potValue = "analog pin:";

	bgImage.loadImage("background.png");
	font.loadFont("franklinGothic.otf", 20);
    smallFont.loadFont("franklinGothic.otf", 14);

    // replace the string below with the serial port for your Arduino board
    // you can get this from the Arduino application or via command line
    // for OSX, in your terminal type "ls /dev/tty.*" to get a list of serial devices
	ard.connect("/dev/tty.usbmodemfd121", 57600);
	
	// listen for EInitialized notification. this indicates that
	// the arduino is ready to receive commands and it is safe to
	// call setupArduino()
	ofAddListener(ard.EInitialized, this, &testApp::setupArduino);
	bSetupArduino	= false;	// flag so we setup arduino when its ready, you don't need to touch this :)
}

//--------------------------------------------------------------
void testApp::update(){

	updateArduino();

}

//--------------------------------------------------------------
void testApp::setupArduino(const int & version) {
	
	// remove listener because we don't need it anymore
	ofRemoveListener(ard.EInitialized, this, &testApp::setupArduino);
    
    // it is now safe to send commands to the Arduino
    bSetupArduino = true;
    
    // print firmware name and version to the console
    ofLogNotice() << ard.getFirmwareName(); 
    ofLogNotice() << "firmata v" << ard.getMajorFirmwareVersion() << "." << ard.getMinorFirmwareVersion();
        
    // Note: pins A0 - A5 can be used as digital input and output.
    // Refer to them as pins 14 - 19 if using StandardFirmata from Arduino 1.0.
    // If using Arduino 0022 or older, then use 16 - 21.
    // Firmata pin numbering changed in version 2.3 (which is included in Arduino 1.0)
    
    // set pins D2 and A5 to digital input
    ard.sendDigitalPinMode(2, ARD_INPUT);
    ard.sendDigitalPinMode(19, ARD_INPUT);  // pin 21 if using StandardFirmata from Arduino 0022 or older

    // set pin A0 to analog input
    ard.sendAnalogPinReporting(0, ARD_ANALOG);
    
    // set pin D13 as digital output
	ard.sendDigitalPinMode(13, ARD_OUTPUT);
    // set pin A4 as digital output
    ard.sendDigitalPinMode(18, ARD_OUTPUT);  // pin 20 if using StandardFirmata from Arduino 0022 or older

    // set pin D11 as PWM (analog output)
	ard.sendDigitalPinMode(11, ARD_PWM);
    
    // attach a servo to pin D9
    // servo motors can only be attached to pin D3, D5, D6, D9, D10, or D11
    ard.sendServoAttach(9);
	
    // Listen for changes on the digital and analog pins
    ofAddListener(ard.EDigitalPinChanged, this, &testApp::digitalPinChanged);
    ofAddListener(ard.EAnalogPinChanged, this, &testApp::analogPinChanged);    
}

//--------------------------------------------------------------
void testApp::updateArduino(){

	// update the arduino, get any data or messages.
    // the call to ard.update() is required
	ard.update();
	
	// do not send anything until the arduino has been set up
	if (bSetupArduino) {
        // fade the led connected to pin D11
		ard.sendPwm(11, (int)(128 + 128 * sin(ofGetElapsedTimef())));   // pwm...
	}

}

// digital pin event handler, called whenever a digital pin value has changed
// note: if an analog pin has been set as a digital pin, it will be handled
// by the digitalPinChanged function rather than the analogPinChanged function.

//--------------------------------------------------------------
void testApp::digitalPinChanged(const int & pinNum) {
    // do something with the digital input. here we're simply going to print the pin number and
    // value to the screen each time it changes
    buttonState = "digital pin: " + ofToString(pinNum) + " = " + ofToString(ard.getDigital(pinNum));
}

// analog pin event handler, called whenever an analog pin value has changed

//--------------------------------------------------------------
void testApp::analogPinChanged(const int & pinNum) {
    // do something with the analog input. here we're simply going to print the pin number and
    // value to the screen each time it changes
    potValue = "analog pin: " + ofToString(pinNum) + " = " + ofToString(ard.getAnalog(pinNum));
}


//--------------------------------------------------------------
void testApp::draw(){
	bgImage.draw(0,0);
    
    ofEnableAlphaBlending();
    ofSetColor(0, 0, 0, 127);
    ofRect(510, 15, 275, 150);
    ofDisableAlphaBlending();
    
    ofSetColor(255, 255, 255);
	if (!bSetupArduino){
		font.drawString("arduino not ready...\n", 515, 40);
	} else {
		font.drawString(potValue + "\n" + buttonState +
						"\nsending pwm: " + ofToString((int)(128 + 128 * sin(ofGetElapsedTimef()))), 515, 40);
        
        ofSetColor(64, 64, 64);
        smallFont.drawString("If a servo is attached, use the left arrow key to rotate " 
                             "\ncounterclockwise and the right arrow key to rotate clockwise.", 200, 550);
        ofSetColor(255, 255, 255);

	}
}

//--------------------------------------------------------------
void testApp::keyPressed  (int key){
    switch (key) {
        case OF_KEY_RIGHT:
            // rotate servo head to 180 degrees
            ard.sendServo(9, 180, false);
            ard.sendDigital(18, ARD_HIGH);  // pin 20 if using StandardFirmata from Arduino 0022 or older
            break;
        case OF_KEY_LEFT:
            // rotate servo head to 0 degrees
            ard.sendServo(9, 0, false);
            ard.sendDigital(18, ARD_LOW);  // pin 20 if using StandardFirmata from Arduino 0022 or older
            break;
        default:
            break;
    }
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
    // turn on the onboard LED when the application window is clicked
	ard.sendDigital(13, ARD_HIGH);
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){
    // turn off the onboard LED when the application window is clicked
	ard.sendDigital(13, ARD_LOW);
}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 

}