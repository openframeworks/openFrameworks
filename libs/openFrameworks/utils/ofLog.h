#pragma once

#include "ofConstants.h"
#include "ofFileUtils.h"
#include "ofTypes.h"

enum ofLogLevel{
	OF_LOG_VERBOSE,
	OF_LOG_NOTICE,
	OF_LOG_WARNING,
	OF_LOG_ERROR,
	OF_LOG_FATAL_ERROR,
	OF_LOG_SILENT	// this one is special and should always be last,
					// set ofSetLogLevel to OF_SILENT to not receive any messages
};

//--------------------------------------------------
void ofSetLogLevel(ofLogLevel level);
void ofSetLogLevel(string module, ofLogLevel level);
ofLogLevel ofGetLogLevel();

/// get log level name as a string
/// set pad to true if you want all log level names to be the same length
string ofGetLogLevelName(ofLogLevel level, bool pad=false);

//--------------------------------------------------
class ofBaseLoggerChannel;

void ofLogToFile(const string & path, bool append=false);
void ofLogToConsole();

void ofSetLoggerChannel(ofPtr<ofBaseLoggerChannel> loggerChannel);

//------------------------------------------------------------------------------
/// \class ofLog
/// \brief a streaming log interface
///
/// ofLog accepts variables via the ostream operator << and builds a string
/// and logs it when the stream is finished (via the destructor). A newline is
/// printed automatically and all the stream controls (endl, flush, hex, etc)
/// work normally. The log level is explicitly OF_LOG_NOTICE unless set, see the 
/// derived wrapper classes:
///
/// Usage: ofLog() << "a string" << 100 << 20.234f;
///
/// It also accepts the legacy ofLog interface: ofLog(ofLogLevel level, string message):
///
/// Usage: ofLog(OF_LOG_ERROR, "another string");
///
/// class idea from:
/// 	http://www.gamedev.net/community/forums/topic.asp?topic_id=525405&whichpage=1&#3406418
/// how to catch std::endl (which is actually a func pointer):
/// 	http://yvan.seth.id.au/Entries/Technology/Code/std__endl.html
///
/// \author Dan Wilcox <danomatika@gmail.com> danomatika.com
///
class ofLog{
	public:
	
		/// log at notice level
		ofLog();
		
		/// set the log level
		ofLog(ofLogLevel level);
		
		/// the legacy ofLog interfaces
		ofLog(ofLogLevel level, const string & message);
		ofLog(ofLogLevel level, const char* format, ...);
		
		/// does the actual printing when the ostream is done
		virtual ~ofLog();
		
		/// catch the << ostream with a template class to read any type of data
		template <class T> 
			ofLog& operator<<(const T& value){
			message << value << padding;
			return *this;
		}
		
		/// catch the << ostream function pointers such as std::endl and std::hex
		ofLog& operator<<(std::ostream& (*func)(std::ostream&)){
			func(message);
			return *this;
		}
		
		/// put a space between stream operator calls?
		static void setAutoSpace(bool autoSpace);
		
		/// set the logging channel destinations for messages
		static void setChannel(ofPtr<ofBaseLoggerChannel> channel);
	
	protected:
		ofLogLevel level;			///< log level
		bool bPrinted;				///< has the msg been printed in the constructor? 
		string module;				///< the destination module for this message
		
		/// print a log line
		void _log(ofLogLevel level, const string & module, const string & message);
		bool checkLog(ofLogLevel level, const string & module);
	
		static ofPtr<ofBaseLoggerChannel> channel;	///< the message destination
	
	private:
		std::ostringstream message;	///< temp buffer
		
		static bool bAutoSpace;		///< add a space between messages? 
		
		ofLog(ofLog const&) {}        					// not defined, not copyable
		ofLog& operator=(ofLog& from) {return *this;}	// not defined, not assignable
		
		static string padding;						///< the padding between ostream calls
};

//--------------------------------------------------------------
///
/// \section Log Aliases
/// derived log classes for easy to use names
///
class ofLogVerbose : public ofLog{
	public:
		ofLogVerbose(const string &module="");
		ofLogVerbose(const string & module, const string & message);
		ofLogVerbose(const string & module, const char* format, ...);
};

class ofLogNotice : public ofLog{
	public:
		ofLogNotice(const string & module="");
		ofLogNotice(const string & module, const string & message);
		ofLogNotice(const string & module, const char* format, ...);
};

class ofLogWarning : public ofLog{
	public:
		ofLogWarning(const string & module="");
		ofLogWarning(const string & module, const string & message);
		ofLogWarning(const string & module, const char* format, ...);
};

class ofLogError : public ofLog{
	public:
		ofLogError(const string & module="");
		ofLogError(const string & module, const string & message);
		ofLogError(const string & module, const char* format, ...);
};

class ofLogFatalError : public ofLog{
	public:
		ofLogFatalError(const string & module="");
		ofLogFatalError(const string & module, const string & message);
		ofLogFatalError(const string & module, const char* format, ...);
};

//--------------------------------------------------------------
///
/// \section Logger Channels
/// swappable logging destinations, derive your own for custom logging
///
class ofBaseLoggerChannel{
public:
	virtual ~ofBaseLoggerChannel(){};
	virtual void log(ofLogLevel level, const string & module, const string & message)=0;
	virtual void log(ofLogLevel level, const string & module, const char* format, ...)=0;
	virtual void log(ofLogLevel level, const string & module, const char* format, va_list args)=0;
};

class ofConsoleLoggerChannel: public ofBaseLoggerChannel{
public:
	virtual ~ofConsoleLoggerChannel(){};
	void log(ofLogLevel level, const string & module, const string & message);
	void log(ofLogLevel level, const string & module, const char* format, ...);
	void log(ofLogLevel level, const string & module, const char* format, va_list args);
};

class ofFileLoggerChannel: public ofBaseLoggerChannel{
public:
	ofFileLoggerChannel();
	ofFileLoggerChannel(const string & path, bool append);
	virtual ~ofFileLoggerChannel();

	void setFile(const string & path,bool append=false);

	void log(ofLogLevel level, const string & module, const string & message);
	void log(ofLogLevel level, const string & module, const char* format, ...);
	void log(ofLogLevel level, const string & module, const char* format, va_list args);

	void close();

private:
	ofFile file;
};
