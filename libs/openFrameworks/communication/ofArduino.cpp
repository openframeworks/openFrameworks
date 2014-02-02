/*
 * 1/9/13:
 *   - Fixed issue where digitalPinchange
 *
 * 9/28/11:
 *   - updated to be Firmata 2.3/Arduino 1.0 compatible
 *   - fixed ability to use analog pins as digital inputs
 *
 * 3/5/11:
 *   - added servo support for firmata 2.2 and greater (should be 
 *     backwards compatible with Erik Sjodin's older firmata servo
 *     implementation)
 * 
 *
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

// TODO thread it?
// TODO throw event or exception if the serial port goes down...
//---------------------------------------------------------------------------
ofArduino::ofArduino(){
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

	bUseDelay = true;
}

ofArduino::~ofArduino() {
	_port.close();
}

// initialize pins once we get the Firmata version back from the Arduino board
// the version is sent automatically by the Arduino board on startup
void ofArduino::initPins() {
    int firstAnalogPin;
    
    if (_initialized) return;   // already initialized
    
    // support Firmata 2.3/Arduino 1.0 with backwards compatibility 
    // to previous protocol versions
    if (_firmwareVersionSum >= FIRMWARE2_3) {
        _totalDigitalPins = 20;
        firstAnalogPin = 14;
    } else {
        _totalDigitalPins = ARD_TOTAL_DIGITAL_PINS;
        firstAnalogPin = 16;
    }
    
    // ports
	for(int i=0; i<ARD_TOTAL_PORTS; ++i) {
		_digitalPortValue[i]=0;
		_digitalPortReporting[i] = ARD_OFF;
	}
    
    // digital pins
	for(int i=0; i<firstAnalogPin; ++i) {
		_digitalPinValue[i] = -1;
		_digitalPinMode[i] = ARD_OUTPUT;
		_digitalPinReporting[i] = ARD_OFF;
	}
    
	// analog in pins
    for (int i=firstAnalogPin; i<_totalDigitalPins; ++i) {
		_analogPinReporting[i-firstAnalogPin] = ARD_OFF;
		// analog pins used as digital
		_digitalPinMode[i]=ARD_ANALOG;
		_digitalPinValue[i] = -1;
	}

	for (int i=0; i<_totalDigitalPins; ++i) {
		_servoValue[i] = -1;
	}
    
    _initialized = true;
}

bool ofArduino::connect(string device, int baud){
	connectTime = ofGetElapsedTimef();
	_initialized = false;
	_port.enumerateDevices();
	connected = _port.setup(device.c_str(), baud);
	return connected;
}

// this method is not recommended
// the preferred method is to listen for the EInitialized event in your application
bool ofArduino::isArduinoReady(){	
	if(bUseDelay) {
		if (_initialized || (ofGetElapsedTimef() - connectTime) > OF_ARDUINO_DELAY_LENGTH) {
			initPins();
			connected = true;
		}
	}
	return connected;
}

void  ofArduino::setUseDelay(bool bDelay){
	bUseDelay = bDelay;
}

void ofArduino::setDigitalHistoryLength(int length){
	if(length>=2)
		_digitalHistoryLength=length;
}

void ofArduino::setAnalogHistoryLength(int length){
	if(length>=2)
		_analogHistoryLength=length;
}

void ofArduino::setSysExHistoryLength(int length){
	if(length>=1)
		_sysExHistoryLength=length;
}

void ofArduino::setStringHistoryLength(int length){
	if(length>=1)
		_stringHistoryLength=length;
}

void ofArduino::disconnect(){
	_port.close();
}

void ofArduino::update(){
	static vector<unsigned char> bytesToProcess;
	int bytesToRead = _port.available();
	if (bytesToRead>0) {
		bytesToProcess.resize(bytesToRead);
		_port.readBytes(&bytesToProcess[0], bytesToRead);
		for (int i = 0; i < bytesToRead; i++) {
			processData((char)(bytesToProcess[i]));
		}
	}
}

int ofArduino::getAnalog(int pin){
	if(_analogHistory[pin].size()>0)
		return _analogHistory[pin].front();
	else
		return -1;
}

int ofArduino::getDigital(int pin){
	if(_digitalPinMode[pin]==ARD_INPUT && _digitalHistory[pin].size()>0)
		return _digitalHistory[pin].front();
	else if (_digitalPinMode[pin]==ARD_OUTPUT)
		return _digitalPinValue[pin];
	else
		return -1;
}

int ofArduino::getPwm(int pin){
	if(_digitalPinMode[pin]==ARD_PWM)
		return _digitalPinValue[pin];
	else
		return -1;
}

vector<unsigned char> ofArduino::getSysEx(){
	return _sysExHistory.front();
}

string ofArduino::getString(){
	return _stringHistory.front();
}

int ofArduino::getDigitalPinMode(int pin){
	return _digitalPinMode[pin];
}

void ofArduino::sendDigital(int pin, int value, bool force){
	if((_digitalPinMode[pin]==ARD_INPUT || _digitalPinMode[pin]==ARD_OUTPUT) && (_digitalPinValue[pin]!=value || force)){

		_digitalPinValue[pin] = value;

		int port=0;
		int bit=0;
        int port1Offset;
        int port2Offset;
        
        // support Firmata 2.3/Arduino 1.0 with backwards compatibility 
        // to previous protocol versions
        if (_firmwareVersionSum >= FIRMWARE2_3) {
            port1Offset = 16;
            port2Offset = 20;
        } else {
            port1Offset = 14;
            port2Offset = 22;
        }

		if(pin < 8 && pin >1){
			port=0;
			bit = pin;
		}
		else if(pin>7 && pin <port1Offset){
			port = 1;
			bit = pin-8;
		}
		else if(pin>15 && pin <port2Offset){
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

void ofArduino::sendPwm(int pin, int value, bool force){
	if(_digitalPinMode[pin]==ARD_PWM && (_digitalPinValue[pin]!=value || force)){
		sendByte(FIRMATA_ANALOG_MESSAGE+pin);
		sendValueAsTwo7bitBytes(value);
		_digitalPinValue[pin] = value;
	}
}

void ofArduino::sendSysEx(int command, vector<unsigned char> data){
	sendByte(FIRMATA_START_SYSEX);
	sendByte(command);
	vector<unsigned char>::iterator it = data.begin();
	while( it != data.end() ) {
		//sendByte(*it);	// need to split data into 2 bytes before sending
		sendValueAsTwo7bitBytes(*it);
		it++;
	}
	sendByte(FIRMATA_END_SYSEX);
}

void ofArduino::sendSysExBegin(){
	sendByte(FIRMATA_START_SYSEX);
}

void ofArduino::sendSysExEnd(){
	sendByte(FIRMATA_END_SYSEX);
}

void ofArduino::sendString(string str){
	sendByte(FIRMATA_START_SYSEX);
	sendByte(FIRMATA_SYSEX_FIRMATA_STRING);
	string::iterator it = str.begin();
	while( it != str.end() ) {
		sendValueAsTwo7bitBytes(*it);
		it++;
	}
	sendByte(FIRMATA_END_SYSEX);
}

void ofArduino::sendProtocolVersionRequest(){
	sendByte(FIRMATA_REPORT_VERSION);
}

void ofArduino::sendFirmwareVersionRequest(){
	sendByte(FIRMATA_START_SYSEX);
	sendByte(FIRMATA_SYSEX_REPORT_FIRMWARE);
	sendByte(FIRMATA_END_SYSEX);
}

void ofArduino::sendReset(){
	sendByte(FIRMATA_SYSTEM_RESET);
}

void ofArduino::sendAnalogPinReporting(int pin, int mode){

    int firstAnalogPin;
    // support Firmata 2.3/Arduino 1.0 with backwards compatibility 
    // to previous protocol versions
    if (_firmwareVersionSum >= FIRMWARE2_3) {
        firstAnalogPin = 14;
    } else {
        firstAnalogPin = 16;
    }
    
    // if this analog pin is set as a digital input, disable digital pin reporting
    if (_digitalPinReporting[pin + firstAnalogPin] == ARD_ON) {
        sendDigitalPinReporting(pin + firstAnalogPin, ARD_OFF);
    }

	_digitalPinMode[firstAnalogPin+pin]=ARD_ANALOG;

	sendByte(FIRMATA_REPORT_ANALOG+pin);
	sendByte(mode);
	_analogPinReporting[pin] = mode;
}

void ofArduino::sendDigitalPinMode(int pin, int mode){
	sendByte(FIRMATA_SET_PIN_MODE);
	sendByte(pin);
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

int ofArduino::getAnalogPinReporting(int pin){
	return _analogPinReporting[pin];
}

list<int>* ofArduino::getAnalogHistory(int pin){
	return &_analogHistory[pin];
}

list<int>* ofArduino::getDigitalHistory(int pin){
	return &_digitalHistory[pin];
}

list<vector<unsigned char> >* ofArduino::getSysExHistory(){
	return &_sysExHistory;
}

list<string>* ofArduino::getStringHistory(){
	return &_stringHistory;
}

int ofArduino::getMajorProtocolVersion(){
	return _majorFirmwareVersion;
}

int ofArduino::getMinorProtocolVersion(){
	return _minorFirmwareVersion;
}

int ofArduino::getMajorFirmwareVersion(){
	return _majorFirmwareVersion;
}

int ofArduino::getMinorFirmwareVersion(){
	return _minorFirmwareVersion;
}

string ofArduino::getFirmwareName(){
	return _firmwareName;
}

bool ofArduino::isInitialized(){
	return _initialized;
}

// ------------------------------ private functions

void ofArduino::processData(unsigned char inputData){

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
					if(_analogHistory[_multiByteChannel].size()>0){
						int previous = _analogHistory[_multiByteChannel].front();

						_analogHistory[_multiByteChannel].push_front((_storedInputData[0] << 7) | _storedInputData[1]);
						if((int)_analogHistory[_multiByteChannel].size()>_analogHistoryLength)
							_analogHistory[_multiByteChannel].pop_back();

						// trigger an event if the pin has changed value
						if(_analogHistory[_multiByteChannel].front()!=previous)
							ofNotifyEvent(EAnalogPinChanged, _multiByteChannel, this);
					}else{
						_analogHistory[_multiByteChannel].push_front((_storedInputData[0] << 7) | _storedInputData[1]);
						if((int)_analogHistory[_multiByteChannel].size()>_analogHistoryLength)
							_analogHistory[_multiByteChannel].pop_back();
					}
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
void ofArduino::processSysExData(vector<unsigned char> data){

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

			_firmwareVersionSum = _majorFirmwareVersion * 10 + _minorFirmwareVersion;
			ofNotifyEvent(EFirmwareVersionReceived, _majorFirmwareVersion, this);

			// trigger the initialization event
            if (!_initialized) {
                initPins();
                ofNotifyEvent(EInitialized, _majorFirmwareVersion, this);
                
            }

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

void ofArduino::processDigitalPort(int port, unsigned char value){

	unsigned char mask;
	int previous;
	int i;
	int pin;
    int port1Pins;
    int port2Pins;
    
    // support Firmata 2.3/Arduino 1.0 with backwards compatibility to previous protocol versions
    if (_firmwareVersionSum >= FIRMWARE2_3) {
        port1Pins = 8;
        port2Pins = 4;
    } else {
        port1Pins = 6;
        port2Pins = 6;
    }
    
	switch(port) {
    case 0: // pins 2-7  (0,1 are ignored as serial RX/TX)
        for(i=2; i<8; ++i) {
            pin = i;
            previous = -1;
            if(_digitalPinMode[pin]==ARD_INPUT){
              if (_digitalHistory[pin].size() > 0)
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
    case 1: // pins 8-13 (in Firmata 2.3/Arduino 1.0, pins 14 and 15 are analog 0 and 1)
        for(i=0; i<port1Pins; ++i) {
            pin = i+8;
            previous = -1;
            if(_digitalPinMode[pin]==ARD_INPUT){
              if (_digitalHistory[pin].size() > 0)
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
	case 2: // analog pins used as digital pins 16-21 (in Firmata 2.3/Arduino 1.0, digital pins 14 - 19)
		for(i=0; i<port2Pins; ++i) {
			//pin = i+analogOffset;
            pin = i+16;
			      previous = -1;
            if(_digitalPinMode[pin]==ARD_INPUT){
              if (_digitalHistory[pin].size() > 0)
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
// port 1: pins 8-13 (in Firmata 2.3/Arduino 1.0, pins 14 and 15 are analog pins 0 and 1 used as digital pins)
// port 2: pins 16-21 analog pins used as digital (in Firmata 2.3/Arduino 1.0, pins 14 - 19),
//         all analog reporting will be turned off if this is set to ARD_ON

void ofArduino::sendDigitalPortReporting(int port, int mode){
	sendByte(FIRMATA_REPORT_DIGITAL+port);
	sendByte(mode);
	_digitalPortReporting[port] = mode;
    int offset;
    
    if (_firmwareVersionSum >= FIRMWARE2_3) {
        offset = 2;
    } else {
        offset = 0;
    }
    
    // for Firmata 2.3 and higher:
    if(port==1 && mode==ARD_ON) {
        for (int i=0; i<2; i++) {
            _analogPinReporting[i] = ARD_OFF;
		} 
    }
    
    // for Firmata 2.3 and all prior Firmata protocol versions:
	if(port==2 && mode==ARD_ON){ // if reporting is turned on on port 2 then ofArduino on the Arduino disables all analog reporting

		for (int i=offset; i<ARD_TOTAL_ANALOG_PINS; i++) {
				_analogPinReporting[i] = ARD_OFF;
		}
	}
}

void ofArduino::sendDigitalPinReporting(int pin, int mode){
	_digitalPinReporting[pin] = mode;
    int port1Offset;
    int port2Offset;
    
    // Firmata backwards compatibility mess
    if (_firmwareVersionSum >= FIRMWARE2_3) {
        port1Offset = 15;
        port2Offset = 19;
    } else {
        port1Offset = 13;
        port2Offset = 21;
    }
    
	if(mode==ARD_ON){	// enable reporting for the port
		if(pin<=7 && pin>=2)
			sendDigitalPortReporting(0, ARD_ON);
        // Firmata backwards compatibility mess
        if(pin<=port1Offset && pin>=8)
            sendDigitalPortReporting(1, ARD_ON);
        if(pin<=port2Offset && pin>=16)
            sendDigitalPortReporting(2, ARD_ON);          
	}
	else if(mode==ARD_OFF){
		int i;
		bool send=true;
		if(pin<=7 && pin>=2){    // check if all pins on the port are off, if so set port reporting to off..
			for(i=2; i<8; ++i) {
				if(_digitalPinReporting[i]==ARD_ON)
						send=false;
			}
			if(send)
				sendDigitalPortReporting(0, ARD_OFF);
		}
        // Firmata backwards compatibility mess
        if(pin<=port1Offset && pin>=8){
            for(i=8; i<=port1Offset; ++i) {
                if(_digitalPinReporting[i]==ARD_ON)
                        send=false;
            }
            if(send)
                sendDigitalPortReporting(1, ARD_OFF);
        }
        if(pin<=port2Offset && pin>=16){
            for(i=16; i<=port2Offset; ++i) {
                if(_digitalPinReporting[i]==ARD_ON)
                        send=false;
            }
            if(send)
                sendDigitalPortReporting(2, ARD_OFF);
        }
	}
}

void ofArduino::sendByte(unsigned char byte){
	//char msg[100];
	//sprintf(msg, "Sending Byte: %i", byte);
	//Logger::get("Application").information(msg);
	_port.writeByte(byte);
}

// in Firmata (and MIDI) data bytes are 7-bits. The 8th bit serves as a flag to mark a byte as either command or data.
// therefore you need two data bytes to send 8-bits (a char).
void ofArduino::sendValueAsTwo7bitBytes(int value)
{
	sendByte(value & 127); // LSB
	sendByte(value >> 7 & 127); // MSB
}

// SysEx data is sent as 8-bit bytes split into two 7-bit bytes, this function merges two 7-bit bytes back into one 8-bit byte.
int ofArduino::getValueFromTwo7bitBytes(unsigned char lsb, unsigned char msb){
   return (msb << 7) | lsb;
}

void ofArduino::sendServo(int pin, int value, bool force){
	// for firmata v2.2 and greater
	if (_firmwareVersionSum >= FIRMWARE2_2) {
		if(_digitalPinMode[pin]==ARD_SERVO && (_digitalPinValue[pin]!=value || force)){
			sendByte(FIRMATA_ANALOG_MESSAGE+pin);
			sendValueAsTwo7bitBytes(value);
			_digitalPinValue[pin] = value;
		}
	} 
	// for versions prior to 2.2
	else {
		if(_digitalPinMode[pin]==ARD_SERVO && (_servoValue[pin]!=value || force)){
			sendByte(FIRMATA_START_SYSEX);
			sendByte(SYSEX_SERVO_WRITE);
			sendByte(pin);
			sendValueAsTwo7bitBytes(value);
			sendByte(FIRMATA_END_SYSEX);
			_servoValue[pin]=value;
		}		
	}
}

// angle parameter is no longer supported. keeping for backwards compatibility
void ofArduino::sendServoAttach(int pin, int minPulse, int maxPulse, int angle) {
	sendByte(FIRMATA_START_SYSEX);
	// for firmata v2.2 and greater
	if (_firmwareVersionSum >= FIRMWARE2_2) {
		sendByte(FIRMATA_SYSEX_SERVO_CONFIG);
	} 
	// for versions prior to 2.2
	else {
		sendByte(SYSEX_SERVO_ATTACH);
	}
	sendByte(pin);
	sendValueAsTwo7bitBytes(minPulse);
	sendValueAsTwo7bitBytes(maxPulse);
	sendByte(FIRMATA_END_SYSEX);
	_digitalPinMode[pin]=ARD_SERVO;
}

// sendServoDetach depricated as of Firmata 2.2
void ofArduino::sendServoDetach(int pin) {
	sendByte(FIRMATA_START_SYSEX);
	sendByte(SYSEX_SERVO_DETACH);
	sendByte(pin);
	sendByte(FIRMATA_END_SYSEX);
	_digitalPinMode[pin]=ARD_OUTPUT;
}

int ofArduino::getServo(int pin){
	if(_digitalPinMode[pin]==ARD_SERVO)
		// for firmata v2.2 and greater
		if (_firmwareVersionSum >= FIRMWARE2_2) {
			return _digitalPinValue[pin];
		} 
		// for versions prior to 2.2
		else {
			return _servoValue[pin];
		}		
	else
		return -1;
}
