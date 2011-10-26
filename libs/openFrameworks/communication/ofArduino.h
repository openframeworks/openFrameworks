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
#include <vector>
#include <string>
#include <iostream>

#include "ofEvents.h"

#include "ofSerial.h"

/*
 * Version numbers for the protocol. The protocol is still changing, so these
 * version numbers are important. This number can be queried so that host
 * software can test whether it will be compatible with the currently installed firmware.
 */

#define FIRMATA_MAJOR_VERSION                           2 // for non-compatible changes
#define FIRMATA_MINOR_VERSION                           0 // for backwards compatible changes
#define FIRMATA_MAX_DATA_BYTES                          32 // max number of data bytes in non-Sysex messages
// message command bytes (128-255/0x80-0xFF)
#define FIRMATA_DIGITAL_MESSAGE                         0x90 // send data for a digital pin
#define FIRMATA_ANALOG_MESSAGE                          0xE0 // send data for an analog pin (or PWM)
#define FIRMATA_REPORT_ANALOG                           0xC0 // enable analog input by pin #
#define FIRMATA_REPORT_DIGITAL                          0xD0 // enable digital input by port pair
//
#define FIRMATA_SET_PIN_MODE                            0xF4 // set a pin to INPUT/OUTPUT/PWM/etc
//
#define FIRMATA_REPORT_VERSION                          0xF9 // report protocol version
#define FIRMATA_SYSTEM_RESET                            0xFF // reset from MIDI
//
#define FIRMATA_START_SYSEX                             0xF0 // start a MIDI Sysex message
#define FIRMATA_END_SYSEX                               0xF7 // end a MIDI Sysex message
// pin modes
#define FIRMATA_INPUT                                   0x00
#define FIRMATA_OUTPUT                                  0x01
#define FIRMATA_ANALOG                                  0x02 // analog pin in analogInput mode
#define FIRMATA_PWM                                     0x03 // digital pin in PWM output mode
#define FIRMATA_SERVO                                   0x04 // digital pin in Servo output mode
#define SHIFT											0x05 // shiftIn/shiftOut mode
#define I2C												0x06 // pin included in I2C setup
#define TOTAL_PIN_MODES 7 
// extended command set using SysEx (0-127/0x00-0x7F)
/* 0x00-0x0F reserved for custom commands */
#define FIRMATA_SYSEX_SERVO_CONFIG                      0x70 // set max angle, minPulse, maxPulse, freq
#define FIRMATA_SYSEX_FIRMATA_STRING					0x71 // a string message with 14-bits per char
#define SHIFT_DATA										0x75 // a bitstram to/from a shift register
#define I2C_REQUEST										0x76 // send an I2C read/write request
#define I2C_REPLY										0x77 // a reply to an I2C request
#define I2C_CONFIG										0x78 // config I2C settings such as delay times and power pins
#define EXTENDED_ANALOG									0x6F // analog write (PWM, Servo, etc) to any pin
#define PIN_STATE_QUERY									0x6D // ask for a pin's current mode and value
#define PIN_STATE_RESPONSE								0x6E // reply with pin's current mode and value
#define CAPABILITY_QUERY								0x6B // ask for supported modes and resolution of all pins
#define CAPABILITY_RESPONSE								0x6C // reply with supported modes and resolution
#define ANALOG_MAPPING_QUERY							0x69 // ask for mapping of analog to pin numbers
#define ANALOG_MAPPING_RESPONSE							0x6A // reply with mapping info
#define FIRMATA_SYSEX_REPORT_FIRMWARE					0x79 // report name and version of the firmware
#define SAMPLING_INTERVAL								0x7A // set the poll rate of the main loop 
#define FIRMATA_SYSEX_NON_REALTIME                      0x7E // MIDI Reserved for non-realtime messages
#define FIRMATA_SYSEX_REALTIME                          0x7F // MIDI Reserved for realtime messages

// ---- arduino constants (for Arduino NG and Diecimila)

// board settings
#define ARD_TOTAL_DIGITAL_PINS							22 // total number of pins currently supported
#define ARD_TOTAL_ANALOG_PINS							6
#define ARD_TOTAL_PORTS                                 3 // total number of ports for the board
// pin modes
#define ARD_INPUT                                       0x00
#define ARD_OUTPUT                                      0x01
#define ARD_ANALOG                                      0x02 // analog pin in analogInput mode
#define ARD_PWM                                         0x03 // digital pin in PWM output mode
#define ARD_SERVO                                       0x04 // digital pin in Servo output mode
#define ARD_HIGH                                        1
#define ARD_LOW                                         0
#define ARD_ON                                          1
#define ARD_OFF                                         0

/*
 #if defined(__AVR_ATmega168__)  // Arduino NG and Diecimila
 #define ARD_TOTAL_ANALOG_PINS       8
 #define ARD_TOTAL_DIGITAL_PINS      22 // 14 digital + 8 analog
 #define ARD_TOTAL_PORTS             3 // total number of ports for the board
 #define ARD_ANALOG_PORT             2 // port# of analog used as digital
 #elif defined(__AVR_ATmega8__)  // old Arduinos
 #define ARD_TOTAL_ANALOG_PINS       6
 #define ARD_TOTAL_DIGITAL_PINS      20 // 14 digital + 6 analog
 #define ARD_TOTAL_PORTS             3  // total number of ports for the board
 #define ARD_ANALOG_PORT             2  // port# of analog used as digital
 #elif defined(__AVR_ATmega128__)// Wiring
 #define ARD_TOTAL_ANALOG_PINS       8
 #define ARD_TOTAL_DIGITAL_PINS      43
 #define ARD_TOTAL_PORTS             5 // total number of ports for the board
 #define ARD_ANALOG_PORT             2 // port# of analog used as digital
 #else // anything else
 #define ARD_TOTAL_ANALOG_PINS       6
 #define ARD_TOTAL_DIGITAL_PINS      14
 #define ARD_TOTAL_PORTS             3 // total number of ports for the board
 #define ARD_ANALOG_PORT             2 // port# of analog used as digital
 #endif
 */

// DEPRECATED as of firmata v2.2
#define SYSEX_SERVO_ATTACH                      0x00
#define SYSEX_SERVO_DETACH                      0x01
#define SYSEX_SERVO_WRITE                       0x02

#define OF_ARDUINO_DELAY_LENGTH					4.0

#define FIRMWARE2_2								22
#define FIRMWARE2_3                             23


/**
        This class extend ofStandardFirmata and provides additional functionality like servo support through SysEx messages.
		use the OFstdFirmata for servo support...

**/


class ofArduino{

        public:
                ofArduino();

                virtual ~ofArduino();


                // --- setup functions
				bool connect(string device, int baud = 57600);
				// opens a serial port connection to the arduino

				void disconnect();
				// closes the serial port connection

				bool isArduinoReady();

				void  setUseDelay(bool bDelay);

				void update();
				// polls data from the serial port, this has to be called periodically

				bool isInitialized();
				// returns true if a succesfull connection has been established and the Arduino has reported a firmware

				void setDigitalHistoryLength(int length);
				void setAnalogHistoryLength(int length);
				void setStringHistoryLength(int length);
				void setSysExHistoryLength(int nSysEx);

				// --- senders

				void sendDigitalPinMode(int pin, int mode);
				// pin: 2-13
				// mode: ARD_INPUT, ARD_OUTPUT, ARD_PWM
				// setting a pins mode to ARD_INPUT turns on reporting for the port the pin is on
				// Note: analog pins 0-5 can be used as digitial pins 16-21 but if the mode of _one_ of these pins is set to ARD_INPUT then _all_ analog pin reporting will be turned off

				void sendAnalogPinReporting(int pin, int mode);
				// pin: 0-5
				// mode: ARD_ON or ARD_OFF
				// Note: analog pins 0-5 can be used as digitial pins 16-21 but if reporting for _one_ analog pin is enabled then reporting for _all_ of digital pin 16-21 will be turned off

				void sendDigital(int pin, int value, bool force = false);
				// pin: 2-13
				// value: ARD_LOW or ARD_HIGH
				// the pins mode has to be set to ARD_OUTPUT or ARD_INPUT (in the latter mode pull-up resistors are enabled/disabled)
				// Note: pin 16-21 can also be used if analog inputs 0-5 are used as digital pins

				void sendPwm(int pin, int value, bool force = false);
				// pin: 3, 5, 6, 9, 10 and 11
				// value: 0 (always off) to 255 (always on).
				// the pins mode has to be set to ARD_PWM
				// TODO check if the PWM bug still is there causing frequent digital port reporting...

				void sendSysEx(int command, vector<unsigned char> data);

				void sendString(string str);
				// firmata can not handle strings longer than 12 characters.

				void sendProtocolVersionRequest();

				void sendFirmwareVersionRequest();

				void sendReset();

				// --- senders for SysEx communication

				void sendSysExBegin();
				// sends the FIRMATA_START_SYSEX command

				void sendSysExEnd();
				// sends the FIRMATA_END_SYSEX command

				void sendByte(unsigned char byte);
				// sends a byte without wrapping it in a firmata message, data has to be in the 0-127 range,
				// values > 127 will be interpreted as commands.

				void sendValueAsTwo7bitBytes(int value);
				// sends a value as two 7-bit bytes without wrapping it in a firmata message
				// values in the range 0 - 16384 will be sent as two bytes within the 0-127 data range.

				// --- getters

				int getPwm(int pin);
				// pin: 3, 5, 6, 9, 10 and 11
				// returns the last set PWM value (0-255) for the given pin
				// the pins mode has to be ARD_PWM
				// Note: pin 16-21 can also be used if analog inputs 0-5 are used as digital pins

				int getDigital(int pin);
				// pin: 2-13
				// returns the last received value (if the pin mode is ARD_INPUT) or the last set value (if the pin mode is ARD_OUTPUT) for the given pin
				// Note: pin 16-21 can also be used if analog inputs 0-5 are used as digital pins

				int getAnalog(int pin);
				// pin: 0-5
				// returns the last received analog value (0-1023) for the given pin

				vector<unsigned char> getSysEx();
				// returns the last received SysEx message

				string getString();
				// returns the last received string

				int getMajorProtocolVersion();
				// returns the major firmware version

				int getMinorProtocolVersion();
				// returns the minor firmware version

				int getMajorFirmwareVersion();
				// returns the major firmware version

				int getMinorFirmwareVersion();
				// returns the minor firmware version

				string getFirmwareName();
				// returns the name of the firmware

				list<int>* getDigitalHistory(int pin);
				// pin: 2-13
				// returns a pointer to the digital data history list for the given pin
				// Note: pin 16-21 can also be used if analog inputs 0-5 are used as digital pins

				list<int>* getAnalogHistory(int pin);
				// pin: 0-5
				// returns a pointer to the analog data history list for the given pin

				list<vector<unsigned char> >* getSysExHistory();
				// returns a pointer to the SysEx history

				list<string>* getStringHistory();
				// returns a pointer to the string history

				int getDigitalPinMode(int pin);
				// returns ARD_INPUT, ARD_OUTPUT, ARD_PWM, ARD_SERVO, ARD_ANALOG

				int getAnalogPinReporting(int pin);
				// returns ARD_ON, ARD_OFF

				int getValueFromTwo7bitBytes(unsigned char lsb, unsigned char msb);
				// useful for parsing SysEx messages

				// --- events

				ofEvent<const int> EDigitalPinChanged;
				// triggered when a digital pin changes value, the pin that changed is passed as an argument

				ofEvent<const int> EAnalogPinChanged;
				// triggered when an analog pin changes value, the pin that changed is passed as an argument

				ofEvent<const vector<unsigned char> > ESysExReceived;
				// triggered when a SysEx message that isn't in the extended command set is received, the SysEx message is passed as an argument

				ofEvent<const int> EProtocolVersionReceived;
				// triggered when a protocol version is received, the major version is passed as an argument

				ofEvent<const int> EFirmwareVersionReceived;
				// triggered when a firmware version is received, the major version is passed as an argument

				ofEvent<const int> EInitialized;
				// triggered when the firmware version is received upon connect, the major firmware version is passed as an argument
				// from this point it's safe to send to the Arduino.

				ofEvent<const string> EStringReceived;
				// triggered when a string is received, the string is passed as an argument




				// -- servo
			    void sendServo(int pin, int value, bool force=false);
                // pin: 9, 10
				// the pin has to have a servo attached

				// angle parameter DEPRECATED as of Firmata 2.2
                void sendServoAttach(int pin, int minPulse=544, int maxPulse=2400, int angle=180);
				// pin: 9, 10
                // attaches a servo to a pin

				// sendServoDetach DEPRECATED as of Firmata 2.2
                void sendServoDetach(int pin);
				// pin: 9, 10
                // detaches a servo from a pin, the pin mode remains as OUTPUT

				int getServo(int pin);
				// returns the last set servo value for a pin if the pin has a servo attached

		protected:
				bool _initialized;
    
                void initPins();
                int _totalDigitalPins;

				void sendDigitalPinReporting(int pin, int mode);
				// sets pin reporting to ARD_ON or ARD_OFF
				// enables / disables reporting for the pins port

				void sendDigitalPortReporting(int port, int mode);
				// sets port reporting to ARD_ON or ARD_OFF
				// enables / disables reporting for ports 0-2
				// port 0: pins 2-7  (0,1 are serial RX/TX)
				// port 1: pins 8-13 (14,15 are disabled for the crystal)
				// port 2: pins 16-21 analog pins used as digital, all analog reporting will be turned off if this is set to ARD_ON

				void processData(unsigned char inputData);
				void processDigitalPort(int port, unsigned char value);
				virtual void processSysExData(vector<unsigned char> data);

				ofSerial _port;
				int _portStatus;

				// --- history variables
				int _analogHistoryLength;
				int _digitalHistoryLength;
				int _stringHistoryLength;
				int _sysExHistoryLength;

				// --- data processing variables
				int _waitForData;
				int _executeMultiByteCommand;
				int _multiByteChannel; // indicates which pin data came from

				// --- data holders
				unsigned char _storedInputData[FIRMATA_MAX_DATA_BYTES];
				vector<unsigned char> _sysExData;
				int _majorProtocolVersion;
				int _minorProtocolVersion;
				int _majorFirmwareVersion;
				int _minorFirmwareVersion;
				string _firmwareName;
	
				// sum of majorFirmwareVersion * 10 + minorFirmwareVersion
				int _firmwareVersionSum;

				list<vector<unsigned char> > _sysExHistory;
				// maintains a history of received sysEx messages (excluding SysEx messages in the extended command set)

				list<string> _stringHistory;
				// maintains a history of received strings

				list<int> _analogHistory[ARD_TOTAL_ANALOG_PINS];
				// a history of received data for each analog pin

				list<int> _digitalHistory[ARD_TOTAL_DIGITAL_PINS];
				// a history of received data for each digital pin

				int _digitalPinMode[ARD_TOTAL_DIGITAL_PINS];
				// the modes for all digital pins

				int _digitalPinValue[ARD_TOTAL_DIGITAL_PINS];
				// the last set values (DIGITAL/PWM) on all digital pins

				int _digitalPortValue[ARD_TOTAL_PORTS];
				// the last set values on all ports

				int _digitalPortReporting[ARD_TOTAL_PORTS];
				// whether pin reporting is enabled / disabled

				int _digitalPinReporting[ARD_TOTAL_DIGITAL_PINS];
				// whether pin reporting is enabled / disabled

				int _analogPinReporting[ARD_TOTAL_ANALOG_PINS];
				// whether pin reporting is enabled / disabled

				bool bUseDelay;

				bool connected;

				float connectTime;

				int _servoValue[ARD_TOTAL_DIGITAL_PINS];
                // the last set servo values

};

typedef ofArduino ofStandardFirmata;

