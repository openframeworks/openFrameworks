#pragma once

#include <climits>
#include "ofConstants.h"

class ofBuffer;

#if defined( TARGET_OSX ) || defined( TARGET_LINUX ) || defined (TARGET_ANDROID)
	#include <termios.h>
#else
	#include <winbase.h>
	#include <tchar.h>
	#include <iostream>
	#include <string.h>
	#include <devpropdef.h>
	#include <setupapi.h>
	#include <regstr.h>
	/// \cond INTERNAL
	#define MAX_SERIAL_PORTS 256
	/// \endcond
	#include <winioctl.h>
	/*#ifndef _MSC_VER
		#define INITGUID
		#include <initguid.h> // needed for dev-c++ & DEFINE_GUID
	#endif*/
#endif

// serial error codes
#define OF_SERIAL_NO_DATA 	-2
#define OF_SERIAL_ERROR		-1



/// \brief Describes a Serial device, including ID, name and path.
class ofSerialDeviceInfo{
	friend class ofSerial;

	public:
		/// \brief Construct an ofSerialDeviceInfo with parameters.
		/// \param devicePathIn The path to the device.
		/// \param deviceNameIn The name of the device.
		/// \param deviceIDIn The ID of the device.
		ofSerialDeviceInfo(std::string devicePathIn, std::string deviceNameIn, int deviceIDIn){
			devicePath = devicePathIn;
			deviceName = deviceNameIn;
			deviceID = deviceIDIn;
		}

		/// \brief Construct an undefined serial device.
		ofSerialDeviceInfo(){
			deviceName = "device undefined";
			deviceID   = -1;
		}

		/// \brief Gets the path to the device
		///
		/// Example: `/dev/tty.cu/usbdevice-a440`.
		///
		/// \returns the device path.
		std::string getDevicePath(){
			return devicePath;
		}

		/// \brief Gets the name of the device
		///
		/// Example: `usbdevice-a440` or `COM4`.
		///
		/// \returns the device name.
		std::string getDeviceName(){
			return deviceName;
		}

		/// \brief Gets the ID of the device
		///
		/// Example: `0`,`1`,`2`,`3` etc.
		///
		/// \returns the device ID.
		int getDeviceID(){
			return deviceID;
		}

	protected:
		/// \cond INTERNAL

		/// \brief The device path (e.g /dev/tty.cu/usbdevice-a440).
		std::string devicePath;

		/// \brief The device name (e.g. usbdevice-a440 / COM4).
		std::string deviceName;

		/// \brief The device ID (e.g. 0, 1, 2, 3, etc).
		int deviceID;

		/// \endcond
};

/// \brief ofSerial provides a cross platform system for interfacing with the
/// serial port. You can choose the port and baud rate, and then read and send
/// data. Please note that the port must be set manually in the code, so you
/// should be clear what port your device is on. For example, Arduino users should
/// check the arduino app to see what port their device is on. Alternatively the
/// ofSerial class can attempt to communicate with the first available device it
/// finds.
///
/// To start up a serial connection to another device you do the following:
///
/// ~~~~{.cpp}
/// serial.listDevices();
/// vector <ofSerialDeviceInfo> deviceList = serial.getDeviceList();
///
/// // Open the first device and talk to it at 57600 baud
/// serial.setup(0, 57600);
/// ~~~~
class ofSerial {

public:
	/// \name Constructor and Destructor
	/// \{

	/// Initializes the serial connection, but doesn't actually open the
	/// connection to any devices. You'll need to use the setup() method
	/// before doing that.
	ofSerial();

	virtual ~ofSerial();

	/// \}
	/// \name List Devices
	/// \{

	/// \brief This lists all the available serial devices to the console or
	/// standard output.
	///
	/// On OSX and Linux this will return all the devices listed in /dev tty
	/// and cu, so you might want to compare it against a list of devices that
	/// you're expecting if you want to use it to dynamically connect to a
	/// device.
	void listDevices();

	/// \brief Prints out the available serial devices.
	///
	/// On OSX and Linux it might list something like this:
	/// ~~~~
	/// device 0 - cu.modem
	/// device 1 - cu.USA19H181P1.1
	/// ~~~~
	///
	/// And on Windows, like:
	/// ~~~~
	/// device 0 - COM2
	/// device 1 - COM4
	/// ~~~~
	OF_DEPRECATED_MSG("Use listDevices() instead", void enumerateDevices());

	/// \brief Returns a vector of ofSerialDeviceInfo instances with the
	/// devicePath, deviceName, deviceID set.
	std::vector <ofSerialDeviceInfo> getDeviceList();

	/// \}
	/// \name Serial Connection
	/// \{

	/// \brief Attempts to setup the first available device at a baud rate of 9600.
	/// ~~~~{.cpp}
	/// ofSerial mySerial;
	/// if( mySerial.setup() ){
	///	 ofLog("serial is setup!");
	/// }
	/// ~~~~
	bool setup();

	/// \brief Opens the serial port, with the given name and baud rate.
	///
	/// On OSX and Linux, it might look like:
	/// ~~~~{.cpp}
	/// ofSerial mySerial;
	/// mySerial.setup("/dev/cu.USA19H181P1.1", 57600);
	/// ~~~~
	///
	/// On Windows, like:
	/// ~~~~{.cpp}
	/// ofSerial mySerial;
	/// mySerial.setup("COM4", 57600);
	/// ~~~~
	bool setup(std::string portName, int baudrate);

	/// \brief Opens the serial port based on the order in which is listed and
	/// sets the baud rate.
	///
	/// The code bellow would open the first serial device found by the system:
	/// ~~~~{.cpp}
	/// ofSerial mySerial;
	/// mySerial.setup(0, 9600);
	/// ~~~~
	bool setup(int deviceNumber, int baudrate);

	bool isInitialized() const;

	/// \brief Closes the connection to the serial device.
	void close();

	/// \}
	/// \name Read Data
	/// \{

	/// \brief The available method is useful when you want to know how many bytes
	/// are available in the serial port. For instance, if you only want to
	/// read when there are 8 bytes waiting for you, you would do:
	///
	/// ~~~~{.cpp}
	/// if(device.available() > 8) {
	///	 device.readBytes(buffer, 8);
	/// }
	/// ~~~~
	///
	/// This is useful when you know how long a complete message from a device
	/// is going to be.
	int available();

	/// \brief Reads 'length' bytes from the connected serial device.
	///
	/// In some cases it may read less than 'length' bytes, so for reliable
	/// reading of >1 bytes of data the return value must be checked against the
	/// number of bytes requested, and if fewer bytes than requested were read
	/// then the call must be tried again.
	///
	/// This function should only be called when Serial.available() is reporting
	/// >0 bytes available.
	///
	/// An example of how to reliably read 8 bytes:
	/// ~~~~{.cpp}
	/// // we want to read 8 bytes
	/// int bytesRequired = 8;
	/// unsigned char bytes[bytesRequired];
	/// int bytesRemaining = bytesRequired;
	/// // loop until we've read everything
	/// while ( bytesRemaining > 0 ){
	///	 // check for data
	///	 if ( serial.available() > 0 ){
	///		 // try to read - note offset into the bytes[] array, this is so
	///		 // that we don't overwrite the bytes we already have
	///		 int bytesArrayOffset = bytesRequired - bytesRemaining;
	///		 int result = serial.readBytes( &bytes[bytesArrayOffset], bytesRemaining );
	///
	///		 // check for error code
	///		 if ( result == OF_SERIAL_ERROR ){
	///			 // something bad happened
	///			 ofLog( OF_LOG_ERROR, "unrecoverable error reading from serial" );
	///			 break;
	///		 } else if ( result == OF_SERIAL_NO_DATA ){
	///			 // nothing was read, try again
	///		 } else {
	///			 // we read some data!
	///			 bytesRemaining -= result;
	///		 }
	///	 }
	/// }
	/// ~~~~
	///
	/// Be aware that the type of your buffer can only be unsigned char. If you're
	/// trying to receieve ints or signed chars over a serial connection you'll
	/// need to do some bit manipulation to correctly interpret that values.
	long readBytes(unsigned char * buffer, size_t length);
	long readBytes(char * buffer, size_t length);
	long readBytes(ofBuffer & buffer, size_t length);

	/// \brief Reads and returns a single byte from the requested device.
	///
	/// ~~~~{.cpp}
	/// ofSerial mySerial;
	/// mySerial.setup(0, 57600);
	///
	/// int myByte = mySerial.readByte();
	///
	/// if ( myByte == OF_SERIAL_NO_DATA ){
	///	 printf("no data was read");
	/// } else if ( myByte == OF_SERIAL_ERROR ){
	///	 printf("an error occurred");
	/// } else {
	///	 printf("myByte is %d", myByte);
	/// }
	/// ~~~~
	///
	/// \returns The single byte as integer. If there is no data it will return
	/// `OF_SERIAL_NO_DATA`, and on error it returns `OF_SERIAL_ERROR`
	int readByte();

	/// \}
	/// \name Write Data
	/// \{

	/// \brief This writes bytes into the serial buffer from the buffer pointer passed in
	///
	/// ~~~~{.cpp}
	/// unsigned char buf[3] = {'o', 'f', '!'};
	/// device.writeBytes(&buf[0], 3);
	/// ~~~~
	long writeBytes(const unsigned char * buffer, size_t length);
	long writeBytes(const char * buffer, size_t length);
	long writeBytes(const ofBuffer & buffer);

	/// \brief Writes a single byte to the connected serial device.
	///
	/// Check the return value to be sure the data was written.
	/// ~~~~{.cpp}
	/// ofSerial mySerial;
	/// mySerial.setup(0, 57600);
	/// unsigned char myByte = 225;
	/// bool byteWasWritten = mySerial.writeByte(myByte);
	/// if ( !byteWasWritten )
	///	 ofLog(OF_LOG_ERROR, "Byte was not written to serial port");
	/// ~~~~
	bool writeByte(unsigned char singleByte);
	bool writeByte(char singleByte);

	/// \}
	/// \name Clear Data
	/// \{

	/// \brief Clears data from one or both of the serial buffers.
	///
	/// Any data in the cleared buffers is discarded.
	/// \param flushIn If true then it clears the incoming data buffer
	/// \param flushOut If true then it clears the outgoing data buffer.
	void flush(bool flushIn = true, bool flushOut = true);

	/// \brief Drain is only available on OSX and Linux and is very similar to
	/// flush(), but blocks until all the data has been written to or read
	/// from the serial port.
	void drain();

	/// \}

protected:
	/// \brief Enumerate all devices attached to a serial port.
	///
	/// This method tries to collect basic information about all devices
	/// attached to a serial port.
	/// \see ofSerial::listDevices()
	/// \see enumerateWin32Ports()
	void buildDeviceList();

	std::string deviceType;  ///\< \brief Name of the device on the other end of the serial connection.
	std::vector <ofSerialDeviceInfo> devices;  ///\< This vector stores information about all serial devices found.
	bool bHaveEnumeratedDevices;  ///\< \brief Indicate having enumerated devices (serial ports) available.
	bool bInited;  ///\< \brief Indicate the successful initialization of the serial connection.

#ifdef TARGET_WIN32

	/// \brief Enumerate all serial ports on Microsoft Windows.
	///
	/// This method tries to collect basic information about all devices
	/// attached to a serial port on Microsoft Windows platforms.
	/// \see ofSerial::listDevices()
	/// \see ofSerial::buildDeviceList()
	void enumerateWin32Ports();

	COMMTIMEOUTS oldTimeout;
		///\< \brief The old serial connection timeout.
		/// This is needed to restore settings on Microsoft Windows
		/// platforms upon closing the serial connection.
	char** portNamesShort;
		///\< \brief Short names of COM ports on Microsoft Windows.
		/// Short names only identify/name the COM port. The length
		/// of the array is limited to MAX_SERIAL_PORTS.
		///\see ofSerial::portNamesFriendly
	char** portNamesFriendly;
		///\< \brief Friendly names of COM ports on Microsoft Windows.
		/// The friendly names often describe/name the device attached to
		/// a particular serial port. The length of the array is limited
		/// to MAX_SERIAL_PORTS.
		///\see ofSerial::portNamesShort
	HANDLE hComm; ///\< This is the handler for the serial port on Microsoft Windows.
	int nPorts;  ///\< \brief Number of serial devices (ports) on Microsoft Windows.
	bool bPortsEnumerated;  ///\< \brief Indicate that all serial ports (on Microsoft Windows) have been enumerated.

#else
	int fd; ///< \brief File descriptor for the serial port.
	struct termios oldoptions;  ///< \brief This is the set of (current) terminal attributes to be reused when changing a subset of options.
#endif

};

//----------------------------------------------------------------------
// this serial code contains small portions of the following code-examples:
// ---------------------------------------------------
// http://todbot.com/arduino/host/arduino-serial/arduino-serial.c
// web.mac.com/miked13/iWeb/Arduino/Serial%20Write_files/main.cpp
// www.racer.nl/docs/libraries/qlib/qserial.htm
// ---------------------------------------------------

// to do:
// ----------------------------
// a) support blocking / non-blocking
// b) support numChars available type functions
// c) can we reduce the number of includes here?

// useful :
//	 http://en.wikibooks.org/wiki/Serial_Programming:Unix/termios
//	 http://www.keyspan.com/downloads-files/developer/win/USBSerial/html/DevDocsUSBSerial.html
// ----------------------------
// (also useful, might be this serial example - worth checking out:
// http://web.mit.edu/kvogt/Public/osrc/src/
// if has evolved ways of dealing with blocking
// and non-blocking instances)
// ----------------------------
