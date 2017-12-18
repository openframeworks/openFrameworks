#include "ofxNetworkUtils.h"
#include "ofLog.h"
#include "ofUtils.h"



int ofxNetworkCheckErrno(const char* file, int line) {
	#ifdef TARGET_WIN32
		int	err	= WSAGetLastError();
	#else
		int err = errno;
	#endif
	switch(err){
	case 0:
		break;
	case OFXNETWORK_ERROR(BADF):
		ofLogError("ofxNetwork") << file << ": " << line << " EBADF: invalid socket";
		break;
	case OFXNETWORK_ERROR(CONNRESET):
		ofLogError("ofxNetwork") << file << ": " << line << " ECONNRESET: connection closed by peer";
		break;
	case OFXNETWORK_ERROR(CONNABORTED):
		ofLogError("ofxNetwork") << file << ": " << line << " ECONNABORTED: connection aborted by peer";
		break;
	case OFXNETWORK_ERROR(NOTCONN):
		ofLogError("ofxNetwork") << file << ": " << line << " ENOTCONN: trying to receive before establishing a connection";
		break;
	case OFXNETWORK_ERROR(NOTSOCK):
		ofLogError("ofxNetwork") << file << ": " << line << " ENOTSOCK: socket argument is not a socket";
		break;
	case OFXNETWORK_ERROR(OPNOTSUPP):
		ofLogError("ofxNetwork") << file << ": " << line << " EOPNOTSUPP: specified flags not valid for this socket";
		break;
	case OFXNETWORK_ERROR(TIMEDOUT):
		ofLogError("ofxNetwork") << file << ": " << line << " ETIMEDOUT: timeout";
		break;
#if !defined(TARGET_WIN32)
	case OFXNETWORK_ERROR(IO):
		ofLogError("ofxNetwork") << file << ": " << line << " EIO: io error";
		break;
#endif
	case OFXNETWORK_ERROR(NOBUFS):
		ofLogError("ofxNetwork") << file << ": " << line << " ENOBUFS: insufficient buffers to complete the operation";
		break;
	case OFXNETWORK_ERROR(NOMEM):
		ofLogError("ofxNetwork") << file << ": " << line << " ENOMEM: insufficient memory to complete the request";
		break;
	case OFXNETWORK_ERROR(ADDRNOTAVAIL):
		ofLogError("ofxNetwork") << file << ": " << line << " EADDRNOTAVAIL: the specified address is not available on the remote machine";
		break;
	case OFXNETWORK_ERROR(AFNOSUPPORT):
		ofLogError("ofxNetwork") << file << ": " << line << " EAFNOSUPPORT: the namespace of the addr is not supported by this socket";
		break;
	case OFXNETWORK_ERROR(ISCONN):
		ofLogError("ofxNetwork") << file << ": " << line << " EISCONN: the socket is already connected";
		break;
	case OFXNETWORK_ERROR(CONNREFUSED):
		ofLogError("ofxNetwork") << file << ": " << line << " ECONNREFUSED: the server has actively refused to establish the connection";
		break;
	case OFXNETWORK_ERROR(NETUNREACH):
		ofLogError("ofxNetwork") << file << ": " << line << " ENETUNREACH: the network of the given addr isn't reachable from this host";
		break;
	case OFXNETWORK_ERROR(ADDRINUSE):
		ofLogError("ofxNetwork") << file << ": " << line << " EADDRINUSE: the socket address of the given addr is already in use";
		break;
	case ENOPROTOOPT:
		ofLogError("ofxNetwork") << file << ": " << line << " ENOPROTOOPT: the optname doesn't make sense for the given level";
		break;
	case EPROTONOSUPPORT:
		ofLogError("ofxNetwork") << file << ": " << line << " EPROTONOSUPPORT: the protocol or style is not supported by the namespace specified";
		break;
	case OFXNETWORK_ERROR(MFILE):
		ofLogError("ofxNetwork") << file << ": " << line << " EMFILE: the process already has too many file descriptors open";
		break;
#if !defined(TARGET_WIN32)
	case OFXNETWORK_ERROR(NFILE):
		ofLogError("ofxNetwork") << file << ": " << line << " ENFILE: the system already has too many file descriptors open";
		break;
#endif
	case OFXNETWORK_ERROR(ACCES):
		ofLogError("ofxNetwork") << file << ": " << line << " EACCES: the process does not have the privilege to create a socket of the specified style or protocol";
		break;
	case OFXNETWORK_ERROR(MSGSIZE):
		ofLogError("ofxNetwork") << file << ": " << line << " EMSGSIZE: the socket type requires that the message be sent atomically, but the message is too large for this to be possible";
		break;
#if !defined(TARGET_WIN32)
	case OFXNETWORK_ERROR(PIPE):
		ofLogError("ofxNetwork") << file << ": " << line << " EPIPE: this socket was connected but the connection is now broken";
		break;
#endif
	case OFXNETWORK_ERROR(INVAL):
		ofLogError("ofxNetwork") << file << ": " << line << " EINVAL: invalid argument";
		break;
#if !defined(TARGET_WIN32)
#if !defined(EWOULDBLOCK) || EAGAIN != EWOULDBLOCK
	case EAGAIN:
		// Not an error worth reporting, this is normal if the socket is non-blocking
		//ofLogError("ofxNetwork") << file << ": " << line << " EAGAIN: try again";
		break;
#endif
#endif
	case OFXNETWORK_ERROR(WOULDBLOCK):
	case OFXNETWORK_ERROR(INPROGRESS):
	case OFXNETWORK_ERROR(ALREADY):
		// Not an error worth reporting, this is normal if the socket is non-blocking
		break;
	case OFXNETWORK_ERROR(INTR):
		//ofLogError("ofxNetwork") << file << ": " << line << " EINTR: receive interrupted by a signal, before any data available";
		break;
	default:
		ofLogVerbose("ofxNetwork") << file << ": " << line << " unknown error: " << err << " see errno.h for description of the error";
		break;
	}

	return err;
}
