#define _CRT_SECURE_NO_DEPRECATE

#include "ofxUDPManager.h"
#include "ofxNetworkUtils.h"

//--------------------------------------------------------------------------------
bool ofxUDPManager::m_bWinsockInit= false;

//--------------------------------------------------------------------------------
ofxUDPManager::ofxUDPManager()
{
	// was winsock initialized?
	#ifdef TARGET_WIN32
		if (!m_bWinsockInit) {
			unsigned short vr;
			WSADATA	wsaData;
			vr=	MAKEWORD(2,	2);
			WSAStartup(vr, &wsaData);
			m_bWinsockInit=	true;
		}
	#endif

	m_hSocket= INVALID_SOCKET;
	m_dwTimeoutReceive=	OF_UDP_DEFAULT_TIMEOUT;
	m_iListenPort= -1;

	canGetRemoteAddress	= false;
	nonBlocking			= true;

};

//--------------------------------------------------------------------------------
///	Closes an open socket.
///	NOTE: A	closed socket cannot be	reused again without a call	to "Create()".
bool ofxUDPManager::Close()
{
	if (m_hSocket == INVALID_SOCKET)
		return(false);

	#ifdef TARGET_WIN32
		if(closesocket(m_hSocket) == SOCKET_ERROR)
	#else
		if(close(m_hSocket) == SOCKET_ERROR)
	#endif
	{
		ofxNetworkCheckError();
		return(false);
	}
	m_hSocket= INVALID_SOCKET;

	return(true);
}

//--------------------------------------------------------------------------------
bool ofxUDPManager::Create()
{
	if (m_hSocket != INVALID_SOCKET)
		return(false);
	m_hSocket =	socket(AF_INET,	SOCK_DGRAM,	0);
	if (m_hSocket != INVALID_SOCKET)
	{
		int unused = true;
		setsockopt(m_hSocket, SOL_SOCKET, SO_REUSEADDR, (char*)&unused, sizeof(unused));
	}
	bool ret = m_hSocket !=	INVALID_SOCKET;
	if(!ret) ofxNetworkCheckError();
	return ret;
}

//--------------------------------------------------------------------------------
///Theo added - Choose to set nonBLocking - default mode is to block
bool ofxUDPManager::SetNonBlocking(bool useNonBlocking)
{
	nonBlocking		= useNonBlocking;

	#ifdef TARGET_WIN32
		unsigned long arg = nonBlocking;
		int retVal = ioctlsocket(m_hSocket,FIONBIO,&arg);
	#else
		int arg			= nonBlocking;
		int retVal = ioctl(m_hSocket,FIONBIO,&arg);
	#endif

	bool ret=(retVal >= 0);
	if(!ret) ofxNetworkCheckError();
	return ret;
}

//--------------------------------------------------------------------------------
bool ofxUDPManager::Bind(unsigned short usPort)
{
	saServer.sin_family	= AF_INET;
	saServer.sin_addr.s_addr = INADDR_ANY;
	//Port MUST	be in Network Byte Order
	saServer.sin_port =	htons(usPort);

	int	ret	= bind(m_hSocket,(struct sockaddr*)&saServer,sizeof(struct sockaddr));
	if(ret==-1)  ofxNetworkCheckError();

	return (ret	== 0);
}

//--------------------------------------------------------------------------------
bool ofxUDPManager::BindMcast(char *pMcast, unsigned short usPort)
{
	// bind to port
	if (!Bind(usPort))
	{
		ofLog(OF_LOG_WARNING, "can't bind to port \n");
		return false;
	}

	// join the multicast group
	struct ip_mreq mreq;
	mreq.imr_multiaddr.s_addr = inet_addr(pMcast);
	mreq.imr_interface.s_addr = INADDR_ANY;

	if (setsockopt(m_hSocket, IPPROTO_IP, IP_ADD_MEMBERSHIP, (char FAR*) &mreq, sizeof (mreq)) == SOCKET_ERROR)
	{
		ofxNetworkCheckError();
		return false;
	}

	// multicast bind successful
	return true;
}

//--------------------------------------------------------------------------------
bool ofxUDPManager::Connect(const char *pHost, unsigned short usPort)
{
	//	sockaddr_in	addr_in= {0};
	memset(&saClient, 0, sizeof(sockaddr_in));
	struct hostent *he;

	if (m_hSocket == INVALID_SOCKET) return(false);

	if ((he	= gethostbyname(pHost))	== NULL)
		return(false);

	saClient.sin_family= AF_INET; // host byte order
	saClient.sin_port  = htons(usPort);	// short, network byte order
	//	saClient.sin_addr  = *((struct g_addr *)he->h_addr_list);
	//cout << inet_addr( pHost ) << endl;
	//saClient.sin_addr.s_addr= inet_addr( pHost );
	//saClient.sin_addr = *((struct in_addr *)he->h_addr);
	memcpy((char *) &saClient.sin_addr.s_addr,
		 he->h_addr_list[0], he->h_length);

    memset(&(saClient.sin_zero), '\0', 8);  // zero the rest of the struct


	return true;
}

//--------------------------------------------------------------------------------
bool ofxUDPManager::ConnectMcast(char* pMcast, unsigned short usPort)
{
	// associate the source socket's address with the socket
	if (!Bind(usPort))
	{
#ifdef _DEBUG
		printf("Binding socket failed! Error: %d", WSAGetLastError());
#endif
		return false;
	}

	// set ttl to default
	if (!SetTTL(1))
	{
#ifdef _DEBUG
		printf("SetTTL failed. Continue anyway. Error: %d", WSAGetLastError());
#endif
	}

	if (!Connect(pMcast, usPort))
	{
#ifdef _DEBUG
		printf("Connecting socket failed! Error: %d", WSAGetLastError ());
#endif
		return false;
	}

	// multicast connect successful
	return true;
}

//--------------------------------------------------------------------------------
///	Return values:
///	SOCKET_TIMEOUT indicates timeout
///	SOCKET_ERROR in	case of	a problem.
int	ofxUDPManager::Send(const char* pBuff,	const int iSize)
{
	if (m_hSocket == INVALID_SOCKET) return(SOCKET_ERROR);

	/*if (m_dwTimeoutSend	!= NO_TIMEOUT)
	{
		fd_set fd;
		FD_ZERO(&fd);
		FD_SET(m_hSocket, &fd);
		timeval	tv=	{m_dwTimeoutSend, 0};
		if(select(m_hSocket+1,NULL,&fd,NULL,&tv)== 0)
		{
			return(SOCKET_TIMEOUT);
		}
	}*/

	int ret = sendto(m_hSocket, (char*)pBuff,	iSize, 0, (sockaddr *)&saClient, sizeof(sockaddr));
	if(ret==-1) ofxNetworkCheckError();
	return ret;
	//	return(send(m_hSocket, pBuff, iSize, 0));
}

//--------------------------------------------------------------------------------
///	Return values:
///	SOCKET_TIMEOUT indicates timeout
///	SOCKET_ERROR in	case of	a problem.
int	ofxUDPManager::SendAll(const char*	pBuff, const int iSize)
{
	if (m_hSocket == INVALID_SOCKET) return(SOCKET_ERROR);

	if (m_dwTimeoutSend	!= NO_TIMEOUT)
	{
		fd_set fd;
		FD_ZERO(&fd);
		FD_SET(m_hSocket, &fd);
		timeval	tv=	{m_dwTimeoutSend, 0};
		if(select(m_hSocket+1,NULL,&fd,NULL,&tv)== 0)
		{
			ofxNetworkCheckError();
			return(SOCKET_TIMEOUT);
		}
	}


	int	total= 0;
	int	bytesleft =	iSize;
	int	n=0;

	while (total < iSize)
	{
		n =	sendto(m_hSocket, (char*)pBuff,	iSize, 0, (sockaddr *)&saClient, sizeof(sockaddr));
		if (n == -1)
			{
				ofxNetworkCheckError();
				break;
			}
		total += n;
		bytesleft -=n;
	}

	return n==-1?SOCKET_ERROR:total;
}



//--------------------------------------------------------------------------------
///	Return values:
///	SOCKET_TIMEOUT indicates timeout
///	SOCKET_ERROR in	case of	a problem.
int	ofxUDPManager::Receive(char* pBuff, const int iSize)
{
	if (m_hSocket == INVALID_SOCKET){
		printf("INVALID_SOCKET");
		return(SOCKET_ERROR);

	}

	/*if (m_dwTimeoutSend	!= NO_TIMEOUT)
	{
		fd_set fd;
		FD_ZERO(&fd);
		FD_SET(m_hSocket, &fd);
		timeval	tv=	{m_dwTimeoutSend, 0};
		if(select(m_hSocket+1,&fd,NULL,NULL,&tv)== 0)
		{
			return(SOCKET_TIMEOUT);
		}
	}*/

	#ifndef TARGET_WIN32
		socklen_t nLen= sizeof(sockaddr);
	#else
		int	nLen= sizeof(sockaddr);
	#endif

	int	ret=0;

	memset(pBuff, 0, iSize);
	ret= recvfrom(m_hSocket, pBuff,	iSize, 0, (sockaddr *)&saClient, &nLen);

	if (ret	> 0)
	{
				//printf("\nreceived from: %s\n",	inet_ntoa((in_addr)saClient.sin_addr));
		canGetRemoteAddress= true;
	}
	else
	{
		ofxNetworkCheckError();
				//printf("\nreceived from: ????\n");
		canGetRemoteAddress= false;
	}

	return ret;
	//	return(recvfrom(m_hSocket, pBuff, iSize, 0));
}

void ofxUDPManager::SetTimeoutSend(int	timeoutInSeconds)
{
	m_dwTimeoutSend= timeoutInSeconds;
}
void ofxUDPManager::SetTimeoutReceive(int timeoutInSeconds)
{
	m_dwTimeoutReceive=	timeoutInSeconds;
}
int	ofxUDPManager::GetTimeoutSend()
{
	return m_dwTimeoutSend;
}
int	ofxUDPManager::GetTimeoutReceive()
{
	return m_dwTimeoutReceive;
}

//--------------------------------------------------------------------------------
bool ofxUDPManager::GetRemoteAddr(char* address)
{
	if (m_hSocket == INVALID_SOCKET) return(false);
	if ( canGetRemoteAddress ==	false) return (false);

	strcpy(address,	inet_ntoa((in_addr)saClient.sin_addr));
	return true;
}

//--------------------------------------------------------------------------------
int	ofxUDPManager::GetMaxMsgSize()
{
	if (m_hSocket == INVALID_SOCKET) return(false);

	int	sizeBuffer=0;

	#ifndef TARGET_WIN32
		socklen_t size = sizeof(int);
	#else
		int size = sizeof(int);
	#endif

	int ret = getsockopt(m_hSocket, SOL_SOCKET, SO_MAX_MSG_SIZE, (char*)&sizeBuffer, &size);
	if(ret==-1) ofxNetworkCheckError();
	return sizeBuffer;
}

//--------------------------------------------------------------------------------
int	ofxUDPManager::GetReceiveBufferSize()
{
	if (m_hSocket == INVALID_SOCKET) return(false);

	int	sizeBuffer=0;

	#ifndef TARGET_WIN32
		socklen_t size = sizeof(int);
	#else
		int size = sizeof(int);
	#endif

	int ret = getsockopt(m_hSocket, SOL_SOCKET, SO_RCVBUF, (char*)&sizeBuffer, &size);
	if(ret==-1) ofxNetworkCheckError();
	return sizeBuffer;
}

//--------------------------------------------------------------------------------
bool ofxUDPManager::SetReceiveBufferSize(int sizeInByte)
{
	if (m_hSocket == INVALID_SOCKET) return(false);

	if ( setsockopt(m_hSocket, SOL_SOCKET, SO_RCVBUF, (char*)&sizeInByte, sizeof(sizeInByte)) == 0){
		return true;
	}else{
		ofxNetworkCheckError();
		return false;
	}
}

//--------------------------------------------------------------------------------
int	ofxUDPManager::GetSendBufferSize()
{
	if (m_hSocket == INVALID_SOCKET) return(false);

	int	sizeBuffer=0;

	#ifndef TARGET_WIN32
		socklen_t size = sizeof(int);
	#else
		int size = sizeof(int);
	#endif

	int ret = getsockopt(m_hSocket, SOL_SOCKET, SO_SNDBUF, (char*)&sizeBuffer, &size);
	if(ret==-1) ofxNetworkCheckError();

	return sizeBuffer;
}

//--------------------------------------------------------------------------------
bool ofxUDPManager::SetSendBufferSize(int sizeInByte)
{
	if (m_hSocket == INVALID_SOCKET) return(false);

	if ( setsockopt(m_hSocket, SOL_SOCKET, SO_SNDBUF, (char*)&sizeInByte, sizeof(sizeInByte)) == 0){
		return true;
	}else{
		ofxNetworkCheckError();
		return false;
	}
}

//--------------------------------------------------------------------------------
bool ofxUDPManager::SetReuseAddress(bool allowReuse)
{
	if (m_hSocket == INVALID_SOCKET) return(false);

	int	on;
	if (allowReuse)	on=1;
	else			on=0;

	if ( setsockopt(m_hSocket, SOL_SOCKET, SO_REUSEADDR, (char*)&on, sizeof(on)) ==	0){
		return true;
	}else{
		ofxNetworkCheckError();
		return false;
	}
}

//--------------------------------------------------------------------------------
bool ofxUDPManager::SetEnableBroadcast(bool enableBroadcast)
{
	int	on;
	if (enableBroadcast)	on=1;
	else					on=0;

	if ( setsockopt(m_hSocket, SOL_SOCKET, SO_BROADCAST, (char*)&on, sizeof(on)) ==	0){
		return true;
	}else{
		ofxNetworkCheckError();
		return false;
	}
}

//--------------------------------------------------------------------------------
int ofxUDPManager::GetTTL()
{
	if (m_hSocket == INVALID_SOCKET) return(false);

	int nTTL;

	#ifndef TARGET_WIN32
		socklen_t nSize = sizeof(int);
	#else
		int nSize = sizeof(int);
	#endif

	if (getsockopt(m_hSocket, IPPROTO_IP, IP_MULTICAST_TTL, (char FAR *) &nTTL, &nSize) == SOCKET_ERROR)
	{
#ifdef _DEBUG
		printf("getsockopt failed! Error: %d", WSAGetLastError());
#endif
		ofxNetworkCheckError();
		return -1;
	}

	return nTTL;
}

//--------------------------------------------------------------------------------
bool ofxUDPManager::SetTTL(int nTTL)
{
	if (m_hSocket == INVALID_SOCKET) return(false);

	// Set the Time-to-Live of the multicast.
	if (setsockopt(m_hSocket, IPPROTO_IP, IP_MULTICAST_TTL, (char FAR *)&nTTL, sizeof (int)) == SOCKET_ERROR)
	{
#ifdef _DEBUG
		printf("setsockopt failed! Error: %d", WSAGetLastError());
#endif
		ofxNetworkCheckError();
		return false;
	}

	return true;
}


/*
//--------------------------------------------------------------------------------
bool ofxUDPManager::GetInetAddr(LPINETADDR	pInetAddr)
{
if (m_hSocket == INVALID_SOCKET) return(false);

int	iSize= sizeof(sockaddr);
return(getsockname(m_hSocket, (sockaddr *)pInetAddr, &iSize) !=	SOCKET_ERROR);
}
*/
