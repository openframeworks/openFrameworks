#pragma once

#include <ofConstants.h>

#include <Poco/AutoPtr.h>
#include <Poco/Logger.h>
#include <Poco/FileChannel.h>
#include <Poco/ConsoleChannel.h>
#include <Poco/SplitterChannel.h>

enum ofLogLevel{
	OF_LOG_VERBOSE 		= Poco::Message::PRIO_TRACE,	///< lowest log level
	OF_LOG_DEBUG 		= Poco::Message::PRIO_DEBUG,
	OF_LOG_NOTICE 		= Poco::Message::PRIO_NOTICE,	///< default log level
	OF_LOG_WARNING 		= Poco::Message::PRIO_WARNING,
	OF_LOG_ERROR 		= Poco::Message::PRIO_ERROR,
	OF_LOG_FATAL_ERROR 	= Poco::Message::PRIO_FATAL,	///< highest log level
	OF_LOG_SILENT	// this one is special and should always be last,
					// set ofSetLogLevel to OF_SILENT to not recieve any messages
};

#define OF_DEFAULT_LOG_LEVEL  OF_LOG_NOTICE

//------------------------------------------------------------------------------
/// \class ofLogger
/// \brief a singleton log class, this is where the magic happens ...
///
/// ofLogger is an interface to a global Poco logger. The logger prints lines
/// to the text console and/or a file with an optional header showing the date,
/// time, frameNum, and elapsed millis.
///
/// Each log line is prefixed by an optionally header and log level:
///
///	HEADER LOGLEVEL: LOG TOPIC: your message
///
/// Note: This class dosen't need to be access directy. Use the stream log classes
/// or the global functions.
///
/// See the singleton pattern for more info: http://en.wikipedia.org/wiki/Singleton_pattern
///
class ofLogger{

	public:
	
		/// singleton object access, creates a new object on the first call
		static ofLogger& instance();
		
		/// \section Log
		/// Log the a message as a full line at a specifed log level. Prints a
		/// newline automatically.
		void log(ofLogLevel logLevel, const string& message);
		void log(const string& logTopic, ofLogLevel logLevel, const string& message);
		
		/// \section Log Level
		/// Set the current log level. Messages with a log level below this level 
		/// are not printed.
		///	For example, if the log level is OF_LOG_WARNING:
		///  - OF_LOG_ERROR, OF_LOG_FATAL_ERROR messages are printed
		///  - OF_LOG_NOTICE, OF_LOG_DEBUG, & OF_LOG_VERBOSE message are not printed
		void setLevel(ofLogLevel logLevel);
		ofLogLevel getLevel();
		
		/// \section Console
		
		/// Log to the text console. (on by default)
		void enableConsole();
		void disableConsole();
		bool usingConsole();
		
		/// \section Log File
		
		//// Log to a file. (off by default)
		void enableFile();
		void disableFile();
		bool usingFile();
		
		/// Set the path to the log file. The default filename is "openframeworks.log"
		/// and is saved to the data folder. When setting your own file use
		/// ofToDataPath()+"/filename" to build the full path.
		void setFilePath(const string& file);
		string getFilePath();
		
		/// Set the automatic rotation of the log file. The log file will grow until
		/// it reaches a specified limit (time or size). It will then have a number or
		/// timestamp appended to it's filename and a new file will be created with the 
		/// original name. This keeps the files from growing arbitrarily large in memory!
		///
		/// Note: only one file rotation type is active at a time
		void enableFileRotationMins(unsigned int minutes);
		void enableFileRotationHours(unsigned int hours);
		void enableFileRotationDays(unsigned int days);
		void enableFileRotationMonths(unsigned int months);
		void enableFileRotationSize(unsigned int sizeKB);
		void disableFileRotation();
	
		/// Set the maximum number of files in the automtic log rotation.
		/// (defualt is 10)
		void setFileRotationMaxNum(unsigned int num);
		
		/// Set the suffix appended to the rotated log files:
		///  - number: logfile.log.#
		///	 - timestamp: logfile.log.YYYYMMDDHHMMSSms
		/// Note: only one type is active at a time
		void setFileRotationNumber();
		void setFileRotationTimestamp();
	
		/// \section Log Topics
		/// Log topics allow fine grained control of logging. Topics are a logging
		/// namespace hierarchy where the logging levels of each topic can be set
		/// independently. The log level is automatically added to the header.
		///
		/// Add a log topic and then log using that topic:
		///		ofLogAddTopic("topic");
		///		ofLogDebug("topic") << "a log message";
		///
		/// Hierarchies can be built by using a period in the topic: "topic.a" &
		/// "topic.b" are children of the "topic" parent topic. The log levels of
		/// the children can be set independently while setting the level of the 
		/// parent sets the level for all children. There is no limit to the 
		/// depth of the hierarchy.
		///
		/// Resetting the level of the log topic sets it to the level of the global
		/// logger.
		void addTopic(const string& logTopic, ofLogLevel logLevel=OF_DEFAULT_LOG_LEVEL);
		void removeTopic(const string& logTopic);
		bool topicExists(const string& logTopic);
		void setTopicLogLevel(const string& logTopic, ofLogLevel logLevel);
		void resetTopicLogLevel(const string& logTopic);
		
		/// \section Header

		/// Prefix the header to each line with a layout of:
		/// DATE TIME FRAMENUM MILLS
		/// 
		/// The header is disabled by default.
		///
		/// Note: The log level and log topic are not part of the header and are
		/// always printed.
		void enableHeader();
		void disableHeader();
		bool usingHeader();
		
		/// Show the date in the header: YYYY-MM-DD
		/// Enabled by default.
		void enableHeaderDate();
		void disableHeaderDate();
		bool usingHeaderDate();
		
		/// Show the time in the header: HH:MM:SS.ms
		/// Enabled by default.
		void enableHeaderTime();
		void disableHeaderTime();
		bool usingHeaderTime();
		
		/// Show the current frame num in the header.
		/// Disabled by default.
		void enableHeaderFrameNum();
		void disableHeaderFrameNum();
		bool usingHeaderFrameNum();
		
		/// Show the current elapsed millis in the header.
		/// Disabled by default.
		void enableHeaderMillis();
		void disableHeaderMillis();
		bool usingHeaderMillis();
		
	protected:

		Poco::AutoPtr<Poco::Logger> 			logger;				///< the logger
		Poco::AutoPtr<Poco::Channel> 			formattingChannel;	///< formatter (needed for creating topics)
		Poco::AutoPtr<Poco::SplitterChannel>	splitterChannel;	///< channel source mixer
		Poco::AutoPtr<Poco::ConsoleChannel> 	consoleChannel;		///< the console io channel
		Poco::AutoPtr<Poco::FileChannel> 		fileChannel;		///< the file io channel
		
		bool bConsole;	///< are we printing to the console?
		bool bFile;		///< are we printing to a file?
		
		bool bHeader;	///< are we printing the header?
		bool bDate;		///< print the date?
		bool bTime;		///< print the time?
		bool bFrameNum;	///< print the frame num?
		bool bMillis;	///< print the elapsed millis?
		
	private:
		
		/// logs the message to the specified logger
		void _log(ofLogLevel logLevel, const string& message, Poco::Logger* theLogger);
		
		/// static date format strings for the header
		static const string s_dateFormat;
		static const string s_timeFormat;
		static const string s_dateAndTimeFormat;
	
		// hide all the constructors, copy functions here
		ofLogger(ofLogger const&);    			// not defined, not copyable
		ofLogger& operator = (ofLogger const&);	// not defined, not assignable
		ofLogger();								// singleton constructor
};

//------------------------------------------------------------------------------
/// \class ofLogNotice
/// \brief a public streaming log interface
///
/// ofLogNotice accepts variables via the ostream operator << and builds a string
/// and logs it when the stream is finished (via the destructor). A newline is
/// printed automatically and all the stream controls (endl, flush, hex, etc)
/// work normally. The log level is explicitly OF_LOG_NOTICE, see the derived 
/// wrapper classes:
///
/// Usage: ofLogNotice() << "a string" << 100 << 20.234f;
///
/// Public control to the ofLogger is provided through wrapper functions.
///
/// class idea from:
/// 	http://www.gamedev.net/community/forums/topic.asp?topic_id=525405&whichpage=1&#3406418
/// how to catch std::endl (which is actually a func pointer):
/// 	http://yvan.seth.id.au/Entries/Technology/Code/std__endl.html
///
///
///	I would be nice if the golab functions ofLog(...) could be removed, then this
/// class could be renamed to ofLog ...
///
class ofLogNotice{

    public:

		ofLogNotice(){
			level = OF_LOG_NOTICE;
		}
		
		ofLogNotice(string logTopic){
			level = OF_LOG_NOTICE;
			topic = logTopic;
		}
// an interface to set the log level when using:
//
// ofLogNotice(OF_LOG_WARNING) << "a string;
//
// This is commented for now as setting the level directly seems redundant when
// the derived log level classes are available. It would be nice if this class
// was named ofLog ...
//
//        ofLogNotice(ofLogLevel logLevel){
//			level = logLevel;
//		}

        /// does the actual printing on when the ostream is done
        ~ofLogNotice();

        /// catch the << ostream with a template class to read any type of data
        template <class T> 
		ofLogNotice& operator<<(const T& value){
            message << value;
            return *this;
        }

        /// catch the << ostream function pointers such as std::endl and std::hex
        ofLogNotice& operator<<(std::ostream& (*func)(std::ostream&)){
			func(message);
            return *this;
        }
		
		/// \section Console Color
		
		/// Set the console text color 
		/// doesn't work in the xcode console - do we need this?
		/// works fine on the terminal though - not much use
		static void setConsoleColor(int color);
		static void restoreConsoleColor();

		/// \section Global Log Function Wrappers
		///
		/// Ok Ok, I know this looks scary ... but it's just a long list
		/// of static public wrappers to access the ofLogger class.
		/// 
		/// I know I probably should put the function implementation in the .cpp
		/// file, but it's late and I want to finish this. Some one else can do
		/// that. :P
		static void setLevel(ofLogLevel logLevel);
		static ofLogLevel getLevel();
		
		static void enableConsole()	{ofLogger::instance().enableConsole();}
		static void disableConsole()	{ofLogger::instance().disableConsole();}
		static bool usingConsole()		{return ofLogger::instance().usingConsole();}
		
		static void enableFile()		{ofLogger::instance().enableFile();}
		static void disableFile()		{ofLogger::instance().disableFile();}
		static bool usingFile()		{return ofLogger::instance().usingFile();}
		
		static bool setFilePath(const string& file)
			{ofLogger::instance().setFilePath(file);}
		static string getFilePath()	{return ofLogger::instance().getFilePath();}
		
		static void enableFileRotationMins(unsigned int minutes)
			{ofLogger::instance().enableFileRotationMins(minutes);}
		static void enableFileRotationHours(unsigned int hours)
			{ofLogger::instance().enableFileRotationHours(hours);}
		static void enableFileRotationDays(unsigned int days)
			{ofLogger::instance().enableFileRotationDays(days);}
		static void enableFileRotationMonths(unsigned int months)
			{ofLogger::instance().enableFileRotationMonths(months);}
		static void enableFileRotationSize(unsigned int sizeKB)
			{ofLogger::instance().enableFileRotationSize(sizeKB);}
		static void disableFileRotation()	{ofLogger::instance().disableFileRotation();}

		static void setFileRotationMaxNum(unsigned int num)
			{ofLogger::instance().setFileRotationMaxNum(num);}
		
		static void setFileRotationNumber()		{ofLogger::instance().setFileRotationNumber();}
		static void setFileRotationTimestamp()	{ofLogger::instance().setFileRotationTimestamp();}
	
		static void addTopic(const string& logTopic, ofLogLevel logLevel=OF_DEFAULT_LOG_LEVEL)
			{ofLogger::instance().addTopic(logTopic, logLevel);}
		static void removeTopic(const string& logTopic)	{ofLogger::instance().removeTopic(logTopic);}
		static bool topicExists(const string& logTopic)
			{return ofLogger::instance().topicExists(logTopic);}
		static void setTopicLogLevel(const string& logTopic, ofLogLevel logLevel)
			{ofLogger::instance().setTopicLogLevel(logTopic, logLevel);}
		static void resetTopicLogLevel(const string& logTopic)
			{ofLogger::instance().resetTopicLogLevel(logTopic);}
		
		static void enableHeader()		{ofLogger::instance().enableHeader();}
		static void disableHeader()		{ofLogger::instance().disableHeader();}
		static bool usingHeader()		{return ofLogger::instance().usingHeader();}
		
		static void enableHeaderDate()		{ofLogger::instance().enableHeaderDate();}
		static void disableHeaderDate()		{ofLogger::instance().disableHeaderDate();}
		static bool usingHeaderDate()		{return ofLogger::instance().usingHeaderDate();}
		
		static void enableHeaderTime()		{ofLogger::instance().enableHeaderTime();}
		static void disableHeaderTime()		{ofLogger::instance().disableHeaderTime();}
		static bool usingHeaderTime()		{return ofLogger::instance().usingHeaderTime();}

		static void enableHeaderFrameNum()	{ofLogger::instance().enableHeaderFrameNum();}
		static void disableHeaderFrameNum()	{ofLogger::instance().disableHeaderFrameNum();}
		static bool usingHeaderFrameNum()	{return ofLogger::instance().usingHeaderFrameNum();}
		
		static void enableHeaderMillis()	{ofLogger::instance().enableHeaderMillis();}
		static void disableHeaderMillis()	{ofLogger::instance().disableHeaderMillis();}
		static bool usingHeaderMillis()		{return ofLogger::instance().usingHeaderMillis();}
		
	protected:
	
		ofLogLevel level;			///< log level
			
	private:
	
		std::string topic;			///< log topic
        std::ostringstream message;	///< temp buffer
		
		ofLogNotice(ofLogNotice const&) {}        		// not defined, not copyable
        ofLogNotice& operator = (ofLogNotice const&) {}	// not defined, not assignable
};

//--------------------------------------------------------------
///
/// \section Log Aliases
/// derived log classes for easy to use names
///
class ofLogVerbose : public ofLogNotice{
	public:
		ofLogVerbose() {level = OF_LOG_VERBOSE;}
		ofLogVerbose(string logTopic) : ofLogNotice(logTopic) {}
};

class ofLogDebug : public ofLogNotice{
	public:
		ofLogDebug() {level = OF_LOG_DEBUG;}
		ofLogDebug(string logTopic) : ofLogNotice(logTopic) {}
};

class ofLogWarning : public ofLogNotice{
	public:
		ofLogWarning() {level = OF_LOG_WARNING;}
		ofLogWarning(string logTopic) : ofLogNotice(logTopic) {}
};

class ofLogError : public ofLogNotice{
	public:
		ofLogError() {level = OF_LOG_ERROR;}
		ofLogError(string logTopic) : ofLogNotice(logTopic) {}
};

class ofLogFatalError : public ofLogNotice{
	public:
		ofLogFatalError() {level = OF_LOG_FATAL_ERROR;}
		ofLogFatalError(string logTopic) : ofLogNotice(logTopic) {}
};

//-------------------------------------------------------
///
///	\section Legacy Log Functions
///	
/// These are implemented as wrappers around the ofLogger class.
///
///	log a message at a specific log level
void ofLog(ofLogLevel logLevel, const string& message);
void ofLog(ofLogLevel logLevel, const char* format, ...);

void ofSetLogLevel(ofLogLevel logLevel);
ofLogLevel ofGetLogLevel();

//----------------------------------------------------------------------------
///
///	\section Global Log Function Wrappers
///
/// I know this looks long and scary, but they are just public wrappers around
/// the ofLogger class.
///
/// Hopefully these could be removed in the near future ...
///
void ofLogEnableConsole();
void ofLogDisableConsole();
bool ofLogUsingConsole();

void ofLogEnableFile();
void ofLogDisableFile();
bool ofLogUsingFile();

void ofLogSetFilePath(const string& file);
string ofLogGetFilePath();

void ofLogEnableFileRotationMins(unsigned int minutes);
void ofLogEnableFileRotationHours(unsigned int hours);
void ofLogEnableFileRotationDays(unsigned int days);
void ofLogEnableFileRotationMonths(unsigned int months);
void ofLogEnableFileRotationSize(unsigned int sizeKB);
void ofLogDisableFileRotation();

void ofLogSetFileRotationMaxNum(unsigned int num);

void ofLogSetFileRotationNumber();
void ofLogSetFileRotationTimestamp();

void ofLogAddTopic(const string& logTopic, ofLogLevel logLevel=OF_DEFAULT_LOG_LEVEL);
void ofLogRemoveTopic(const string& logTopic);
bool ofLogTopicExists(const string& logTopic);
void ofLogSetTopicLogLevel(const string& logTopic, ofLogLevel logLevel);
void ofLogResetTopicLogLevel(const string& logTopic);

void ofLogEnableHeader();
void ofLogDisableHeader();
bool ofLogUsingHeader();

void ofLogEnableHeaderDate();
void ofLogDisableHeaderDate();
bool ofLogUsingHeaderDate();

void ofLogEnableHeaderTime();
void ofLogDisableHeaderTime();
bool ofLogUsingHeaderTime();

void ofLogEnableHeaderFrameNum();
void ofLogDisableHeaderFrameNum();
bool ofLogUsingHeaderFrameNum();

void ofLogEnableHeaderMillis();
void ofLogDisableHeaderMillis();
bool ofLogUsingHeaderMillis();
