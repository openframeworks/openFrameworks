#include "ofSerial.h"
#include "ofUtils.h"
#include "ofTypes.h"

#if defined( TARGET_OSX ) || defined( TARGET_LINUX )
	#include <sys/ioctl.h>
	#include <getopt.h>
	#include <dirent.h>
#endif


#include <fcntl.h>
#include <errno.h>
#include <ctype.h>
#include <algorithm>


#ifdef TARGET_WIN32

// needed for serial bus enumeration:
// 4d36e978-e325-11ce-bfc1-08002be10318}
DEFINE_GUID (GUID_SERENUM_BUS_ENUMERATOR, 0x4D36E978, 0xE325,
0x11CE, 0xBF, 0xC1, 0x08, 0x00, 0x2B, 0xE1, 0x03, 0x18);


void ofSerial::enumerateWin32Ports(){
	if(bPortsEnumerated == true){
		return;
	}

	HDEVINFO hDevInfo = nullptr;
	SP_DEVINFO_DATA DeviceInterfaceData;
	DWORD dataType, actualSize = 0;

	// Reset Port List
	nPorts = 0;
	// Search device set
	hDevInfo = SetupDiGetClassDevs((struct _GUID *)&GUID_SERENUM_BUS_ENUMERATOR, 0, 0, DIGCF_PRESENT);
	if(hDevInfo){
		int i = 0;
		unsigned char dataBuf[MAX_PATH + 1];
		while(TRUE){
			ZeroMemory(&DeviceInterfaceData, sizeof(DeviceInterfaceData));
			DeviceInterfaceData.cbSize = sizeof(DeviceInterfaceData);
			if(!SetupDiEnumDeviceInfo(hDevInfo, i, &DeviceInterfaceData)){
				// SetupDiEnumDeviceInfo failed
				break;
			}

		if(SetupDiGetDeviceRegistryPropertyA(hDevInfo,
											 &DeviceInterfaceData,
											 SPDRP_FRIENDLYNAME,
											 &dataType,
											 dataBuf,
											 sizeof(dataBuf),
											 &actualSize)){

			 sprintf(portNamesFriendly[nPorts], "%s", dataBuf);
			 portNamesShort[nPorts][0] = 0;

			 // turn blahblahblah(COM4) into COM4

			 char * begin = nullptr;
			 char * end = nullptr;
			 begin = strstr((char *)dataBuf, "COM");

			 if(begin){
				 end = strstr(begin, ")");
				 if(end){
					 *end = 0;   // get rid of the )...
					 strcpy(portNamesShort[nPorts], begin);
				 }
				 if(nPorts++ > MAX_SERIAL_PORTS)
					 break;
				}
			}
			i++;
		}
	}
	SetupDiDestroyDeviceInfoList(hDevInfo);

	bPortsEnumerated = false;
}

#endif  // TARGET_WIN32



//----------------------------------------------------------------
ofSerial::ofSerial(){

	#ifdef TARGET_WIN32

		nPorts = 0;
		bPortsEnumerated = false;

		portNamesShort = new char * [MAX_SERIAL_PORTS];
		portNamesFriendly = new char * [MAX_SERIAL_PORTS];
		for(int i = 0; i < MAX_SERIAL_PORTS; i++){
			portNamesShort[i] = new char[10];
			portNamesFriendly[i] = new char[MAX_PATH];
		}

	#else

	fd = -1;

	#endif

	bHaveEnumeratedDevices = false;
	bInited = false;
}


//----------------------------------------------------------------
ofSerial::~ofSerial(){
	close();

	#ifdef TARGET_WIN32

		nPorts = 0;
		bPortsEnumerated = false;

		for(int i = 0; i < MAX_SERIAL_PORTS; i++){
			delete [] portNamesShort[i];
			delete [] portNamesFriendly[i];
		}
		delete [] portNamesShort;
		delete [] portNamesFriendly;

	#endif

	bInited = false;
}

//----------------------------------------------------------------
static bool isDeviceArduino( ofSerialDeviceInfo & A ){
	//TODO - this should be ofStringInString
	return (strstr(A.getDeviceName().c_str(), "usbserial") != nullptr
			|| strstr(A.getDeviceName().c_str(), "usbmodem") != nullptr);
}

//----------------------------------------------------------------
void ofSerial::buildDeviceList(){
	deviceType = "serial";
	devices.clear();
	vector <string> prefixMatch;

	#ifdef TARGET_OSX

		prefixMatch.push_back("cu.");
		prefixMatch.push_back("tty.");

	#endif

	#ifdef TARGET_LINUX

		prefixMatch.push_back("ttyACM");
		prefixMatch.push_back("ttyS");
		prefixMatch.push_back("ttyUSB");
		prefixMatch.push_back("rfc");

	#endif

	#if defined( TARGET_OSX ) || defined( TARGET_LINUX )
		DIR *dir;
		dir = opendir("/dev");

		string deviceName = "";

		if(dir == nullptr){
			ofLogError("ofSerial") << "buildDeviceList(): error listing devices in /dev";
		} else {
			int deviceCount = 0;
			//for each device
			struct dirent *entry;
			while((entry = readdir(dir)) != nullptr){
				deviceName = (char *)entry->d_name;

				//we go through the prefixes
				for(int k = 0; k < (int)prefixMatch.size(); k++){
					//if the device name is longer than the prefix
					if(deviceName.size() > prefixMatch[k].size()){
						//do they match ?
						if(deviceName.substr(0, prefixMatch[k].size()) == prefixMatch[k].c_str()){
							devices.push_back(ofSerialDeviceInfo("/dev/"+deviceName, deviceName, deviceCount));
							deviceCount++;
							break;
						}
					}
				}
			}
			closedir(dir);
		}

	#endif

	#ifdef TARGET_WIN32

		enumerateWin32Ports();
		ofLogNotice("ofSerial") << "found " << nPorts << " devices";
		for(int i = 0; i < nPorts; i++){
			//NOTE: we give the short port name for both as that is what the user should pass and the short name is more friendly
			devices.push_back(ofSerialDeviceInfo(string(portNamesShort[i]), string(portNamesShort[i]), i));
		}

	#endif

	//here we sort the device to have the aruino ones first.
	partition(devices.begin(), devices.end(), isDeviceArduino);
	//we are reordering the device ids. too!
	for(int k = 0; k < (int)devices.size(); k++){
		devices[k].deviceID = k;
	}

	bHaveEnumeratedDevices = true;
}


//----------------------------------------------------------------
void ofSerial::listDevices(){
	buildDeviceList();
	for(int k = 0; k < (int)devices.size(); k++){
		ofLogNotice("ofSerial") << "[" << devices[k].getDeviceID() << "] = "<< devices[k].getDeviceName().c_str();
	}
}

//----------------------------------------------------------------
vector <ofSerialDeviceInfo> ofSerial::getDeviceList(){
	buildDeviceList();
	return devices;
}

//----------------------------------------------------------------
void ofSerial::enumerateDevices(){
	listDevices();
}

//----------------------------------------------------------------
void ofSerial::close(){

	#ifdef TARGET_WIN32

		if(bInited){
			SetCommTimeouts(hComm, &oldTimeout);
			CloseHandle(hComm);
			hComm = INVALID_HANDLE_VALUE;
			bInited = false;
		}

	#else

		if(bInited){
			tcsetattr(fd, TCSANOW, &oldoptions);
			::close(fd);
			bInited = false;
		}
		// [CHECK] -- anything else need to be reset?

	#endif
}

//----------------------------------------------------------------
bool ofSerial::setup(){
	return setup(0, 9600); // the first one, at 9600 is a good choice...
}

//----------------------------------------------------------------
bool ofSerial::setup(int deviceNumber, int baud){
	buildDeviceList();
	if(deviceNumber < (int)devices.size()){
		return setup(devices[deviceNumber].devicePath, baud);
	} else {
		ofLogError("ofSerial") << "couldn't find device " << deviceNumber << ", only " << devices.size() << " devices found";
		return false;
	}

}

//----------------------------------------------------------------
bool ofSerial::setup(string portName, int baud){
	bInited = false;

	#if defined( TARGET_OSX ) || defined( TARGET_LINUX )

		//lets account for the name being passed in instead of the device path
		if(portName.size() > 5 && portName.substr(0, 5) != "/dev/"){
			portName = "/dev/" + portName;
		}

		ofLogNotice("ofSerial") << "opening " << portName << " @ " << baud << " bps";
		fd = open(portName.c_str(), O_RDWR | O_NOCTTY | O_NONBLOCK);
		if(fd == -1){
			ofLogError("ofSerial") << "unable to open " << portName;
			return false;
		}

		struct termios options;
		tcgetattr(fd, &oldoptions);
		options = oldoptions;
		switch(baud){
			case 300:
			cfsetispeed(&options, B300);
				cfsetospeed(&options, B300);
				break;
		   case 1200:
			cfsetispeed(&options, B1200);
				cfsetospeed(&options, B1200);
				break;
		   case 2400:
			cfsetispeed(&options, B2400);
				cfsetospeed(&options, B2400);
				break;
		   case 4800:
			cfsetispeed(&options, B4800);
		cfsetospeed(&options, B4800);
		break;
		   case 9600:
			cfsetispeed(&options, B9600);
		cfsetospeed(&options, B9600);
		break;
		   case 14400:
			cfsetispeed(&options, B14400);
		cfsetospeed(&options, B14400);
		break;
		   case 19200:
			cfsetispeed(&options, B19200);
		cfsetospeed(&options, B19200);
		break;
		   case 28800:
			cfsetispeed(&options, B28800);
		cfsetospeed(&options, B28800);
		break;
		   case 38400:
			cfsetispeed(&options, B38400);
		cfsetospeed(&options, B38400);
		break;
		   case 57600:
			cfsetispeed(&options, B57600);
		cfsetospeed(&options, B57600);
		break;
		   case 115200:
			cfsetispeed(&options, B115200);
		cfsetospeed(&options, B115200);
		break;
		   case 230400:
			cfsetispeed(&options, B230400);
		cfsetospeed(&options, B230400);
		break;
		   default:
			cfsetispeed(&options, B9600);
		cfsetospeed(&options, B9600);
		ofLogError("ofSerial") << "setup(): cannot set " << baud << " bps, setting to 9600";
		break;
		}

		options.c_cflag |= (CLOCAL | CREAD);
		options.c_cflag &= ~PARENB;
		options.c_cflag &= ~CSTOPB;
		options.c_cflag &= ~CSIZE;
		options.c_iflag &= (tcflag_t) ~(INLCR | IGNCR | ICRNL | IGNBRK);
		options.c_oflag &= (tcflag_t) ~(OPOST);
		options.c_cflag |= CS8;
		tcsetattr(fd, TCSANOW, &options);

		bInited = true;
		ofLogNotice("ofSerial") << "opened " << portName << " sucessfully @ " << baud << " bps";

		return true;

	#elif defined( TARGET_WIN32 )

		char pn[sizeof(portName)];
		int num;
		if(sscanf(portName.c_str(), "COM%d", &num) == 1){
			// Microsoft KB115831 a.k.a if COM > COM9 you have to use a different
			// syntax
			sprintf(pn, "\\\\.\\COM%d", num);
		} else {
			strncpy(pn, (const char *)portName.c_str(), sizeof(portName)-1);
		}

		// open the serial port:
		// "COM4", etc...

		hComm = CreateFileA(pn, GENERIC_READ|GENERIC_WRITE, 0, 0,
							OPEN_EXISTING, 0, 0);

		if(hComm == INVALID_HANDLE_VALUE){
			ofLogError("ofSerial") << "setup(): unable to open " << portName;
			return false;
		}


		// now try the settings:
		COMMCONFIG cfg;
		DWORD cfgSize;
		WCHAR buf[80];

		cfgSize = sizeof(cfg);
		GetCommConfig(hComm, &cfg, &cfgSize);
		int bps = baud;
		swprintf(buf, L"baud=%d parity=N data=8 stop=1", bps);

		if(!BuildCommDCBW(buf, &cfg.dcb)){
			ofLogError("ofSerial") << "setup(): unable to build comm dcb, (" << buf << ")";
		}

		// Set baudrate and bits etc.
		// Note that BuildCommDCB() clears XON/XOFF and hardware control by default

		if(!SetCommState(hComm, &cfg.dcb)){
			ofLogError("ofSerial") << "setup(): couldn't set comm state: " << cfg.dcb.BaudRate << " bps, xio " << cfg.dcb.fInX << "/" << cfg.dcb.fOutX;
		}
		//ofLogNotice("ofSerial") << "bps=" << cfg.dcb.BaudRate << ", xio=" << cfg.dcb.fInX << "/" << cfg.dcb.fOutX;

		// Set communication timeouts (NT)
		COMMTIMEOUTS tOut;
		GetCommTimeouts(hComm, &oldTimeout);
		tOut = oldTimeout;
		// Make timeout so that:
		// - return immediately with buffered characters
		tOut.ReadIntervalTimeout = MAXDWORD;
		tOut.ReadTotalTimeoutMultiplier = 0;
		tOut.ReadTotalTimeoutConstant = 0;
		SetCommTimeouts(hComm, &tOut);

		bInited = true;
		return true;

	#else

		ofLogError("ofSerial") << "not implemented in this platform";
		return false;

	#endif
}


//----------------------------------------------------------------
int ofSerial::writeBytes(unsigned char * buffer, int length){
	if(!bInited){
		ofLogError("ofSerial") << "writeBytes(): serial not inited";
		return OF_SERIAL_ERROR;
	}

	#if defined( TARGET_OSX ) || defined( TARGET_LINUX )
		int numWritten = write(fd, buffer, length);
		if(numWritten <= 0){
		if(errno == EAGAIN){
				return 0;
		}
			ofLogError("ofSerial") << "writeBytes(): couldn't write to port: " << errno << " " << strerror(errno);
			return OF_SERIAL_ERROR;
		}

		ofLogVerbose("ofSerial") << "wrote " << (int) numWritten << " bytes";
		return numWritten;

	#endif

	#ifdef TARGET_WIN32

		DWORD written;
		if(!WriteFile(hComm, buffer, length, &written,0)){
			 ofLogError("ofSerial") << "writeBytes(): couldn't write to port";
			 return OF_SERIAL_ERROR;
		}
		ofLogVerbose("ofSerial") <<  "wrote " << (int) written << " bytes";
		return (int)written;

	#else

		return 0;

	#endif
}

//----------------------------------------------------------------
int ofSerial::readBytes(unsigned char * buffer, int length){
	if (!bInited){
		ofLogError("ofSerial") << "readBytes(): serial not inited";
		return OF_SERIAL_ERROR;
	}

	#if defined( TARGET_OSX ) || defined( TARGET_LINUX )

		int nRead = read(fd, buffer, length);
		if(nRead < 0){
			if ( errno == EAGAIN )
				return OF_SERIAL_NO_DATA;
			ofLogError("ofSerial") << "readBytes(): couldn't read from port: " << errno << " " << strerror(errno);
			return OF_SERIAL_ERROR;
		}
		return nRead;

	#elif defined( TARGET_WIN32 )

		DWORD nRead = 0;
		if (!ReadFile(hComm, buffer, length, &nRead, 0)){
			ofLogError("ofSerial") << "readBytes(): couldn't read from port";
			return OF_SERIAL_ERROR;
		}
		return (int)nRead;

	#else

		ofLogError("ofSerial") << "not defined in this platform";
		return -1;

	#endif
}

//----------------------------------------------------------------
bool ofSerial::writeByte(unsigned char singleByte){
	if (!bInited){
		ofLogError("ofSerial") << "writeByte(): serial not inited";
		//return OF_SERIAL_ERROR; // this looks wrong.
		return false;
	}

	#if defined( TARGET_OSX ) || defined( TARGET_LINUX )

		int numWritten = 0;
		numWritten = write(fd, &singleByte, 1);
		if(numWritten <= 0 ){
		if(errno == EAGAIN){
				return 0;
		}
			ofLogError("ofSerial") << "writeByte(): couldn't write to port: " << errno << " " << strerror(errno);
			//return OF_SERIAL_ERROR; // this looks wrong.
			return false;
		}
		ofLogVerbose("ofSerial") << "wrote byte";

		return (numWritten > 0 ? true : false);

	#elif defined( TARGET_WIN32 )

		DWORD written = 0;
		if(!WriteFile(hComm, &singleByte, 1, &written,0)){
			 ofLogError("ofSerial") << "writeByte(): couldn't write to port";
			 //return OF_SERIAL_ERROR; // this looks wrong.
			 return false;
		}

		ofLogVerbose("ofSerial") << "wrote byte";

		return ((int)written > 0 ? true : false);

	#else

		ofLogError("ofSerial") << "not defined in this platform";
		return false;

	#endif
}

//----------------------------------------------------------------
int ofSerial::readByte(){
	if(!bInited){
		ofLogError("ofSerial") << "readByte(): serial not inited";
		return OF_SERIAL_ERROR;
	}

	unsigned char tmpByte = 0;

	#if defined( TARGET_OSX ) || defined( TARGET_LINUX )

		int nRead = read(fd, &tmpByte, 1);
		if(nRead < 0){
			if ( errno == EAGAIN ){
				return OF_SERIAL_NO_DATA;
			}
			ofLogError("ofSerial") << "readByte(): couldn't read from port: " << errno << " " << strerror(errno);
			return OF_SERIAL_ERROR;
		}

		if(nRead == 0){
			return OF_SERIAL_NO_DATA;
		}

	#elif defined( TARGET_WIN32 )

		DWORD nRead;
		if(!ReadFile(hComm, &tmpByte, 1, &nRead, 0)){
			ofLogError("ofSerial") << "readByte(): couldn't read from port";
			return OF_SERIAL_ERROR;
		}
	
		if(nRead == 0){
			return OF_SERIAL_NO_DATA;
		}

	#else

		ofLogError("ofSerial") << "not defined in this platform";
		return OF_SERIAL_ERROR;

	#endif

	return tmpByte;
}


//----------------------------------------------------------------
void ofSerial::flush(bool flushIn, bool flushOut){
	if(!bInited){
		ofLogError("ofSerial") << "flush(): serial not inited";
		return;
	}

	#if defined( TARGET_OSX ) || defined( TARGET_LINUX )
		int flushType = 0;
		if(flushIn && flushOut) flushType = TCIOFLUSH;
		else if(flushIn) flushType = TCIFLUSH;
		else if(flushOut) flushType = TCOFLUSH;
		else return;

		tcflush(fd, flushType);
	#elif defined( TARGET_WIN32 )

		int flushType = 0;
		if(flushIn && flushOut) flushType = PURGE_TXCLEAR | PURGE_RXCLEAR;
		else if(flushIn) flushType = PURGE_RXCLEAR;
		else if(flushOut) flushType = PURGE_TXCLEAR;
		else return;

		PurgeComm(hComm, flushType);

	#endif
}

void ofSerial::drain(){
	if(!bInited){
		ofLogError("ofSerial") << "drain(): serial not inited";
		return;
	}

	#if defined( TARGET_OSX ) || defined( TARGET_LINUX )

		tcdrain(fd);

	#endif
}

//-------------------------------------------------------------
int ofSerial::available(){
	if(!bInited){
		ofLogError("ofSerial") << "available(): serial not inited";
		return OF_SERIAL_ERROR;
	}

	int numBytes = 0;

	#if defined( TARGET_OSX ) || defined( TARGET_LINUX )

		ioctl(fd, FIONREAD, &numBytes);

	#endif

	#ifdef TARGET_WIN32

		COMSTAT stat;
		DWORD err;
		if(hComm!=INVALID_HANDLE_VALUE){
			if(!ClearCommError(hComm, &err, &stat)){
				numBytes = 0;
			} else {
				numBytes = stat.cbInQue;
			}
		 } else {
			numBytes = 0;
		 }

	#endif

	return numBytes;
}

bool ofSerial::isInitialized() const{
	return bInited;
}
