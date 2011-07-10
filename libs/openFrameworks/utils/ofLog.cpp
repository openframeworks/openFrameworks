#include "ofLog.h"
#include "ofConstants.h"
#include <ofUtils.h>
#include <map>

static ofLogLevel currentLogLevel =  OF_LOG_WARNING;
static map<string,ofLogLevel> & getModules(){
	static map<string,ofLogLevel> * modules = new map<string,ofLogLevel>;
	return *modules;
}

static void noopDeleter(ofBaseLoggerChannel*){}
#ifdef TARGET_ANDROID
	#include "ofxAndroidLogChannel.h"
	ofPtr<ofBaseLoggerChannel> ofLog::channel = ofPtr<ofxAndroidLogChannel>(new ofxAndroidLogChannel,std::ptr_fun(noopDeleter));
#else
	ofPtr<ofBaseLoggerChannel> ofLog::channel = ofPtr<ofConsoleLoggerChannel>(new ofConsoleLoggerChannel,std::ptr_fun(noopDeleter));
#endif

//--------------------------------------------------
void ofSetLogLevel(ofLogLevel logLevel){
	currentLogLevel = logLevel;
}

//--------------------------------------------------
void ofSetLogLevel(string module, ofLogLevel logLevel){
	getModules()[module] = logLevel;
}

//--------------------------------------------------
ofLogLevel ofGetLogLevel(){
	return currentLogLevel;
}

//--------------------------------------------------
void ofLogToFile(const string & path, bool append){
	ofLog::setChannel(ofPtr<ofFileLoggerChannel>(new ofFileLoggerChannel(path,append)));
}

//--------------------------------------------------
void ofLogToConsole(){
	ofLog::setChannel(ofPtr<ofConsoleLoggerChannel>(new ofConsoleLoggerChannel,std::ptr_fun(noopDeleter)));
}

//--------------------------------------------------
ofLog::ofLog(){
	level = OF_LOG_NOTICE;
	module = "OF";
	bPrinted = false;
}
		
//--------------------------------------------------
ofLog::ofLog(ofLogLevel logLevel){
	level = logLevel;
	module = "OF";
	bPrinted = false;
}

//--------------------------------------------------
ofLog::ofLog(ofLogLevel logLevel, const string & message){
	_log(logLevel,"OF",message);
	bPrinted = true;
}

//--------------------------------------------------
ofLog::ofLog(ofLogLevel logLevel, const char* format, ...){
	if(checkLog(logLevel,"OF")){
		va_list args;
		va_start( args, format );
		channel->log(logLevel,"OF",format,args);
		va_end( args );
	}
	bPrinted = true;
}

//-------------------------------------------------------
ofLog::~ofLog(){
	// don't log if we printed in the constructor already
	if(!bPrinted){
		if(message.str().empty()){
			message << module;
			module = "OF";
		}
		_log(level,module,message.str());
	}
}

bool ofLog::checkLog(ofLogLevel level, const string & module){
	if(module=="" || getModules().find(module)==getModules().end()){
		if(level >= currentLogLevel) return true;
	}else{
		if(level >= getModules()[module]) return true;
	}
	return false;
}

//-------------------------------------------------------
void ofLog::_log(ofLogLevel level, const string & module, const string & message){
	if(checkLog(level,module)){
		channel->log(level,module, message);
	}
}
	
ofLogVerbose::ofLogVerbose(const string & _module){
	level = OF_LOG_VERBOSE;
	module = _module;
	bPrinted=false;
}

ofLogVerbose::ofLogVerbose(const string & _module, const string & _message){
	_log(OF_LOG_VERBOSE,_module,_message);
	bPrinted = true;
}

ofLogNotice::ofLogNotice(const string & _module) {
	level = OF_LOG_NOTICE;
	module = _module;
	bPrinted=false;
}

ofLogNotice::ofLogNotice(const string & _module, const string & _message){
	_log(OF_LOG_NOTICE,_module,_message);
	bPrinted = true;
}

ofLogWarning::ofLogWarning(const string & _module) {
	level = OF_LOG_WARNING;
	module = _module;
	bPrinted=false;
}

ofLogWarning::ofLogWarning(const string & _module, const string & _message){
	_log(OF_LOG_WARNING,_module,_message);
	bPrinted = true;
}

ofLogError::ofLogError(const string & _module) {
	level = OF_LOG_ERROR;
	module = _module;
	bPrinted=false;
}

ofLogError::ofLogError(const string & _module, const string & _message){
	_log(OF_LOG_ERROR,_module,_message);
	bPrinted = true;
}

ofLogFatalError::ofLogFatalError(const string &  _module) {
	level = OF_LOG_FATAL_ERROR;
	module = _module;
	bPrinted=false;
}

ofLogFatalError::ofLogFatalError(const string & _module, const string & _message){
	_log(OF_LOG_FATAL_ERROR,_module,_message);
	bPrinted = true;
}

void ofLog::setChannel(ofPtr<ofBaseLoggerChannel> _channel){
	channel = _channel;
}

void ofSetLoggerChannel(ofPtr<ofBaseLoggerChannel> loggerChannel){
	ofLog::setChannel(loggerChannel);
}

string ofGetLogLevelName(ofLogLevel level){
	switch(level){
	case OF_LOG_VERBOSE:
		return "OF_VERBOSE";
		break;
	case OF_LOG_NOTICE:
		return "OF_LOG_NOTICE";
		break;
	case OF_LOG_WARNING:
		return "OF_LOG_WARNING";
		break;
	case OF_LOG_ERROR:
		return "OF_LOG_ERROR";
		break;
	case OF_LOG_FATAL_ERROR:
		return "OF_LOG_FATAL_ERROR";
		break;
	case OF_LOG_SILENT:
		return "OF_LOG_SILENT";
		break;
	default:
		return "";
	}
}

void ofConsoleLoggerChannel::log(ofLogLevel level, const string & module, const string & message){
	if(level<OF_LOG_ERROR) cout << module << ": " << ofGetLogLevelName(level) << ": " << message << endl;
	else cerr << module << ": " << ofGetLogLevelName(level) << ": " << message << endl;
}

void ofConsoleLoggerChannel::log(ofLogLevel logLevel, const string & module, const char* format, ...){
	va_list args;
	va_start(args, format);
	log(logLevel, module, format, args);
	va_end(args);
}

void ofConsoleLoggerChannel::log(ofLogLevel logLevel, const string & module, const char* format, va_list args){
	//thanks stefan!
	//http://www.ozzu.com/cpp-tutorials/tutorial-writing-custom-printf-wrapper-function-t89166.html
	if(logLevel<OF_LOG_ERROR){
		printf("%s: ", module.c_str());
		printf("%s: ", ofGetLogLevelName(logLevel).c_str());
		vprintf( format, args );
		printf("\n");
	}else{
		fprintf(stderr,"%s: ", module.c_str());
		fprintf(stderr,"%s: ", ofGetLogLevelName(logLevel).c_str());
		vfprintf( stderr, format, args );
		fprintf(stderr,"\n");
	}
}

ofFileLoggerChannel::ofFileLoggerChannel(){

}

ofFileLoggerChannel::ofFileLoggerChannel(const string & path, bool append){
	setFile(path,append);
}

ofFileLoggerChannel::~ofFileLoggerChannel(){
	close();
}

void ofFileLoggerChannel::close(){
	file.close();
}

void ofFileLoggerChannel::setFile(const string & path,bool append){
	file.open(path,append?ofFile::Append:ofFile::WriteOnly);
	file << endl;
	file << endl;
	file << "---------------------------------------  " << ofGetTimestampString() << endl;
}

void ofFileLoggerChannel::log(ofLogLevel level, const string & module, const string & message){
	file << module << ": " << ofGetLogLevelName(level) << ": " << message << endl;
}

void ofFileLoggerChannel::log(ofLogLevel logLevel, const string & module, const char* format, ...){
	va_list args;
	va_start(args, format);
	log(logLevel, module, format, args);
	va_end(args);
}

void ofFileLoggerChannel::log(ofLogLevel logLevel, const string & module, const char* format, va_list args){
	file << module << ": " << ofGetLogLevelName(logLevel) << ": ";
	file << ofVAArgsToString(format,args) << endl;
}
