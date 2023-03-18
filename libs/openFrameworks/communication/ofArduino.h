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

#include "ofEvents.h"
#include "ofSerial.h"
#include "ofConstants.h"
#include <list>
#include <map>

 /* Version numbers for the protocol.  The protocol is still changing, so these
 * version numbers are important.  This number can be queried so that host
 * software can test whether it will be compatible with the currently
 * installed firmware. */
#define FIRMATA_MAJOR_VERSION	2 // for non-compatible changes
#define FIRMATA_MINOR_VERSION	5 // for backwards compatible changes
#define FIRMATA_BUGFIX_VERSION	0 // for bugfix releases

#define FIRMATA_MAX_DATA_BYTES	64 // max number of data bytes in incoming messages

 // message command bytes (128-255/0x80-0xFF)
#define DIGITAL_MESSAGE			0x90 // send data for a digital port (collection of 8 pins)
#define ANALOG_MESSAGE			0xE0 // send data for an analog pin (or PWM)
#define REPORT_ANALOG			0xC0 // enable analog input by pin #
#define REPORT_DIGITAL			0xD0 // enable digital input by port pair
 //
#define SET_PIN_MODE			0xF4 // set a pin to INPUT/OUTPUT/PWM/etc
#define SET_DIGITAL_PIN_VALUE   0xF5 // set value of an individual digital pin
 //
#define REPORT_VERSION			0xF9 // report protocol version
#define SYSTEM_RESET			0xFF // reset from MIDI
 //
#define START_SYSEX				0xF0 // start a MIDI Sysex message
#define END_SYSEX				0xF7 // end a MIDI Sysex message

 // extended command set using sysex (0-127/0x00-0x7F)
 /* 0x00-0x0F reserved for user-defined commands */
#define SERIAL_MESSAGE          0x60 // communicate with serial devices, including other boards
#define ENCODER_DATA            0x61 // reply with encoders current positions
#define SERVO_CONFIG            0x70 // set max angle, minPulse, maxPulse, freq
#define STRING_DATA             0x71 // a string message with 14-bits per char
#define STEPPER_DATA            0x72 // control a stepper motor
#define ONEWIRE_DATA            0x73 // send an OneWire read/write/reset/select/skip/search request
#define SHIFT_DATA              0x75 // a bitstream to/from a shift register
#define I2C_REQUEST             0x76 // send an I2C read/write request
#define I2C_REPLY               0x77 // a reply to an I2C read request
#define I2C_CONFIG              0x78 // config I2C settings such as delay times and power pins
#define EXTENDED_ANALOG         0x6F // analog write (PWM, Servo, etc) to any pin
#define PIN_STATE_QUERY         0x6D // ask for a pin's current mode and value
#define PIN_STATE_RESPONSE      0x6E // reply with pin's current mode and value
#define CAPABILITY_QUERY        0x6B // ask for supported modes and resolution of all pins
#define CAPABILITY_RESPONSE     0x6C // reply with supported modes and resolution
#define ANALOG_MAPPING_QUERY    0x69 // ask for mapping of analog to pin numbers
#define ANALOG_MAPPING_RESPONSE 0x6A // reply with mapping info
#define REPORT_FIRMWARE         0x79 // report name and version of the firmware
#define SAMPLING_INTERVAL       0x7A // set the poll rate of the main loop
#define SCHEDULER_DATA          0x7B // send a createtask/deletetask/addtotask/schedule/querytasks/querytask request to the scheduler
#define SYSEX_NON_REALTIME      0x7E // MIDI Reserved for non-realtime messages
#define SYSEX_REALTIME          0x7F // MIDI Reserved for realtime messages

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
#define ARD_SERIAL              0x0A // pin configured for serial communication
#define ARD_INPUT_PULLUP        0x0B // enable internal pull-up resistor for pin
#define ARD_IGNORE				0x7F // pin configured to be ignored by digitalWrite and capabilityResponse
#define TOTAL_PIN_MODES			13

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
#define I2C_END_TX_MASK					B01000000
#define I2C_STOP_TX						1
#define I2C_RESTART_TX					0
#define I2C_MAX_QUERIES					8
#define I2C_REGISTER_NOT_SPECIFIED		-1

//Encoder Subcommands
#define MAX_ENCODERS					5 // arbitrary value, may need to adjust
#define ENCODER_ATTACH					0x00
#define ENCODER_REPORT_POSITION			0x01
#define ENCODER_REPORT_POSITIONS		0x02
#define ENCODER_RESET_POSITION			0x03
#define ENCODER_REPORT_AUTO				0x04
#define ENCODER_DETACH					0x05
#define ENCODER_DIRECTION_MASK			0x40 // B01000000
#define ENCODER_CHANNEL_MASK			0x3F // B00111111

//Serial Subcommands
#define SERIAL_CONFIG					0x10
#define SERIAL_WRITE					0x20
#define SERIAL_READ						0x30
#define SERIAL_REPLY					0x40
#define SERIAL_CLOSE					0x50
#define SERIAL_FLUSH					0x60
#define SERIAL_LISTEN					0x70

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
	bool	serialSupported = false;
	bool	onewireSupported = false;
	bool	stepperSupported = false;
	bool	encoderSupported = false;
};

enum Firmata_Pin_Modes {
	MODE_INPUT,
	MODE_OUTPUT,
	MODE_INPUT_PULLUP,
	MODE_ANALOG,
	MODE_PWM,
	MODE_SERVO,
	MODE_I2C,
	MODE_SERIAL,
	MODE_ONEWIRE,
	MODE_STEPPER,
	MODE_ENCODER
};

enum Firmata_Stepper_Interface {
	FIRMATA_STEPPER_DRIVER = 1,
	FIRMATA_STEPPER_TWO_WIRE = 2,
	FIRMATA_STEPPER_FOUR_WIRE = 3
};

enum Firmata_Stepper_Direction {
	FIRMATA_STEPPER_CCW = 0,
	FIRMATA_STEPPER_CW = 1
};
enum Firmata_I2C_Modes {
	FIRMATA_I2C_WRITE = 0x00,
	FIRMATA_I2C_READ = 1,
	FIRMATA_I2C_CONTINUOUS_READ = 2,
	FIRMATA_I2C_STOP_READING = 3
};

struct Firmata_I2C_Data {
	int		address;
	int		reg;
	std::string	data;
};

struct Firmata_Encoder_Data {
	int		ID;
	bool	direction;
	int		position;
};

struct Firmata_Stepper_Data {
	int		id;
	int		type;
	int		data;
};

enum Firmata_Serial_Modes {
	FIRMATA_SERIAL_READ_CONTINUOUS = 0x00,
	FIRMATA_SERIAL_STOP_READING = 0x01
};

/// ids for hardware and software serial ports on the board
enum Firmata_Serial_Ports {
	HW_SERIAL0 = 0x00,
	HW_SERIAL1 = 0x01,
	HW_SERIAL2 = 0x02,
	HW_SERIAL3 = 0x03,
	SW_SERIAL0 = 0x08,
	SW_SERIAL1 = 0x09,
	SW_SERIAL2 = 0x10,
	SW_SERIAL3 = 0x11
};

struct Firmata_Serial_Data {
	Firmata_Serial_Ports	portID;
	std::string			data;
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

	void sendSysEx(int command, std::vector <unsigned char> data);

	bool isAttached();



	/// \brief Send a string to the Arduino
	/// \note Firmata can not handle strings longer than 12 characters.
	void sendString(std::string str);

	void sendProtocolVersionRequest();

	void sendFirmwareVersionRequest();

	void sendPinCofigurationRequest();

	void sendPinCapabilityRequest();

	void sendAnalogMappingRequest();

	void sendPinStateQuery(int pin);

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
	std::vector <unsigned char> getSysEx() const;

	/// \returns the last received string.
	std::string getString() const;

	/// \returns the major firmware version.
	int getMajorFirmwareVersion() const;

	/// \returns the minor firmware version.
	int getMinorFirmwareVersion() const;

	/// \returns the name of the firmware.
	std::string getFirmwareName() const;

	/// \brief Returns a pointer to the digital data history list for the
	/// given pin
	/// \note Pin 16-21 can also be used if analog inputs 0-5 are used as
	/// digital pins
	/// \param pin The pin number (2-13)
	std::list <int> * getDigitalHistory(int pin);

	/// \brief Returns a pointer to the analog data history list for the given pin.
	/// \param pin The Arduino Uno pin: 0-5
	std::list <int> * getAnalogHistory(int pin);

	/// \returns a pointer to the SysEx history.
	std::list <std::vector <unsigned char> > * getSysExHistory();

	/// \returns a pointer to the string history.
	std::list <std::string> * getStringHistory();

	/// \brief Get the pin mode of the given pin
	///
	/// \returns `ARD_INPUT`, `ARD_OUTPUT`, `ARD_PWM`, `ARD_SERVO`, `ARD_ANALOG`
	int getDigitalPinMode(int pin) const;

	/// \returns `ARD_ON` or `ARD_OFF`
	int getAnalogPinReporting(int pin) const;

	/// \brief Useful for parsing SysEx messages
	int getValueFromTwo7bitBytes(unsigned char lsb, unsigned char msb);

	int getInvertedValueFromTwo7bitBytes(unsigned char lsb, unsigned char msb);

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
	ofEvent <const std::vector <unsigned char> > ESysExReceived;

	/// \brief Triggered when a firmware version is received, the major version
	/// is passed as an argument.
	ofEvent <const int> EFirmwareVersionReceived;

	/// \brief Triggered when the firmware version is received upon connect,
	/// the major firmware version is passed as an argument. From this point
	/// it's safe to send to the Arduino.
	ofEvent <const int> EInitialized;

	/// \brief Triggered when a string is received, the string is passed as an
	/// argument
	ofEvent <const std::string> EStringReceived;

	/// \brief triggered when a stepper has finished rotating. Returns which 
	/// stepper has complted its rotation
	ofEvent<const Firmata_Stepper_Data> EStepperDataReceived;

	/// \brief triggered when the I2C bus returns data after a read request
	ofEvent<const Firmata_I2C_Data> EI2CDataRecieved;

	/// \brief triggered when the encoder returns data after a read request
	ofEvent<const std::vector<Firmata_Encoder_Data> > EEncoderDataReceived;

	/// \brief triggered when a Serial message is received. Returns which 
	/// port and its data
	ofEvent<const Firmata_Serial_Data> ESerialDataReceived;

	ofEvent<const std::pair<int, Firmata_Pin_Modes> > EPinStateResponseReceived;

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
	/// \name Stepper
	/// \{

	/// -- stepper
	void sendStepper2Wire(int dirPin, int stepPin, int stepsPerRev = 200);
	/// the pins has to have a stepper attached

	void sendStepper4Wire(int pin1, int pin2, int pin3, int pin4, int stepsPerRev = 200);
	/// the pins has to have a stepper attached

	void sendStepperMove(int stepperID, int direction, int steps, int speed = 0, float acceleration = 0, float deceleration = 0);
	/// set the steps to move


	/// \}
	/// \name I2C
	/// \{

	/// \brief Sends a I2C config request
	///
	///  Must be called before an I2C Read or Write
	/// \param {number} delay in microseconds to set for I2C Read
	void sendI2CConfig(int delay);
	bool isI2CConfigured();

	/// \brief Asks the arduino to send an I2C request to a device
	///
	/// \param {number} slaveAddress The address of the I2C device
	/// \param {Array} bytes The bytes to send to the device
	void sendI2CWriteRequest(char slaveAddress, unsigned char * bytes, int numOfBytes, int reg = -1);
	void sendI2CWriteRequest(char slaveAddress, const char * bytes, int numOfBytes, int reg = -1);
	void sendI2CWriteRequest(char slaveAddress, char * bytes, int numOfBytes, int reg = -1);
	void sendI2CWriteRequest(char slaveAddress, std::vector<char> bytes, int reg = -1);

	/// \brief Asks the arduino to request bytes from an I2C device
	///
	/// \param {number} slaveAddress The address of the I2C device
	/// \param {number} numBytes The number of bytes to receive.
	/// \param {function} callback A function to call when we have received the bytes.
	void sendI2CReadRequest(char address, int numBytes, int reg = -1);

	/// \brief Initialize a continuous I2C read.
	///
	/// \param {number} address    The address of the I2C device
	/// \param {number} register   Optionally set the register to read from.
	/// \param {number} numBytes   The number of bytes to receive.
	void sendI2ContinuousReadRequest(char address, int numBytes, int reg = -1);

	/// \}
	/// \name OneWire
	/// \{

	/// \brief Configure the passed pin as the controller in a 1-wire bus.
	///
	/// Pass as enableParasiticPower true if you want the data pin to power the bus.
	/// \param pin
	/// \param enableParasiticPower
	///
	void sendOneWireConfig(int pin, bool enableParasiticPower);

	/// \brief Searches for 1-wire devices on the bus.  
	///
	/// \param pin
	void sendOneWireSearch(int pin);

	/// \brief Searches for 1-wire devices on the bus in an alarmed state.  
	///
	/// \param pin
	void sendOneWireAlarmsSearch(int pin);
	void sendOneWireSearch(char type, int pin);

	/// \brief Reads data from a device on the bus.
	///
	/// N.b. ConfigurableFirmata will issue the 1-wire select command internally.
	/// \param pin
	/// \param device
	/// \param numBytesToRead
	/// \param callback
	void sendOneWireRead(int pin, std::vector<unsigned char> devices, int numBytesToRead);
	
	/// \brief Resets all devices on the bus.
	/// \param pin
	void sendOneWireReset(int pin);

	/// \brief Writes data to the bus to be received by the passed device.  
	///
	/// The device should be obtained from a previous call to sendOneWireSearch. ConfigurableFirmata will issue the 1-wire select command internally.
	/// \param pin
	/// \param device
	/// \param data
	void sendOneWireWrite(int pin, std::vector<unsigned char> devices, std::vector<unsigned char> data);

	/// \brief Tells firmata to not do anything for the passed amount of ms.
	/// 
	/// For when you need to give a device attached to the bus time to do a calculation.
	/// \param pin
	void sendOneWireDelay(int pin, unsigned int delay);

	/// \brief Sends the passed data to the passed device on the bus, reads the specified number of bytes.
	///
	/// ConfigurableFirmata will issue the 1-wire select command internally.
	/// \param pin
	/// \param device
	/// \param data
	/// \param numBytesToRead
	/// \param callback
	void sendOneWireWriteAndRead(int pin, std::vector<unsigned char> devices, std::vector<unsigned char> data, int numBytesToRead);
	void sendOneWireRequest(int pin, unsigned char subcommand, std::vector<unsigned char> devices, int numBytesToRead, unsigned char correlationId, unsigned int delay, std::vector<unsigned char> dataToWrite);

	/// \}
	/// \name Encoder
	/// \{

	void attachEncoder(int pinA, int pinB);
	void getEncoderPosition(int encoderNum);
	void getAllEncoderPositions();
	void resetEncoderPosition(int encoderNum);
	void enableEncoderReporting();
	void disableEncoderReporting();
	void detachEncoder(int encoderNum);

	/// \}
	/// \name Serial
	/// \{

	/// \brief Asks the Arduino to configure a hardware or serial port.
	///
	/// \param portID The serial port to use (HW_SERIAL1, HW_SERIAL2, HW_SERIAL3, SW_SERIAL0, SW_SERIAL1, SW_SERIAL2, SW_SERIAL3)
	/// \param baud  The baud rate of the serial port
	/// \param rxPin [SW Serial only] The RX pin of the SoftwareSerial instance
	/// \param txPin [SW Serial only] The TX pin of the SoftwareSerial instance
	void sendSerialConfig(Firmata_Serial_Ports portID, int baud, int rxPin, int txPin);

	/// \brief Write an array of bytes to the specified serial port.
	///
	/// \param portId The serial port to write to.
	/// \param bytes An array of bytes to write to the serial port.
	/// \param numOfBytes length of the array of bytes.
	void serialWrite(Firmata_Serial_Ports port, unsigned char * bytes, int numOfBytes);

	/// \brief  Start continuous reading of the specified serial port.
	///
	/// The port is checked for data each iteration of the main Arduino loop.
	/// \param portId The serial port to start reading continuously.
	/// \param  maxBytesToRead [Optional] The maximum number of bytes to read per iteration.
	/// \note If there are less bytes in the buffer, the lesser number of bytes will be returned. A value of 0 indicates that all available bytes in the buffer should be read.
	void serialRead(Firmata_Serial_Ports port, int maxBytesToRead);

	/// \brief Stop continuous reading of the specified serial port.
	///
	/// This does not close the port, it stops reading it but keeps the port open.
	/// \param portId The serial port to stop reading.
	void serialStop(Firmata_Serial_Ports portID);

	/// \brief  Close the specified serial port.
	///
	/// \param portId The serial port to close.
	void serialClose(Firmata_Serial_Ports portID);

	/// \brief Flush the specified serial port.
	///
	/// For hardware serial, this waits for the transmission of outgoing serial data to complete.For software serial, this removed any buffered incoming serial data.
	/// \param portId The serial port to listen on.
	void serialFlush(Firmata_Serial_Ports portID);

	/// \brief For SoftwareSerial only. Only a single SoftwareSerial instance can read data at a time.
	///
	/// Call this method to set this port to be the reading port in the case there are multiple SoftwareSerial instances.
	/// \param portId The serial port to flush.
	void serialListen(Firmata_Serial_Ports portID);


	std::map<int, supportedPinTypes> getPinCapabilities() { return pinCapabilities; }

	int getTotalPins() { return _totalDigitalPins; }

	int getNumAnalogPins() { return _totalAnalogPins; }

private:
	mutable bool _initialized; ///\< \brief Indicate that pins are initialized.

	void initPins();
	mutable int _totalDigitalPins; ///\< \brief Indicate the total number of digital pins of the board in use.
	mutable int _totalAnalogPins;

	void sendDigitalPinReporting(int pin, int mode);

	void sendDigitalPortReporting(int port, int mode);

	void purge();

	void processData(unsigned char inputData);
	void processDigitalPort(int port, unsigned char value);
	virtual void processSysExData(std::vector <unsigned char> data);

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
	std::vector <unsigned char> _sysExData;
	int _majorFirmwareVersion;
	int _minorFirmwareVersion;
	std::string _firmwareName;

	std::list <std::vector <unsigned char> > _sysExHistory;
	// maintains a history of received sysEx messages (excluding SysEx messages in the extended command set)

	std::list <std::string> _stringHistory;
	// maintains a history of received strings

	//we dont know the number of pintypes until we do a configuration request so just a placeholder for now
	mutable std::vector<std::list <int> > _analogHistory;
	// a history of received data for each analog pin

	mutable std::vector<std::list <int> > _digitalHistory;
	// a history of received data for each digital pin

	mutable std::vector<int> _digitalPinMode;
	// the modes for all digital pins

	mutable std::vector<int> _digitalPinValue;
	// the last set values (DIGITAL/PWM) on all digital pins

	mutable int _digitalPortValue[ARD_TOTAL_PORTS];
	// the last set values on all ports

	mutable int _digitalPortReporting[ARD_TOTAL_PORTS];
	// whether pin reporting is enabled / disabled

	mutable std::vector<int> _digitalPinReporting;
	// whether pin reporting is enabled / disabled

	mutable std::vector<int> _analogPinReporting;
	// whether pin reporting is enabled / disabled

	bool bUseDelay;

	mutable bool connected; ///< \brief This yields true if a serial connection to Arduino exists.

	float connectTime; ///< \brief This represents the (running) time of establishing a serial connection.

	mutable std::vector<int> _servoValue;
	// the last set servo values

	bool _i2cConfigured;

	int _numSteppers;

	// int _numEncoders;

	int _encoderID;

	int _firstAnalogPin;

	bool firmataInputSupported,
		firmataOutputSupported,
		firmataAnalogSupported,
		firmataPwmSupported,
		firmataServoSupported,
		firmataI2cSupported,
		firmataOnewireSupported,
		firmataStepperSupported,
		firmataEncoderSupported,
		firmataSerialSupported;

	mutable std::map<int, supportedPinTypes> pinCapabilities;
	mutable std::map<int, int> analogPinMap;


	bool isAnalogPin(int pin) const;
	bool isPin(int pin) const;

	int convertAnalogPinToDigital(size_t pin) const;
	int convertDigitalPinToAnalog(size_t pin) const;
};

typedef ofArduino ofStandardFirmata;

