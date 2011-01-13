#pragma once

#include <ofConstants.h>

#include <Poco/Logger.h>

enum ofLogLevel{
	OF_LOG_VERBOSE 		= Poco::Message::PRIO_TRACE,
	OF_LOG_DEBUG 		= Poco::Message::PRIO_DEBUG,
	OF_LOG_NOTICE 		= Poco::Message::PRIO_NOTICE,
	OF_LOG_WARNING 		= Poco::Message::PRIO_WARNING,
	OF_LOG_ERROR 		= Poco::Message::PRIO_ERROR,
	OF_LOG_FATAL_ERROR 	= Poco::Message::PRIO_FATAL,
	OF_LOG_SILENT	//this one is special and should always be last - set ofSetLogLevel to OF_SILENT to not recieve any messages
};		

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
		
		friend void ofLog(ofLogLevel level, string message);
		friend void ofLog(ofLogLevel logLevel, const char *format, ...);
	
		/**
			\brief singleton data access
			\return a reference to itself

			creates a new object on the first call
		**/
		static ofLogger& instance();
		
		void log(ofLogLevel logLevel, string message);
		
//		void addVerboseName(const string name, const ofLogLevel logLevel)
//		{
//			verboseNames.insert(pair<string,ofLogLevel>(name, logLevel));
//		}
//		
//		void removeVerboseName(const string name)
//		{
//			verboseNames.erase(verboseNames.find(name));
//		}
//		
//		void clearVerboseName()
//		{
//			verboseNames.clear();
//		}

		Poco::Logger* logger;
		
		static bool bTimestamp;
		
	private:
		
		static const string s_timestampFormat;
		
		//std::map<string, ofLogLevel> verboseNames;
	
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
class ofLogNotice{
    public:

        //
        //    \brief  select log type
        //    \param  type    log type to log to
        //
		ofLogNotice(){
			logLevel = OF_LOG_WARNING;
		}
        ofLogNotice(ofLogLevel logLevel){
			this->logLevel = logLevel;
		}

        /// does the actual printing on exit
        ~ofLogNotice();

        /// catch << with a template class to read any type of data
        template <class T> 
		ofLogNotice& operator<<(const T& value){
            line << value;
            return *this;
        }

        /// catch << ostream function pointers such as std::endl and std::hex
        ofLogNotice& operator<<(std::ostream& (*func)(std::ostream&)){
			func(line);
            return *this;
        }
		
		/// timestamp controls
		static void enableTimestamp();
		static void disableTimestamp();
		static bool usingTimestamp();

		static void setLevel(ofLogLevel logLevel){
			ofLogger::instance().logger->setLevel(logLevel);
		}
		
	private:
	
        ofLogLevel logLevel;			///< log level
        std::ostringstream line;	///< temp buffer
		
		ofLogNotice(ofLogNotice const&) {}        		// not defined, not copyable
        ofLogNotice& operator = (ofLogNotice const&) {}	// not defined, not assignable
};

// derived log classes for nice names

//--------------------------------------------------
class ofLogVerbose : public ofLogNotice{
	public:
		ofLogVerbose() : ofLogNotice(OF_LOG_VERBOSE) {}
};

//--------------------------------------------------
class ofLogDebug : public ofLogNotice{
	public:
		ofLogDebug() : ofLogNotice(OF_LOG_DEBUG) {}
};

//--------------------------------------------------
class ofLogWarning : public ofLogNotice{
	public:
		ofLogWarning() : ofLogNotice(OF_LOG_WARNING) {}
};

//--------------------------------------------------
class ofLogError : public ofLogNotice{
	public:
		ofLogError() : ofLogNotice(OF_LOG_ERROR) {}
};

//--------------------------------------------------
class ofLogFatalError : public ofLogNotice{
	public:
		ofLogFatalError() : ofLogNotice(OF_LOG_FATAL_ERROR) {}
};

//----------------------------------------------
void ofLog(ofLogLevel logLevel, string message);
void ofLog(ofLogLevel logLevel, const char* format, ...);
