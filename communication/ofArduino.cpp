/*
 * Copyright 2007-2008 (c) Erik Sjodin, eriksjodin.net
 *
 * Devloped at: The Interactive Institutet / Art and Technology,
 * OF Lab / Ars Electronica
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use,
 * copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following
 * conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial _portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 */

#include "ofArduino.h"
#include "ofUtils.h"

ofArduino::ofArduino():ofStandardFirmata(){
	for (int i=0; i<ARD_TOTAL_DIGITAL_PINS; ++i) {
		_servoValue[i] = -1;
	}
	bUseDelay = true;
}

ofArduino::~ofArduino() {
}

int ofArduino::connect(string device, int baud){
	connectTime = ofGetElapsedTimef();
	connected = ofStandardFirmata::connect(device, baud);
	return connected;
}

bool ofArduino::isArduinoReady(){
	if(bUseDelay)
		return (ofGetElapsedTimef() - connectTime) > OF_ARDUINO_DELAY_LENGTH ? connected : false;
	else
		return connected;
}

void  ofArduino::setUseDelay(bool bDelay){
	bUseDelay = bDelay;
}

void ofArduino::sendServo(int pin, int value, bool force){
	if(_digitalPinMode[pin]==ARD_SERVO && (_servoValue[pin]!=value || force)){
		sendByte(FIRMATA_START_SYSEX);
		sendByte(SYSEX_SERVO_WRITE);
		sendByte(pin);
		sendValueAsTwo7bitBytes(value);
		sendByte(FIRMATA_END_SYSEX);
		_servoValue[pin]=value;
	}
}

void ofArduino::sendServoAttach(int pin, int minPulse, int maxPulse, int angle) {
	sendByte(FIRMATA_START_SYSEX);
	sendByte(SYSEX_SERVO_ATTACH);
	sendByte(pin);
	sendValueAsTwo7bitBytes(minPulse);
	sendValueAsTwo7bitBytes(maxPulse);
	sendByte(FIRMATA_END_SYSEX);
	_digitalPinMode[pin]=ARD_SERVO;
}

void ofArduino::sendServoDetach(int pin) {
	sendByte(FIRMATA_START_SYSEX);
	sendByte(SYSEX_SERVO_DETACH);
	sendByte(pin);
	sendByte(FIRMATA_END_SYSEX);
	_digitalPinMode[pin]=ARD_OUTPUT;
}

int ofArduino::getServo(int pin){
	if(_digitalPinMode[pin]==ARD_SERVO)
		return _servoValue[pin];
	else
		return -1;
}


// sysex data is assumed to be 8-bit bytes split into two 7-bit bytes.
void ofArduino::processSysExData(vector<unsigned char> data){

	vector<unsigned char>::iterator it;
	//unsigned char buffer;
	//int i = 1;

	// act on reserved sysEx messages (extended commands) or trigger SysEx event...
	/*
	
	// zach - altered because VS is unhappy with switches that have no case.

	switch(data.front()) { //first byte in buffer is command
		default: // the message isn't in this class extended command set, pass it...
			ofStandardFirmata::processSysExData(data);
		break;
	}*/

	ofStandardFirmata::processSysExData(data);
}

