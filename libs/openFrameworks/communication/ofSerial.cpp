#include "ofSerial.h"
#include "ofMain.h"


//---------------------------------------------
#ifdef TARGET_WIN32
//---------------------------------------------

//------------------------------------
   // needed for serial bus enumeration:
   //4d36e978-e325-11ce-bfc1-08002be10318}
   DEFINE_GUID (GUID_SERENUM_BUS_ENUMERATOR, 0x4D36E978, 0xE325,
   0x11CE, 0xBF, 0xC1, 0x08, 0x00, 0x2B, 0xE1, 0x03, 0x18);
//------------------------------------

void ofSerial::enumerateWin32Ports(){

    // thanks joerg for fixes...

	if (bPortsEnumerated == true) return;

	HDEVINFO hDevInfo = NULL;
	SP_DEVINFO_DATA DeviceInterfaceData;
	int i = 0;
	DWORD dataType, actualSize = 0;
	unsigned char dataBuf[MAX_PATH + 1];

	// Reset Port List
	nPorts = 0;
	// Search device set
	hDevInfo = SetupDiGetClassDevs((struct _GUID *)&GUID_SERENUM_BUS_ENUMERATOR,0,0,DIGCF_PRESENT);
	if ( hDevInfo ){
      while (TRUE){
         ZeroMemory(&DeviceInterfaceData, sizeof(DeviceInterfaceData));
         DeviceInterfaceData.cbSize = sizeof(DeviceInterfaceData);
         if (!SetupDiEnumDeviceInfo(hDevInfo, i, &DeviceInterfaceData)){
             // SetupDiEnumDeviceInfo failed
             break;
         }

         if (SetupDiGetDeviceRegistryProperty(hDevInfo,
             &DeviceInterfaceData,
             SPDRP_FRIENDLYNAME,
             &dataType,
             dataBuf,
             sizeof(dataBuf),
             &actualSize)){

			sprintf(portNamesFriendly[nPorts], "%s", dataBuf);
			portNamesShort[nPorts][0] = 0;

			// turn blahblahblah(COM4) into COM4

            char *   begin    = NULL;
            char *   end    = NULL;
            begin          = strstr((char *)dataBuf, "COM");


            if (begin)
                {
                end          = strstr(begin, ")");
                if (end)
                    {
                      *end = 0;   // get rid of the )...
                      strcpy(portNamesShort[nPorts], begin);
                }
                if (nPorts++ > MAX_SERIAL_PORTS)
                        break;
            }
         }
            i++;
      }
   }
   SetupDiDestroyDeviceInfoList(hDevInfo);

   bPortsEnumerated = false;
}


//---------------------------------------------
#endif
//---------------------------------------------



//----------------------------------------------------------------
ofSerial::ofSerial(){

	//---------------------------------------------
	#ifdef TARGET_WIN32
	//---------------------------------------------
		nPorts 				= 0;
		bPortsEnumerated 	= false;

		portNamesShort = new char * [MAX_SERIAL_PORTS];
		portNamesFriendly = new char * [MAX_SERIAL_PORTS];
		for (int i = 0; i < MAX_SERIAL_PORTS; i++){
			portNamesShort[i] = new char[10];
			portNamesFriendly[i] = new char[MAX_PATH];
		}
	//---------------------------------------------
	#endif
	//---------------------------------------------
	bVerbose = false;
}

//----------------------------------------------------------------
ofSerial::~ofSerial(){

	close();



	//---------------------------------------------
	#ifdef TARGET_WIN32
	//---------------------------------------------
		nPorts 				= 0;
		bPortsEnumerated 	= false;

		for (int i = 0; i < MAX_SERIAL_PORTS; i++) {
			delete [] portNamesShort[i];
			delete [] portNamesFriendly[i];
		}
		delete [] portNamesShort;
		delete [] portNamesFriendly;

	//---------------------------------------------
	#endif
	//---------------------------------------------

	bVerbose = false;
	bInited = false;
}


//----------------------------------------------------------------
void ofSerial::enumerateDevices(){

	//---------------------------------------------
	#if defined( TARGET_OSX )
	//---------------------------------------------

		//----------------------------------------------------
		//We will find serial devices by listing the directory

		DIR *dir;
		struct dirent *entry;
		dir = opendir("/dev");
		string str			= "";
		string device		= "";
		int deviceCount		= 0;

		if (dir == NULL){
			ofLog(OF_LOG_ERROR,"ofSerial: error listing devices in /dev");
		} else {
			printf("ofSerial: listing devices\n");
			while ((entry = readdir(dir)) != NULL){
				str = (char *)entry->d_name;
				if( str.substr(0,3) == "cu." || str.substr(0,4) == "tty." ){
					printf("device %i - %s\n", deviceCount, str.c_str());
					deviceCount++;
				}
			}
		}

	//---------------------------------------------
    #endif
    //---------------------------------------------

	//---------------------------------------------
	#if defined( TARGET_LINUX )
	//---------------------------------------------

		//----------------------------------------------------
		//We will find serial devices by listing the directory

		DIR *dir;
		struct dirent *entry;
		dir = opendir("/dev");
		string str			= "";
		string device		= "";
		int deviceCount		= 0;

		if (dir == NULL){
			ofLog(OF_LOG_ERROR,"ofSerial: error listing devices in /dev");
		} else {
			printf("ofSerial: listing devices\n");
			while ((entry = readdir(dir)) != NULL){
				str = (char *)entry->d_name;
				if( str.substr(0,4) == "ttyS" || str.substr(0,6) == "ttyUSB" || str.substr(0,3) == "rfc" ){
					printf("device %i - %s\n", deviceCount, str.c_str());
					deviceCount++;
				}
			}
		}

	//---------------------------------------------
	#endif
	//---------------------------------------------

	//---------------------------------------------
	#ifdef TARGET_WIN32
	//---------------------------------------------

		enumerateWin32Ports();
		printf("ofSerial: listing devices (%i total)\n", nPorts);
		for (int i = 0; i < nPorts; i++){
			printf("device %i -- %s", i, portNamesFriendly[i]);
		}

	//---------------------------------------------
    #endif
    //---------------------------------------------

}

//----------------------------------------------------------------
void ofSerial::close(){

	//---------------------------------------------
	#ifdef TARGET_WIN32
	//---------------------------------------------
		if (bInited){
			SetCommTimeouts(hComm,&oldTimeout);
			CloseHandle(hComm);
			hComm 		= INVALID_HANDLE_VALUE;
			bInited 	= false;
		}
	//---------------------------------------------
    #else
    //---------------------------------------------
    	if (bInited){
    		tcsetattr(fd,TCSANOW,&oldoptions);
    		::close(fd);
    	}
    	// [CHECK] -- anything else need to be reset?
    //---------------------------------------------
    #endif
    //---------------------------------------------

}

//----------------------------------------------------------------
bool ofSerial::setup(){
	return setup(0,9600);		// the first one, at 9600 is a good choice...
}

//----------------------------------------------------------------
bool ofSerial::setup(int deviceNumber, int baud){

	int deviceCount = 0;

	string str			= "";
	string device		= "";
	bool deviceFound	= false;

	//---------------------------------------------
	#if defined( TARGET_OSX ) || defined( TARGET_LINUX )
	//---------------------------------------------

		//----------------------------------------------------
		//We will find serial devices by listing the directory

		DIR *dir;
		struct dirent *entry;
		dir = opendir("/dev");

		if (dir == NULL){
			ofLog(OF_LOG_ERROR,"ofSerial: error listing devices in /dev");
		}

		while ((entry = readdir(dir)) != NULL){
			str = (char *)entry->d_name;
			#ifdef TARGET_OSX
			if( str.substr(0,3) == "cu." || str.substr(0,4) == "tty." ){
			#else
			if( str.substr(0,4) == "ttyS" || str.substr(0,6) == "ttyUSB" || str.substr(0,3) == "rfc" ){
			#endif
				if(deviceCount == deviceNumber){
					device = "/dev/"+str;
					deviceFound = true;
					ofLog(OF_LOG_NOTICE,"ofSerial device %i - /dev/%s  <--selected", deviceCount, str.c_str());
				}else ofLog(OF_LOG_NOTICE,"ofSerial device %i - /dev/%s", deviceCount, str.c_str());
				deviceCount++;
			}
		}

        if(deviceFound){
            return setup(device, baud);
        }else{
            ofLog(OF_LOG_ERROR,"ofSerial: could not find device %i - only %i devices found", deviceNumber, deviceCount);
            return false;
        }

	//---------------------------------------------
    #endif
    //---------------------------------------------

	//---------------------------------------------
	#ifdef TARGET_WIN32
	//---------------------------------------------

		enumerateWin32Ports();
		if (deviceNumber < nPorts){
			device = portNamesShort[deviceNumber];
			deviceFound = true;
		}

        if(deviceFound){
            return setup(device, baud);
        }else{
            ofLog(OF_LOG_ERROR,"ofSerial: could not find device %i - only %i devices found", deviceNumber, nPorts);
            return false;
        }

	//---------------------------------------------
    #endif
    //---------------------------------------------


}

//----------------------------------------------------------------
bool ofSerial::setup(string portName, int baud){

	bInited = false;

	//---------------------------------------------
	#if defined( TARGET_OSX ) || defined( TARGET_LINUX )
	//---------------------------------------------

	    ofLog(OF_LOG_NOTICE,"ofSerialInit: opening port %s @ %d bps", portName.c_str(), baud);
		fd = open(portName.c_str(), O_RDWR | O_NOCTTY | O_NONBLOCK);
		if(fd == -1){
			ofLog(OF_LOG_ERROR,"ofSerial: unable to open port");
			return false;
		}

		struct termios options;
		tcgetattr(fd,&oldoptions);
		options = oldoptions;
		switch(baud){
		   case 300: 	cfsetispeed(&options,B300);
						cfsetospeed(&options,B300);
						break;
		   case 1200: 	cfsetispeed(&options,B1200);
						cfsetospeed(&options,B1200);
						break;
		   case 2400: 	cfsetispeed(&options,B2400);
						cfsetospeed(&options,B2400);
						break;
		   case 4800: 	cfsetispeed(&options,B4800);
						cfsetospeed(&options,B4800);
						break;
		   case 9600: 	cfsetispeed(&options,B9600);
						cfsetospeed(&options,B9600);
						break;
		   case 14400: 	cfsetispeed(&options,B14400);
						cfsetospeed(&options,B14400);
						break;
		   case 19200: 	cfsetispeed(&options,B19200);
						cfsetospeed(&options,B19200);
						break;
		   case 28800: 	cfsetispeed(&options,B28800);
						cfsetospeed(&options,B28800);
						break;
		   case 38400: 	cfsetispeed(&options,B38400);
						cfsetospeed(&options,B38400);
						break;
		   case 57600:  cfsetispeed(&options,B57600);
						cfsetospeed(&options,B57600);
						break;
		   case 115200: cfsetispeed(&options,B115200);
						cfsetospeed(&options,B115200);
						break;

			default:	cfsetispeed(&options,B9600);
						cfsetospeed(&options,B9600);
						ofLog(OF_LOG_ERROR,"ofSerialInit: cannot set %i baud setting baud to 9600\n", baud);
						break;
		}

		options.c_cflag |= (CLOCAL | CREAD);
		options.c_cflag &= ~PARENB;
		options.c_cflag &= ~CSTOPB;
		options.c_cflag &= ~CSIZE;
		options.c_cflag |= CS8;
		tcsetattr(fd,TCSANOW,&options);

		bInited = true;
		ofLog(OF_LOG_NOTICE,"sucess in opening serial connection");

	    return true;
	//---------------------------------------------
    #endif
    //---------------------------------------------


    //---------------------------------------------
	#ifdef TARGET_WIN32
	//---------------------------------------------

	// open the serial port:
	// "COM4", etc...

	hComm=CreateFileA(portName.c_str(),GENERIC_READ|GENERIC_WRITE,0,0,
					OPEN_EXISTING,0,0);

	if(hComm==INVALID_HANDLE_VALUE){
		ofLog(OF_LOG_ERROR,"ofSerial: unable to open port");
		return false;
	}


	// now try the settings:
	COMMCONFIG cfg;
	DWORD cfgSize;
	char  buf[80];

	cfgSize=sizeof(cfg);
	GetCommConfig(hComm,&cfg,&cfgSize);
	int bps = baud;
	sprintf(buf,"baud=%d parity=N data=8 stop=1",bps);

	#if (_MSC_VER)       // microsoft visual studio
		// msvc doesn't like BuildCommDCB,
		//so we need to use this version: BuildCommDCBA
		if(!BuildCommDCBA(buf,&cfg.dcb)){
			ofLog(OF_LOG_ERROR,"ofSerial: unable to build comm dcb; (%s)",buf);
		}
	#else
		if(!BuildCommDCB(buf,&cfg.dcb)){
			ofLog(OF_LOG_ERROR,"ofSerial: Can't build comm dcb; %s",buf);
		}
	#endif


	// Set baudrate and bits etc.
	// Note that BuildCommDCB() clears XON/XOFF and hardware control by default

	if(!SetCommState(hComm,&cfg.dcb)){
		ofLog(OF_LOG_ERROR,"ofSerial: Can't set comm state");
	}
	//ofLog(OF_LOG_NOTICE,buf,"bps=%d, xio=%d/%d",cfg.dcb.BaudRate,cfg.dcb.fOutX,cfg.dcb.fInX);

	// Set communication timeouts (NT)
	COMMTIMEOUTS tOut;
	GetCommTimeouts(hComm,&oldTimeout);
	tOut = oldTimeout;
	// Make timeout so that:
	// - return immediately with buffered characters
	tOut.ReadIntervalTimeout=MAXDWORD;
	tOut.ReadTotalTimeoutMultiplier=0;
	tOut.ReadTotalTimeoutConstant=0;
	SetCommTimeouts(hComm,&tOut);

	bInited = true;
	return true;
	//---------------------------------------------
	#endif
	//---------------------------------------------
}


//----------------------------------------------------------------
int ofSerial::writeBytes(unsigned char * buffer, int length){

	if (!bInited){
		ofLog(OF_LOG_ERROR,"ofSerial: serial not inited");
		return OF_SERIAL_ERROR;
	}

	//---------------------------------------------
	#if defined( TARGET_OSX ) || defined( TARGET_LINUX )
	    int numWritten = write(fd, buffer, length);
		if(numWritten <= 0){
			ofLog(OF_LOG_ERROR,"ofSerial: Can't write to com port");
			return OF_SERIAL_ERROR;
		}

		ofLog(OF_LOG_VERBOSE,"ofSerial: numWritten %i", numWritten);

	    return numWritten;
    #endif
    //---------------------------------------------

    //---------------------------------------------
	#ifdef TARGET_WIN32
		DWORD written;
		if(!WriteFile(hComm, buffer, length, &written,0)){
			 ofLog(OF_LOG_ERROR,"ofSerial: Can't write to com port");
			 return OF_SERIAL_ERROR;
		}
		ofLog(OF_LOG_VERBOSE,"ofSerial: numWritten %i", (int)written);
		return (int)written;
	#endif
	//---------------------------------------------

}

//----------------------------------------------------------------
int ofSerial::readBytes(unsigned char * buffer, int length){

	if (!bInited){
		ofLog(OF_LOG_ERROR,"ofSerial: serial not inited");
		return OF_SERIAL_ERROR;
	}

	//---------------------------------------------
	#if defined( TARGET_OSX ) || defined( TARGET_LINUX )
		int nRead = read(fd, buffer, length);
		if(nRead < 0){
			ofLog(OF_LOG_ERROR,"ofSerial: trouble reading from port");
			return OF_SERIAL_ERROR;
		}
		return nRead;
    #endif
    //---------------------------------------------

    //---------------------------------------------
	#ifdef TARGET_WIN32
		DWORD nRead = 0;
		if (!ReadFile(hComm,buffer,length,&nRead,0)){
			ofLog(OF_LOG_ERROR,"ofSerial: trouble reading from port");
			return OF_SERIAL_ERROR;
		}
		return (int)nRead;
	#endif
	//---------------------------------------------
}



//----------------------------------------------------------------
bool ofSerial::writeByte(unsigned char singleByte){

	if (!bInited){
		ofLog(OF_LOG_ERROR,"ofSerial: serial not inited");
		return OF_SERIAL_ERROR;
	}

	unsigned char tmpByte[1];
	tmpByte[0] = singleByte;

	//---------------------------------------------
	#if defined( TARGET_OSX ) || defined( TARGET_LINUX )
	    int numWritten = 0;
	    numWritten = write(fd, tmpByte, 1);
		if(numWritten <= 0 ){
			 ofLog(OF_LOG_ERROR,"ofSerial: Can't write to com port");
			 return OF_SERIAL_ERROR;
		}
		ofLog(OF_LOG_VERBOSE,"ofSerial: written byte");


		return (numWritten > 0 ? true : false);
    #endif
    //---------------------------------------------

    //---------------------------------------------
	#ifdef TARGET_WIN32
		DWORD written = 0;
		if(!WriteFile(hComm, tmpByte, 1, &written,0)){
			 ofLog(OF_LOG_ERROR,"ofSerial: Can't write to com port");
			 return OF_SERIAL_ERROR;;
		}

		ofLog(OF_LOG_VERBOSE,"ofSerial: written byte");

		return ((int)written > 0 ? true : false);
	#endif
	//---------------------------------------------

}

//----------------------------------------------------------------
int ofSerial::readByte(){

	if (!bInited){
		ofLog(OF_LOG_ERROR,"ofSerial: serial not inited");
		return OF_SERIAL_ERROR;
	}

	unsigned char tmpByte[1];
	memset(tmpByte, 0, 1);

	//---------------------------------------------
	#if defined( TARGET_OSX ) || defined( TARGET_LINUX )
		int nRead = read(fd, tmpByte, 1);
		if(nRead < 0){
			ofLog(OF_LOG_ERROR,"ofSerial: trouble reading from port");
            return OF_SERIAL_ERROR;
		}
		if(nRead == 0)
			return OF_SERIAL_NO_DATA;
    #endif
    //---------------------------------------------

    //---------------------------------------------
	#ifdef TARGET_WIN32
		DWORD nRead;
		if (!ReadFile(hComm, tmpByte, 1, &nRead, 0)){
			ofLog(OF_LOG_ERROR,"ofSerial: trouble reading from port");
			return OF_SERIAL_ERROR;
		}
	#endif
	//---------------------------------------------

	return (int)(tmpByte[0]);
}


//----------------------------------------------------------------
void ofSerial::flush(bool flushIn, bool flushOut){

	if (!bInited){
		ofLog(OF_LOG_ERROR,"ofSerial: serial not inited");
		return;
	}

	int flushType = 0;

	//---------------------------------------------
	#if defined( TARGET_OSX ) || defined( TARGET_LINUX )
		if( flushIn && flushOut) flushType = TCIOFLUSH;
		else if(flushIn) flushType = TCIFLUSH;
		else if(flushOut) flushType = TCOFLUSH;
		else return;

		tcflush(fd, flushType);
    #endif
    //---------------------------------------------

    //---------------------------------------------
	#ifdef TARGET_WIN32
		if( flushIn && flushOut) flushType = PURGE_TXCLEAR | PURGE_RXCLEAR;
		else if(flushIn) flushType = PURGE_RXCLEAR;
		else if(flushOut) flushType = PURGE_TXCLEAR;
		else return;

		PurgeComm(hComm, flushType);
	#endif
	//---------------------------------------------

}

//-------------------------------------------------------------
int ofSerial::available(){

	if (!bInited){
		ofLog(OF_LOG_ERROR,"ofSerial: serial not inited");
		return OF_SERIAL_ERROR;
	}

	int numBytes = 0;

	//---------------------------------------------
	#if defined( TARGET_OSX ) || defined( TARGET_LINUX )
		ioctl(fd,FIONREAD,&numBytes);
	#endif
    //---------------------------------------------

    //---------------------------------------------
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
    //---------------------------------------------

	return numBytes;
}

