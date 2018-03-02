/*
 * ofxNetworkUtils.h
 *
 *  Created on: 25/09/2010
 *      Author: arturo
 */

#pragma once

#include <cerrno>
#include "ofConstants.h"


#ifdef TARGET_WIN32
#include <WinSock2.h>
#define WSAENOMEM	WSA_NOT_ENOUGH_MEMORY
#define OFXNETWORK_ERROR(name)	WSAE ## name
#else
#define OFXNETWORK_ERROR(name)	E ## name
#endif

#define ofxNetworkCheckError() ofxNetworkCheckErrno(__FILE__, __LINE__)

int ofxNetworkCheckErrno(const char* file, int line);
