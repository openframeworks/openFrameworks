/*
 * 11/5/15:
 *   - updated for arduino 2.5.0 and configurable firmata
 *
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
 * Wiring version 2011 (c) Carlos Mario Rodriguez and Hernando Barragan
 * Updates 2015 (c) Dom Amato
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
#include "ofMath.h"
#include "ofLog.h"

using std::vector;
using std::string;
using std::list;
using std::pair;


 // TODO thread it?
 // TODO throw event or exception if the serial port goes down...
 //---------------------------------------------------------------------------
ofArduino::ofArduino() {
	_portStatus = -1;
	_waitForData = 0;
	_analogHistoryLength = 2;
	_digitalHistoryLength = 2;
	_stringHistoryLength = 1;
	_sysExHistoryLength = 1;
	_initialized = false;
	_totalDigitalPins = 0;
	_executeMultiByteCommand = 0x00; // 0x00 a pin mode (input), not a command in Firmata -> fail hard
	_multiByteChannel = 0;
	_firstAnalogPin = -1;

	for (unsigned char & e : _storedInputData) {
		e = UCHAR_MAX;
	}
	for (int & e : _digitalPinMode) {
		e = INT_MAX;
	}
	for (int & e : _digitalPinValue) {
		e = INT_MAX;
	}
	for (int & e : _digitalPortValue) {
		e = INT_MAX;
	}
	for (int & e : _digitalPortReporting) {
		e = INT_MAX;
	}
	for (int & e : _digitalPinReporting) {
		e = INT_MAX;
	}
	for (int & e : _analogPinReporting) {
		e = INT_MAX;
	}
	for (int & e : _servoValue) {
		e = INT_MAX;
	}
	connected = false;
	connectTime = 0.0f;

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

	if (_initialized) {
		return;                 // already initialized
	}

	_digitalHistory.resize(_totalDigitalPins + 1);
	_digitalPinMode.resize(_totalDigitalPins + 1);
	_digitalPinValue.resize(_totalDigitalPins + 1);
	_digitalPinReporting.resize(_totalDigitalPins + 1);
	_servoValue.resize(_totalDigitalPins + 1);

	_analogHistory.resize(_totalDigitalPins - _firstAnalogPin + 1);
	_analogPinReporting.resize(_totalDigitalPins - _firstAnalogPin + 1);

	// ports
	for (int i = 0; i < ARD_TOTAL_PORTS; ++i) {
		_digitalPortValue[i] = 0;
		_digitalPortReporting[i] = ARD_OFF;
	}

	// digital pins
	for (int i = 0; i < _firstAnalogPin; ++i) {
		_digitalPinValue[i] = -1;
		_digitalPinMode[i] = ARD_OUTPUT;
		_digitalPinReporting[i] = ARD_OFF;
	}

	// analog in pins
	for (int i = _firstAnalogPin; i < _totalDigitalPins; ++i) {
		_analogPinReporting[i - _firstAnalogPin] = ARD_OFF;
		// analog pins used as digital
		_digitalPinMode[i] = ARD_ANALOG;
		_digitalPinValue[i] = -1;
	}

	for (int i = 0; i < _totalDigitalPins; ++i) {
		_servoValue[i] = -1;
	}

	_initialized = true;
	ofNotifyEvent(EInitialized, _majorFirmwareVersion, this);
}

bool ofArduino::connect(const std::string & device, int baud) {
	connectTime = ofGetElapsedTimef();
	_initialized = false;
	connected = _port.setup(device.c_str(), baud);
	sendFirmwareVersionRequest();
	return connected;
}

// this method is not recommended
// the preferred method is to listen for the EInitialized event in your application
bool ofArduino::isArduinoReady() {
	if (bUseDelay) {
		if (!_initialized && (ofGetElapsedTimef() - connectTime) > OF_ARDUINO_DELAY_LENGTH) {
			sendPinCapabilityRequest();
			connected = true;
		}
	}
	return connected;
}

void ofArduino::setUseDelay(bool bDelay) {
	bUseDelay = bDelay;
}

void ofArduino::setDigitalHistoryLength(int length) {
	if (length >= 2) {
		_digitalHistoryLength = length;
	}
}

void ofArduino::setAnalogHistoryLength(int length) {
	if (length >= 2) {
		_analogHistoryLength = length;
	}
}

void ofArduino::setSysExHistoryLength(int length) {
	if (length >= 1) {
		_sysExHistoryLength = length;
	}
}

void ofArduino::setStringHistoryLength(int length) {
	if (length >= 1) {
		_stringHistoryLength = length;
	}
}

void ofArduino::disconnect() {
	_port.close();
}

void ofArduino::update() {
	vector <unsigned char> bytesToProcess;
	int bytesToRead = _port.available();
	if (bytesToRead > 0) {
		bytesToProcess.resize(bytesToRead);
		//its possible we dont get all the bytes
		int bytesRead = _port.readBytes(&bytesToProcess[0], bytesToRead);
		for (int i = 0; i < bytesRead; i++) {
			processData((char)(bytesToProcess[i]));
		}
	}
}

int ofArduino::getAnalog(int pin) const {
	if (!isAnalogPin(pin)) {
		return -1;
	}
	if (_digitalPinMode[convertAnalogPinToDigital(pin)] != ARD_ANALOG) {
		ofLogError("ofArduino") << "Analog Input has not been configured for pin " << pin;
		return -1;
	}
	pin = convertDigitalPinToAnalog(pin);
	if (_analogHistory[pin].size() > 0) {
		return _analogHistory[pin].front();
	}
	else {
		return -1;
	}
}

int ofArduino::getDigital(int pin) const {
	if (!isPin(pin)) {
		return -1;
	}
	if ((_digitalPinMode[pin] == ARD_INPUT || _digitalPinMode[pin] == ARD_INPUT_PULLUP) && _digitalHistory[pin].size() > 0) {
		return _digitalHistory[pin].front();
	}
	else if (_digitalPinMode[pin] == ARD_OUTPUT) {
		return _digitalPinValue[pin];
	}
	else {
		return -1;
	}
}

int ofArduino::getPwm(int pin) const {
	if (!isPin(pin)) {
		return -1;
	}
	if (!pinCapabilities[pin].pwmSupported) {
		ofLogError("ofArduino") << "PWM is not supported for pin " << pin;
		return -1;
	}
	if (_digitalPinMode[pin] == ARD_PWM) {
		return _digitalPinValue[pin];
	}
	else {
		return -1;
	}
}

vector <unsigned char> ofArduino::getSysEx() const {
	return _sysExHistory.front();
}

string ofArduino::getString() const {
	return _stringHistory.front();
}

int ofArduino::getDigitalPinMode(int pin) const {
	if (!isPin(pin)) {
		return -1;
	}
	return _digitalPinMode[pin];
}

void ofArduino::sendDigital(int pin, int value, bool force) {
	if (!isPin(pin)) {
		return;
	}
	if ((_digitalPinMode[pin] == ARD_INPUT || _digitalPinMode[pin] == ARD_INPUT_PULLUP || _digitalPinMode[pin] == ARD_OUTPUT) && (_digitalPinValue[pin] != value || force)) {

		_digitalPinValue[pin] = value;

		// set the bit
		int port = (pin >> 3) & 0x0F;

		if (value == 1)
			_digitalPortValue[port] |= (1 << (pin & 0x07));

		// clear the bit
		if (value == 0)
			_digitalPortValue[port] &= ~(1 << (pin & 0x07));

		sendByte(DIGITAL_MESSAGE | port);
		sendValueAsTwo7bitBytes(_digitalPortValue[port]);

	}
}

void ofArduino::sendPwm(int pin, int value, bool force) {
	if (!isPin(pin)) {
		return;
	}
	if (!pinCapabilities[pin].pwmSupported) {
		ofLogError("ofArduino") << "PWM is not supported for pin " << pin;
		return;
	}
	if (_digitalPinMode[pin] == ARD_PWM && (_digitalPinValue[pin] != value || force)) {
		sendByte(ANALOG_MESSAGE | pin);
		sendValueAsTwo7bitBytes(value);
		_digitalPinValue[pin] = value;
	}
}

void ofArduino::sendSysEx(int command, vector <unsigned char> data) {
	sendByte(START_SYSEX);
	sendByte(command);
	vector <unsigned char>::iterator it = data.begin();
	while (it != data.end()) {
		sendValueAsTwo7bitBytes(*it);
		it++;
	}
	sendByte(END_SYSEX);
}

void ofArduino::sendSysExBegin() {
	sendByte(START_SYSEX);
}

void ofArduino::sendSysExEnd() {
	sendByte(END_SYSEX);
}

void ofArduino::sendString(string str) {
	sendByte(START_SYSEX);
	sendByte(STRING_DATA);
	string::iterator it = str.begin();
	while (it != str.end()) {
		sendValueAsTwo7bitBytes(*it);
		it++;
	}
	sendByte(END_SYSEX);
}

void ofArduino::sendProtocolVersionRequest() {
	sendByte(REPORT_VERSION);
}

void ofArduino::sendFirmwareVersionRequest() {
	sendByte(START_SYSEX);
	sendByte(REPORT_FIRMWARE);
	sendByte(END_SYSEX);
}

//this currently isn't supported as the resonse is not mapped
void ofArduino::sendPinCofigurationRequest() {
	sendByte(START_SYSEX);
	sendByte(PIN_STATE_QUERY);
	sendByte(END_SYSEX);
}

void ofArduino::sendPinCapabilityRequest() {
	sendByte(START_SYSEX);
	sendByte(CAPABILITY_QUERY);
	sendByte(END_SYSEX);
}

void ofArduino::sendAnalogMappingRequest()
{
	sendByte(START_SYSEX);
	sendByte(ANALOG_MAPPING_QUERY);
	sendByte(END_SYSEX);
}

void ofArduino::sendPinStateQuery(int pin)
{
	sendByte(START_SYSEX);
	sendByte(PIN_STATE_QUERY);
	sendByte(pin & 0x7f);
	sendByte(END_SYSEX);
}

void ofArduino::sendReset() {
	sendByte(SYSTEM_RESET);
}

void ofArduino::sendAnalogPinReporting(int pin, int mode) {
	if (!isAnalogPin(pin)) {
		return;
	}

	//the digital pin mode needs the digital pin #
	_digitalPinMode[convertAnalogPinToDigital(pin)] = ARD_ANALOG;

	//the sent message needs the analog pin #
	sendByte(REPORT_ANALOG | convertDigitalPinToAnalog(pin));
	sendByte(mode);
	_analogPinReporting[convertDigitalPinToAnalog(pin)] = mode;
}

void ofArduino::sendDigitalPinMode(int pin, int mode) {
	if (!isPin(pin)) {
		return;
	}
	switch (mode) {
	case ARD_INPUT:
	case ARD_INPUT_PULLUP:
		if (!pinCapabilities[pin].inputSupported) {
			ofLogError("ofArduino") << "Input is not supported for pin " << pin;
			return;
		}
		break;
	case ARD_OUTPUT:
		if (!pinCapabilities[pin].outputSupported) {
			ofLogError("ofArduino") << "Output is not supported for pin " << pin;
			return;
		}
		break;
	case ARD_ANALOG:
		if (!pinCapabilities[pin].analogSupported) {
			ofLogError("ofArduino") << "Analog is not supported for pin " << pin;
			return;
		}
		break;
	case ARD_PWM:
		if (!pinCapabilities[pin].pwmSupported) {
			ofLogError("ofArduino") << "PWM is not supported for pin " << pin;
			return;
		}
		break;
	case ARD_SERVO:
		if (!pinCapabilities[pin].servoSupported) {
			ofLogError("ofArduino") << "Servo Control is not supported for pin " << pin;
			return;
		}
		break;
	case ARD_I2C:
		if (!pinCapabilities[pin].i2cSupported) {
			ofLogError("ofArduino") << "I2C is not supported for pin " << pin;
			return;
		}
		break;
	case ARD_ONEWIRE:
		if (!pinCapabilities[pin].onewireSupported) {
			ofLogError("ofArduino") << "Onewire is not supported for pin " << pin;
			return;
		}
		break;
	case ARD_STEPPER:
		if (!pinCapabilities[pin].stepperSupported) {
			ofLogError("ofArduino") << "Stepper Control is not supported for pin " << pin;
			return;
		}
		break;
	case ARD_ENCODER:
		if (!pinCapabilities[pin].encoderSupported) {
			ofLogError("ofArduino") << "Encoder Control is not supported for pin " << pin;
			return;
		}
		break;
	case ARD_SERIAL:
		if (!pinCapabilities[pin].serialSupported) {
			ofLogError("ofArduino") << "Serial is not supported for pin" + ofToString(pin);
			return;
		}
		break;
	default:
		ofLogError("ofArduino") << "Mode is not supported";
		return;
		break;
	}
	sendByte(SET_PIN_MODE);
	sendByte(pin); // Tx pins 0-6
	sendByte(mode);
	_digitalPinMode[pin] = mode;

	// turn on or off reporting on the port

	if (mode == ARD_INPUT || mode == ARD_INPUT_PULLUP) {
		sendDigitalPinReporting(pin, ARD_ON);
	}
	else {
		sendDigitalPinReporting(pin, ARD_OFF);
	}

}

int ofArduino::getAnalogPinReporting(int pin) const {
	if (!isAnalogPin(pin)) {
		return -1;
	}
	if (_digitalPinMode[convertAnalogPinToDigital(pin)] != ARD_ANALOG) {
		ofLogError("ofArduino") << "Analog Input has not been configured for pin " << pin;
		return -1;
	}
	//these are just safety checks
	pin = convertDigitalPinToAnalog(pin);
	return _analogPinReporting[pin];
}

list <int> * ofArduino::getAnalogHistory(int pin) {
	if (!isAnalogPin(pin)) {
		return NULL;
	}
	if (_digitalPinMode[convertAnalogPinToDigital(pin)] != ARD_ANALOG) {
		ofLogError("ofArduino") << "Analog Input has not been configured for pin " << pin;
		return NULL;
	}
	pin = convertDigitalPinToAnalog(pin);
	return &_analogHistory[pin];
}

list <int> * ofArduino::getDigitalHistory(int pin) {
	if (!isPin(pin)) {
		return NULL;
	}
	return &_digitalHistory[pin];
}

list <vector <unsigned char> > * ofArduino::getSysExHistory() {
	return &_sysExHistory;
}

list <string> * ofArduino::getStringHistory() {
	return &_stringHistory;
}

int ofArduino::getMajorFirmwareVersion() const {
	return _majorFirmwareVersion;
}

int ofArduino::getMinorFirmwareVersion() const {
	return _minorFirmwareVersion;
}

string ofArduino::getFirmwareName() const {
	return _firmwareName;
}

bool ofArduino::isInitialized() const {
	return _initialized;
}

bool ofArduino::isAttached() {
	//should return false if there is a serial error thus the arduino is not attached
	return _port.writeByte(static_cast<unsigned char>(END_SYSEX));
}

// ------------------------------ private functions

void ofArduino::processData(unsigned char inputData) {

	char msg[100];
	sprintf(msg, "Received Byte: %i", inputData);
	//Logger::get("Application").information(msg);

	// we have command data
	if (_waitForData > 0 && inputData < 128) {
		_waitForData--;

		// collect the data
		_storedInputData[_waitForData] = inputData;

		// we have all data executeMultiByteCommand
		if (_waitForData == 0) {
			switch (_executeMultiByteCommand) {
			case DIGITAL_MESSAGE:
				processDigitalPort(_multiByteChannel, (_storedInputData[0] << 7) | _storedInputData[1]);
				break;

			case REPORT_VERSION:    // report version
				_majorFirmwareVersion = _storedInputData[1];
				_minorFirmwareVersion = _storedInputData[0];
				ofNotifyEvent(EFirmwareVersionReceived, _majorFirmwareVersion, this);
				break;

			case ANALOG_MESSAGE:
				if (_initialized) {
					if (_analogHistory[_multiByteChannel].size() > 0) {
						int previous = _analogHistory[_multiByteChannel].front();

						_analogHistory[_multiByteChannel].push_front((_storedInputData[0] << 7) | _storedInputData[1]);
						if ((int)_analogHistory[_multiByteChannel].size() > _analogHistoryLength) {
							_analogHistory[_multiByteChannel].pop_back();
						}

						// trigger an event if the pin has changed value
						if (_analogHistory[_multiByteChannel].front() != previous) {
							ofNotifyEvent(EAnalogPinChanged, _multiByteChannel, this);
						}
					}
					else {
						_analogHistory[_multiByteChannel].push_front((_storedInputData[0] << 7) | _storedInputData[1]);
						if ((int)_analogHistory[_multiByteChannel].size() > _analogHistoryLength) {
							_analogHistory[_multiByteChannel].pop_back();
						}
					}
				}
				break;
			}

		}
	}
	// we have SysEx command data
	else if (_waitForData < 0) {

		// we have all sysex data
		if (inputData == END_SYSEX) {
			_waitForData = 0;
			processSysExData(_sysExData);
			_sysExData.clear();
		}
		// still have data, collect it
		else {
			_sysExData.push_back((unsigned char)inputData);
		}
	}
	// we have a command
	else {

		int command;

		// extract the command and channel info from a byte if it is less than 0xF0
		if (inputData < 0xF0) {
			command = inputData & 0xF0;
			_multiByteChannel = inputData & 0x0F;
		}
		else {
			// commands in the 0xF* range don't use channel data
			command = inputData;
		}

		switch (command) {
		case REPORT_VERSION:
		case DIGITAL_MESSAGE:
		case ANALOG_MESSAGE:
			_waitForData = 2;     // 2 bytes needed
			_executeMultiByteCommand = command;
			break;

		case START_SYSEX:
			_sysExData.clear();
			_waitForData = -1;     // n bytes needed, -1 is used to indicate sysex message
			_executeMultiByteCommand = command;
			break;
		}

	}
}

// sysex data is assumed to be 8-bit bytes split into two 7-bit bytes.
void ofArduino::processSysExData(vector <unsigned char> data) {

	string str;

	vector <unsigned char>::iterator it;
	unsigned char buffer;
	//int i = 1;

	// act on reserved sysEx messages (extended commands) or trigger SysEx event...
	switch (data.front()) {  //first byte in buffer is command
	case REPORT_FIRMWARE:
		ofLogVerbose("ofArduino") << "Recieved Firmware Report";
		it = data.begin();
		it++;    // skip the first byte, which is the firmware version command
		_majorFirmwareVersion = *it;
		it++;
		_minorFirmwareVersion = *it;
		it++;

		while (it != data.end()) {
			buffer = *it;
			it++;
			buffer += *it << 7;
			it++;
			str += buffer;
		}
		_firmwareName = str;

		ofNotifyEvent(EFirmwareVersionReceived, _majorFirmwareVersion, this);

		// trigger the initialization event
		if (!_initialized) {
			sendPinCapabilityRequest();
		}

		break;

	case STRING_DATA:
		ofLogVerbose("ofArduino") << "Recieved Firamta String";
		it = data.begin();
		it++;    // skip the first byte, which is the string command
		while (it != data.end()) {
			buffer = *it;
			it++;
			buffer += *it << 7;
			it++;
			str += buffer;
		}

		_stringHistory.push_front(str);
		if ((int)_stringHistory.size() > _stringHistoryLength) {
			_stringHistory.pop_back();
		}

		ofNotifyEvent(EStringReceived, str, this);
		break;
	case I2C_REPLY:
		ofLogVerbose("ofArduino") << "Recieved I2C Reply";
		if (data.size() > 7 && (data.size() - 5) % 2 == 0) {
			Firmata_I2C_Data i2creply;
			it = data.begin();

			i2creply.address = *++it | (*++it << 7);
			i2creply.reg = *++it | (*++it << 7);

			it++;
			while (it != data.end()) {
				str += *it | (*++it << 7);
				it++;
			}
			i2creply.data = str;
			ofNotifyEvent(EI2CDataRecieved, i2creply, this);
		}
		else {
			ofLogError("Arduino I2C") << "Incorrect Number of Bytes recieved, possible buffer overflow";
			purge();
		}
		break;
	case ENCODER_DATA:
		ofLogVerbose("ofArduino") << "Recieved Encoder Data";
		if (data.size() % 5 == 1) {

			vector<Firmata_Encoder_Data> encoderReply;
			Firmata_Encoder_Data tempEncoderReply;
			int encoderPos = 0;
			unsigned char encBuffer[4];

			it = data.begin();
			it++; // skip the first byte, which is the string command

			while (it != data.end()) {
				tempEncoderReply.ID = (*it & ENCODER_CHANNEL_MASK);
				tempEncoderReply.direction = (*it & ENCODER_DIRECTION_MASK);

				it++;

				encBuffer[0] = *it++ & 0x7F;
				encBuffer[1] = *it++ & 0x7F;
				encBuffer[2] = *it++ & 0x7F;
				encBuffer[3] = *it++ & 0x7F;

				encoderPos = encBuffer[3];
				encoderPos <<= 7;
				encoderPos |= encBuffer[2];
				encoderPos <<= 7;
				encoderPos |= encBuffer[1];
				encoderPos <<= 7;
				encoderPos |= encBuffer[0];

				tempEncoderReply.position = encoderPos;
				encoderReply.push_back(tempEncoderReply);
			}
			ofNotifyEvent(EEncoderDataReceived, encoderReply, this);
		}
		else {
			ofLogError("Arduino Encoder") << "Incorrect Number of Bytes recieved, possible buffer overflow";
			purge();
		}

		break;
	case SERIAL_MESSAGE:
	{
		ofLogVerbose("ofArduino") << "Recieved Serial Message";
		Firmata_Serial_Data reply;

		it = data.begin();
		//unsigned char command = *it & 0xF0;
		unsigned char portId = *it & 0x0F;
		it++;    // skip the first byte, which is the command and port

		while (it != data.end()) {
			buffer = *it;
			it++;
			buffer += *it << 7;
			it++;
			str += buffer;
		}
		switch (portId) {
		case HW_SERIAL0:
			reply.portID = HW_SERIAL0;
			break;
		case HW_SERIAL1:
			reply.portID = HW_SERIAL1;
			break;
		case HW_SERIAL2:
			reply.portID = HW_SERIAL2;
			break;
		case HW_SERIAL3:
			reply.portID = HW_SERIAL3;
			break;
		case SW_SERIAL0:
			reply.portID = SW_SERIAL0;
			break;
		case SW_SERIAL1:
			reply.portID = SW_SERIAL1;
			break;
		case SW_SERIAL2:
			reply.portID = SW_SERIAL2;
			break;
		case SW_SERIAL3:
			reply.portID = SW_SERIAL3;
			break;
		default:
			ofLogError("Arduino Serial") << "Port does not exist or is not defined";
		}
		reply.data = str;
		ofNotifyEvent(ESerialDataReceived, reply, this);
	}
	break;
	case CAPABILITY_RESPONSE:
	{
		ofLogVerbose("ofArduino") << "Recieved Capability Response";
		it = data.begin();
		it += 2; // skip the first byte, which is the string command

		int pin = 1;

		firmataInputSupported = false;
		firmataOutputSupported = false;
		firmataAnalogSupported = false;
		firmataPwmSupported = false;
		firmataServoSupported = false;
		firmataI2cSupported = false;
		firmataSerialSupported = false;
		firmataOnewireSupported = false;
		firmataStepperSupported = false;
		firmataEncoderSupported = false;

		while (it != data.end()) {
			switch (*it) {
			case ARD_INPUT:
				pinCapabilities[pin].inputSupported = true;
				pinCapabilities[pin].outputSupported = true;
				firmataInputSupported = true;
				firmataOutputSupported = true;
				it += 6;
				break;
			case ARD_ANALOG:
				pinCapabilities[pin].analogSupported = true;
				it += 2;
				if (!firmataAnalogSupported)
					_firstAnalogPin = pin;
				firmataAnalogSupported = true;
				break;
			case ARD_PWM:
				pinCapabilities[pin].pwmSupported = true;
				firmataPwmSupported = true;
				it += 2;
				break;
			case ARD_SERVO:
				pinCapabilities[pin].servoSupported = true;
				firmataServoSupported = true;
				it += 2;
				break;
			case ARD_I2C:
				pinCapabilities[pin].i2cSupported = true;
				firmataI2cSupported = true;
				it += 2;
				break;
			case ARD_SERIAL:
				pinCapabilities[pin].serialSupported = true;
				firmataI2cSupported = true;
				it += 2;
				break;
			case ARD_ONEWIRE:
				pinCapabilities[pin].onewireSupported = true;
				firmataOnewireSupported = true;
				it += 2;
				break;
			case ARD_STEPPER:
				pinCapabilities[pin].stepperSupported = true;
				firmataStepperSupported = true;
				it += 2;
				break;
			case ARD_ENCODER:
				pinCapabilities[pin].encoderSupported = true;
				firmataEncoderSupported = true;
				it += 2;
				break;
			default:
				it++;
				if (it != data.end()) {
					supportedPinTypes temp;
					pinCapabilities.emplace(++pin, temp);
				}
				break;
			}
		}
		_totalDigitalPins = pin;
		if (!_initialized) {
			sendAnalogMappingRequest();
		}
	}
	break;
	case ANALOG_MAPPING_RESPONSE:
	{
		ofLogVerbose("ofArduino") << "Recieved Analog Map Query Response";
		it = data.begin();
		int pin = 0;
		bool fAPin = false;
		it++;    // skip the first byte, which is the string command
		_totalAnalogPins = 0;
		while (it != data.end()) {
			//from the firmata protocol
			//analog channel corresponding to pin x, or 127 if pin x does not support analog
			if (*it != 127) {
				analogPinMap.emplace(int(*it), pin);
				_totalAnalogPins++;
				//these should be set by the capability query but just incase
				pinCapabilities[pin].analogSupported = true;
				if (!firmataAnalogSupported)
					firmataAnalogSupported = true;

				//lets also make sure that the capability response and this match up
				if (!fAPin) {
					if (pin != _firstAnalogPin)
						ofLogNotice("ofArduino") << "Capabaility Query and Analog Map don't match up";
					_firstAnalogPin = pin;
					fAPin = true;
				}
			}
			pin++;
			*it++;
		}
		if (!_initialized) {
			initPins();
		}
	}
	break;
	case PIN_STATE_RESPONSE:
	{
		ofLogVerbose("ofArduino") << "Recieved Pin State Query Response";
		it = data.begin();
		it++;    // skip the first byte, which is the string command
		int pin = *it++;
		Firmata_Pin_Modes mode;
		switch (*it++) {
		case ARD_INPUT:
			mode = Firmata_Pin_Modes::MODE_INPUT;
			break;
		case ARD_INPUT_PULLUP:
			mode = Firmata_Pin_Modes::MODE_INPUT_PULLUP;
			break;
		case ARD_OUTPUT:
			mode = Firmata_Pin_Modes::MODE_OUTPUT;
			break;
		case ARD_ANALOG:
			mode = Firmata_Pin_Modes::MODE_ANALOG;
			break;
		case ARD_PWM:
			mode = Firmata_Pin_Modes::MODE_PWM;
			break;
		case ARD_SERVO:
			mode = Firmata_Pin_Modes::MODE_SERVO;
			break;
		case ARD_I2C:
			mode = Firmata_Pin_Modes::MODE_I2C;
			break;
		case ARD_SERIAL:
			mode = Firmata_Pin_Modes::MODE_SERIAL;
			break;
		case ARD_ONEWIRE:
			mode = Firmata_Pin_Modes::MODE_ONEWIRE;
			break;
		case ARD_STEPPER:
			mode = Firmata_Pin_Modes::MODE_STEPPER;
			break;
		case ARD_ENCODER:
			mode = Firmata_Pin_Modes::MODE_ENCODER;
			break;
		}

		pair<int, Firmata_Pin_Modes> reply(pin, mode);
		ofNotifyEvent(EPinStateResponseReceived, reply, this);
	}
	break;
	default:    // the message isn't in Firmatas extended command set
		ofLogVerbose("ofArduino") << "This message isn't in Firmatas extended command set";
		_sysExHistory.push_front(data);
		if ((int)_sysExHistory.size() > _sysExHistoryLength) {
			_sysExHistory.pop_back();
		}
		ofNotifyEvent(ESysExReceived, data, this);
		break;

	}
}

void ofArduino::processDigitalPort(int port, unsigned char value) {

	if (!_initialized)
		return;
	unsigned char mask;
	int previous;
	int pin;

	for (int i = 0; i < 8; ++i) {
		pin = i + (port * 8);
		if (pin <= _totalDigitalPins && (_digitalPinMode[pin] == ARD_INPUT || _digitalPinMode[pin] == ARD_INPUT_PULLUP)) {
			if (!_digitalHistory[pin].empty())
				previous = _digitalHistory[pin].front();
			else previous = 0;

			mask = 1 << i;
			_digitalHistory[pin].push_front((value & mask) >> i);

			if ((int)_digitalHistory[pin].size() > _digitalHistoryLength)
				_digitalHistory[pin].pop_back();

			// trigger an event if the pin has changed value
			if (_digitalHistory[pin].front() != previous) {
				ofNotifyEvent(EDigitalPinChanged, pin, this);
			}
		}
	}
}

void ofArduino::sendDigitalPortReporting(int port, int mode) {
	sendByte(REPORT_DIGITAL | port);
	sendByte(mode);
	_digitalPortReporting[port] = mode;
}

void ofArduino::sendDigitalPinReporting(int pin, int mode) {

	int port = floor(pin / 8);
	if (mode == ARD_OFF || mode == ARD_ON) {
		_digitalPinReporting[pin] = mode;
		sendDigitalPortReporting(port, mode);
	}
}

void ofArduino::sendByte(unsigned char byte) {
	_port.writeByte(byte);
}

// in Firmata (and MIDI) data bytes are 7-bits. The 8th bit serves as a flag to mark a byte as either command or data.
// therefore you need two data bytes to send 8-bits (a char).
void ofArduino::sendValueAsTwo7bitBytes(int value) {
	sendByte(value & 127); // LSB
	sendByte(value >> 7 & 127); // MSB
}

// SysEx data is sent as 8-bit bytes split into two 7-bit bytes, this function merges two 7-bit bytes back into one 8-bit byte.
int ofArduino::getValueFromTwo7bitBytes(unsigned char lsb, unsigned char msb) {
	return (msb << 7) | lsb;
}

int ofArduino::getInvertedValueFromTwo7bitBytes(unsigned char lsb, unsigned char msb) {
	return msb | (lsb << 7);
}

/********************************************
*
*
*				Servo Functions
*
*
********************************************/

void ofArduino::sendServoAttach(int pin, int minPulse, int maxPulse) {
	if (!isPin(pin)) {
		return;
	}
	if (!pinCapabilities[pin].servoSupported) {
		ofLogError("ofArduino") << "Servo Control is not supported for pin" + ofToString(pin);
		return;
	}
	sendByte(START_SYSEX);
	sendByte(SERVO_CONFIG);
	sendByte(pin);
	sendValueAsTwo7bitBytes(minPulse);
	sendValueAsTwo7bitBytes(maxPulse);
	sendByte(END_SYSEX);
	_digitalPinMode[pin] = ARD_SERVO;
}

void ofArduino::sendServo(int pin, int value, bool force) {
	if (!isPin(pin)) {
		return;
	}
	if (!pinCapabilities[pin].servoSupported) {
		ofLogError("ofArduino") << "Servo Control is not supported for pin " + ofToString(pin);
		return;
	}
	if (_digitalPinMode[pin] != ARD_SERVO) {
		ofLogError("ofArduino") << "Servo Control is not configured for pin " + ofToString(pin) + ". Did you send a servo attach message?";
		return;
	}
	if (_digitalPinValue[pin] != value || force) {
		if (pin > 15) {
			sendByte(START_SYSEX);
			sendByte(EXTENDED_ANALOG);
			sendByte(pin);
			sendValueAsTwo7bitBytes(value);
			sendByte(END_SYSEX);

		}
		else {
			sendByte(ANALOG_MESSAGE | pin);
			sendValueAsTwo7bitBytes(value);
			_digitalPinValue[pin] = value;
		}
	}
}

int ofArduino::getServo(int pin) const {
	if (!isPin(pin)) {
		return -1;
	}
	if (!pinCapabilities[pin].servoSupported) {
		ofLogError("ofArduino") << "Servo Control is not supported for pin" + ofToString(pin);
		return -1;
	}
	if (_digitalPinMode[pin] == ARD_SERVO) {
		return _digitalPinValue[pin];
	}
	else {
		return -1;
	}
}

/********************************************
*
*
*				Stepper Functions
*
*
********************************************/

void  ofArduino::sendStepper2Wire(int dirPin, int stepPin, int stepsPerRev) {
	if (!isPin(dirPin)) {
		return;
	}
	if (!isPin(stepPin)) {
		return;
	}
	if (!pinCapabilities[dirPin].stepperSupported) {
		ofLogError("ofArduino") << "Stepper Control is not supported for pin" + ofToString(dirPin);
		return;
	}
	if (!pinCapabilities[stepPin].stepperSupported) {
		ofLogError("ofArduino") << "Stepper Control is not supported for pin" + ofToString(stepPin);
		return;
	}
	if (_numSteppers < MAX_STEPPERS) {
		sendByte(START_SYSEX);
		sendByte(STEPPER_DATA);
		sendByte(STEPPER_CONFIG);
		sendByte(_numSteppers);
		sendByte(FIRMATA_STEPPER_DRIVER);
		sendValueAsTwo7bitBytes(stepsPerRev);
		sendByte(dirPin);
		sendByte(stepPin);
		sendByte(END_SYSEX);
		_digitalPinMode[dirPin] = ARD_STEPPER;
		_digitalPinMode[stepPin] = ARD_STEPPER;
		_numSteppers++;
	}
	else {
		ofLogNotice("Arduino") << "Reached max number of steppers";
	}
}

void  ofArduino::sendStepper4Wire(int pin1, int pin2, int pin3, int pin4, int stepsPerRev) {
	if (!isPin(pin1)) {
		return;
	}
	if (!isPin(pin2)) {
		return;
	}
	if (!isPin(pin3)) {
		return;
	}
	if (!isPin(pin4)) {
		return;
	}
	if (!pinCapabilities[pin1].stepperSupported) {
		ofLogError("ofArduino") << "Stepper Control is not supported for pin" + ofToString(pin1);
		return;
	}
	if (!pinCapabilities[pin2].stepperSupported) {
		ofLogError("ofArduino") << "Stepper Control is not supported for pin" + ofToString(pin2);
		return;
	}
	if (!pinCapabilities[pin3].stepperSupported) {
		ofLogError("ofArduino") << "Stepper Control is not supported for pin" + ofToString(pin3);
		return;
	}
	if (!pinCapabilities[pin4].stepperSupported) {
		ofLogError("ofArduino") << "Stepper Control is not supported for pin" + ofToString(pin4);
		return;
	}
	if (_numSteppers < MAX_STEPPERS) {
		sendByte(START_SYSEX);
		sendByte(STEPPER_DATA);
		sendByte(STEPPER_CONFIG);
		sendByte(_numSteppers);
		sendByte(FIRMATA_STEPPER_FOUR_WIRE);
		sendValueAsTwo7bitBytes(stepsPerRev);
		sendByte(pin1);
		sendByte(pin2);
		sendByte(pin3);
		sendByte(pin4);
		sendByte(END_SYSEX);

		_digitalPinMode[pin1] = ARD_STEPPER;
		_digitalPinMode[pin2] = ARD_STEPPER;
		_digitalPinMode[pin3] = ARD_STEPPER;
		_digitalPinMode[pin4] = ARD_STEPPER;
		_numSteppers++;
	}
	else {
		ofLogNotice("Arduino") << "Reached max number of steppers";
	}

}

void ofArduino::sendStepperMove(int stepperID, int direction, int numSteps, int speed, float acceleration, float deceleration) {

	if (stepperID <= _numSteppers && stepperID >= 0) {
		unsigned char steps[3] = { static_cast<unsigned char>(abs(numSteps) & 0x0000007F), static_cast<unsigned char>((abs(numSteps) >> 7) & 0x0000007F), static_cast<unsigned char>((abs(numSteps) >> 14) & 0x0000007F) };

		// the stepper interface expects decimal expressed an an integer
		if (acceleration != 0 && deceleration != 0) {
			int accel = floor(acceleration * 100);
			int decel = floor(deceleration * 100);

			sendByte(START_SYSEX);
			sendByte(STEPPER_DATA);
			sendByte(STEPPER_STEP);
			sendByte(stepperID);
			sendByte(direction);
			sendByte(steps[0]);
			sendByte(steps[1]);
			sendByte(steps[2]);
			sendValueAsTwo7bitBytes(speed);
			sendValueAsTwo7bitBytes(accel);
			sendValueAsTwo7bitBytes(decel);
			sendByte(END_SYSEX);

		}
		else {
			sendByte(START_SYSEX);
			sendByte(STEPPER_DATA);
			sendByte(STEPPER_STEP);
			sendByte(stepperID);
			sendByte(direction);
			sendByte(steps[0]);
			sendByte(steps[1]);
			sendByte(steps[2]);
			sendValueAsTwo7bitBytes(speed);
			sendByte(END_SYSEX);
		}
	}
}


/********************************************
*
*
*				I2C Functions
*
*
********************************************/

void  ofArduino::sendI2CConfig(int delay) {
	sendByte(START_SYSEX);
	sendByte(I2C_CONFIG);
	sendByte(delay & 0xFF);
	sendByte((delay >> 8) & 0xFF);
	sendByte(END_SYSEX);

	_i2cConfigured = true;
}

void  ofArduino::sendI2CWriteRequest(char slaveAddress, unsigned char * bytes, int numOfBytes, int reg) {

	if (_i2cConfigured) {
		sendByte(START_SYSEX);
		sendByte(I2C_REQUEST);
		sendByte(slaveAddress);
		sendByte(FIRMATA_I2C_WRITE << 3);
		if (reg >= 0) {
			sendValueAsTwo7bitBytes(reg);
		}
		for (int i = 0, length = numOfBytes; i < length; i++) {
			sendValueAsTwo7bitBytes(bytes[i]);
		}

		sendByte(END_SYSEX);
	}
	else {
		ofLogError("Arduino") << "I2C was not configured, did you send an I2C config request?";
	}
}

void  ofArduino::sendI2CWriteRequest(char slaveAddress, char * bytes, int numOfBytes, int reg) {
	if (_i2cConfigured) {
		sendByte(START_SYSEX);
		sendByte(I2C_REQUEST);
		sendByte(slaveAddress);
		sendByte(FIRMATA_I2C_WRITE << 3);
		if (reg >= 0) {
			sendValueAsTwo7bitBytes(reg);
		}
		for (int i = 0, length = numOfBytes; i < length; i++) {
			sendValueAsTwo7bitBytes(bytes[i]);
		}

		sendByte(END_SYSEX);
	}
	else {
		ofLogError("Arduino") << "I2C was not configured, did you send an I2C config request?";
	}
}

void  ofArduino::sendI2CWriteRequest(char slaveAddress, const char * bytes, int numOfBytes, int reg) {
	if (_i2cConfigured) {
		sendByte(START_SYSEX);
		sendByte(I2C_REQUEST);
		sendByte(slaveAddress);
		sendByte(FIRMATA_I2C_WRITE << 3);
		if (reg >= 0) {
			sendValueAsTwo7bitBytes(reg);
		}
		for (int i = 0, length = numOfBytes; i < length; i++) {
			sendValueAsTwo7bitBytes(bytes[i]);
		}

		sendByte(END_SYSEX);
	}
	else {
		ofLogError("Arduino") << "I2C was not configured, did you send an I2C config request?";
	}
}

void  ofArduino::sendI2CWriteRequest(char slaveAddress, vector<char> bytes, int reg) {

	if (_i2cConfigured) {
		sendByte(START_SYSEX);
		sendByte(I2C_REQUEST);
		sendByte(slaveAddress);
		sendByte(FIRMATA_I2C_WRITE << 3);
		if (reg >= 0) {
			sendValueAsTwo7bitBytes(reg);
		}
		for (int i = 0, length = bytes.size(); i < length; i++) {
			sendValueAsTwo7bitBytes(bytes[i]);
		}

		sendByte(END_SYSEX);
	}
	else {
		ofLogError("Arduino") << "I2C was not configured, did you send an I2C config request?";
	}
}

void  ofArduino::sendI2CReadRequest(char address, int numBytes, int reg) {

	if (_i2cConfigured) {
		sendByte(START_SYSEX);
		sendByte(I2C_REQUEST);
		sendByte(address);
		sendByte(FIRMATA_I2C_READ << 3);
		if (reg >= 0) {
			sendValueAsTwo7bitBytes(reg);
		}
		sendValueAsTwo7bitBytes(numBytes);
		sendByte(END_SYSEX);
	}
	else {
		ofLogError("Arduino") << "I2C was not configured, did you send an I2C config request?";
	}
}

void  ofArduino::sendI2ContinuousReadRequest(char address, int numBytes, int reg) {
	if (_i2cConfigured) {
		sendByte(START_SYSEX);
		sendByte(I2C_REQUEST);
		sendByte(address);
		sendByte(FIRMATA_I2C_CONTINUOUS_READ << 3);
		if (reg >= 0) {
			sendValueAsTwo7bitBytes(reg);
		}
		sendValueAsTwo7bitBytes(numBytes);
		sendByte(END_SYSEX);
	}
	else {
		ofLogError("Arduino") << "I2C was not configured, did you send an I2C config request?";
	}
}

bool ofArduino::isI2CConfigured() {
	return  _i2cConfigured;
}

// CONTINUOUS_READ

/********************************************
*
*
*				One Wire Functions
*
*
********************************************/

void  ofArduino::sendOneWireConfig(int pin, bool enableParasiticPower) {
	if (!isPin(pin)) {
		return;
	}
	if (!pinCapabilities[pin].onewireSupported) {
		ofLogError("ofArduino") << "Onewire is not supported for pin " << pin;
		return;
	}
	sendByte(START_SYSEX);
	sendByte(ONEWIRE_DATA);
	sendByte(ONEWIRE_CONFIG_REQUEST);
	sendByte(pin);
	sendByte(enableParasiticPower ? 0x01 : 0x00);
	sendByte(END_SYSEX);
};

void  ofArduino::sendOneWireSearch(int pin) {
	sendOneWireSearch(ONEWIRE_SEARCH_REQUEST, pin);
};

void  ofArduino::sendOneWireAlarmsSearch(int pin) {
	sendOneWireSearch(ONEWIRE_SEARCH_ALARMS_REQUEST, pin);
};

//needs to notify event handler
void  ofArduino::sendOneWireSearch(char type, int pin) {
	if (!isPin(pin)) {
		return;
	}
	if (!pinCapabilities[pin].onewireSupported) {
		ofLogError("ofArduino") << "Onewire is not supported for pin " << pin;
		return;
	}
	sendByte(START_SYSEX);
	sendByte(ONEWIRE_DATA);
	sendByte(type);
	sendByte(pin);
	sendByte(END_SYSEX);
}

void  ofArduino::sendOneWireRead(int pin, vector<unsigned char> devices, int numBytesToRead) {
	int correlationId = floor(ofRandomuf() * 255);
	vector<unsigned char> b;
	sendOneWireRequest(pin, ONEWIRE_READ_REQUEST_BIT, devices, numBytesToRead, correlationId, 0, b);
}

void  ofArduino::sendOneWireReset(int pin) {
	vector<unsigned char> a, b;
	sendOneWireRequest(pin, ONEWIRE_RESET_REQUEST_BIT, a, 0, 0, 0, b);
};

void  ofArduino::sendOneWireWrite(int pin, vector<unsigned char> devices, vector<unsigned char> data) {
	sendOneWireRequest(pin, ONEWIRE_WRITE_REQUEST_BIT, devices, 0, 0, 0, data);
};

void  ofArduino::sendOneWireDelay(int pin, unsigned int delay) {
	vector<unsigned char> a, b;
	sendOneWireRequest(pin, ONEWIRE_DELAY_REQUEST_BIT, a, 0, 0, delay, b);
};

void  ofArduino::sendOneWireWriteAndRead(int pin, vector<unsigned char> devices, vector<unsigned char> data, int numBytesToRead) {
	int correlationId = floor(ofRandomuf() * 255);
	sendOneWireRequest(pin, ONEWIRE_WRITE_REQUEST_BIT | ONEWIRE_READ_REQUEST_BIT, devices, numBytesToRead, correlationId, 0, data);
}

//// see http://firmata.org/wiki/Proposals#OneWire_Proposal
void  ofArduino::sendOneWireRequest(int pin, unsigned char subcommand, vector<unsigned char> devices, int numBytesToRead, unsigned char correlationId, unsigned int delay, vector<unsigned char> dataToWrite) {
	if (!isPin(pin)) {
		return;
	}
	if (!pinCapabilities[pin].onewireSupported) {
		ofLogError("ofArduino") << "Onewire is not supported for pin " << pin;
		return;
	}
	vector<unsigned char> bytes;
	bytes.resize(16);

	if (devices.size() > 0 || numBytesToRead > 0 || correlationId || delay > 0 || dataToWrite.size() > 0) {
		subcommand = subcommand | ONEWIRE_WITHDATA_REQUEST_BITS;
	}

	if (devices.size() > 0) {
		for (size_t i = 0; i < devices.size(); i++) {
			bytes[i] = devices[i];
		}
	}

	if (numBytesToRead > 0) {
		bytes[8] = numBytesToRead & 0xFF;
		bytes[9] = (numBytesToRead >> 8) & 0xFF;
	}

	if (correlationId) {
		bytes[10] = correlationId & 0xFF;
		bytes[11] = (correlationId >> 8) & 0xFF;
	}

	if (delay > 0) {
		bytes[12] = delay & 0xFF;
		bytes[13] = (delay >> 8) & 0xFF;
		bytes[14] = (delay >> 16) & 0xFF;
		bytes[15] = (delay >> 24) & 0xFF;
	}

	if (dataToWrite.size() > 0) {
		for (size_t i = 0; i < dataToWrite.size(); i++) {
			bytes.push_back(dataToWrite[i]);
		}
	}

	sendByte(START_SYSEX);
	sendByte(ONEWIRE_DATA);
	sendByte(subcommand);
	sendByte(pin);
	int shift = 0;
	int previous = 0;
	//i dont think this is safe
	for (size_t i = 0; i < bytes.size(); i++) {
		if (shift == 0) {
			sendByte(bytes[i] & 0x7f);
			shift++;
			previous = bytes[i] >> 7;
		}
		else {
			sendByte(((bytes[i] << shift) & 0x7f) | previous);
			if (shift == 6) {
				sendByte(bytes[i] >> 1);
				shift = 0;
			}
			else {
				shift++;
				previous = bytes[i] >> (8 - shift);
			}
		}
	}
	if (shift > 0) {
		sendByte(previous);
	}

	sendByte(END_SYSEX);
}

/********************************************
*
*
*				Encoder Functions
*
*
********************************************/

void ofArduino::attachEncoder(int pinA, int pinB) {
	if (!isPin(pinA)) {
		return;
	}
	if (!isPin(pinB)) {
		return;
	}
	if (!pinCapabilities[pinA].encoderSupported) {
		ofLogError("ofArduino") << "Encoder Control is not supported for pin " + ofToString(pinA);
		return;
	}
	if (!pinCapabilities[pinB].encoderSupported) {
		ofLogError("ofArduino") << "Encoder Control is not supported for pin " + ofToString(pinB);
		return;
	}
	if (_encoderID < MAX_ENCODERS) {
		sendByte(START_SYSEX);
		sendByte(ENCODER_DATA);
		sendByte(ENCODER_ATTACH);
		sendByte(_encoderID);
		sendByte(pinA);
		sendByte(pinB);
		sendByte(END_SYSEX);
		_encoderID++;
	}

}
void ofArduino::getEncoderPosition(int encoderID) {
	if (encoderID <= _encoderID && encoderID >= 0) {
		sendByte(START_SYSEX);
		sendByte(ENCODER_DATA);
		sendByte(ENCODER_REPORT_POSITION);
		sendByte(encoderID);
		sendByte(END_SYSEX);
	}
}
void ofArduino::getAllEncoderPositions() {
	sendByte(START_SYSEX);
	sendByte(ENCODER_DATA);
	sendByte(ENCODER_REPORT_POSITIONS);
	sendByte(END_SYSEX);
}
void ofArduino::resetEncoderPosition(int encoderID) {
	if (encoderID <= _encoderID && encoderID >= 0) {
		sendByte(START_SYSEX);
		sendByte(ENCODER_DATA);
		sendByte(ENCODER_RESET_POSITION);
		sendByte(encoderID);
		sendByte(END_SYSEX);
	}
}
void ofArduino::enableEncoderReporting() {
	sendByte(START_SYSEX);
	sendByte(ENCODER_DATA);
	sendByte(ENCODER_REPORT_AUTO);
	sendByte(1);
	sendByte(END_SYSEX);
}
void ofArduino::disableEncoderReporting() {
	sendByte(START_SYSEX);
	sendByte(ENCODER_DATA);
	sendByte(ENCODER_REPORT_AUTO);
	sendByte(0);
	sendByte(END_SYSEX);
}
void ofArduino::detachEncoder(int encoderID) {
	if (encoderID <= _encoderID && encoderID >= 0) {
		sendByte(START_SYSEX);
		sendByte(ENCODER_DATA);
		sendByte(ENCODER_DETACH);
		sendByte(encoderID);
		sendByte(END_SYSEX);
		_encoderID--;
	}
}

//if the buffer gets out of sync we have to purge everything to get back on track
void ofArduino::purge() {
	while (_port.readByte() != -1);
	for (int i = 0; i < 5; i++)
		sendByte(END_SYSEX);
}

/********************************************
*
*
*				Serial Functions
*
*
********************************************/

void ofArduino::sendSerialConfig(Firmata_Serial_Ports portID, int baud, int rxPin, int txPin) {

	if (portID > 7 && rxPin < 0 && txPin < 0) {
		ofLogError("ofArduino") << "Both RX and TX pins must be defined when using Software Serial.";
		return;
	}

	sendByte(START_SYSEX);
	sendByte(SERIAL_MESSAGE);
	sendByte(SERIAL_CONFIG | portID);
	sendByte(baud & 0x007F);
	sendByte((baud >> 7) & 0x007F);
	sendByte((baud >> 14) & 0x007F);

	if (portID > 7 && rxPin >= 0 && txPin >= 0) {
		sendByte(rxPin);
		sendByte(txPin);
	}
	else if (portID > 7) {
		ofLogError("ofArduino") << "Both RX and TX pins must be defined when using Software Serial.";
	}

	sendByte(END_SYSEX);
};

void ofArduino::serialWrite(Firmata_Serial_Ports portID, unsigned char * bytes, int numOfBytes) {
	sendByte(START_SYSEX);
	sendByte(SERIAL_MESSAGE);
	sendByte(SERIAL_WRITE | portID);
	for (int i = 0; i < numOfBytes; i++) {
		sendValueAsTwo7bitBytes(bytes[i]);
	}
	sendByte(END_SYSEX);
};

void ofArduino::serialRead(Firmata_Serial_Ports portID, int maxBytesToRead) {
	sendByte(START_SYSEX);
	sendByte(SERIAL_MESSAGE);
	sendByte(SERIAL_READ | portID);
	sendByte(FIRMATA_SERIAL_READ_CONTINUOUS);

	if (maxBytesToRead > 0) {
		sendValueAsTwo7bitBytes(maxBytesToRead);
	}

	sendByte(END_SYSEX);
};

void ofArduino::serialStop(Firmata_Serial_Ports portID) {
	sendByte(START_SYSEX);
	sendByte(SERIAL_MESSAGE);
	sendByte(SERIAL_READ | portID);
	sendByte(FIRMATA_SERIAL_STOP_READING);
	sendByte(END_SYSEX);
};

void ofArduino::serialClose(Firmata_Serial_Ports portID) {
	sendByte(START_SYSEX);
	sendByte(SERIAL_MESSAGE);
	sendByte(SERIAL_CLOSE | portID);
	sendByte(END_SYSEX);
};

void ofArduino::serialFlush(Firmata_Serial_Ports portID) {
	sendByte(START_SYSEX);
	sendByte(SERIAL_MESSAGE);
	sendByte(SERIAL_FLUSH | portID);
	sendByte(END_SYSEX);
};

void ofArduino::serialListen(Firmata_Serial_Ports portID) {
	// listen only applies to software serial ports
	if (portID < 8) {
		return;
	}
	sendByte(START_SYSEX);
	sendByte(SERIAL_MESSAGE);
	sendByte(SERIAL_LISTEN | portID);
	sendByte(END_SYSEX);
};



//this is mostly safe except for the teensy 2.0, it has 12 analog outs starting at 22 going down to 11
//the problem arises if they mean digital pin 11 or analog pin 11 which are not the same
//digital pin 11 is analog pin 10 and analog pin 11 is digital pin 22
bool ofArduino::isAnalogPin(int pin) const
{
	if (isPin(convertAnalogPinToDigital(pin))) {
		if (pin < _firstAnalogPin) // this probably means they are using the Analog pin #
		{
			if (analogPinMap.count(pin) < 1) { //the pin doesnt exist in the analog pin mapping
				ofLogError("ofArduino") << "Analog is not supported for pin " + ofToString(pin);
				return false;
			}
			return true;
		}
		else //they are using the digital pin #
		{
			if (pinCapabilities.count(pin) < 1) {
				ofLogError("ofArduino") << "Analog is not supported for pin " + ofToString(pin);
				return false;
			}
			return true;
		}
	}
	return false;
}


//this should only be false if the pin is negative or greater than the number of pins on the board
//just incase we check against the pin capability map if for some reason a pin doesn't exist
//pin 0 & 1 are outliers because they dont exist in the capability query but technically exist
//we cant really use pin 0 & 1 though as it messes up the serial communication
bool ofArduino::isPin(int pin) const
{
	if (pin < 0 || pin > _totalDigitalPins) {
		ofLogError("ofArduino") << "Pin " + ofToString(pin) + " does not exist on the current board";
		return false;
	}
	if (pinCapabilities.count(pin) < 1) {
		ofLogError("ofArduino") << "Pin " + ofToString(pin) + " does not exist on the current board";
		return false;
	}
	return true;
}

//this returns the pin if its already not within the analog pin map
//we need this to check between digital and analog pin mappings
int ofArduino::convertAnalogPinToDigital(size_t pin) const
{
	if (pin < analogPinMap.size()) {
		if (analogPinMap.count(pin) > 0) {
			return analogPinMap[pin];
		}
		else {
			ofLogError("ofArduino") << "Pin " + ofToString(pin) + " is not an Analog Pin";
			return -1;
		}
	}
	return pin;
}

//this returns the pin if its already within the analog pin map
//we need this to check between digital and analog pin mappings
int ofArduino::convertDigitalPinToAnalog(size_t pin) const
{
	if (pin > analogPinMap.size()) {
		for (auto aPin : analogPinMap)
			if (aPin.second == (int)pin) {
				return aPin.first;
			}
		ofLogError("ofArduino") << "Pin " + ofToString(pin) + " is not an Analog Pin";
		return -1;
	}
	return pin; //this pin is already in the range of analog pins
}

//these functions can't really account for user error 
