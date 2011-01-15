#include "ofLog.h"

#include <ofUtils.h>

#include <Poco/PatternFormatter.h>
#include <Poco/FormattingChannel.h>
#include <Poco/Message.h>
#include <Poco/LocalDateTime.h>
#include <Poco/DateTimeFormatter.h>

const string ofLogger::s_dateFormat = "%Y-%m-%d %H:%M:%S.%i";
const string ofLogger::s_timeFormat = "%H:%M:%S.%i";
const string ofLogger::s_dateAndTimeFormat = "%Y-%m-%d %H:%M:%S.%i";

//
// Useful references:
//  - http://pocoproject.org/docs/Poco.Logger.html
//  - http://pocoproject.org/docs/Poco.FileChannel.html
//
//------------------------------------------------------------------------------------
// inspired by the Poco LogRotation sample
ofLogger::ofLogger() {	
		
	bConsole = true;
	bFile = false;
	
	bHeader = false;
	bDate = true;
	bTime = true;
	bFrameNum = false;
	bMillis = false;
	
	splitterChannel = new Poco::SplitterChannel();
	
	consoleChannel = new Poco::ConsoleChannel();
	fileChannel = new Poco::FileChannel("output.log");

	// console open, file not opened (added) by default
	splitterChannel->addChannel(consoleChannel);
	splitterChannel->addChannel(fileChannel);

	Poco::AutoPtr<Poco::Formatter> formatter(new Poco::PatternFormatter("%t"));
	formattingChannel = new Poco::FormattingChannel(formatter, splitterChannel);

	// the root logger has an empty name
	logger = &Poco::Logger::create("", formattingChannel, (int) OF_DEFAULT_LOG_LEVEL);
	
	// setup file logger
	fileChannel->setProperty("times", "local");		// use local system time
	fileChannel->setProperty("archive", "number");	// use number suffixs
	fileChannel->setProperty("compress", "false"); 	// don't compress
	fileChannel->setProperty("purgeCount", "10");	// max number of log files
	
	// add default of topic
	addTopic("of");
}

//-----------------------------------------------------------------
ofLogger& ofLogger::instance() {
	static ofLogger* pointerToTheSingletonInstance = new ofLogger;
	return *pointerToTheSingletonInstance;
}

//--------------------------------------------------------------------------------------
void ofLogger::log(ofLogLevel logLevel, const string& message) {
	_log(logLevel, message, logger);
}

void ofLogger::log(const string& logTopic, ofLogLevel logLevel, const string& message) {
	Poco::Logger* topicLogger = Poco::Logger::has(logTopic);
	if(!topicLogger) {
		_log(OF_LOG_WARNING, "log topic \""+logTopic+"\" not found", logger);
	}
	else {
		_log(logLevel, logTopic+": "+message, topicLogger);
	}
}

//-------------------------------------------------
void ofLogger::enableConsole() {
	splitterChannel->addChannel(consoleChannel);
	consoleChannel->open();
	bConsole = true;
}

void ofLogger::disableConsole() {
	splitterChannel->removeChannel(consoleChannel);
	consoleChannel->close();
	bConsole = false;
}	

bool ofLogger::usingConsole() {
	return bConsole;
}

//----------------------------------------------
bool ofLogger::setFilePath(const string& file) {
	fileChannel->setProperty("path", file);
}

string ofLogger::getFilePath() {
	return fileChannel->getProperty("path");
}

void ofLogger::enableFile() {
	fileChannel->open();
	splitterChannel->addChannel(fileChannel);
	bFile = true;
}

void ofLogger::disableFile() {
	fileChannel->close();
	splitterChannel->addChannel(fileChannel);
	bFile = false;
}

bool ofLogger::usingFile() {
	return bFile;
}

//-----------------------------------------------------------------------
void ofLogger::enableFileRotationMins(unsigned int minutes) {
	fileChannel->setProperty("rotation", ofToString(minutes)+" minutes");
}

void ofLogger::enableFileRotationHours(unsigned int hours) {
	fileChannel->setProperty("rotation", ofToString(hours)+" hours");
}

void ofLogger::enableFileRotationDays(unsigned int days) {
	fileChannel->setProperty("rotation", ofToString(days)+" days");
}

void ofLogger::enableFileRotationMonths(unsigned int months) {
	fileChannel->setProperty("rotation", ofToString(months)+" months");
}

void ofLogger::enableFileRotationSize(unsigned int sizeKB) {
	fileChannel->setProperty("rotation", ofToString(sizeKB)+" K");
}

void ofLogger::disableFileRotation() {
	fileChannel->setProperty("rotation", "never");
}

//--------------------------------------------------------------------------------
void ofLogger::addTopic(const string& logTopic, ofLogLevel logLevel) {
	try {
		Poco::Logger::create(logTopic, formattingChannel, (int) OF_DEFAULT_LOG_LEVEL);
	}
	catch (Poco::ExistsException& e) {
		log(OF_LOG_WARNING, "log topic \""+logTopic+"\" does not exist");
	}
}

void ofLogger::removeTopic(const string& logTopic) {
	Poco::Logger::destroy(logTopic);
}

void ofLogger::clearTopics() {
//	std::vector<std::string>& names = Poco::Logger::names();
//	for(int i = 0; i < names.size(); ++i)
//	{
//		if(names[i] != "") {	// don't delete the root logger
//			Poco::Logger::destroy(names[i]);
//		}
//	}
}

bool ofLogger::topicExists(const string& logTopic) {
	return (bool) Poco::Logger::has(logTopic); // has returns NULL if topic not found
}

void ofLogger::setTopicLogLevel(const string& logTopic, ofLogLevel logLevel) {
	Poco::Logger* topicLogger = Poco::Logger::has(logTopic);
	if(topicLogger) {
		topicLogger->setLevel(logLevel);
	}
	else {
		log(OF_LOG_WARNING, "log topic \""+logTopic+"\" not found");
	}
}
void ofLogger::resetTopicLogLevel(const string& logTopic) {
	setTopicLogLevel(logTopic, (ofLogLevel) logger->getLevel());
}

//--------------------------------------------------------
void ofLogger::enableHeader() {
	bHeader = true;
}

void ofLogger::disableHeader() {
	bHeader = false;
}

bool ofLogger::usingHeader() {
	return bHeader;
}

//----------------------------
void ofLogger::enableDate() {
	bDate = true;
}
void ofLogger::disableDate() {
	bDate = false;
}

bool ofLogger::usingDate() {
	return bDate;
}
//----------------------------
void ofLogger::enableTime() {
	bTime = true;
}

void ofLogger::disableTime() {
	bTime = false;
}

bool ofLogger::usingTime() {
	return bTime;
}

//-----------------------------
void ofLogger::enableFrameNum(){
	bFrameNum = true;
}

void ofLogger::disableFrameNum() {
	bFrameNum = false;
}

bool ofLogger::usingFrameNum() {
	return bFrameNum;
}

//-----------------------------
void ofLogger::enableMillis() {
	bMillis = true;
}

void ofLogger::disableMillis() {
	bMillis = false;
}

bool ofLogger::usingMillis() {
	return bMillis;
}

//---------------------------------------------------------------------------------
void ofLogger::_log(ofLogLevel logLevel, const string& message, Poco::Logger* theLogger) {
	
	string timestamp;
	
	// build the header
	if(bHeader) {
	
		Poco::LocalDateTime now;
		
		if(bDate && bTime) {
			timestamp += Poco::DateTimeFormatter::format(now, s_dateAndTimeFormat)+" ";
		}
		else if(bDate) {
			timestamp += Poco::DateTimeFormatter::format(now, s_dateFormat)+" ";
		}
		else if(bTime) {
			timestamp += Poco::DateTimeFormatter::format(now, s_timeFormat)+" ";	
		}
		
		if(bFrameNum) {
			timestamp += ofToString(ofGetFrameNum())+" ";
		}
		
		if(bMillis) {
			timestamp += ofToString(ofGetElapsedTimeMillis())+" ";
		}
	}
	
	// log
	switch(logLevel) {
		case OF_LOG_SILENT:
			break;
			
		case OF_LOG_VERBOSE:
			theLogger->trace(timestamp+"OF_VERBOSE: "+message);
			break;
			
		case OF_LOG_DEBUG:
			theLogger->debug(timestamp+"OF_DEBUG: "+message);
			break;
			
		case OF_LOG_NOTICE:
			theLogger->notice(timestamp+message);
			break;

		case OF_LOG_WARNING:
			theLogger->warning(timestamp+"OF_WARNING: "+message);
			break;

		case OF_LOG_ERROR:
			theLogger->error(timestamp+"OF_ERROR: "+message);
			break;
			
		case OF_LOG_FATAL_ERROR:
			theLogger->fatal(timestamp+"OF_FATAL_ERROR: "+message);
			break;
	}
}

//-------------------------------------------------------
ofLogNotice::~ofLogNotice() {
	if(topic.empty()) {
		ofLogger::instance().log(level, message.str());
	}
	else {
		ofLogger::instance().log(topic, level, message.str());
	}
}

//----------------------------------------------
//void ofLogNotice::enableTimestamp() {
//	ofLogger::instance().bTimestamp = true;
//}
//void ofLogNotice::disableTimestamp() {
//	ofLogger::instance().bTimestamp = false;
//}
//bool ofLogNotice::usingTimestamp() {
//	return ofLogger::instance().bTimestamp;
//}

//--------------------------------------------------------------
void ofLogNotice::setLevel(ofLogLevel logLevel) {
	ofLogger::instance().logger->setLevel(logLevel);
}

ofLogLevel ofLogNotice::getLevel() {
	return (ofLogLevel) ofLogger::instance().logger->getLevel();
}

//--------------------------------------------------------
void ofLog(ofLogLevel logLevel, const string& message) {
	ofLogger::instance().log(logLevel, message);
}

void ofLog(ofLogLevel logLevel, const char* format, ...) {
	char line[512];	// maybe too small?
	va_list(args);
	va_start(args, format);
		vsprintf(line, format, args);
	va_end(args);
	ofLogger::instance().log(logLevel, (string) line);
}

		