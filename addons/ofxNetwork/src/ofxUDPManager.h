#pragma once

//////////////////////////////////////////////////////////////////////////////////////
// Original author: ???????? we think Christian Naglhofer
// Crossplatform port by: Theodore Watson May 2007 - update Jan 2008
// Changes: Mac (and should be nix) equivilant functions and data types for
// win32 calls, as well as non blocking option SetNonBlocking(bool nonBlocking);
//
//////////////////////////////////////////////////////////////////////////////////////

/*-----------------------------------------------------------------------------------
USAGE
-------------------------------------------------------------------------------------

!!!!!!!!!!!!!!!!!!!!!!!!!!!!
!!! LINK WITH ws2_32.lib !!!
!!!!!!!!!!!!!!!!!!!!!!!!!!!!

UDP Socket Client (sending):
------------------

1) create()
2) connect()
3) send()
...
x) close()

optional:
SetTimeoutSend()

UDP Multicast (sending):
--------------

1) Create()
2) ConnectMcast()
3) Send()
...
x) Close()

extra optional:
SetTTL() - default is 1 (current subnet)

UDP Socket Server (receiving):
------------------

1) create()
2) bind()
3) receive()
...
x) close()

optional:
SetTimeoutReceive()

UDP Multicast (receiving):
--------------

1) Create()
2) BindMcast()
3) Receive()
...
x) Close()

--------------------------------------------------------------------------------*/
#include "ofConstants.h"
#include <string.h>
#include <wchar.h>
#include <stdio.h>

#ifndef TARGET_WIN32

	//unix includes - works for osx should be same for *nix
	#include <ctype.h>
	#include <netdb.h>
	#include <string.h>
	#include <fcntl.h>
	#include <errno.h>
	#include <unistd.h>
	#include <arpa/inet.h>
	#include <netinet/in.h>
	#include <sys/types.h>
	#include <sys/socket.h>
	#include <sys/time.h>
	#include <sys/ioctl.h>

    //#ifdef TARGET_LINUX
        // linux needs this:
        #include <netinet/tcp.h>		/* for TCP_MAXSEG value */
    //#endif


	#define SO_MAX_MSG_SIZE TCP_MAXSEG
	#define INVALID_SOCKET -1
	#define SOCKET_ERROR -1
	#define FAR

#else
	//windows includes
	#include <winsock2.h>
	#include <ws2tcpip.h>		// TCP/IP annex needed for multicasting
#endif

/// Socket constants.
#define SOCKET_TIMEOUT			SOCKET_ERROR - 1
#define NO_TIMEOUT				0xFFFF
#define OF_UDP_DEFAULT_TIMEOUT   NO_TIMEOUT

//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------

// Implementation of a UDP socket.
class ofxUDPManager
{
public:

	//constructor
	ofxUDPManager();

	//destructor
	virtual ~ofxUDPManager()
	{
		if ((m_hSocket)&&(m_hSocket != INVALID_SOCKET)) Close();
	}

	bool Close();
	bool Create();
	bool Connect(const char *pHost, unsigned short usPort);
	bool ConnectMcast(char *pMcast, unsigned short usPort);
	bool Bind(unsigned short usPort);
	bool BindMcast(char *pMcast, unsigned short usPort);
	int  Send(const char* pBuff, const int iSize);
	//all data will be sent guaranteed.
	int  SendAll(const char* pBuff, const int iSize);
	int  Receive(char* pBuff, const int iSize);
	void SetTimeoutSend(int timeoutInSeconds);
	void SetTimeoutReceive(int timeoutInSeconds);
	int  GetTimeoutSend();
	int  GetTimeoutReceive();
	bool GetRemoteAddr(char* address);							//returns the IP of last received packet
	bool SetReceiveBufferSize(int sizeInByte);
	bool SetSendBufferSize(int sizeInByte);
	int  GetReceiveBufferSize();
	int  GetSendBufferSize();
	bool SetReuseAddress(bool allowReuse);
	bool SetEnableBroadcast(bool enableBroadcast);
	bool SetNonBlocking(bool useNonBlocking);
	int  GetMaxMsgSize();
	/// returns -1 on failure
	int  GetTTL();
	bool SetTTL(int nTTL);

protected:
	int m_iListenPort;

	#ifdef TARGET_WIN32
		SOCKET m_hSocket;
	#else
		int m_hSocket;
	#endif


	unsigned long m_dwTimeoutReceive;
	unsigned long m_dwTimeoutSend;

	bool nonBlocking;

	struct sockaddr_in saServer;
	struct sockaddr_in saClient;

	static bool m_bWinsockInit;
	bool canGetRemoteAddress;

};
