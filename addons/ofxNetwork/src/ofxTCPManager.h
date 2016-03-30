#pragma once

//////////////////////////////////////////////////////////////////////////////////////
// Original author: ???????? we think Christian Naglhofer
// Crossplatform port by: Theodore Watson May 2007 - update Jan 2008
// Changes: Mac (and should be nix) equivilant functions and data types for
// win32 calls, artificial nix version of GetTickCount() used for timestamp
//
//////////////////////////////////////////////////////////////////////////////////////


/***************************************************************
	USAGE
****************************************************************

TCP Socket Client:
------------------

1) create()
2) connect()
3) send()
...
x) close()

optional:
SetTimeoutSend()


TCP Socket Server:
------------------

1) create()
2) bind()
3) listen()
4) accept()
5) receive()
...
x) close()

optional:
SetTimeoutAccept()
SetTimeoutReceive()

****************************************************************/
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
	#include <time.h>
	#include <errno.h>
	#include <unistd.h>
	#include <netinet/in.h>
	#include <arpa/inet.h>
	#include <sys/timeb.h>
	#include <sys/types.h>
	#include <sys/socket.h>
	#include <sys/time.h>
	#include <sys/ioctl.h>

#ifndef TARGET_ANDROID
	#include <sys/signal.h>
#else
	#include <signal.h>
#endif

	//other types
	#define INVALID_SOCKET -1
	#define SOCKET_ERROR -1
	#define FAR
	#define SO_MAX_MSG_SIZE TCP_MAXSEG
#else
	//windows includes
	#include <winsock2.h>
	#include <ws2tcpip.h>		// TCP/IP annex needed for multicasting
#endif

//--------------------------------------------------------------------------------
class InetAddr : public sockaddr_in
{
public:
  // constructors
	InetAddr() {
		memset(this, 0, sizeof(InetAddr));
		sin_family= AF_INET;
		sin_port= 0;
		sin_addr.s_addr= 0;
	};

	InetAddr(const sockaddr& sockAddr) {
		memcpy(this, &sockAddr, sizeof(sockaddr));
	};

	InetAddr(const sockaddr_in& sin) {
		memcpy(this, &sin, sizeof(sockaddr_in));
	};

	InetAddr(const unsigned long ulAddr, const unsigned short ushPort= 0) {
		memset(this, 0, sizeof(InetAddr));
		sin_family= AF_INET;
		sin_port= htons(ushPort);
		sin_addr.s_addr= htonl(ulAddr);
	};

  InetAddr(const wchar_t* pStrIP, const unsigned short usPort= 0) {
		char szStrIP[32];

		#ifdef TARGET_WIN32
			WideCharToMultiByte(CP_ACP, 0, pStrIP, (int)wcslen(pStrIP) + 1, szStrIP, 32, 0, 0);
		#else
			//theo note:
			//do we need to set the codepage here first?
			//or is the default one okay?
			wcstombs(szStrIP, pStrIP, 32);
		#endif

		memset(this, 0, sizeof(InetAddr));
		sin_family= AF_INET;
		sin_port= htons(usPort);
		sin_addr.s_addr= inet_addr(szStrIP);
	}

	InetAddr(const char* pStrIP, const unsigned short usPort= 0) {
		memset(this, 0, sizeof(InetAddr));
		sin_family= AF_INET;
		sin_port= htons(usPort);
		sin_addr.s_addr= inet_addr(pStrIP);
	}
	/// returns the address in dotted-decimal format
	char* DottedDecimal() { return inet_ntoa(sin_addr); }
	unsigned short GetPort() const { return ntohs(sin_port); }
	unsigned long GetIpAddr() const { return ntohl(sin_addr.s_addr); }
	/// operators added for efficiency
	const InetAddr& operator=(const sockaddr& sa) {
		memcpy(this, &sa, sizeof(sockaddr));
		return *this;
	}
	const InetAddr& operator=(const sockaddr_in& sin) {
		memcpy(this, &sin, sizeof(sockaddr_in));
		return *this;
	}
	operator sockaddr() { return *((sockaddr *)this); }
	operator sockaddr *() { return (sockaddr *)this; }
	operator sockaddr_in*() { return (sockaddr_in*) this; }
};

typedef const InetAddr*		LPCINETADDR;
typedef InetAddr*			LPINETADDR;


//--------------------------------------------------------------------------------
/// Socket constants.
#define SOCKET_TIMEOUT      SOCKET_ERROR - 1
#define NO_TIMEOUT          0xFFFF
#define OF_TCP_DEFAULT_TIMEOUT     NO_TIMEOUT


//--------------------------------------------------------------------------------
/// Implementation of a TCP socket.
class ofxTCPManager
{
public:
	ofxTCPManager();
	virtual ~ofxTCPManager() {
		if ((m_hSocket)&&(m_hSocket != INVALID_SOCKET)) Close();
	};

	bool Close();
	bool Create();
	bool Listen(int iMaxConnections);
	bool Connect(const char *pAddrStr, unsigned short usPort);
	bool Bind(unsigned short usPort);
	bool Accept(ofxTCPManager& sock);
	//sends the data, but it is not guaranteed that really all data will be sent
	int  Send(const char* pBuff, const int iSize);
	//all data will be sent guaranteed.
	int  SendAll(const char* pBuff, const int iSize);
	int  PeekReceive(char* pBuff, const int iSize);
	int  Receive(char* pBuff, const int iSize);
	int  ReceiveAll(char* pBuff, const int iSize);
	int  Write(const char* pBuff, const int iSize);
	bool GetRemoteAddr(LPINETADDR pIntAddr);
	bool GetInetAddr(LPINETADDR pInetAddr);
	void SetTimeoutConnect(int timeoutInSeconds);
    void SetTimeoutSend(int timeoutInSeconds);
	void SetTimeoutReceive(int timeoutInSeconds);
	void SetTimeoutAccept(int timeoutInSeconds);
	int  GetTimeoutConnect();
    int  GetTimeoutSend();
	int  GetTimeoutReceive();
	int  GetTimeoutAccept();
	bool SetReceiveBufferSize(int sizeInByte);
	bool SetSendBufferSize(int sizeInByte);
	int  GetReceiveBufferSize();
	int  GetSendBufferSize();
	int  GetMaxConnections();
	bool SetNonBlocking(bool useNonBlocking);
    bool IsNonBlocking();
	bool CheckHost(const char *pAddrStr);
	void CleanUp();

	// Tries to detect half open connection http://stackoverflow.com/a/283387
	bool CheckIsConnected();


private:
	// private copy so this can't be copied to avoid problems with destruction
	ofxTCPManager(const ofxTCPManager & mom){};
	ofxTCPManager & operator=(const ofxTCPManager & mom){return *this;}
	int WaitReceive(time_t timeoutSeconds, time_t timeoutMillis);
	int WaitSend(time_t timeoutSeconds, time_t timeoutMillis);

  int m_iListenPort;
  int m_iMaxConnections;

  #ifdef TARGET_WIN32
    SOCKET m_hSocket;
  #else
	int m_hSocket;
  #endif

  unsigned long m_dwTimeoutConnect;
  unsigned long m_dwTimeoutSend;
  unsigned long m_dwTimeoutReceive;
  unsigned long m_dwTimeoutAccept;
  bool nonBlocking;
  static bool m_bWinsockInit;
  bool m_closing;
};
