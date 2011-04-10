#pragma once

#include "ofConstants.h"

enum ofLogLevel{
	OF_LOG_VERBOSE,
	OF_LOG_NOTICE,
	OF_LOG_WARNING,
	OF_LOG_ERROR,
	OF_LOG_FATAL_ERROR,
	OF_LOG_SILENT	// this one is special and should always be last,
					// set ofSetLogLevel to OF_SILENT to not recieve any messages
};

//--------------------------------------------------
void ofSetLogLevel(ofLogLevel logLevel);
ofLogLevel ofGetLogLevel();

//------------------------------------------------------------------------------
/// \class ofLog
/// \brief a public streaming log interface
///
/// ofLog accepts variables via the ostream operator << and builds a string
/// and logs it when the stream is finished (via the destructor). A newline is
/// printed automatically and all the stream controls (endl, flush, hex, etc)
/// work normally. The log level is explicitly OF_LOG_NOTICE unless set, see the 
/// derived wrapper classes:
///
/// Usage: ofLog() << "a string" << 100 << 20.234f;
///
/// or:    ofLog(OF_LOG_ERROR, "another string"); 
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

		/// use the default log level
		ofLog();
		
		/// set the log level
		ofLog(ofLogLevel logLevel);
		
		/// the legacy ofLog interfaces
		ofLog(ofLogLevel logLevel, string message);
		ofLog(ofLogLevel logLevel, const char* format, ...);

        /// does the actual printing when the ostream is done
        ~ofLog();

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
		
	protected:
	
		ofLogLevel level;			///< log level
		bool bPrinted;				///< has the msg been printed in the constructor? 
					
	private:
	
		/// print a log line
		void _log(ofLogLevel logLevel, string message);
	
        std::ostringstream message;	///< temp buffer
		
		ofLog(ofLog const&) {}        					// not defined, not copyable
        ofLog& operator=(ofLog& from) {return *this;}	// not defined, not assignable
};

//--------------------------------------------------------------
///
/// \section Log Aliases
/// derived log classes for easy to use names
///
class ofLogVerbose : public ofLog{
	public:
		ofLogVerbose()	{level = OF_LOG_VERBOSE; bPrinted=false;}
};

class ofLogWarning : public ofLog{
	public:
		ofLogWarning() {level = OF_LOG_WARNING; bPrinted=false;}
};

class ofLogError : public ofLog{
	public:
		ofLogError() {level = OF_LOG_ERROR; bPrinted=false;}
};

class ofLogFatalError : public ofLog{
	public:
		ofLogFatalError() {level = OF_LOG_FATAL_ERROR; bPrinted=false;}
};

