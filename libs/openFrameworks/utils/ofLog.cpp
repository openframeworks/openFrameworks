#include "ofLog.h"
#include <ofUtils.h>
#include <map>
#ifdef TARGET_ANDROID
	#include "ofxAndroidLogChannel.h"
#endif

using std::map;
using std::string;
using std::shared_ptr;

static ofLogLevel currentLogLevel =  OF_LOG_NOTICE;

bool ofLog::bAutoSpace = false;
string & ofLog::getPadding() {
	static string * padding = new string;
	return *padding;
}

static map<string,ofLogLevel> & getModules(){
	static map<string,ofLogLevel> * modules = new map<string,ofLogLevel>;
	return *modules;
}

static void noopDeleter(ofBaseLoggerChannel*){}

shared_ptr<ofBaseLoggerChannel> & ofLog::channel(){
#ifdef TARGET_ANDROID
	static shared_ptr<ofBaseLoggerChannel> channel = shared_ptr<ofxAndroidLogChannel>(new ofxAndroidLogChannel, std::function<void(ofBaseLoggerChannel *)>(noopDeleter));
#elif defined(TARGET_WIN32)
	static shared_ptr<ofBaseLoggerChannel> channel = IsDebuggerPresent() ? shared_ptr<ofBaseLoggerChannel>(new ofDebugViewLoggerChannel, std::function<void(ofBaseLoggerChannel *)>(noopDeleter)) : shared_ptr<ofBaseLoggerChannel>(new ofConsoleLoggerChannel, std::function<void(ofBaseLoggerChannel *)>(noopDeleter));
#else
	static shared_ptr<ofBaseLoggerChannel> channel = shared_ptr<ofConsoleLoggerChannel>(new ofConsoleLoggerChannel, std::function<void(ofBaseLoggerChannel *)>(noopDeleter));
#endif

	return channel;
}

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
ofLogLevel ofGetLogLevel(string module){
	if (getModules().find(module) == getModules().end()) {
		return currentLogLevel;
	} else {
		return getModules()[module];
	}
}

//--------------------------------------------------
void ofLogToFile(const of::filesystem::path & path, bool append){
	ofLog::setChannel(std::make_shared<ofFileLoggerChannel>(path,append));
}

//--------------------------------------------------
void ofLogToConsole(){
	ofLog::setChannel(shared_ptr<ofConsoleLoggerChannel>(new ofConsoleLoggerChannel, std::function<void(ofBaseLoggerChannel *)>(noopDeleter)));
}

#ifdef TARGET_WIN32
void ofLogToDebugView() {
	ofLog::setChannel(shared_ptr<ofDebugViewLoggerChannel>(new ofDebugViewLoggerChannel, std::function<void(ofBaseLoggerChannel *)>(noopDeleter)));
}
#endif

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
void ofLog::setAutoSpace(bool autoSpace){
	bAutoSpace = autoSpace;
	if(bAutoSpace){
		ofLog::getPadding() = " ";
	}
	else{
		ofLog::getPadding() = "";
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
		channel()->log(level,module, message);
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


//--------------------------------------------------
void ofLog::setChannel(shared_ptr<ofBaseLoggerChannel> _channel){
	channel() = _channel;
}

void ofSetLoggerChannel(shared_ptr<ofBaseLoggerChannel> loggerChannel){
	ofLog::setChannel(loggerChannel);
}

shared_ptr<ofBaseLoggerChannel> ofLog::getChannel(){
	return channel();
}

shared_ptr<ofBaseLoggerChannel> ofGetLoggerChannel(){
	return ofLog::getChannel();
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
	std::ostream& out = level < OF_LOG_ERROR ? std::cout : std::cerr;
	out << "[" << ofGetLogLevelName(level, true)  << "] ";
	// only print the module name if it's not ""
	if(module != ""){
		out << module << ": ";
	}
	out << message << std::endl;
}


#ifdef TARGET_WIN32
#include <array>
void ofDebugViewLoggerChannel::log(ofLogLevel level, const string & module, const string & message) {
	// print to cerr for OF_LOG_ERROR and OF_LOG_FATAL_ERROR, everything else to cout 
	std::stringstream out;
	out << "[" << ofGetLogLevelName(level, true) << "] ";
	// only print the module name if it's not ""
	if (module != "") {
		out << module << ": ";
	}
	out << message << std::endl;
	OutputDebugStringA(out.str().c_str());
}
#endif

//--------------------------------------------------
ofFileLoggerChannel::ofFileLoggerChannel(){
}

ofFileLoggerChannel::ofFileLoggerChannel(const of::filesystem::path & path, bool append){
	setFile(path,append);
}

ofFileLoggerChannel::~ofFileLoggerChannel(){
	close();
}

void ofFileLoggerChannel::close(){
	file.close();
}

void ofFileLoggerChannel::setFile(const of::filesystem::path & path,bool append){
	file.open(path,append?ofFile::Append:ofFile::WriteOnly);
	file << std::endl;
	file << std::endl;
	file << "--------------------------------------- " << ofGetTimestampString() << std::endl;
}

void ofFileLoggerChannel::log(ofLogLevel level, const string & module, const string & message){
	file << "[" << ofGetLogLevelName(level, true) << "] ";
	if(module != ""){
		file << module << ": ";
	}
	file << message << std::endl;
}

