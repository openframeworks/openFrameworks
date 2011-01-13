#include "ofLog.h"

#include "Poco/AutoPtr.h"
#include "Poco/ConsoleChannel.h"
#include "Poco/SplitterChannel.h"
#include "Poco/FileChannel.h"
#include "Poco/PatternFormatter.h"
#include "Poco/FormattingChannel.h"
#include "Poco/Message.h"
#include "Poco/LocalDateTime.h"
#include "Poco/DateTimeFormatter.h"

const string ofLogger::s_timestampFormat = "%Y-%m-%d %H:%M:%S.%i";
bool ofLogger::bTimestamp = false;

ofLogger::ofLogger(){		
	// code from Poco LogRotation sample
	
	Poco::AutoPtr<Poco::SplitterChannel> splitterChannel(new Poco::SplitterChannel());

	Poco::AutoPtr<Poco::Channel> consoleChannel(new Poco::ConsoleChannel());
	Poco::AutoPtr<Poco::Channel> fileChannel(new Poco::FileChannel("sample.log"));
	Poco::AutoPtr<Poco::FileChannel> rotatedFileChannel(new Poco::FileChannel("rotated.log"));

	rotatedFileChannel->setProperty("rotation", "100");
	rotatedFileChannel->setProperty("archive", "timestamp");

	splitterChannel->addChannel(consoleChannel);
	splitterChannel->addChannel(fileChannel);
	splitterChannel->addChannel(rotatedFileChannel);

	Poco::AutoPtr<Poco::Formatter> formatter(new Poco::PatternFormatter("%t"));
	Poco::AutoPtr<Poco::Channel> formattingChannel(new Poco::FormattingChannel(formatter, splitterChannel));

	logger = &Poco::Logger::create("Logger", formattingChannel, Poco::Message::PRIO_TRACE);
}

ofLogger& ofLogger::instance(){
	static ofLogger * pointerToTheSingletonInstance = new ofLogger;
	return *pointerToTheSingletonInstance;
}

void ofLogger::log(ofLogLevel logLevel, string message){
	string timestamp;
	
	if(bTimestamp){
		Poco::LocalDateTime now;
		timestamp = Poco::DateTimeFormatter::format(now, s_timestampFormat)+" ";
	}
	
	switch(logLevel){
		case OF_LOG_SILENT:
			break;
			
		case OF_LOG_VERBOSE:
			ofLogger::instance().logger->trace(timestamp+"OF_VERBOSE: "+message);
			break;
			
		case OF_LOG_DEBUG:
			ofLogger::instance().logger->debug(timestamp+"OF_DEBUG: "+message);
			break;
			
		case OF_LOG_NOTICE:
			ofLogger::instance().logger->notice(timestamp+message);
			break;

		case OF_LOG_WARNING:
			ofLogger::instance().logger->warning(timestamp+"OF_WARNING: "+message);
			break;

		case OF_LOG_ERROR:
			ofLogger::instance().logger->error(timestamp+"OF_ERROR: "+message);
			break;
			
		case OF_LOG_FATAL_ERROR:
			ofLogger::instance().logger->fatal(timestamp+"OF_FATAL_ERROR: "+message);
			break;
	}
}


//-------------------------------------------------------
ofLogNotice::~ofLogNotice(){
	ofLogger::instance().log(logLevel, line.str());
}

/// timestamp controls
void ofLogNotice::enableTimestamp(){
	ofLogger::instance().bTimestamp = true;
}
void ofLogNotice::disableTimestamp(){
	ofLogger::instance().bTimestamp = false;
}
bool ofLogNotice::usingTimestamp(){
	return ofLogger::instance().bTimestamp;
}

//----------------------------------------------
void ofLog(ofLogLevel logLevel, string message){
	ofLogger::instance().log(logLevel, message);
}

//-------------------------------------------------------
void ofLog(ofLogLevel logLevel, const char* format, ...){
	char line[512];	// maybe too small?
	va_list(args);
	va_start(args, format);
		vsprintf(line, format, args);
	va_end(args);
	ofLogger::instance().log(logLevel, (string) line);
}

		