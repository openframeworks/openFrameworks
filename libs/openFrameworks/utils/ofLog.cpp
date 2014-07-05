#include "ofLog.h"
#include "ofConstants.h"
#include <ofUtils.h>
#include <map>

static ofLogLevel currentLogLevel =  OF_LOG_NOTICE;

bool ofLog::bAutoSpace = false;
string ofLog::padding = "";

static map<string,ofLogLevel> & getModules(){
	static map<string,ofLogLevel> * modules = new map<string,ofLogLevel>;
	return *modules;
}

static void noopDeleter(ofBaseLoggerChannel*){}
#ifdef TARGET_ANDROID
	#include "ofxAndroidLogChannel.h"
	shared_ptr<ofBaseLoggerChannel> ofLog::channel = shared_ptr<ofxAndroidLogChannel>(new ofxAndroidLogChannel,std::ptr_fun(noopDeleter));
#else
	shared_ptr<ofBaseLoggerChannel> ofLog::channel = shared_ptr<ofConsoleLoggerChannel>(new ofConsoleLoggerChannel,std::ptr_fun(noopDeleter));
#endif

//--------------------------------------------------
void ofSetLogLevel(ofLogLevel level){
	currentLogLevel = level;
}

//--------------------------------------------------
void ofSetLogLevel(string module, ofLogLevel level){
	getModules()[module] = level;
}

//--------------------------------------------------
ofLogLevel ofGetLogLevel(){
	return currentLogLevel;
}

//--------------------------------------------------
void ofLogToFile(const string & path, bool append){
	ofLog::setChannel(shared_ptr<ofFileLoggerChannel>(new ofFileLoggerChannel(path,append)));
}

//--------------------------------------------------
void ofLogToConsole(){
	ofLog::setChannel(shared_ptr<ofConsoleLoggerChannel>(new ofConsoleLoggerChannel,std::ptr_fun(noopDeleter)));
}

//--------------------------------------------------
ofLog::ofLog(){
	level = OF_LOG_NOTICE;
	module = "";
	bPrinted = false;
}
		
//--------------------------------------------------
ofLog::ofLog(ofLogLevel _level){
	level = _level;
	module = "";
	bPrinted = false;
}

//--------------------------------------------------
ofLog::ofLog(ofLogLevel level, const string & message){
	_log(level,"",message);
	bPrinted = true;
}

//--------------------------------------------------
ofLog::ofLog(ofLogLevel level, const char* format, ...){
	if(checkLog(level,"")){
		va_list args;
		va_start( args, format );
		channel->log(level,"",format,args);
		va_end( args );
	}
	bPrinted = true;
}

//--------------------------------------------------
void ofLog::setAutoSpace(bool autoSpace){
	bAutoSpace = autoSpace;
	if(bAutoSpace){
		padding = " ";
	}
	else{
		padding = "";
	}
}

//-------------------------------------------------------
ofLog::~ofLog(){
	// don't log if we printed in the constructor already
	if(!bPrinted){
		_log(level,module,message.str());
	}
}

bool ofLog::checkLog(ofLogLevel level, const string & module){
	if(getModules().find(module)==getModules().end()){
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

//--------------------------------------------------
ofLogVerbose::ofLogVerbose(const string & _module){
	level = OF_LOG_VERBOSE;
	module = _module;
	bPrinted=false;
}

ofLogVerbose::ofLogVerbose(const string & _module, const string & _message){
	_log(OF_LOG_VERBOSE,_module,_message);
	bPrinted = true;
}

ofLogVerbose::ofLogVerbose(const string & module, const char* format, ...){
	if(checkLog(OF_LOG_VERBOSE, module)){
		va_list args;
		va_start(args, format);
		channel->log(OF_LOG_VERBOSE, module, format, args);
		va_end(args);
	}
	bPrinted = true;
}

//--------------------------------------------------
ofLogNotice::ofLogNotice(const string & _module){
	level = OF_LOG_NOTICE;
	module = _module;
	bPrinted=false;
}

ofLogNotice::ofLogNotice(const string & _module, const string & _message){
	_log(OF_LOG_NOTICE,_module,_message);
	bPrinted = true;
}

ofLogNotice::ofLogNotice(const string & module, const char* format, ...){
	if(checkLog(OF_LOG_NOTICE, module)){
		va_list args;
		va_start(args, format);
		channel->log(OF_LOG_NOTICE, module, format, args);
		va_end(args);
	}
	bPrinted = true;
}

//--------------------------------------------------
ofLogWarning::ofLogWarning(const string & _module){
	level = OF_LOG_WARNING;
	module = _module;
	bPrinted=false;
}

ofLogWarning::ofLogWarning(const string & _module, const string & _message){
	_log(OF_LOG_WARNING,_module,_message);
	bPrinted = true;
}

ofLogWarning::ofLogWarning(const string & module, const char* format, ...){
	if(checkLog(OF_LOG_WARNING, module)){
		va_list args;
		va_start(args, format);
		channel->log(OF_LOG_WARNING, module, format, args);
		va_end(args);
	}
	bPrinted = true;
}

//--------------------------------------------------
ofLogError::ofLogError(const string & _module){
	level = OF_LOG_ERROR;
	module = _module;
	bPrinted=false;
}

ofLogError::ofLogError(const string & _module, const string & _message){
	_log(OF_LOG_ERROR,_module,_message);
	bPrinted = true;
}

ofLogError::ofLogError(const string & module, const char* format, ...){
	if(checkLog(OF_LOG_ERROR, module)){
		va_list args;
		va_start(args, format);
		channel->log(OF_LOG_ERROR, module, format, args);
		va_end(args);
	}
	bPrinted = true;
}

//--------------------------------------------------
ofLogFatalError::ofLogFatalError(const string &  _module){
	level = OF_LOG_FATAL_ERROR;
	module = _module;
	bPrinted=false;
}

ofLogFatalError::ofLogFatalError(const string & _module, const string & _message){
	_log(OF_LOG_FATAL_ERROR,_module,_message);
	bPrinted = true;
}

ofLogFatalError::ofLogFatalError(const string & module, const char* format, ...){
	if(checkLog(OF_LOG_FATAL_ERROR, module)){
		va_list args;
		va_start(args, format);
		channel->log(OF_LOG_FATAL_ERROR, module, format, args);
		va_end(args);
	}
	bPrinted = true;
}

//--------------------------------------------------
void ofLog::setChannel(shared_ptr<ofBaseLoggerChannel> _channel){
	channel = _channel;
}

void ofSetLoggerChannel(shared_ptr<ofBaseLoggerChannel> loggerChannel){
	ofLog::setChannel(loggerChannel);
}

string ofGetLogLevelName(ofLogLevel level, bool pad){
	switch(level){
		case OF_LOG_VERBOSE:
			return "verbose";
		case OF_LOG_NOTICE:
			return pad ? "notice " : "notice";
		case OF_LOG_WARNING:
			return "warning";
		case OF_LOG_ERROR:
			return pad ? " error " : "error";
		case OF_LOG_FATAL_ERROR:
			return pad ? " fatal " : "fatal";
		case OF_LOG_SILENT:
			return pad ? "silent " : "silent";
		default:
			return "";
	}
}

//--------------------------------------------------
void ofConsoleLoggerChannel::log(ofLogLevel level, const string & module, const string & message){
	// print to cerr for OF_LOG_ERROR and OF_LOG_FATAL_ERROR, everything else to cout 
	ostream& out = level < OF_LOG_ERROR ? cout : cerr;
	out << "[" << ofGetLogLevelName(level, true)  << "] ";
	// only print the module name if it's not ""
	if(module != ""){
		out << module << ": ";
	}
	out << message << endl;
}

void ofConsoleLoggerChannel::log(ofLogLevel level, const string & module, const char* format, ...){
	va_list args;
	va_start(args, format);
	log(level, module, format, args);
	va_end(args);
}

void ofConsoleLoggerChannel::log(ofLogLevel level, const string & module, const char* format, va_list args){
	//thanks stefan!
	//http://www.ozzu.com/cpp-tutorials/tutorial-writing-custom-printf-wrapper-function-t89166.html
	FILE* out = level < OF_LOG_ERROR ? stdout : stderr;
	fprintf(out, "[%s] ", ofGetLogLevelName(level, true).c_str());
	if(module != ""){
		fprintf(out, "%s: ", module.c_str());
	}
	vfprintf(out, format, args);
	fprintf(out, "\n");
}

//--------------------------------------------------
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
	file << "--------------------------------------- " << ofGetTimestampString() << endl;
}

void ofFileLoggerChannel::log(ofLogLevel level, const string & module, const string & message){
	file << "[" << ofGetLogLevelName(level, true) << "] ";
	if(module != ""){
		file << module << ": ";
	}
	file << message << endl;
}

void ofFileLoggerChannel::log(ofLogLevel level, const string & module, const char* format, ...){
	va_list args;
	va_start(args, format);
	log(level, module, format, args);
	va_end(args);
}

void ofFileLoggerChannel::log(ofLogLevel level, const string & module, const char* format, va_list args){
	file << "[" << ofGetLogLevelName(level, true) << "] ";
	if(module != ""){
		file << module << ": ";
	}
	file << ofVAArgsToString(format,args) << endl;
}
