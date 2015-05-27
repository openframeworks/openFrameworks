#pragma once

#include "ofConstants.h"
#include "ofTypes.h"

#if defined( TARGET_OSX ) || defined( TARGET_LINUX ) || defined (TARGET_ANDROID)
	#include <termios.h>
#else
	#include <winbase.h>
	#include <tchar.h>
	#include <iostream>
	#include <string.h>
	#include <setupapi.h>
	#include <regstr.h>
	/// \cond INTERNAL
	#define MAX_SERIAL_PORTS 256
	/// \endcond
	#include <winioctl.h>
	#ifdef __MINGW32__
			#define INITGUID
			#include <initguid.h> // needed for dev-c++ & DEFINE_GUID
    #endif
#endif


/// \brief  ofSerial provides a cross platform system for interfacing with the
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
/// 	serial.listDevices();
///     vector <ofSerialDeviceInfo> deviceList = serial.getDeviceList();
///
///		// Open the first device and talk to it at 57600 baud
/// 	serial.setup(0, 57600);
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
	
	//old method - deprecated
	void enumerateDevices();
	
	/// \brief Returns a vector of ofSerialDeviceInfo instances with the
	/// devicePath, deviceName, deviceID set.
	vector <ofSerialDeviceInfo> getDeviceList();
	
	/// \}
	/// \name Serial Connection
	/// \{
	
	/// \brief Attempts to setup the first available device at a baud rate of 9600.
	/// ~~~~{.cpp}
	/// 	ofSerial mySerial;
	/// 	if( mySerial.setup() ){
	/// 		ofLog("serial is setup!");
	/// 	}
	/// ~~~~
	bool setup();
	
	/// \brief Opens the serial port, with the given name and baud rate.
	///
	/// On OSX and Linux, it might look like:
	/// ~~~~{.cpp}
	/// 	ofSerial mySerial;
	/// 	mySerial.setup("/dev/cu.USA19H181P1.1", 57600);
	/// ~~~~
	///
	/// On Windows, like:
	/// ~~~~{.cpp}
	/// 	ofSerial mySerial;
	/// 	mySerial.setup("COM4", 57600);
	/// ~~~~
	bool setup(string portName, int baudrate);
	
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
	///   device.readBytes(buffer, 8);
	/// }
	/// ~~~~
	///
	/// This is useful when you know how long a complete message from a device
	/// is going to be.
	int	available();
	
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
	/// 	// we want to read 8 bytes
	/// 	int bytesRequired = 8;
	/// 	unsigned char bytes[bytesRequired];
	/// 	int bytesRemaining = bytesRequired;
	/// 	// loop until we've read everything
	/// 	while ( bytesRemaining > 0 ){
	/// 	  // check for data
	/// 	  if ( serial.available() > 0 ){
	/// 	    // try to read - note offset into the bytes[] array, this is so
	/// 	    // that we don't overwrite the bytes we already have
	/// 	    int bytesArrayOffset = bytesRequired - bytesRemaining;
	/// 	    int result = serial.readBytes( &bytes[bytesArrayOffset], bytesRemaining );
	///
	/// 	    // check for error code
	/// 	    if ( result == OF_SERIAL_ERROR ){
	/// 	      // something bad happened
	/// 	      ofLog( OF_LOG_ERROR, "unrecoverable error reading from serial" );
	/// 	      break;
	/// 	    } else if ( result == OF_SERIAL_NO_DATA ){
	/// 	      // nothing was read, try again
	/// 	    } else {
	/// 	      // we read some data!
	/// 	      bytesRemaining -= result;
	/// 	    }
	/// 	  }
	/// 	}
	/// ~~~~
	///
	/// Be aware that the type of your buffer can only be unsigned char. If you're
	/// trying to receieve ints or signed chars over a serial connection you'll
	/// need to do some bit manipulation to correctly interpret that values.
	int readBytes(unsigned char * buffer, int length);
	
	/// \brief Reads and returns a single byte from the requested device.
	///
	/// ~~~~{.cpp}
	/// 	ofSerial mySerial;
	/// 	mySerial.setup(0, 57600);
	///
	/// 	int myByte = mySerial.readByte();
	///
	/// 	if ( myByte == OF_SERIAL_NO_DATA ){
	/// 		printf("no data was read");
	/// 	} else if ( myByte == OF_SERIAL_ERROR ){
	/// 		printf("an error occurred");
	/// 	} else {
	/// 		printf("myByte is %d", myByte);
	///		}
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
	/// 	unsigned char buf[3] = {'o', 'f', '!'};
	/// 	device.writeBytes(&buf[0], 3);
	/// ~~~~
	int writeBytes(unsigned char * buffer, int length);
	
	/// \brief Writes a single byte to the connected serial device.
	///
	/// Check the return value to be sure the data was written.
	/// ~~~~{.cpp}
	/// 	ofSerial mySerial;
	/// 	mySerial.setup(0, 57600);
	/// 	unsigned char myByte = 225;
	/// 	bool byteWasWritten = mySerial.writeByte(myByte);
	/// 	if ( !byteWasWritten )
	/// 	  ofLog(OF_LOG_ERROR, "Byte was not written to serial port");
	/// ~~~~
	bool writeByte(unsigned char singleByte);
	
	/// \}
	/// \name Clear Data
	/// \{
	
	/// \brief Clears data from one or both of the serial buffers.
	///
	/// Any data in the cleared buffers is discarded.
	/// \param flushIn If true then it clears the incoming data buffer
	/// \param fluhOut If true then it clears the outgoing data buffer.
	void flush(bool flushIn = true, bool flushOut = true);
	
	/// \brief Drain is only available on OSX and Linux and is very similar to
	/// flush(), but blocks until all the data has been written to or read
	/// from the serial port.
	void drain();
	
	/// \}
	
protected:
	void buildDeviceList();
	
	string deviceType;
	vector <ofSerialDeviceInfo> devices;
	
	bool bHaveEnumeratedDevices;
	
	bool bInited;
	
#ifdef TARGET_WIN32
	
	char** portNamesShort;//[MAX_SERIAL_PORTS];
	char** portNamesFriendly; ///[MAX_SERIAL_PORTS];
	HANDLE hComm;		// the handle to the serial port pc
	int	nPorts;
	bool bPortsEnumerated;
	void enumerateWin32Ports();
	COMMTIMEOUTS oldTimeout;	// we alter this, so keep a record
	
#else
	int fd;	// the handle to the serial port mac
	struct termios oldoptions;
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
// a) 	support blocking / non-blocking
// b) 	support numChars available type functions
// c)   can we reduce the number of includes here?

// 	useful :
// 	http://en.wikibooks.org/wiki/Serial_Programming:Unix/termios
// 	http://www.keyspan.com/downloads-files/developer/win/USBSerial/html/DevDocsUSBSerial.html
// ----------------------------
// (also useful, might be this serial example - worth checking out:
// http://web.mit.edu/kvogt/Public/osrc/src/
// if has evolved ways of dealing with blocking
// and non-blocking instances)
// ----------------------------


