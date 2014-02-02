/*
 * ofxNetworkUtils.h
 *
 *  Created on: 25/09/2010
 *      Author: arturo
 */

#pragma once

#include <cerrno>
#include "ofConstants.h"
#include "ofUtils.h"

#ifdef TARGET_WIN32
#define ENOTCONN        WSAENOTCONN
#define EWOULDBLOCK     WSAEWOULDBLOCK
#define ENOBUFS         WSAENOBUFS
#define ECONNRESET      WSAECONNRESET
#define ESHUTDOWN       WSAESHUTDOWN
#define EAFNOSUPPORT    WSAEAFNOSUPPORT
#define EPROTONOSUPPORT WSAEPROTONOSUPPORT
#define EINPROGRESS     WSAEINPROGRESS
#define EISCONN         WSAEISCONN
#define ENOTSOCK        WSAENOTSOCK
#define EOPNOTSUPP      WSAEOPNOTSUPP
#define ETIMEDOUT       WSAETIMEDOUT
#define EADDRNOTAVAIL   WSAEADDRNOTAVAIL
#define ECONNREFUSED    WSAECONNREFUSED
#define ENETUNREACH     WSAENETUNREACH
#define EADDRINUSE      WSAEADDRINUSE
#define EADDRINUSE      WSAEADDRINUSE
#define EALREADY        WSAEALREADY
#define ENOPROTOOPT     WSAENOPROTOOPT
#define EMSGSIZE        WSAEMSGSIZE
#define ECONNABORTED    WSAECONNABORTED
#endif


#define ofxNetworkCheckError() ofxNetworkCheckErrno(__FILE__,ofToString(__LINE__))

inline int ofxNetworkCheckErrno(const string & file, const string & line){
	#ifdef TARGET_WIN32
		int	err	= WSAGetLastError();
	#else
		int err = errno;
	#endif
	switch(err){
	case 0:
		break;
	case EBADF:
		ofLogError("ofxNetwork") << file << ": " << line << " EBADF: invalid socket";
		break;
	case ECONNRESET:
		ofLogError("ofxNetwork") << file << ": " << line << " ECONNRESET: connection closed by peer";
		break;
	case EINTR:
		ofLogError("ofxNetwork") << file << ": " << line << " EINTR: receive interrupted by a signal, before any data available";
		break;
	case ENOTCONN:
		ofLogError("ofxNetwork") << file << ": " << line << " ENOTCONN: trying to receive before establishing a connection";
		break;
	case ENOTSOCK:
		ofLogError("ofxNetwork") << file << ": " << line << " ENOTSOCK: socket argument is not a socket";
		break;
	case EOPNOTSUPP:
		ofLogError("ofxNetwork") << file << ": " << line << " EOPNOTSUPP: specified flags not valid for this socket";
		break;
	case ETIMEDOUT:
		ofLogError("ofxNetwork") << file << ": " << line << " ETIMEDOUT: timeout";
		break;
	case EIO:
		ofLogError("ofxNetwork") << file << ": " << line << " EIO: io error";
		break;
	case ENOBUFS:
		ofLogError("ofxNetwork") << file << ": " << line << " ENOBUFS: insufficient buffers to complete the operation";
		break;
	case ENOMEM:
		ofLogError("ofxNetwork") << file << ": " << line << " ENOMEM: insufficient memory to complete the request";
		break;
	case EADDRNOTAVAIL:
		ofLogError("ofxNetwork") << file << ": " << line << " EADDRNOTAVAIL: the specified address is not available on the remote machine";
		break;
	case EAFNOSUPPORT:
		ofLogError("ofxNetwork") << file << ": " << line << " EAFNOSUPPORT: the namespace of the addr is not supported by this socket";
		break;
	case EISCONN:
		ofLogError("ofxNetwork") << file << ": " << line << " EISCONN: the socket is already connected";
		break;
	case ECONNREFUSED:
		ofLogError("ofxNetwork") << file << ": " << line << " ECONNREFUSED: the server has actively refused to establish the connection";
		break;
	case ENETUNREACH:
		ofLogError("ofxNetwork") << file << ": " << line << " ENETUNREACH: the network of the given addr isn't reachable from this host";
		break;
	case EADDRINUSE:
		ofLogError("ofxNetwork") << file << ": " << line << " EADDRINUSE: the socket address of the given addr is already in use";
		break;
	case EINPROGRESS:
		ofLogWarning("ofxNetwork") << file << ": " << line << " EINPROGRESS: the socket is non-blocking and the connection could not be established immediately";
		break;
	case EALREADY:
		ofLogError("ofxNetwork") << file << ": " << line << " EALREADY: the socket is non-blocking and already has a pending connection in progress";
		break;
	case ENOPROTOOPT:
		ofLogError("ofxNetwork") << file << ": " << line << " ENOPROTOOPT: the optname doesn't make sense for the given level";
		break;
	case EPROTONOSUPPORT:
		ofLogError("ofxNetwork") << file << ": " << line << " EPROTONOSUPPORT: the protocol or style is not supported by the namespace specified";
		break;
	case EMFILE:
		ofLogError("ofxNetwork") << file << ": " << line << " EMFILE: the process already has too many file descriptors open";
		break;
	case ENFILE:
		ofLogError("ofxNetwork") << file << ": " << line << " ENFILE: the system already has too many file descriptors open";
		break;
	case EACCES:
		ofLogError("ofxNetwork") << file << ": " << line << " EACCES: the process does not have the privilege to create a socket of the specified style or protocol";
		break;
	case EMSGSIZE:
		ofLogError("ofxNetwork") << file << ": " << line << " EMSGSIZE: the socket type requires that the message be sent atomically, but the message is too large for this to be possible";
		break;
	case EPIPE:
		ofLogError("ofxNetwork") << file << ": " << line << " EPIPE: this socket was connected but the connection is now broken";
		break;
	case EINVAL:
		ofLogError("ofxNetwork") << file << ": " << line << " EINVAL: invalid argument";
		break;
	case EAGAIN:
		//ofLogError("ofxNetwork") << file << ": " << line << " EAGAIN: try again";
		break;
#ifdef TARGET_WIN32
	case WSAEWOULDBLOCK:
		// represents "resource temporarily unavailable", can be ignored
		break;
#endif
	default:
		ofLogError("ofxNetwork") << file << ": " << line << " unknown error: " << err << " see errno.h for description of the error";
		break;
	}

	return err;
}
