#pragma once

#include <ofConstants.h>

#include <Poco/AutoPtr.h>
#include <Poco/Logger.h>
#include <Poco/FileChannel.h>
#include <Poco/ConsoleChannel.h>
#include <Poco/SplitterChannel.h>

enum ofLogLevel{
	OF_LOG_VERBOSE 		= Poco::Message::PRIO_TRACE,
	OF_LOG_DEBUG 		= Poco::Message::PRIO_DEBUG,
	OF_LOG_NOTICE 		= Poco::Message::PRIO_NOTICE,
	OF_LOG_WARNING 		= Poco::Message::PRIO_WARNING,
	OF_LOG_ERROR 		= Poco::Message::PRIO_ERROR,
	OF_LOG_FATAL_ERROR 	= Poco::Message::PRIO_FATAL,
	OF_LOG_SILENT	//this one is special and should always be last - set ofSetLogLevel to OF_SILENT to not recieve any messages
};

#define OF_DEFAULT_LOG_LEVEL  OF_LOG_NOTICE

//-----------------------------------------------------------
/// singleton Log data class
class ofLogger{
	protected:
	
		// only the loggers have access
		friend class ofLogNotice;
		friend class ofLogDebug;
		friend class ofLogVerbose;
		friend class ofLogWarning;
		friend class ofLogError;
		friend class ofLogFatalError;
		
		friend void ofLog(ofLogLevel level, const string& message);
		friend void ofLog(ofLogLevel logLevel, const char* format, ...);
	
	public:
		/**
			\brief singleton data access
			\return a reference to itself

			creates a new object on the first call
		**/
		static ofLogger& instance();
		
		void log(ofLogLevel logLevel, const string& message);
		void log(const string& logTopic, ofLogLevel logLevel, const string& message);
		
		/// log to the console (on by default)
		void enableConsole();
		void disableConsole();
		bool usingConsole();
		
		/// log to a file (off by default)
		bool setFilePath(const string& file);
		string getFilePath();
		void enableFile();
		void disableFile();
		bool usingFile();
		
		/// set the automatic rotation of the log file
		void enableFileRotationMins(unsigned int minutes);
		void enableFileRotationHours(unsigned int hours);
		void enableFileRotationDays(unsigned int days);
		void enableFileRotationMonths(unsigned int months);
		void enableFileRotationSize(unsigned int sizeKB);
		void disableFileRotation();
	
		/// log topics
		void addTopic(const string& logTopic, ofLogLevel logLevel=OF_LOG_NOTICE);
		void removeTopic(const string& logTopic);
		void clearTopics();
		bool topicExists(const string& logTopic);
		void setTopicLogLevel(const string& logTopic, ofLogLevel logLevel);
		void resetTopicLogLevel(const string& logTopic);
		
		/// header control
		void enableHeader();
		void disableHeader();
		bool usingHeader();
		
		/// datestamp controls
		void enableDate();
		void disableDate();
		bool usingDate();
		
		/// timestamp controls
		void enableTime();
		void disableTime();
		bool usingTime();
		
		/// framestamp controls
		void enableFrameNum();
		void disableFrameNum();
		bool usingFrameNum();
		
		/// millisstamp controls
		void enableMillis();
		void disableMillis();
		bool usingMillis();
		
	protected:

		Poco::AutoPtr<Poco::Logger> 			logger;				///< the logger
		Poco::AutoPtr<Poco::Channel> 			formattingChannel;	///< formatter
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
		
		// logs the message to the specified logger
		void _log(ofLogLevel logLevel, const string& message, Poco::Logger* theLogger);
		
		// date formats for the header
		static const string s_dateFormat;
		static const string s_timeFormat;
		static const string s_dateAndTimeFormat;
		
		std::map<string, ofLogLevel> topics;
	
		// hide all the constructors, copy functions here
		ofLogger(ofLogger const&);    			// not defined, not copyable
		ofLogger& operator = (ofLogger const&);	// not defined, not assignable
		ofLogger();					// singleton constructor
};

/**
    \class  Log
    \brief  a simple logger

    class idea from:
        http://www.gamedev.net/community/forums/topic.asp?topic_id=525405&whichpage=1&#3406418
    how to catch std::endl (which is actually a func pointer):
        http://yvan.seth.id.au/Entries/Technology/Code/std__endl.html
**/
class ofLogNotice {

    public:

        //
        //    \brief  select log type
        //    \param  type    log type to log to
        //
		ofLogNotice() {
			level = OF_LOG_NOTICE;
		}
        ofLogNotice(ofLogLevel logLevel) {
			level = logLevel;
		}
		ofLogNotice(string logTopic) {
			level = OF_LOG_NOTICE;
			topic = logTopic;
		}

        /// does the actual printing on exit
        ~ofLogNotice();

        /// catch << with a template class to read any type of data
        template <class T> 
		ofLogNotice& operator<<(const T& value) {
            message << value;
            return *this;
        }

        /// catch << ostream function pointers such as std::endl and std::hex
        ofLogNotice& operator<<(std::ostream& (*func)(std::ostream&)) {
			func(message);
            return *this;
        }

//		/// datestamp constrols
//		static void enableDate();
//		static void disableDate();
//		static bool usingDate();
//		
//		/// timestamp controls
//		static void enableTime();
//		static void disableTime();
//		static bool usingTime();
//		
//		/// timestamp controls
//		static void enableFrame();
//		static void disableFrame();
//		static bool usingFrame();

		/// global log level controls
		static void setLevel(ofLogLevel logLevel);
		static ofLogLevel getLevel();
		
	protected:
	
		ofLogLevel level;	///< log level
			
	private:
	
		string topic;		///< log topic

        std::ostringstream message;	///< temp buffer
		
		ofLogNotice(ofLogNotice const&) {}        		// not defined, not copyable
        ofLogNotice& operator = (ofLogNotice const&) {}	// not defined, not assignable
};

// derived log classes for nice names

//--------------------------------------------------
class ofLogVerbose : public ofLogNotice {
	public:
		ofLogVerbose() : ofLogNotice(OF_LOG_VERBOSE) {}
		ofLogVerbose(string logTopic) : ofLogNotice(logTopic) {}
};

//--------------------------------------------------
class ofLogDebug : public ofLogNotice {
	public:
		ofLogDebug() : ofLogNotice(OF_LOG_DEBUG) {}
		ofLogDebug(string logTopic) : ofLogNotice(logTopic) {}
};

//--------------------------------------------------
class ofLogWarning : public ofLogNotice {
	public:
		ofLogWarning() : ofLogNotice(OF_LOG_WARNING) {}
		ofLogWarning(string logTopic) : ofLogNotice(logTopic) {}
};

//--------------------------------------------------
class ofLogError : public ofLogNotice {
	public:
		ofLogError() : ofLogNotice(OF_LOG_ERROR) {}
		ofLogError(string logTopic) : ofLogNotice(logTopic) {}
};

//--------------------------------------------------
class ofLogFatalError : public ofLogNotice {
	public:
		ofLogFatalError() : ofLogNotice(OF_LOG_FATAL_ERROR) {}
		ofLogFatalError(string logTopic) : ofLogNotice(logTopic) {}
};

//----------------------------------------------
void ofLog(ofLogLevel logLevel, const string& message);
void ofLog(ofLogLevel logLevel, const char* format, ...);
