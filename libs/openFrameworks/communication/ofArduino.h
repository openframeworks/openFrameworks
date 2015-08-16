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
 * included in all copies or substantial portions of the Software.
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
#pragma once

#include <list>
#include "ofConstants.h"
#include "ofEvents.h"
#include "ofSerial.h"

 /* Version numbers for the protocol.  The protocol is still changing, so these
 * version numbers are important.  This number can be queried so that host
 * software can test whether it will be compatible with the currently
 * installed firmware. */
#define FIRMATA_MAJOR_VERSION	2 // for non-compatible changes
#define FIRMATA_MINOR_VERSION	4 // for backwards compatible changes
#define FIRMATA_BUGFIX_VERSION	4 // for bugfix releases

#define FIRMATA_MAX_DATA_BYTES	64 // max number of data bytes in incoming messages

 // message command bytes (128-255/0x80-0xFF)
#define DIGITAL_MESSAGE			0x90 // send data for a digital port (collection of 8 pins)
#define ANALOG_MESSAGE			0xE0 // send data for an analog pin (or PWM)
#define REPORT_ANALOG			0xC0 // enable analog input by pin #
#define REPORT_DIGITAL			0xD0 // enable digital input by port pair
 //
#define SET_PIN_MODE			0xF4 // set a pin to INPUT/OUTPUT/PWM/etc
 //
#define REPORT_VERSION			0xF9 // report protocol version
#define SYSTEM_RESET			0xFF // reset from MIDI
 //
#define START_SYSEX				0xF0 // start a MIDI Sysex message
#define END_SYSEX				0xF7 // end a MIDI Sysex message

 // extended command set using sysex (0-127/0x00-0x7F)
 /* 0x00-0x0F reserved for user-defined commands */
#define ENCODER_DATA			0x61 // reply with encoders current positions
#define SERVO_CONFIG			0x70 // set max angle, minPulse, maxPulse, freq
#define STRING_DATA				0x71 // a string message with 14-bits per char
#define STEPPER_DATA			0x72 // control a stepper motor
#define ONEWIRE_DATA			0x73 // send an OneWire read/write/reset/select/skip/search request
#define SHIFT_DATA				0x75 // a bitstream to/from a shift register
#define I2C_REQUEST				0x76 // send an I2C read/write request
#define I2C_REPLY				0x77 // a reply to an I2C read request
#define I2C_CONFIG				0x78 // config I2C settings such as delay times and power pins
#define EXTENDED_ANALOG			0x6F // analog write (PWM, Servo, etc) to any pin
#define PIN_STATE_QUERY			0x6D // ask for a pin's current mode and value
#define PIN_STATE_RESPONSE		0x6E // reply with pin's current mode and value
#define CAPABILITY_QUERY		0x6B // ask for supported modes and resolution of all pins
#define CAPABILITY_RESPONSE		0x6C // reply with supported modes and resolution
#define ANALOG_MAPPING_QUERY	0x69 // ask for mapping of analog to pin numbers
#define ANALOG_MAPPING_RESPONSE	0x6A // reply with mapping info
#define REPORT_FIRMWARE			0x79 // report name and version of the firmware
#define SAMPLING_INTERVAL		0x7A // set the poll rate of the main loop
#define SCHEDULER_DATA			0x7B // send a createtask/deletetask/addtotask/schedule/querytasks/querytask request to the scheduler
#define SYSEX_NON_REALTIME		0x7E // MIDI Reserved for non-realtime messages
#define SYSEX_REALTIME			0x7F // MIDI Reserved for realtime messages
 // these are DEPRECATED to make the naming more consistent
#define FIRMATA_STRING			0x71 // same as STRING_DATA
#define SYSEX_I2C_REQUEST		0x76 // same as I2C_REQUEST
#define SYSEX_I2C_REPLY			0x77 // same as I2C_REPLY
#define SYSEX_SAMPLING_INTERVAL	0x7A // same as SAMPLING_INTERVAL

 // pin modes
#define ARD_INPUT				0x00 // defined in Arduino.h
#define ARD_OUTPUT				0x01 // defined in Arduino.h
#define ARD_ANALOG				0x02 // analog pin in analogInput mode
#define ARD_PWM					0x03 // digital pin in PWM output mode
#define ARD_SERVO				0x04 // digital pin in Servo output mode
#define ARD_SHIFT				0x05 // shiftIn/shiftOut mode
#define ARD_I2C					0x06 // pin included in I2C setup
#define ARD_ONEWIRE				0x07 // pin configured for 1-wire
#define ARD_STEPPER				0x08 // pin configured for stepper motor
#define ARD_ENCODER				0x09 // pin configured for rotary encoders
#define ARD_IGNORE				0x7F // pin configured to be ignored by digitalWrite and capabilityResponse
#define TOTAL_PIN_MODES			11

//Stepper Subcommands
#define MAX_STEPPERS			6 // arbitrary value... may need to adjust
#define STEPPER_CONFIG			0
#define STEPPER_STEP			1

//Onewire Subcommands:
#define ONEWIRE_SEARCH_REQUEST			0x40
#define ONEWIRE_CONFIG_REQUEST			0x41
#define ONEWIRE_SEARCH_REPLY			0x42
#define ONEWIRE_READ_REPLY				0x43
#define ONEWIRE_SEARCH_ALARMS_REQUEST	0x44
#define ONEWIRE_SEARCH_ALARMS_REPLY		0x45

#define ONEWIRE_RESET_REQUEST_BIT		0x01
#define ONEWIRE_SKIP_REQUEST_BIT		0x02
#define ONEWIRE_SELECT_REQUEST_BIT		0x04
#define ONEWIRE_READ_REQUEST_BIT		0x08
#define ONEWIRE_DELAY_REQUEST_BIT		0x10
#define ONEWIRE_WRITE_REQUEST_BIT		0x20

#define ONEWIRE_WITHDATA_REQUEST_BITS	0x3C

//default value for power:
#define ONEWIRE_POWER 1

//I2C Subcommands
#define I2C_WRITE						B00000000
#define I2C_READ						B00001000
#define I2C_READ_CONTINUOUSLY			B00010000
#define I2C_STOP_READING				B00011000
#define I2C_READ_WRITE_MODE_MASK		B00011000
#define I2C_10BIT_ADDRESS_MODE_MASK		B00100000

#define MAX_QUERIES						8
#define REGISTER_NOT_SPECIFIED			-1

//Encoder Subcommands
#define MAX_ENCODERS									5 // arbitrary value, may need to adjust
#define ENCODER_ATTACH									0x00
#define ENCODER_REPORT_POSITION							0x01
#define ENCODER_REPORT_POSITIONS						0x02
#define ENCODER_RESET_POSITION							0x03
#define ENCODER_REPORT_AUTO								0x04
#define ENCODER_DETACH									0x05
#define ENCODER_DIRECTION_MASK							0x40 // B01000000
#define ENCODER_CHANNEL_MASK							0x3F // B00111111

// OF specific settings
//these are given by the capability query
//#define ARD_TOTAL_DIGITAL_PINS        70 // total number of pins currently supported
//#define ARD_TOTAL_ANALOG_PINS         6
#define ARD_TOTAL_PORTS					15 // total number of ports for the board

#define ARD_HIGH						1
#define ARD_LOW							0
#define ARD_ON							1
#define ARD_OFF							0

#define OF_ARDUINO_DELAY_LENGTH			4.0

struct supportedPinTypes {
	bool	inputSupported = false;
	bool	outputSupported = false;
	bool	analogSupported = false;
	bool	pwmSupported = false;
	bool	servoSupported = false;
	bool	i2cSupported = false;
	bool	onewireSupported = false;
	bool	stepperSupported = false;
	bool	encoderSupported = false;
};

enum Stepper_Interface {
	DRIVER = 1,
	TWO_WIRE = 2,
	FOUR_WIRE = 3
};

enum Direction {
	CCW = 0,
	CW = 1
};
enum i2c_modes {
	WRITE = 0x00,
	READ = 1,
	CONTINUOUS_READ = 2,
	STOP_READING = 3
};

struct I2C_Data {
	int		address;
	int		reg;
	string	data;
};

struct Encoder_Data {
	int		ID;
	bool	direction;
	int		position;
};

struct Stepper_Data {
	int		id;
	int		type;
	int		data;
};

/// \brief This is a way to control an Arduino that has had the firmata library
/// loaded onto it, from OF.
///
/// To load firmata onto your Arduino, run the Arduino IDE, open the Examples >
/// Firmata > StandardFirmata sketch, and upload it to the Arduino board.
///
/// Once the ofArduino instance returns true from isArduinoReady() you can set
/// the mode of the different digital pins using sendDigitalPinMode()
///
/// This sets pin 9 to input so that it can read a button press
/// ~~~~{.cpp}
///     sendDigitalPinMode(9, ARD_INPUT)
/// ~~~~
///
/// This sets pin 9 to be a PWM out pin. Note that this only works on pins
/// that are PWM enabled.
/// ~~~~{.cpp}
///     sendDigitalPinMode(9, ARD_PWM)
/// ~~~~
class ofArduino {

public:
	/// \name Constructor and Destructor
	/// \{

	ofArduino();

	virtual ~ofArduino();

	/// \}
	/// \name Connect
	/// \{

	/// \brief Opens a serial port connection to the arduino
	/// \param device The name of the device.
	/// You can get the name from the Arduino IDE
	/// \param baud The baud rate the connection uses
	bool connect(const std::string & device, int baud = 57600);

	/// \brief Returns true if a succesfull connection has been established
	/// and the Arduino has reported a firmware
	bool isInitialized() const;

	bool isArduinoReady();

	/// \brief Closes the serial port connection.
	/// Does not turn the Arduino off.
	void disconnect();

	/// \}
	/// \name Update
	/// \{

	/// \brief Polls data from the serial port, this has to be called periodically
	void update();

	/// \}
	/// \name Setup
	/// \{

	/// \brief Setting a pins mode to ARD_INPUT turns on reporting for the port the pin is on
	/// \param pin Pin on arduino (2-13)
	/// \param mode `ARD_INPUT`, `ARD_OUTPUT`, `ARD_PWM`
	/// \note Analog pins 0-5 can be used as digitial pins 16-21 but if the
	/// mode of _one_ of these pins is set to `ARD_INPUT` then _all_ analog pin
	/// reporting will be turned off
	void sendDigitalPinMode(int pin, int mode);

	void sendAnalogPinReporting(int pin, int mode);

	void setUseDelay(bool bDelay);

	void setDigitalHistoryLength(int length);
	void setAnalogHistoryLength(int length);
	void setStringHistoryLength(int length);
	void setSysExHistoryLength(int nSysEx);

	/// \}
	/// \name Senders
	/// \{

	void sendDigital(int pin, int value, bool force = false);

	void sendPwm(int pin, int value, bool force = false);

	void sendSysEx(int command, vector <unsigned char> data);

	bool isAttached();



	/// \brief Send a string to the Arduino
	/// \note Firmata can not handle strings longer than 12 characters.
	void sendString(string str);

	void sendProtocolVersionRequest();

	void sendFirmwareVersionRequest();

	void sendPinCofigurationRequest();

	void sendPinCapabilityRequest();

	/// \brief This will cause your Arduino to reset and boot into the program again.
	void sendReset();

	/// \brief Sends the `FIRMATA_START_SYSEX` command
	void sendSysExBegin();

	/// \brief Sends the `FIRMATA_END_SYSEX` command
	void sendSysExEnd();

	/// \brief Sends a byte without wrapping it in a firmata message.
	///
	/// Data has to be in the 0-127 range. Values > 127 will be interpreted as
	/// commands.
	void sendByte(unsigned char byte);

	/// \brief Send value as two 7 bit bytes.
	///
	/// Sends a value as two 7-bit bytes without wrapping it in a firmata
	/// message.  Values in the range 0 - 16384 will be sent as two bytes
	/// within the 0-127 data range.
	///
	/// \param value The value to send.
	void sendValueAsTwo7bitBytes(int value);

	/// \}
	/// \name Getters
	/// \{

	/// \brief Returns the last set PWM value (0-255) for the given pin
	///
	/// The pins mode has to be ARD_PWM
	///
	/// On the Arduino Uno the following pins are supported: 3, 5, 6, 9, 10 and 11
	/// \note Pin 16-21 can also be used if analog inputs 0-5 are used as digital pins
	int getPwm(int pin) const;

	/// \brief Returns the last received value (if the pin mode is ARD_INPUT)
	/// or the last set value (if the pin mode is ARD_OUTPUT) for the given
	/// pin
	///
	/// Returns whether the pin is reading high or low, 1 or 0. You can test
	/// against this with an if() statement which is handy:
	/// ~~~~{.cpp}
	///     if(arduino.getDigital(pin)){
	///         // do something on high
	///     } else {
	///         // do something on low
	///     }
	/// ~~~~
	/// \note Pin 16-21 can also be used if analog inputs 0-5 are used as digital pins
	int getDigital(int pin) const;

	/// \brief Returns the analog in value that the pin is currently reading.
	/// because the Arduino has a 10 bit ADC you get between 0 and 1023 for
	/// possible values.
	///
	/// \param pin The pin number (0-5)
	int getAnalog(int pin) const;

	/// \returns the last received SysEx message.
	vector <unsigned char> getSysEx() const;

	/// \returns the last received string.
	string getString() const;

	/// \brief Returns the major firmware version
	int getMajorProtocolVersion() const;

	/// \returns the minor firmware version.
	int getMinorProtocolVersion() const;

	/// \returns the major firmware version.
	int getMajorFirmwareVersion() const;

	/// \returns the minor firmware version.
	int getMinorFirmwareVersion() const;

	/// \returns the name of the firmware.
	string getFirmwareName() const;

	/// \brief Returns a pointer to the digital data history list for the
	/// given pin
	/// \note Pin 16-21 can also be used if analog inputs 0-5 are used as
	/// digital pins
	/// \param pin The pin number (2-13)
	list <int> * getDigitalHistory(int pin);

	/// \brief Returns a pointer to the analog data history list for the given pin.
	/// \param pin The Arduino Uno pin: 0-5
	list <int> * getAnalogHistory(int pin);

	/// \returns a pointer to the SysEx history.
	list <vector <unsigned char> > * getSysExHistory();

	/// \returns a pointer to the string history.
	list <string> * getStringHistory();

	/// \brief Get the pin mode of the given pin
	///
	/// \returns `ARD_INPUT`, `ARD_OUTPUT`, `ARD_PWM`, `ARD_SERVO`, `ARD_ANALOG`
	int getDigitalPinMode(int pin) const;

	/// \returns `ARD_ON` or `ARD_OFF`
	int getAnalogPinReporting(int pin) const;

	/// \brief Useful for parsing SysEx messages
	int getValueFromTwo7bitBytes(unsigned char lsb, unsigned char msb);

	/// \}
	/// \name Events
	/// \{

	/// \brief Triggered when a digital pin changes value, the pin that
	/// changed is passed as an argument.
	ofEvent <const int> EDigitalPinChanged;

	/// \brief Triggered when an analog pin changes value, the pin that
	/// changed is passed as an argument.
	ofEvent <const int> EAnalogPinChanged;

	/// \brief Triggered when a SysEx message that isn't in the extended
	/// command set is received, the SysEx message is passed as an argument
	ofEvent <const vector <unsigned char> > ESysExReceived;

	/// \brief Triggered when a protocol version is received, the major version
	/// is passed as an argument.
	ofEvent <const int> EProtocolVersionReceived;

	/// \brief Triggered when a firmware version is received, the major version
	/// is passed as an argument.
	ofEvent <const int> EFirmwareVersionReceived;

	/// \brief Triggered when the firmware version is received upon connect,
	/// the major firmware version is passed as an argument. From this point
	/// it's safe to send to the Arduino.
	ofEvent <const int> EInitialized;

	/// \brief Triggered when a string is received, the string is passed as an
	/// argument
	ofEvent <const string> EStringReceived;

	/// \brief triggered when a stepper has finished rotating. Returns which 
	/// stepper has complted its rotation
	ofEvent<const Stepper_Data> EStepperDataRecieved;

	/// \brief triggered when the I2C bus returns data after a read request
	ofEvent<const I2C_Data> EI2CDataRecieved;

	/// \brief triggered when the encoder returns data after a read request
	ofEvent<const vector<Encoder_Data> > EEncoderDataRecieved;

	/// \}
	/// \name Servos
	/// \{

	/// \brief Send a value to a servo.
	///
	/// A servo has to be atached to the pin prior
	/// \param pin 9 or 10
	/// \param value The value to send
	void sendServo(int pin, int value, bool force = false);

	/// \param angle parameter DEPRECATED as of Firmata 2.2
	void sendServoAttach(int pin, int minPulse = 544, int maxPulse = 2400);

	/// \brief Detaches a servo on a pin
	/// \note sendServoDetach DEPRECATED as of Firmata 2.2
	//void sendServoDetach(int pin);

	/// \returns the last set servo value for a pin if the pin has a servo attached.
	int getServo(int pin) const;

	/// \}

	/// -- stepper
	void sendStepper2Wire(int dirPin, int stepPin, int stepsPerRev = 200);
	/// the pins has to have a stepper attached

	void sendStepper4Wire(int pin1, int pin2, int pin3, int pin4, int stepsPerRev = 200);
	/// the pins has to have a stepper attached

	void sendStepperMove(int stepperID, int direction, int steps, int speed = 0, float acceleration = 0, float deceleration = 0);
	/// set the steps to move


	void sendI2CConfig(int delay);
	bool isI2CConfigured();
	void sendI2CWriteRequest(char slaveAddress, unsigned char * bytes, int numOfBytes);
	void sendI2CWriteRequest(char slaveAddress, vector<char> bytes);
	void i2cWrite(char address, unsigned char * bytes, int numOfBytes);
	void i2cWriteReg(char address, int reg, int byte);
	void sendI2CReadRequest(char address, unsigned char numBytes);
	void i2cRead(char address, unsigned char reg, int bytesToRead);
	void i2cReadOnce(char address, unsigned char reg, int bytesToRead);

	void sendOneWireConfig(int pin, bool enableParasiticPower);
	void sendOneWireSearch(int pin);
	void sendOneWireAlarmsSearch(int pin);
	void sendOneWireSearch(char type, int pin);
	void sendOneWireRead(int pin, vector<unsigned char> devices, int numBytesToRead);
	void sendOneWireReset(int pin);
	void sendOneWireWrite(int pin, vector<unsigned char> devices, vector<unsigned char> data);
	void sendOneWireDelay(int pin, unsigned int delay);
	void sendOneWireWriteAndRead(int pin, vector<unsigned char> devices, vector<unsigned char> data, int numBytesToRead);
	void sendOneWireRequest(int pin, unsigned char subcommand, vector<unsigned char> devices, int numBytesToRead, unsigned char correlationId, unsigned int delay, vector<unsigned char> dataToWrite);

	void attachEncoder(int pinA, int pinB);
	void getEncoderPosition(int encoderNum);
	void getAllEncoderPositions();
	void resetEncoderPosition(int encoderNum);
	void enableEncoderReporting();
	void disableEncoderReporting();
	void detachEncoder(int encoderNum);

	map<int, supportedPinTypes> getPinCapabilities() { return pinCapabilities; }

private:
	mutable bool _initialized; ///\< \brief Indicate that pins are initialized.

	void initPins();
	mutable int _totalDigitalPins; ///\< \brief Indicate the total number of digital pins of the board in use.

	void sendDigitalPinReporting(int pin, int mode);
	// sets pin reporting to ARD_ON or ARD_OFF
	// enables / disables reporting for the pins port

	void sendDigitalPortReporting(int port, int mode);
	// sets port reporting to ARD_ON or ARD_OFF
	// enables / disables reporting for ports 0-2
	// port 0: pins 2-7  (0,1 are serial RX/TX)
	// port 1: pins 8-13 (14,15 are disabled for the crystal)
	// port 2: pins 16-21 analog pins used as digital, all analog reporting will be turned off if this is set to ARD_ON

	void purge();

	void processData(unsigned char inputData);
	void processDigitalPort(int port, unsigned char value);
	virtual void processSysExData(vector <unsigned char> data);

	ofSerial _port;
	int _portStatus;

	// --- history variables
	int _analogHistoryLength;
	int _digitalHistoryLength;
	int _stringHistoryLength;
	int _sysExHistoryLength;

	// --- data processing variables
	int _waitForData;
	int _executeMultiByteCommand; ///< \brief Indicate Firmata command to execute.
	int _multiByteChannel; ///< \brief Indicates which pin the data came from.

	// --- data holders
	unsigned char _storedInputData[FIRMATA_MAX_DATA_BYTES];
	vector <unsigned char> _sysExData;
	int _majorProtocolVersion;
	int _minorProtocolVersion;
	int _majorFirmwareVersion;
	int _minorFirmwareVersion;
	string _firmwareName;

	// sum of majorFirmwareVersion * 10 + minorFirmwareVersion -> Firmata (?)
	//int _firmwareVersionSum;

	list <vector <unsigned char> > _sysExHistory;
	// maintains a history of received sysEx messages (excluding SysEx messages in the extended command set)

	list <string> _stringHistory;
	// maintains a history of received strings

	//we dont know the number of pintypes until we do a configuration request so just a placeholder for now
	//mutable list <int> _analogHistory[ARD_TOTAL_ANALOG_PINS];
	mutable vector<list <int> > _analogHistory;
	// a history of received data for each analog pin

	//mutable list <int> _digitalHistory[ARD_TOTAL_DIGITAL_PINS];
	mutable vector<list <int> > _digitalHistory;
	// a history of received data for each digital pin

	//mutable int _digitalPinMode[ARD_TOTAL_DIGITAL_PINS];
	mutable vector<int> _digitalPinMode;
	// the modes for all digital pins

	//mutable int _digitalPinValue[ARD_TOTAL_DIGITAL_PINS];
	mutable vector<int> _digitalPinValue;
	// the last set values (DIGITAL/PWM) on all digital pins

	mutable int _digitalPortValue[ARD_TOTAL_PORTS];
	// the last set values on all ports

	mutable int _digitalPortReporting[ARD_TOTAL_PORTS];
	// whether pin reporting is enabled / disabled

	//mutable int _digitalPinReporting[ARD_TOTAL_DIGITAL_PINS];
	mutable vector<int> _digitalPinReporting;
	// whether pin reporting is enabled / disabled

	//mutable int _analogPinReporting[ARD_TOTAL_ANALOG_PINS];
	mutable vector<int> _analogPinReporting;
	// whether pin reporting is enabled / disabled

	bool bUseDelay;

	mutable bool connected; ///< \brief This yields true if a serial connection to Arduino exists.

	float connectTime; ///< \brief This represents the (running) time of establishing a serial connection.

	//mutable int _servoValue[ARD_TOTAL_DIGITAL_PINS];
	mutable vector<int> _servoValue;
	// the last set servo values

	bool _i2cConfigured;

	int _numSteppers;

	int _numEncoders;

	int _encoderID;

	int firstAnalogPin;

	bool firmataInputSupported,
		firmataOutputSupported,
		firmataAnalogSupported,
		firmataPwmSupported,
		firmataServoSupported,
		firmataI2cSupported,
		firmataOnewireSupported,
		firmataStepperSupported,
		firmataEncoderSupported;

	map<int, supportedPinTypes> pinCapabilities;
};

typedef ofArduino ofStandardFirmata;

