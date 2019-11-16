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

/**
 * @brief Log the latest network error and where it happened (file and line)
 * 
 */
#define ofxNetworkCheckError() ofxNetworkCheckErrno(__FILE__, __LINE__)

/**
 * @brief Log the latest network error and where it happened (file and line)
 * 
 */
#define ofxNetworkLogLastError() ofxNetworkLogError(ofxNetworkGetLastError(), __FILE__, __LINE__)

/**
 * @brief returns error code from the last network function
 * 
 * @return int the last network error
 */
int ofxNetworkGetLastError();


/**
 * @brief Logs the network error err using ofLogError.
 * 
 * @param err the network error
 * @param file the file where the error happened, generally __FILE__
 * @param line the line where the error happened, generally __LINE__
 */
void ofxNetworkLogError(int err, const char* file=__FILE__, int line=__LINE__-1);

/**
 * @brief Logs the last network error and returns it; 
 * 
 * @deprecated use ofxNetworkLogError(ofxNetworkGetLastError(), file, line) instead
 * 
 * @param file the file where the error happened, generally __FILE__
 * @param line the line where the error happened, generally __LINE__
 * @return int the last network error
 */
OF_DEPRECATED_MSG("use ofxNetworkLogError(ofxNetworkGetLastError(), file, line) instead", int ofxNetworkCheckErrno(const char* file, int line) );


