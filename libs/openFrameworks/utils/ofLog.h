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
					// set ofSetLogLevel to OF_SILENT to not recieve any messages
};


class ofBaseLoggerChannel;

//--------------------------------------------------
void ofSetLogLevel(ofLogLevel logLevel);
void ofSetLogLevel(string module, ofLogLevel logLevel);
ofLogLevel ofGetLogLevel();

void ofSetLoggerChannel(ofPtr<ofBaseLoggerChannel> loggerChannel);
string ofGetLogLevelName(ofLogLevel level);

void ofLogToFile(const string & path, bool append=false);
void ofLogToConsole();

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
		ofLog(ofLogLevel logLevel);
		
		/// the legacy ofLog interfaces
		ofLog(ofLogLevel logLevel, const string & message);
		ofLog(ofLogLevel logLevel, const char* format, ...);

        /// does the actual printing when the ostream is done
        virtual ~ofLog();

        /// catch the << ostream with a template class to read any type of data
        template <class T> 
		ofLog& operator<<(const T& value){
            message << value;
            return *this;
        }

        /// catch the << ostream function pointers such as std::endl and std::hex
        ofLog& operator<<(std::ostream& (*func)(std::ostream&)){
			func(message);
            return *this;
        }

        static void setChannel(ofPtr<ofBaseLoggerChannel> channel);
		
	protected:
		ofLogLevel level;			///< log level
		bool bPrinted;				///< has the msg been printed in the constructor? 
		string module;

	
		/// print a log line
		void _log(ofLogLevel level, const string & module, const string & message);
		bool checkLog(ofLogLevel level, const string & module);

	private:
        std::ostringstream message;	///< temp buffer
		
		ofLog(ofLog const&) {}        					// not defined, not copyable
        ofLog& operator=(ofLog& from) {return *this;}	// not defined, not assignable

        static ofPtr<ofBaseLoggerChannel> channel;
};

//--------------------------------------------------------------
///
/// \section Log Aliases
/// derived log classes for easy to use names
///
class ofLogVerbose : public ofLog{
	public:
		ofLogVerbose(const string &module="OF");
		ofLogVerbose(const string & module, const string & message);
};

class ofLogNotice : public ofLog{
	public:
		ofLogNotice(const string & module="OF");
		ofLogNotice(const string & module, const string & message);
};

class ofLogWarning : public ofLog{
	public:
		ofLogWarning(const string & module="OF");
		ofLogWarning(const string & module, const string & message);
};

class ofLogError : public ofLog{
	public:
		ofLogError(const string & module="OF");
		ofLogError(const string & module, const string & message);
};

class ofLogFatalError : public ofLog{
	public:
		ofLogFatalError(const string & module="OF");
		ofLogFatalError(const string & module, const string & message);
};

class ofBaseLoggerChannel{
public:
	virtual ~ofBaseLoggerChannel(){};
	virtual void log(ofLogLevel level, const string & module, const string & message)=0;
	virtual void log(ofLogLevel logLevel, const string & module, const char* format, ...)=0;
	virtual void log(ofLogLevel logLevel, const string & module, const char* format, va_list args)=0;
};

class ofConsoleLoggerChannel: public ofBaseLoggerChannel{
public:
	virtual ~ofConsoleLoggerChannel(){};
	void log(ofLogLevel level, const string & module, const string & message);
	void log(ofLogLevel logLevel, const string & module, const char* format, ...);
	void log(ofLogLevel logLevel, const string & module, const char* format, va_list args);
};

class ofFileLoggerChannel: public ofBaseLoggerChannel{
public:
	ofFileLoggerChannel();
	ofFileLoggerChannel(const string & path, bool append);
	virtual ~ofFileLoggerChannel();

	void setFile(const string & path,bool append=false);

	void log(ofLogLevel level, const string & module, const string & message);
	void log(ofLogLevel logLevel, const string & module, const char* format, ...);
	void log(ofLogLevel logLevel, const string & module, const char* format, va_list args);

	void close();

private:
	ofFile file;
};
