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

//TODO: logfile currently overwrites the file - make it append

//
// Useful references:
//  - http://pocoproject.org/docs/Poco.Logger.html
//  - http://pocoproject.org/docs/Poco.FileChannel.html
//
//------------------------------------------------------------------------------------
// inspired by the Poco LogRotation sample
ofLogger::ofLogger(){	
		
	bConsole = true;
	bFile = false;
	
	bHeader = false;
	bDate = true;
	bTime = true;
	bFrameNum = false;
	bMillis = false;
	
	splitterChannel = new Poco::SplitterChannel();
	
	consoleChannel = new Poco::ConsoleChannel();
	fileChannel = new Poco::FileChannel(ofToDataPath("openframeworks.log"));

	// console open, file not opened (added) by default
	splitterChannel->addChannel(consoleChannel);

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
ofLogger& ofLogger::instance(){
	static ofLogger* pointerToTheSingletonInstance = new ofLogger;
	return *pointerToTheSingletonInstance;
}

//--------------------------------------------------------------------------------------
void ofLogger::log(ofLogLevel logLevel, const string& message){
	_log(logLevel, message, logger);
}

void ofLogger::log(const string& logTopic, ofLogLevel logLevel, const string& message){
	Poco::Logger* topicLogger = Poco::Logger::has(logTopic);
	if(!topicLogger){
		_log(OF_LOG_WARNING, "log topic \""+logTopic+"\" not found", logger);
	}
	else{
		_log(logLevel, logTopic+": "+message, topicLogger);
	}
}

//--------------------------------------------------------------
void ofLogger::setLevel(ofLogLevel logLevel){
	logger->setLevel((int) logLevel);
}

ofLogLevel ofLogger::getLevel(){
	return (ofLogLevel) logger->getLevel();
}

//-------------------------------------------------
void ofLogger::enableConsole(){
	splitterChannel->addChannel(consoleChannel);
	consoleChannel->open();
	bConsole = true;
}

void ofLogger::disableConsole(){
	splitterChannel->removeChannel(consoleChannel);
	consoleChannel->close();
	bConsole = false;
}	

bool ofLogger::usingConsole(){
	return bConsole;
}

//----------------------------------------------
void ofLogger::setFilePath(const string& file){
	fileChannel->setProperty("path", file);
}

string ofLogger::getFilePath(){
	return fileChannel->getProperty("path");
}

void ofLogger::enableFile(){
	fileChannel->open();
	splitterChannel->addChannel(fileChannel);
	bFile = true;
}

void ofLogger::disableFile(){
	fileChannel->close();
	splitterChannel->addChannel(fileChannel);
	bFile = false;
}

bool ofLogger::usingFile(){
	return bFile;
}

//-----------------------------------------------------------------------
void ofLogger::enableFileRotationMins(unsigned int minutes){
	fileChannel->setProperty("rotation", ofToString(minutes)+" minutes");
}

void ofLogger::enableFileRotationHours(unsigned int hours){
	fileChannel->setProperty("rotation", ofToString(hours)+" hours");
}

void ofLogger::enableFileRotationDays(unsigned int days){
	fileChannel->setProperty("rotation", ofToString(days)+" days");
}

void ofLogger::enableFileRotationMonths(unsigned int months){
	fileChannel->setProperty("rotation", ofToString(months)+" months");
}

void ofLogger::enableFileRotationSize(unsigned int sizeKB){
	fileChannel->setProperty("rotation", ofToString(sizeKB)+" K");
}

void ofLogger::disableFileRotation(){
	fileChannel->setProperty("rotation", "never");
}

//--------------------------------------------------------------------------------
void ofLogger::setFileRotationMaxNum(unsigned int num){
	fileChannel->setProperty("purgeCount", ofToString(num));
}
		
//--------------------------------------------------------------------------------
void ofLogger::setFileRotationNumber(){
	fileChannel->setProperty("archive", "number");
}

void ofLogger::setFileRotationTimestamp(){
	fileChannel->setProperty("archive", "timestamp");
}

//--------------------------------------------------------------------------------
void ofLogger::addTopic(const string& logTopic, ofLogLevel logLevel){
	try{
		Poco::Logger::create(logTopic, formattingChannel, (int) OF_DEFAULT_LOG_LEVEL);
	}
	catch (Poco::ExistsException& e){
		log(OF_LOG_WARNING, "log topic \""+logTopic+"\" does not exist");
	}
}

void ofLogger::removeTopic(const string& logTopic){
	Poco::Logger::destroy(logTopic);
}

bool ofLogger::topicExists(const string& logTopic){
	return (bool) Poco::Logger::has(logTopic); // has returns NULL if topic not found
}

void ofLogger::setTopicLogLevel(const string& logTopic, ofLogLevel logLevel){
	Poco::Logger* topicLogger = Poco::Logger::has(logTopic);
	if(topicLogger){
		topicLogger->setLevel(logLevel);
	}
	else{
		log(OF_LOG_WARNING, "log topic \""+logTopic+"\" not found");
	}
}
void ofLogger::resetTopicLogLevel(const string& logTopic){
	setTopicLogLevel(logTopic, (ofLogLevel) logger->getLevel());
}

//--------------------------------------------------------
void ofLogger::enableHeader(){
	bHeader = true;
}

void ofLogger::disableHeader(){
	bHeader = false;
}

bool ofLogger::usingHeader(){
	return bHeader;
}

//----------------------------
void ofLogger::enableHeaderDate(){
	bDate = true;
}
void ofLogger::disableHeaderDate(){
	bDate = false;
}

bool ofLogger::usingHeaderDate(){
	return bDate;
}
//----------------------------
void ofLogger::enableHeaderTime(){
	bTime = true;
}

void ofLogger::disableHeaderTime(){
	bTime = false;
}

bool ofLogger::usingHeaderTime(){
	return bTime;
}

//-----------------------------
void ofLogger::enableHeaderFrameNum(){
	bFrameNum = true;
}

void ofLogger::disableHeaderFrameNum(){
	bFrameNum = false;
}

bool ofLogger::usingHeaderFrameNum(){
	return bFrameNum;
}

//-----------------------------
void ofLogger::enableHeaderMillis(){
	bMillis = true;
}

void ofLogger::disableHeaderMillis(){
	bMillis = false;
}

bool ofLogger::usingHeaderMillis(){
	return bMillis;
}

//---------------------------------------------------------------------------------
void ofLogger::_log(ofLogLevel logLevel, const string& message, Poco::Logger* theLogger){
	
	string timestamp;
	
	// build the header
	if(bHeader){
	
		Poco::LocalDateTime now;
		
		if(bDate && bTime){
			timestamp += Poco::DateTimeFormatter::format(now, s_dateAndTimeFormat)+" ";
		}
		else if(bDate){
			timestamp += Poco::DateTimeFormatter::format(now, s_dateFormat)+" ";
		}
		else if(bTime){
			timestamp += Poco::DateTimeFormatter::format(now, s_timeFormat)+" ";	
		}
		
		if(bFrameNum){
			timestamp += ofToString(ofGetFrameNum())+" ";
		}
		
		if(bMillis){
			timestamp += ofToString(ofGetElapsedTimeMillis())+" ";
		}
	}
	
	// log
	switch(logLevel){
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
ofLogNotice::~ofLogNotice(){
	if(topic.empty()){
		ofLogger::instance().log(level, message.str());
	}
	else{
		ofLogger::instance().log(topic, level, message.str());
	}
}

//--------------------------------------------------------------
void ofLogNotice::setLevel(ofLogLevel logLevel){
	ofLogger::instance().setLevel(logLevel);
}

ofLogLevel ofLogNotice::getLevel(){
	return (ofLogLevel) ofLogger::instance().getLevel();
}

//----------------------------------------------------------------------------------------
// using printf instead of the logger since we don't want to write this as a line
void ofLogNotice::setConsoleColor(int color){
	#ifdef TARGET_WIN32
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
	#else
		printf("\033[%im",  color);
	#endif
}

void ofLogNotice::restoreConsoleColor(){
	#ifdef TARGET_WIN32
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), OF_CONSOLE_COLOR_RESTORE);
	#else
		printf("\033[%im",  OF_CONSOLE_COLOR_RESTORE);
	#endif
}

//--------------------------------------------------------
void ofLog(ofLogLevel logLevel, const string& message){
	ofLogger::instance().log(logLevel, message);
}

void ofLog(ofLogLevel logLevel, const char* format, ...){
	char line[512];	// maybe too small?
	va_list(args);
	va_start(args, format);
		vsprintf(line, format, args);
	va_end(args);
	ofLogger::instance().log(logLevel, (string) line);
}

void ofSetLogLevel(ofLogLevel logLevel){
	ofLogger::instance().setLevel(logLevel);
}

ofLogLevel ofGetLogLevel(){
	return ofLogger::instance().getLevel();
}

//---------------------------------------------------------------------------
void ofLogEnableConsole()	{ofLogger::instance().enableConsole();}
void ofLogDisableConsole()	{ofLogger::instance().disableConsole();}
bool ofLogUsingConsole()	{return ofLogger::instance().usingConsole();}

void ofLogEnableFile()		{ofLogger::instance().enableFile();}
void ofLogDisableFile()		{ofLogger::instance().disableFile();}
bool ofLogUsingFile()		{return ofLogger::instance().usingFile();}

void ofLogSetFilePath(const string& file)
	{ofLogger::instance().setFilePath(file);}
string ofLogGetFilePath()	{return ofLogger::instance().getFilePath();}

void ofLogEnableFileRotationMins(unsigned int minutes)
	{ofLogger::instance().enableFileRotationMins(minutes);}
void ofLogEnableFileRotationHours(unsigned int hours)
	{ofLogger::instance().enableFileRotationHours(hours);}
void ofLogEnableFileRotationDays(unsigned int days)
	{ofLogger::instance().enableFileRotationDays(days);}
void ofLogEnableFileRotationMonths(unsigned int months)
	{ofLogger::instance().enableFileRotationMonths(months);}
void ofLogEnableFileRotationSize(unsigned int sizeKB)
	{ofLogger::instance().enableFileRotationSize(sizeKB);}
void ofLogDisableFileRotation()	{ofLogger::instance().disableFileRotation();}

void ofLogSetFileRotationMaxNum(unsigned int num)
	{ofLogger::instance().setFileRotationMaxNum(num);}

void ofLogSetFileRotationNumber()		{ofLogger::instance().setFileRotationNumber();}
void ofLogSetFileRotationTimestamp()	{ofLogger::instance().setFileRotationTimestamp();}

void ofLogAddTopic(const string& logTopic, ofLogLevel logLevel)
	{ofLogger::instance().addTopic(logTopic, logLevel);}
void ofLogRemoveTopic(const string& logTopic)	{ofLogger::instance().removeTopic(logTopic);}
bool ofLogTopicExists(const string& logTopic)
	{return ofLogger::instance().topicExists(logTopic);}
void ofLogSetTopicLogLevel(const string& logTopic, ofLogLevel logLevel)
	{ofLogger::instance().setTopicLogLevel(logTopic, logLevel);}
void ofLogResetTopicLogLevel(const string& logTopic)
	{ofLogger::instance().resetTopicLogLevel(logTopic);}

void ofLogEnableHeader()	{ofLogger::instance().enableHeader();}
void ofLogDisableHeader()	{ofLogger::instance().disableHeader();}
bool ofLogUsingHeader()		{return ofLogger::instance().usingHeader();}

void ofLogEnableHeaderDate()	{ofLogger::instance().enableHeaderDate();}
void ofLogDisableHeaderDate()	{ofLogger::instance().disableHeaderDate();}
bool ofLogUsingHeaderDate()		{return ofLogger::instance().usingHeaderDate();}

void ofLogEnableHeaderTime()	{ofLogger::instance().enableHeaderTime();}
void ofLogDisableHeaderTime()	{ofLogger::instance().disableHeaderTime();}
bool ofLogUsingHeaderTime()		{return ofLogger::instance().usingHeaderTime();}

void ofLogEnableHeaderFrameNum()	{ofLogger::instance().enableHeaderFrameNum();}
void ofLogDisableHeaderFrameNum()	{ofLogger::instance().disableHeaderFrameNum();}
bool ofLogUsingHeaderFrameNum()		{return ofLogger::instance().usingHeaderFrameNum();}

void ofLogEnableHeaderMillis()	{ofLogger::instance().enableHeaderMillis();}
void ofLogDisableHeaderMillis()	{ofLogger::instance().disableHeaderMillis();}
bool ofLogUsingHeaderMillis()	{return true;}//ofLogger::instance().usingHeaderMillis();}
	