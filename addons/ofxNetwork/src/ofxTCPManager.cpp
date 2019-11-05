#include "ofxTCPManager.h"
#include <stdio.h>
#include "ofxNetworkUtils.h"
#include "ofUtils.h"

//--------------------------------------------------------------------------------
bool ofxTCPManager::m_bWinsockInit= false;

//--------------------------------------------------------------------------------
ofxTCPManager::ofxTCPManager()
{
  // was winsock initialized?
  	#ifdef TARGET_WIN32
		if (!m_bWinsockInit) {
			unsigned short vr;
			WSADATA	wsaData;
			vr = MAKEWORD(2,	2);
			WSAStartup(vr, &wsaData);
			m_bWinsockInit=	true;
		}
	#else
		//this disables the other apps from shutting down if the client
		//or server disconnects.
		signal(SIGPIPE,SIG_IGN);
		signal(EPIPE,SIG_IGN);
	#endif

  nonBlocking = false;
  m_hSocket= INVALID_SOCKET;
  m_dwTimeoutSend= OF_TCP_DEFAULT_TIMEOUT;
  m_dwTimeoutReceive= OF_TCP_DEFAULT_TIMEOUT;
  m_dwTimeoutAccept= OF_TCP_DEFAULT_TIMEOUT;
  m_dwTimeoutConnect= 5;//OF_TCP_DEFAULT_TIMEOUT;
  m_iListenPort= -1;
  m_closing = false;
  m_iMaxConnections = 100;
};

//--------------------------------------------------------------------------------
/// Closes an open socket.
/// NOTE: A closed socket cannot be reused again without a call to "Create()".
bool ofxTCPManager::Close()
{
    if (m_hSocket == INVALID_SOCKET) return(true);

	#ifdef TARGET_WIN32
		if(closesocket(m_hSocket) == SOCKET_ERROR)
	#else
		m_closing = true;
		shutdown(m_hSocket,SHUT_RDWR);
		if(close(m_hSocket) == SOCKET_ERROR)
	#endif
		{
			//	if it's reported we're not/no longer a socket, let it fall through and be invalidated
			int Error = ofxNetworkCheckError();
			if ( Error != OFXNETWORK_ERROR(NOTSOCK) )
			{
				return(false);
			}
		}

	m_hSocket= INVALID_SOCKET;

	#ifdef TARGET_WIN32
		//This was commented out in the original
		//WSACleanup();
	#endif
	return(true);
}

//--------------------------------------------------------------------------------
void ofxTCPManager::CleanUp() {
	#ifdef TARGET_WIN32
		WSACleanup();
	#endif
  m_bWinsockInit = false;
}

//--------------------------------------------------------------------------------
bool ofxTCPManager::CheckIsConnected(){
#ifdef TARGET_WIN32
	fd_set fd;
	FD_ZERO(&fd);
	FD_SET(m_hSocket, &fd);
	timeval tv = { (time_t)0, 1 };
	if (select(0, &fd, NULL, NULL, &tv) == 1) {
		int so_error;
		socklen_t len = sizeof so_error;
		getsockopt(m_hSocket, SOL_SOCKET, SO_ERROR, (char*)&so_error, &len);
		if (so_error == 0) {
			u_long toread;
#ifdef TARGET_WIN32
			ioctlsocket(m_hSocket, FIONREAD, &toread);
#else
			ioctl(m_hSocket, FIONREAD, &toread);
#endif
			if (toread == 0) {
				return false;
			}
		}
    }
    return true;
#else
    bool wasBlocking = nonBlocking;
    SetNonBlocking(false);
    struct timeval timeout;
    timeout.tv_sec = 0;
    timeout.tv_usec = 1;
    if(setsockopt(m_hSocket, SOL_SOCKET, SO_RCVTIMEO, (char*)&timeout, sizeof(timeout))<0){
        return true;
    }
    char buffer;
    int ret = recv(m_hSocket, &buffer, 1, MSG_PEEK);
    SetNonBlocking(wasBlocking);
    timeout.tv_sec = 0;
    timeout.tv_usec = 0;
    setsockopt(m_hSocket, SOL_SOCKET, SO_RCVTIMEO, (char*)&timeout, sizeof(timeout));
    return ret!=0;
#endif
}

//--------------------------------------------------------------------------------
bool ofxTCPManager::Create()
{
	if (m_hSocket != INVALID_SOCKET) return(false);
	m_closing = false;

	m_hSocket = socket( AF_INET, SOCK_STREAM, IPPROTO_IP);

	bool ret = (m_hSocket != INVALID_SOCKET);

	if(!ret) ofxNetworkCheckError();

	return ret;
}


//--------------------------------------------------------------------------------
bool ofxTCPManager::Listen(int iMaxConnections)
{
	if (m_hSocket == INVALID_SOCKET) return(false);
	m_iMaxConnections = iMaxConnections;
	bool ret = (listen(m_hSocket, iMaxConnections)!= SOCKET_ERROR);
	if(!ret) ofxNetworkCheckError();
	return ret;
}

bool ofxTCPManager::Bind(unsigned short usPort, bool bReuse)
{
	struct sockaddr_in local;
	memset(&local, 0, sizeof(sockaddr_in));

	local.sin_family = AF_INET;
	local.sin_addr.s_addr = INADDR_ANY;
	//Port MUST be in Network Byte Order
	local.sin_port = htons(usPort);

	if (bReuse) {
		int enable = 1;
		if (setsockopt(m_hSocket,SOL_SOCKET,SO_REUSEADDR,(char*)&enable,sizeof(int)) < 0){
			ofxNetworkCheckError();
			return false;
		}
	}

	if (::bind(m_hSocket,(struct sockaddr*)&local,sizeof(local))){
		ofxNetworkCheckError();
		return false;
	}
	return true;
}

//--------------------------------------------------------------------------------
bool ofxTCPManager::Accept(ofxTCPManager& sConnect)
{
  sockaddr_in addr;

  #ifndef TARGET_WIN32
	socklen_t iSize;
  #else
	int iSize;
  #endif

  if (m_hSocket == INVALID_SOCKET) return(false);

  if (m_dwTimeoutAccept != NO_TIMEOUT) {
	  fd_set fd;
	  FD_ZERO(&fd);
	  FD_SET(m_hSocket, &fd);
	  timeval tv= {(time_t)m_dwTimeoutAccept, 0};
	  if(select(0, &fd, NULL, NULL, &tv) == 0) {
		  ofxNetworkCheckError();
		  return(false);
	  }
  }

  iSize= sizeof(sockaddr_in);
  sConnect.m_hSocket = accept(m_hSocket, (sockaddr*)&addr, &iSize);
  bool ret = (sConnect.m_hSocket != INVALID_SOCKET);
  if(!ret && !m_closing) ofxNetworkCheckError();
  return ret;
}


//--------------------------------------------------------------------------------
bool ofxTCPManager::Connect(const char *pAddrStr, unsigned short usPort)
{
  sockaddr_in addr_in= {0};
  struct hostent *he;

  if (m_hSocket == INVALID_SOCKET){
	  return(false);
  }

  if ((he = gethostbyname(pAddrStr)) == NULL)
    return(false);

	addr_in.sin_family= AF_INET; // host byte order
	addr_in.sin_port  = htons(usPort); // short, network byte order
	addr_in.sin_addr  = *((struct in_addr *)he->h_addr);

	// set to non-blocking before connect
    bool wasBlocking = nonBlocking;
	if(m_dwTimeoutConnect != NO_TIMEOUT){
		SetNonBlocking(true);
	}

    int ret = connect(m_hSocket, (sockaddr *)&addr_in, sizeof(sockaddr));
    int err = 0;
    if(ret<0) err = ofxNetworkCheckError();
    // set a timeout
    if (ret < 0 && (err == OFXNETWORK_ERROR(INPROGRESS) || err == OFXNETWORK_ERROR(WOULDBLOCK)) && m_dwTimeoutConnect != NO_TIMEOUT) {
		ret = WaitSend(m_dwTimeoutConnect, 0);
		if(ret == 0) {
			socklen_t len = sizeof err;
			if (getsockopt(m_hSocket, SOL_SOCKET, SO_ERROR, (char*)&err, &len)<0){
				ret = SOCKET_ERROR;
			}else if(err != 0) {
				ret = SOCKET_ERROR;
            } 
		}
    }

	if(m_dwTimeoutConnect != NO_TIMEOUT){
		SetNonBlocking(wasBlocking);
	}
    
	return ret>=0;
}

//--------------------------------------------------------------------------------
int ofxTCPManager::WaitReceive(time_t timeoutSeconds, time_t timeoutMicros){
	if (m_hSocket == INVALID_SOCKET) return SOCKET_ERROR;

	fd_set fd;
	FD_ZERO(&fd);
	FD_SET(m_hSocket, &fd);
	timeval	tv;
	tv.tv_sec = timeoutSeconds;
	tv.tv_usec = timeoutMicros;
	auto ret = select(m_hSocket+1,&fd,NULL,NULL,&tv);
	if(ret == 0){
		return SOCKET_TIMEOUT;
	}else if(ret < 0){
		return SOCKET_ERROR;
	}else{
		return 0;
	}
}

//--------------------------------------------------------------------------------
int ofxTCPManager::WaitSend(time_t timeoutSeconds, time_t timeoutMicros){
	if (m_hSocket == INVALID_SOCKET) return SOCKET_ERROR;

	fd_set fd;
	FD_ZERO(&fd);
	FD_SET(m_hSocket, &fd);
	timeval	tv;
	tv.tv_sec = timeoutSeconds;
	tv.tv_usec = timeoutMicros;
	auto ret = select(m_hSocket+1,NULL,&fd,NULL,&tv);
	if(ret == 0){
		return SOCKET_TIMEOUT;
	}else if(ret < 0){
		return SOCKET_ERROR;
	}else{
		return 0;
	}
}

//--------------------------------------------------------------------------------
bool ofxTCPManager::SetNonBlocking(bool useNonBlocking)
{
	//if(useNonBlocking==nonBlocking){
		//return true;
	//}
    auto prevNonBlocking = nonBlocking;
    nonBlocking = useNonBlocking;

	#ifdef TARGET_WIN32
		unsigned long arg = nonBlocking;
		int retVal = ioctlsocket(m_hSocket,FIONBIO,&arg);
	#else
		int flags = fcntl(m_hSocket, F_GETFL, 0);
		int retVal;
		if(useNonBlocking){
			retVal = fcntl(m_hSocket, F_SETFL, flags | O_NONBLOCK);
		}else{
			retVal = fcntl(m_hSocket, F_SETFL, flags & ~O_NONBLOCK);
		}
	#endif

	bool ret = (retVal >= 0);
	if(!ret){
		ofxNetworkCheckError();
		nonBlocking = prevNonBlocking;
	}

	return ret;
}

bool ofxTCPManager::IsNonBlocking(){
    return nonBlocking;
}

//--------------------------------------------------------------------------------
int ofxTCPManager::Write(const char* pBuff, const int iSize)
{
	int iBytesSent= 0;
	int iBytesTemp;
	const char* pTemp= pBuff;

	do {
		iBytesTemp= Send(pTemp, iSize - iBytesSent);
		// error occured?
		if (iBytesTemp == SOCKET_ERROR) return(SOCKET_ERROR);
		if (iBytesTemp == SOCKET_TIMEOUT) return(SOCKET_TIMEOUT);

		iBytesSent+= iBytesTemp;
		pTemp+= iBytesTemp;
	} while(iBytesSent < iSize);

	return(iBytesSent);
}

//--------------------------------------------------------------------------------
/// Return values:
/// SOCKET_TIMEOUT indicates timeout
/// SOCKET_ERROR in case of a problem.
int ofxTCPManager::Send(const char* pBuff, const int iSize)
{
    if (m_hSocket == INVALID_SOCKET) return(SOCKET_ERROR);

    if (m_dwTimeoutSend	!= NO_TIMEOUT){
		auto ret = WaitSend(m_dwTimeoutSend,0);
		if(ret!=0){
			return ret;
		}
	}
	return send(m_hSocket, pBuff, iSize, 0);
}

//--------------------------------------------------------------------------------
/// Return values:
/// SOCKET_TIMEOUT indicates timeout
/// SOCKET_ERROR in case of a problem.
int ofxTCPManager::SendAll(const char* pBuff, const int iSize)
{
	if (m_hSocket == INVALID_SOCKET) return(SOCKET_ERROR);

	auto timestamp = ofGetElapsedTimeMicros();
	auto timeleftSecs = m_dwTimeoutSend;
	auto timeleftMicros = 0;
	int total= 0;
	int bytesleft = iSize;
	int ret=-1;

	while (total < iSize) {
		if (m_dwTimeoutSend	!= NO_TIMEOUT){
			auto ret = WaitSend(timeleftSecs,timeleftMicros);
			if(ret!=0){
				return ret;
			}
		}
		ret = send(m_hSocket, pBuff + total, bytesleft, 0);
		if (ret == SOCKET_ERROR) {
			return SOCKET_ERROR;
		}
		total += ret;
		bytesleft -=ret;
		if (m_dwTimeoutSend	!= NO_TIMEOUT){
			auto now = ofGetElapsedTimeMicros();
			auto diff = now - timestamp;
			if (diff > m_dwTimeoutSend * 1000000){
				return SOCKET_TIMEOUT;
			}
			float timeFloat = m_dwTimeoutSend - diff/1000000.;
			timeleftSecs = timeFloat;
			timeleftMicros = (timeFloat - timeleftSecs) * 1000000;
		}
	}

	return total;
}


//--------------------------------------------------------------------------------
/// Return values:
/// SOCKET_TIMEOUT indicates timeout
/// SOCKET_ERROR in case of a problem.
///
int ofxTCPManager::Receive(char* pBuff, const int iSize)
{
    if (m_hSocket == INVALID_SOCKET) return(SOCKET_ERROR);

	if (m_dwTimeoutReceive	!= NO_TIMEOUT){
		auto ret = WaitReceive(m_dwTimeoutReceive,0);
		if(ret!=0){
			return ret;
		}
	}
	return recv(m_hSocket, pBuff, iSize, 0);
}



//--------------------------------------------------------------------------------
/// Return values:
/// SOCKET_TIMEOUT indicates timeout
/// SOCKET_ERROR in case of a problem.
///
int ofxTCPManager::PeekReceive(char* pBuff, const int iSize)
{
	if (m_hSocket == INVALID_SOCKET) return(SOCKET_ERROR);

	if (m_dwTimeoutReceive	!= NO_TIMEOUT){
		auto ret = WaitReceive(m_dwTimeoutReceive,0);
		if(ret!=0){
			return ret;
		}
	}
 
	return recv(m_hSocket, pBuff, iSize, MSG_PEEK);
}

//--------------------------------------------------------------------------------
/// Return values:
/// SOCKET_TIMEOUT indicates timeout
/// SOCKET_ERROR in case of a problem.
int ofxTCPManager::ReceiveAll(char* pBuff, const int iSize)
{
	if (m_hSocket == INVALID_SOCKET) return(SOCKET_ERROR);

	auto timestamp = ofGetElapsedTimeMicros();
	auto timeleftSecs = m_dwTimeoutReceive;
	auto timeleftMicros = 0;
	int totalBytes=0;

	do {
		if (m_dwTimeoutReceive	!= NO_TIMEOUT){
			auto ret = WaitReceive(timeleftSecs, timeleftMicros);
			if(ret!=0){
				return ret;
			}
		}
		int ret = recv(m_hSocket, pBuff+totalBytes, iSize-totalBytes, 0);
		if (ret==0 && totalBytes != iSize){
			return SOCKET_ERROR;
		}
		if (ret < 0){
			return SOCKET_ERROR;
		}
		totalBytes += ret;

		if (m_dwTimeoutReceive	!= NO_TIMEOUT){
			auto now = ofGetElapsedTimeMicros();
			auto diff = now - timestamp;
			if(diff > m_dwTimeoutReceive){
				return SOCKET_TIMEOUT;
			}
			float timeFloat = m_dwTimeoutSend - diff/1000000.;
			timeleftSecs = timeFloat;
			timeleftMicros = (timeFloat - timeleftSecs) * 1000000;
		}
	}while(totalBytes < iSize);

	return totalBytes;
}

//--------------------------------------------------------------------------------
bool ofxTCPManager::GetRemoteAddr(LPINETADDR pInetAddr)
{
  if (m_hSocket == INVALID_SOCKET) return(false);

	#ifndef TARGET_WIN32
		socklen_t iSize;
	#else
		int iSize;
	#endif

	iSize= sizeof(sockaddr);
	bool ret = (getpeername(m_hSocket, (sockaddr *)pInetAddr, &iSize) != SOCKET_ERROR);
	if(!ret) ofxNetworkCheckError();
	return ret;
}

//--------------------------------------------------------------------------------
bool ofxTCPManager::GetInetAddr(LPINETADDR pInetAddr)
{
  if (m_hSocket == INVALID_SOCKET) return(false);

	#ifndef TARGET_WIN32
		socklen_t iSize;
	#else
		int iSize;
	#endif

	iSize= sizeof(sockaddr);
	bool ret = (getsockname(m_hSocket, (sockaddr *)pInetAddr, &iSize) != SOCKET_ERROR);
	if(!ret) ofxNetworkCheckError();
	return ret;
}

void ofxTCPManager::SetTimeoutConnect(int timeoutInSeconds) {
	m_dwTimeoutConnect= timeoutInSeconds;
}
void ofxTCPManager::SetTimeoutSend(int timeoutInSeconds) {
	m_dwTimeoutSend= timeoutInSeconds;
}
void ofxTCPManager::SetTimeoutReceive(int timeoutInSeconds) {
	m_dwTimeoutReceive= timeoutInSeconds;
}
void ofxTCPManager::SetTimeoutAccept(int timeoutInSeconds) {
	m_dwTimeoutAccept= timeoutInSeconds;
}
int ofxTCPManager::GetTimeoutConnect() {
	return m_dwTimeoutConnect;
}
int ofxTCPManager::GetTimeoutSend() {
	return m_dwTimeoutSend;
}
int ofxTCPManager::GetTimeoutReceive() {
	return m_dwTimeoutReceive;
}
int ofxTCPManager::GetTimeoutAccept() {
	return m_dwTimeoutAccept;
}

int ofxTCPManager::GetReceiveBufferSize() {
	if (m_hSocket == INVALID_SOCKET) return(false);

	#ifndef TARGET_WIN32
		socklen_t size;
	#else
		int size;
	#endif

	int sizeBuffer=0;
	size = sizeof(int);
	int ret = getsockopt(m_hSocket, SOL_SOCKET, SO_RCVBUF, (char*)&sizeBuffer, &size);
	if(ret==-1) ofxNetworkCheckError();
	return sizeBuffer;
}

bool ofxTCPManager::SetReceiveBufferSize(int sizeInByte) {
	if (m_hSocket == INVALID_SOCKET) return(false);

	if ( setsockopt(m_hSocket, SOL_SOCKET, SO_RCVBUF, (char*)&sizeInByte, sizeof(sizeInByte)) == 0){
		return true;
	}else{
		 ofxNetworkCheckError();
		return false;
	}
}

int ofxTCPManager::GetSendBufferSize() {
	if (m_hSocket == INVALID_SOCKET) return(false);

	#ifndef TARGET_WIN32
		socklen_t size;
	#else
		int size;
	#endif

	int sizeBuffer=0;
	size = sizeof(int);
	int ret = getsockopt(m_hSocket, SOL_SOCKET, SO_SNDBUF, (char*)&sizeBuffer, &size);
	if(ret==-1) ofxNetworkCheckError();
	return sizeBuffer;
}

bool ofxTCPManager::SetSendBufferSize(int sizeInByte) {
	if (m_hSocket == INVALID_SOCKET) return(false);

	if ( setsockopt(m_hSocket, SOL_SOCKET, SO_SNDBUF, (char*)&sizeInByte, sizeof(sizeInByte)) == 0){
		return true;
	}else{
		ofxNetworkCheckError();
		return false;
	}
}

int ofxTCPManager::GetMaxConnections() {
  return m_iMaxConnections;
}

bool ofxTCPManager::CheckHost(const char *pAddrStr) {
  hostent * hostEntry;
  in_addr iaHost;
  iaHost.s_addr = inet_addr(pAddrStr);
  hostEntry = gethostbyaddr((const char *)&iaHost, sizeof(struct in_addr), AF_INET);
  return ((!hostEntry) ? false : true);
}

