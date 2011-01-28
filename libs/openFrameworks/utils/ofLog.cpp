#include "ofLog.h"
#include "ofConstants.h"
#include <ofUtils.h>

int currentLogLevel =  OF_LOG_WARNING;
//--------------------------------------------------
void ofSetLogLevel(int logLevel){
	currentLogLevel = logLevel;
}

//--------------------------------------------------
void ofLog(int logLevel, string message){
	if(logLevel >= currentLogLevel){
		if(logLevel == OF_LOG_VERBOSE){
			printf("OF_VERBOSE: ");
		}
		else if(logLevel == OF_LOG_NOTICE){
			printf("OF_NOTICE: ");
		}
		else if(logLevel == OF_LOG_WARNING){
			printf("OF_WARNING: ");
		}
		else if(logLevel == OF_LOG_ERROR){
			printf("OF_ERROR: ");
		}
		else if(logLevel == OF_LOG_FATAL_ERROR){
			printf("OF_FATAL_ERROR: ");
		}
		printf("%s\n",message.c_str());
	}
}

//--------------------------------------------------
void ofLog(int logLevel, const char* format, ...){
	//thanks stefan!
	//http://www.ozzu.com/cpp-tutorials/tutorial-writing-custom-printf-wrapper-function-t89166.html

	if(logLevel >= currentLogLevel){
		va_list args;
		va_start( args, format );
		if(logLevel == OF_LOG_VERBOSE){
			printf("OF_VERBOSE: ");
		}
		else if(logLevel == OF_LOG_NOTICE){
			printf("OF_NOTICE: ");
		}
		else if(logLevel == OF_LOG_WARNING){
			printf("OF_WARNING: ");
		}
		else if(logLevel == OF_LOG_ERROR){
			printf("OF_ERROR: ");
		}
		else if(logLevel == OF_LOG_FATAL_ERROR){
			printf("OF_FATAL_ERROR: ");
		}
		vprintf( format, args );
		printf("\n");
		va_end( args );
	}
}
	