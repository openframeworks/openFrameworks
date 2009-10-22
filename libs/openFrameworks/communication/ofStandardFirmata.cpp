/*
 * Copyright 2007-2008 (c) Erik Sjodin, eriksjodin.net
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

#include "ofStandardFirmata.h"


// TODO thread it?
// TODO throw event or exception if the serial port goes down...
//---------------------------------------------------------------------------
ofStandardFirmata::ofStandardFirmata(){
	_portStatus=-1;
	_waitForData=0;
	_analogHistoryLength = 2;
	_digitalHistoryLength = 2;
	_stringHistoryLength = 1;
	_sysExHistoryLength = 1;

	_majorProtocolVersion = 0;
	_minorProtocolVersion = 0;
	_majorFirmwareVersion = 0;
	_minorFirmwareVersion = 0;
	_firmwareName = "Unknown";

    // ports
	for(int i=0; i<ARD_TOTAL_PORTS; ++i) {
		_digitalPortValue[i]=0;
		_digitalPortReporting[i] = ARD_OFF;
	}

    // digital pins
	for(int i=0; i<16; ++i) {
		_digitalPinValue[i] = -1;
		_digitalPinMode[i] = ARD_OUTPUT;
		_digitalPinReporting[i] = ARD_OFF;
	}

	// analog in pins
	for (int i=0; i<ARD_TOTAL_ANALOG_PINS; ++i) {
		_analogPinReporting[i] = ARD_OFF;
		// analog pins used as digital
		_digitalPinMode[i]=ARD_ANALOG;
		_digitalPinValue[i] = -1;
	}
}

ofStandardFirmata::~ofStandardFirmata() {
	_port.close();
}

bool ofStandardFirmata::connect(string device, int baud){
	_initialized = false;
	_port.enumerateDevices();
	return _port.setup(device.c_str(), baud);
}

void ofStandardFirmata::setDigitalHistoryLength(int length){
	if(length>=2)
		_digitalHistoryLength=length;
}

void ofStandardFirmata::setAnalogHistoryLength(int length){
	if(length>=2)
		_analogHistoryLength=length;
}

void ofStandardFirmata::setSysExHistoryLength(int length){
	if(length>=1)
		_sysExHistoryLength=length;
}

void ofStandardFirmata::setStringHistoryLength(int length){
	if(length>=1)
		_stringHistoryLength=length;
}

void ofStandardFirmata::disconnect(){
	_port.close();
}

void ofStandardFirmata::update(){
	int dataRead=0;
	// try to empty the _port buffer
	while (dataRead<512) {

		int byte = _port.readByte();

		// process data....
		if (byte!=-1) {
			processData((char)(byte));
			dataRead++;
		}
		// _port buffer is empty
		else{
			break;
		}
	}
}

int ofStandardFirmata::getAnalog(int pin){
	if(_analogHistory[pin].size()>0)
		return _analogHistory[pin].front();
	else
		return -1;
}

int ofStandardFirmata::getDigital(int pin){
	if(_digitalPinMode[pin]==ARD_INPUT && _digitalHistory[pin].size()>0)
		return _digitalHistory[pin].front();
	else if (_digitalPinMode[pin]==ARD_OUTPUT)
		return _digitalPinValue[pin];
	else
		return -1;
}

int ofStandardFirmata::getPwm(int pin){
	if(_digitalPinMode[pin]==ARD_PWM)
		return _digitalPinValue[pin];
	else
		return -1;
}

vector<unsigned char> ofStandardFirmata::getSysEx(){
	return _sysExHistory.front();
}

string ofStandardFirmata::getString(){
	return _stringHistory.front();
}

int ofStandardFirmata::getDigitalPinMode(int pin){
	return _digitalPinMode[pin];
}

void ofStandardFirmata::sendDigital(int pin, int value, bool force){
	if((_digitalPinMode[pin]==ARD_INPUT || _digitalPinMode[pin]==ARD_OUTPUT) && (_digitalPinValue[pin]!=value || force)){

		_digitalPinValue[pin] = value;

		int port=0;
		int bit=0;

		if(pin < 8 && pin >1){
			port=0;
			bit = pin;
		}
		else if(pin>7 && pin <14){
			port = 1;
			bit = pin-8;
		}
		else if(pin>15 && pin <22){
			port = 2;
			bit = pin-16;
		}

		// set the bit
		if(value==1)
			_digitalPortValue[port] |= (1 << bit);

		// clear the bit
		if(value==0)
			_digitalPortValue[port] &= ~(1 << bit);

		sendByte(FIRMATA_DIGITAL_MESSAGE+port);
		sendValueAsTwo7bitBytes(_digitalPortValue[port]);

	}
}

void ofStandardFirmata::sendPwm(int pin, int value, bool force){
	if(_digitalPinMode[pin]==ARD_PWM && (_digitalPinValue[pin]!=value || force)){
		sendByte(FIRMATA_ANALOG_MESSAGE+pin);
		sendValueAsTwo7bitBytes(value);
		_digitalPinValue[pin] = value;
	}
}

void ofStandardFirmata::sendSysEx(int command, vector<unsigned char> data){
	sendByte(FIRMATA_START_SYSEX);
	sendByte(command);
	vector<unsigned char>::iterator it = data.begin();
	while( it != data.end() ) {
		sendByte(*it);
		it++;
	}
	sendByte(FIRMATA_END_SYSEX);
}

void ofStandardFirmata::sendSysExBegin(){
	sendByte(FIRMATA_START_SYSEX);
}

void ofStandardFirmata::sendSysExEnd(){
	sendByte(FIRMATA_END_SYSEX);
}

void ofStandardFirmata::sendString(string str){
	sendByte(FIRMATA_START_SYSEX);
	sendByte(FIRMATA_SYSEX_FIRMATA_STRING);
	string::iterator it = str.begin();
	while( it != str.end() ) {
		sendValueAsTwo7bitBytes(*it);
		it++;
	}
	sendByte(FIRMATA_END_SYSEX);
}

void ofStandardFirmata::sendProtocolVersionRequest(){
	sendByte(FIRMATA_REPORT_VERSION);
}

void ofStandardFirmata::sendFirmwareVersionRequest(){
	sendByte(FIRMATA_START_SYSEX);
	sendByte(FIRMATA_SYSEX_REPORT_FIRMWARE);
	sendByte(FIRMATA_END_SYSEX);
}

void ofStandardFirmata::sendReset(){
	sendByte(FIRMATA_SYSTEM_RESET);
}

void ofStandardFirmata::sendAnalogPinReporting(int pin, int mode){
	int i;
	//bool send;

    // disable reporting for all pins on port 2
	for(i=16; i<22; ++i) {
		if(_digitalPinReporting[i]==ARD_ON)
			sendDigitalPinReporting(i, ARD_OFF);
	}

	_digitalPinMode[16+pin]=ARD_ANALOG;

	sendByte(FIRMATA_REPORT_ANALOG+pin);
	sendByte(mode);
	_analogPinReporting[pin] = mode;
}

void ofStandardFirmata::sendDigitalPinMode(int pin, int mode){
	sendByte(FIRMATA_SET_PIN_MODE);
	sendByte(pin); // Tx pins 0-6
	sendByte(mode);
	_digitalPinMode[pin]=mode;

	// turn on or off reporting on the port
	if(mode==ARD_INPUT){
		sendDigitalPinReporting(pin, ARD_ON);
	}
	else {
		sendDigitalPinReporting(pin, ARD_OFF);
	}
}

int ofStandardFirmata::getAnalogPinReporting(int pin){
	return _analogPinReporting[pin];
}

list<int>* ofStandardFirmata::getAnalogHistory(int pin){
	return &_analogHistory[pin];
}

list<int>* ofStandardFirmata::getDigitalHistory(int pin){
	return &_digitalHistory[pin];
}

list<vector<unsigned char> >* ofStandardFirmata::getSysExHistory(){
	return &_sysExHistory;
}

list<string>* ofStandardFirmata::getStringHistory(){
	return &_stringHistory;
}

int ofStandardFirmata::getMajorProtocolVersion(){
	return _majorFirmwareVersion;
}

int ofStandardFirmata::getMinorProtocolVersion(){
	return _minorFirmwareVersion;
}

int ofStandardFirmata::getMajorFirmwareVersion(){
	return _majorFirmwareVersion;
}

int ofStandardFirmata::getMinorFirmwareVersion(){
	return _minorFirmwareVersion;
}

string ofStandardFirmata::getFirmwareName(){
	return _firmwareName;
}

bool ofStandardFirmata::isInitialized(){
	return _initialized;
}

// ------------------------------ private functions

void ofStandardFirmata::processData(unsigned char inputData){

	char msg[100];
	sprintf(msg, "Received Byte: %i", inputData);
	//Logger::get("Application").information(msg);

	// we have command data
	if(_waitForData>0 && inputData<128) {
		_waitForData--;

		// collect the data
		_storedInputData[_waitForData] = inputData;

		// we have all data executeMultiByteCommand
		if(_waitForData==0) {
			switch (_executeMultiByteCommand) {
				case FIRMATA_DIGITAL_MESSAGE:
					processDigitalPort(_multiByteChannel, (_storedInputData[0] << 7) | _storedInputData[1]);
				break;
				case FIRMATA_REPORT_VERSION: // report version
					_majorProtocolVersion = _storedInputData[1];
					_minorProtocolVersion = _storedInputData[0];
					ofNotifyEvent(EProtocolVersionReceived, _majorProtocolVersion, this);
				break;
				case FIRMATA_ANALOG_MESSAGE:
					int previous = _analogHistory[_multiByteChannel].front();

					_analogHistory[_multiByteChannel].push_front((_storedInputData[0] << 7) | _storedInputData[1]);
					if((int)_analogHistory[_multiByteChannel].size()>_analogHistoryLength)
						_analogHistory[_multiByteChannel].pop_back();

					// trigger an event if the pin has changed value
					if(_analogHistory[_multiByteChannel].front()!=previous)
						ofNotifyEvent(EAnalogPinChanged, _multiByteChannel, this);
				break;
			}

		}
	}
	// we have SysEx command data
	else if(_waitForData<0){
		// we have all sysex data
		if(inputData==FIRMATA_END_SYSEX){
			_waitForData=0;
			processSysExData(_sysExData);
			_sysExData.clear();
		}
		// still have data, collect it
		else {
			_sysExData.push_back((unsigned char)inputData);
		}
	}
	// we have a command
	else{

		int command;

		// extract the command and channel info from a byte if it is less than 0xF0
		if(inputData < 0xF0) {
		  command = inputData & 0xF0;
		  _multiByteChannel = inputData & 0x0F;
		}
		else {
		  // commands in the 0xF* range don't use channel data
		  command = inputData;
		}

		switch (command) {
			case FIRMATA_REPORT_VERSION:
			case FIRMATA_DIGITAL_MESSAGE:
			case FIRMATA_ANALOG_MESSAGE:
				_waitForData = 2;  // 2 bytes needed
				_executeMultiByteCommand = command;
			break;
			case FIRMATA_START_SYSEX:
				_sysExData.clear();
				_waitForData = -1;  // n bytes needed, -1 is used to indicate sysex message
				_executeMultiByteCommand = command;
			break;
		}

	}
}

// sysex data is assumed to be 8-bit bytes split into two 7-bit bytes.
void ofStandardFirmata::processSysExData(vector<unsigned char> data){

	string str;

	vector<unsigned char>::iterator it;
	unsigned char buffer;
	//int i = 1;

	// act on reserved sysEx messages (extended commands) or trigger SysEx event...
	switch(data.front()) { //first byte in buffer is command
		case FIRMATA_SYSEX_REPORT_FIRMWARE:
			it = data.begin();
			it++; // skip the first byte, which is the firmware version command
			_majorFirmwareVersion = *it;
			it++;
			_minorFirmwareVersion = *it;
			it++;

			while( it != data.end() ) {
					buffer = *it;
					it++;
					buffer += *it << 7;
					it++;
					str+=buffer;
			}
			_firmwareName = str;

			ofNotifyEvent(EFirmwareVersionReceived, _majorFirmwareVersion, this);

			// trigger the initialization event
			ofNotifyEvent(EInitialized, _majorFirmwareVersion, this);
			_initialized = true;

		break;
		case FIRMATA_SYSEX_FIRMATA_STRING:
			it = data.begin();
			it++; // skip the first byte, which is the string command
			while( it != data.end() ) {
					buffer = *it;
					it++;
					buffer += *it << 7;
					it++;
					str+=buffer;
			}

			_stringHistory.push_front(str);
			if((int)_stringHistory.size()>_stringHistoryLength)
					_stringHistory.pop_back();

			ofNotifyEvent(EStringReceived, str, this);
		break;
		default: // the message isn't in Firmatas extended command set
			_sysExHistory.push_front(data);
			if((int)_sysExHistory.size()>_sysExHistoryLength)
					_sysExHistory.pop_back();
			ofNotifyEvent(ESysExReceived, data, this);
		break;

	}
}

void ofStandardFirmata::processDigitalPort(int port, unsigned char value){

	unsigned char mask;
	int previous;
	int i;
	int pin;
	switch(port) {
		case 0: // pins 2-7  (0,1 are ignored as serial RX/TX)
			for(i=2; i<8; ++i) {
				pin = i;
				if(_digitalPinMode[pin]==ARD_INPUT){
					previous = _digitalHistory[pin].front();

					mask = 1 << i;
					_digitalHistory[pin].push_front((value & mask)>>i);

					if((int)_digitalHistory[pin].size()>_digitalHistoryLength)
							_digitalHistory[pin].pop_back();

					// trigger an event if the pin has changed value
					if(_digitalHistory[pin].front()!=previous){
						ofNotifyEvent(EDigitalPinChanged, pin, this);
					}
				}
			}
	break;
		case 1: // pins 8-13 (14,15 are disabled for the crystal)
		 for(i=0; i<6; ++i) {
			pin = i+8;

			if(_digitalPinMode[pin]==ARD_INPUT){
				previous = _digitalHistory[pin].front();

				mask = 1 << i;
				_digitalHistory[pin].push_front((value & mask)>>i);

				if((int)_digitalHistory[pin].size()>_digitalHistoryLength)
					_digitalHistory[pin].pop_back();

				// trigger an event if the pin has changed value
				if(_digitalHistory[pin].front()!=previous){
					ofNotifyEvent(EDigitalPinChanged, pin, this);
				}
			}
		 }
	break;
	case 2: // analog pins used as digital pins 16-21
		for(i=0; i<6; ++i) {
			pin = i+16;
			if(_digitalPinMode[pin]==ARD_INPUT){
				previous = _digitalHistory[pin].front();

				mask = 1 << i;
				_digitalHistory[pin].push_front((value & mask)>>i);

				if((int)_digitalHistory[pin].size()>_digitalHistoryLength)
					_digitalHistory[pin].pop_back();

				// trigger an event if the pin has changed value
				if(_digitalHistory[pin].front()!=previous){
					ofNotifyEvent(EDigitalPinChanged, pin, this);
				}
			}
		}
	break;
	}
}

// port 0: pins 2-7  (0,1 are serial RX/TX, don't change their values)
// port 1: pins 8-13 (14,15 are disabled for the crystal)
// port 2: pins 16-21 analog pins used as digital, all analog reporting will be turned off if this is set to ARD_ON
void ofStandardFirmata::sendDigitalPortReporting(int port, int mode){
	sendByte(FIRMATA_REPORT_DIGITAL+port);
	sendByte(mode);
	_digitalPortReporting[port] = mode;
	if(port==2 && mode==ARD_ON){ // if reporting is turned on on port 2 then ofStandardFirmata on the Arduino disables all analog reporting

		for (int i=0; i<ARD_TOTAL_ANALOG_PINS; i++) {
				_analogPinReporting[i] = ARD_OFF;
		}
	}
}

void ofStandardFirmata::sendDigitalPinReporting(int pin, int mode){
	_digitalPinReporting[pin] = mode;
	if(mode==ARD_ON){	// enable reporting for the port
		if(pin<=7 && pin>=2)
			sendDigitalPortReporting(0, ARD_ON);
		if(pin<=13 && pin>=8)
			sendDigitalPortReporting(1, ARD_ON);
		if(pin<=21 && pin>=16)
			sendDigitalPortReporting(2, ARD_ON);
	}
	else if(mode==ARD_OFF){
		int i;
		bool send=true;
		if(pin<8 && pin>=2){    // check if all pins on the port are off, if so set port reporting to off..
			for(i=2; i<8; ++i) {
				if(_digitalPinReporting[i]==ARD_ON)
						send=false;
			}
			if(send)
				sendDigitalPortReporting(0, ARD_OFF);
		}
		if(pin<14 && pin>=8){
			for(i=8; i<14; ++i) {
				if(_digitalPinReporting[i]==ARD_ON)
						send=false;
			}
			if(send)
				sendDigitalPortReporting(1, ARD_OFF);
		}
		if(pin<22 && pin>=16){
			for(i=16; i<22; ++i) {
				if(_digitalPinReporting[i]==ARD_ON)
						send=false;
			}
			if(send)
				sendDigitalPortReporting(2, ARD_OFF);
		}

	}
}

void ofStandardFirmata::sendByte(unsigned char byte){
	//char msg[100];
	//sprintf(msg, "Sending Byte: %i", byte);
	//Logger::get("Application").information(msg);
	_port.writeByte(byte);
}

// in Firmata (and MIDI) data bytes are 7-bits. The 8th bit serves as a flag to mark a byte as either command or data.
// therefore you need two data bytes to send 8-bits (a char).
void ofStandardFirmata::sendValueAsTwo7bitBytes(int value)
{
	sendByte(value & 127); // LSB
	sendByte(value >> 7 & 127); // MSB
}

// SysEx data is sent as 8-bit bytes split into two 7-bit bytes, this function merges two 7-bit bytes back into one 8-bit byte.
int ofStandardFirmata::getValueFromTwo7bitBytes(unsigned char lsb, unsigned char msb){
   return (msb << 7) | lsb;
}
