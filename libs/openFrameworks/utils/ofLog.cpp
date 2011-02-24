#include "ofLog.h"
#include "ofConstants.h"
#include <ofUtils.h>

ofLogLevel currentLogLevel =  OF_LOG_WARNING;
//--------------------------------------------------
void ofSetLogLevel(ofLogLevel logLevel){
	currentLogLevel = logLevel;
}

#ifdef TARGET_ANDROID

#include <android/log.h>

#define  LOG_TAG    "OF"
#define  LOGNOTICE(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#define  LOGWARNING(...)  __android_log_print(ANDROID_LOG_WARN,LOG_TAG,__VA_ARGS__)
#define  LOGERROR(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)
#define  LOGFATAL(...)  __android_log_print(ANDROID_LOG_FATAL,LOG_TAG,__VA_ARGS__)
#define  LOGVERBOSE(...)  __android_log_print(ANDROID_LOG_VERBOSE,LOG_TAG,__VA_ARGS__)
#define  vLOGNOTICE(...)  __android_log_vprint(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#define  vLOGWARNING(...)  __android_log_vprint(ANDROID_LOG_WARN,LOG_TAG,__VA_ARGS__)
#define  vLOGERROR(...)  __android_log_vprint(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)
#define  vLOGFATAL(...)  __android_log_vprint(ANDROID_LOG_FATAL,LOG_TAG,__VA_ARGS__)
#define  vLOGVERBOSE(...)  __android_log_vprint(ANDROID_LOG_VERBOSE,LOG_TAG,__VA_ARGS__)
#endif

//--------------------------------------------------
void ofLog(ofLogLevel logLevel, string message){
	if(logLevel >= currentLogLevel){
		if(logLevel == OF_LOG_VERBOSE){
			#ifdef TARGET_ANDROID
				LOGVERBOSE(message.c_str());
			#else
				printf("OF_VERBOSE: ");
			#endif
		}
		else if(logLevel == OF_LOG_NOTICE){
			#ifdef TARGET_ANDROID
				LOGNOTICE(message.c_str());
			#else
						printf("OF_NOTICE: ");
			#endif
		}
		else if(logLevel == OF_LOG_WARNING){
			#ifdef TARGET_ANDROID
				LOGWARNING(message.c_str());
			#else
				printf("OF_WARNING: ");
			#endif
		}
		else if(logLevel == OF_LOG_ERROR){
			#ifdef TARGET_ANDROID
				LOGERROR(message.c_str());
			#else
				printf("OF_ERROR: ");
			#endif
		}
		else if(logLevel == OF_LOG_FATAL_ERROR){
			#ifdef TARGET_ANDROID
				LOGFATAL(message.c_str());
			#else
				printf("OF_FATAL_ERROR: ");
			#endif
		}
		#ifndef TARGET_ANDROID
			printf("%s\n",message.c_str());
		#endif
	}
}

//--------------------------------------------------
void ofLog(ofLogLevel logLevel, const char* format, ...){
	//thanks stefan!
	//http://www.ozzu.com/cpp-tutorials/tutorial-writing-custom-printf-wrapper-function-t89166.html

	if(logLevel >= currentLogLevel){
		va_list args;
		va_start( args, format );
		if(logLevel == OF_LOG_VERBOSE){
			#ifdef TARGET_ANDROID
				vLOGVERBOSE( format, args );
			#else
				printf("OF_VERBOSE: ");
			#endif
		}
		else if(logLevel == OF_LOG_NOTICE){
			#ifdef TARGET_ANDROID
				vLOGNOTICE( format, args );
			#else
						printf("OF_NOTICE: ");
			#endif
		}
		else if(logLevel == OF_LOG_WARNING){
			#ifdef TARGET_ANDROID
				vLOGWARNING( format, args );
			#else
				printf("OF_WARNING: ");
			#endif
		}
		else if(logLevel == OF_LOG_ERROR){
			#ifdef TARGET_ANDROID
				vLOGERROR( format, args );
			#else
				printf("OF_ERROR: ");
			#endif
		}
		else if(logLevel == OF_LOG_FATAL_ERROR){
			#ifdef TARGET_ANDROID
				vLOGFATAL( format, args );
			#else
				printf("OF_FATAL_ERROR: ");
			#endif
		}
		#ifndef TARGET_ANDROID
			vprintf( format, args );
			printf("\n");
		#endif
		va_end( args );
	}
}
	
