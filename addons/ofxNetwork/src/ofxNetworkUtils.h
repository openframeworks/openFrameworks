/*
 * ofxNetworkUtils.h
 *
 *  Created on: 25/09/2010
 *      Author: arturo
 */

#ifndef OFXNETWORKUTILS_H_
#define OFXNETWORKUTILS_H_

#include <cerrno>
#include <ofMain.h>

#define ofxNetworkCheckError() ofxNetworkCheckErrno(__FILE__,ofToString(__LINE__))

inline int ofxNetworkCheckErrno(const string & file, const string & line){
	#ifdef TARGET_WIN32
		int	err	= WSAGetLastError();
	#else
		int err = errno;
	#endif
	switch(err){
	case EBADF:
		ofLog(OF_LOG_ERROR,"ofxNetwork:"+file+": " +line+" EBADF: invalid socket");
		break;
	case ECONNRESET:
		ofLog(OF_LOG_ERROR,"ofxNetwork:"+file+": " +line+" ECONNRESET: connection closed by peer");
		break;
	case EINTR:
		ofLog(OF_LOG_ERROR,"ofxNetwork:"+file+": " +line+" EINTR: receive interrupted by a signal, before any data available");
		break;
	case ENOTCONN:
		ofLog(OF_LOG_ERROR,"ofxNetwork:"+file+": " +line+" ENOTCONN: trying to receive before establishing a connection");
		break;
	case ENOTSOCK:
		ofLog(OF_LOG_ERROR,"ofxNetwork:"+file+": " +line+" ENOTSOCK: socket argument is not a socket");
		break;
	case EOPNOTSUPP:
		ofLog(OF_LOG_ERROR,"ofxNetwork:"+file+": " +line+" EOPNOTSUPP: specified flags not valid for this socket");
		break;
	case ETIMEDOUT:
		ofLog(OF_LOG_ERROR,"ofxNetwork:"+file+": " +line+" ETIMEDOUT: timeout");
		break;
	case EIO:
		ofLog(OF_LOG_ERROR,"ofxNetwork:"+file+": " +line+" EIO: io error");
		break;
	case ENOBUFS:
		ofLog(OF_LOG_ERROR,"ofxNetwork:"+file+": " +line+" ENOBUFS: insufficient buffers to complete the operation");
		break;
	case ENOMEM:
		ofLog(OF_LOG_ERROR,"ofxNetwork:"+file+": " +line+" ENOMEM: insufficient memory to complete the request");
		break;
	case EADDRNOTAVAIL:
		ofLog(OF_LOG_ERROR,"ofxNetwork:"+file+": " +line+" EADDRNOTAVAIL: the specified address is not available on the remote machine");
		break;
	case EAFNOSUPPORT:
		ofLog(OF_LOG_ERROR,"ofxNetwork:"+file+": " +line+" EAFNOSUPPORT: the namespace of the addr is not supported by this socket");
		break;
	case EISCONN:
		ofLog(OF_LOG_ERROR,"ofxNetwork:"+file+": " +line+" EISCONN: the socket is already connected");
		break;
	case ECONNREFUSED:
		ofLog(OF_LOG_ERROR,"ofxNetwork:"+file+": " +line+" ECONNREFUSED: the server has actively refused to establish the connection");
		break;
	case ENETUNREACH:
		ofLog(OF_LOG_ERROR,"ofxNetwork:"+file+": " +line+" ENETUNREACH: the network of the given addr isn't reachable from this host");
		break;
	case EADDRINUSE:
		ofLog(OF_LOG_ERROR,"ofxNetwork:"+file+": " +line+" EADDRINUSE: the socket address of the given addr is already in use");
		break;
	case EINPROGRESS:
		ofLog(OF_LOG_WARNING,"ofxNetwork:"+file+": " +line+" EINPROGRESS: the socket is non-blocking and the connection could not be established immediately" );
		break;
	case EALREADY:
		ofLog(OF_LOG_ERROR,"ofxNetwork:"+file+": " +line+" EALREADY: the socket is non-blocking and already has a pending connection in progress");
		break;
	case ENOPROTOOPT:
		ofLog(OF_LOG_ERROR,"ofxNetwork:"+file+": " +line+" ENOPROTOOPT: The optname doesn't make sense for the given level.");
		break;
	case EPROTONOSUPPORT:
		ofLog(OF_LOG_ERROR,"ofxNetwork:"+file+": " +line+" EPROTONOSUPPORT: The protocol or style is not supported by the namespace specified.");
		break;
	case EMFILE:
		ofLog(OF_LOG_ERROR,"ofxNetwork:"+file+": " +line+" EMFILE: The process already has too many file descriptors open.");
		break;
	case ENFILE:
		ofLog(OF_LOG_ERROR,"ofxNetwork:"+file+": " +line+" ENFILE: The system already has too many file descriptors open.");
		break;
	case EACCES:
		ofLog(OF_LOG_ERROR,"ofxNetwork:"+file+": " +line+" EACCES: The process does not have the privilege to create a socket of the specified 	 style or protocol.");
		break;
	case EMSGSIZE:
		ofLog(OF_LOG_ERROR,"ofxNetwork:"+file+": " +line+" EMSGSIZE: The socket type requires that the message be sent atomically, but the message is too large for this to be possible.");
		break;
	case EPIPE:
		ofLog(OF_LOG_ERROR,"ofxNetwork:"+file+": " +line+" EPIPE: This socket was connected but the connection is now broken.");
		break;
	case EAGAIN:
		//ofLog(OF_LOG_VERBOSE,"ofxNetwork:"+file+": " +line+" EAGAIN: try again");
		break;
	default:
		ofLog(OF_LOG_ERROR,"ofxNetwork:"+file+": " +line+" unknown error: " + ofToString(errno) + " see errno.h for description of the error");
		break;
	}

	return err;
}


#endif /* OFXNETWORKUTILS_H_ */
