#pragma once

#include "Poco/ErrorHandler.h"
#include "ofConstants.h"
#include "ofFileUtils.h"
#include "ofTypes.h"

/// \file
/// ofLog provides an interface for writing text output from your app.
/// It's basically a more useful version of `cout` or `printf` where
/// the output can be filtered and written to the console or to a file.
///
/// Sometimes you want to be able to see when something has happened inside
/// the code, but don't need to draw something visually. Oftentimes it's
/// more then enough to print out the state of a few variables when debugging.
/// Other times you need to know if a crash happened while your app was
/// running somewhere, so you log messages and variables to a file you can
/// read after the program crashes.
///
/// ### Log Levels
/// You can set the logging level so only messages above a certain priority
/// are shown. This is useful if you want see lots of messages when debugging,
/// but then set a higher level so only warnings and errors appear for users.
///
/// See ofSetLogLevel(ofLogLevel level) for more details
///
/// ### Usage
/// There are 2 ways you can use ofLog:
///
/// #### Functional: as a function taking a message
///
/// ~~~~{.cpp}
/// // send a single string message, setting the log level
/// ofLog(OF_LOG_NOTICE, "the number is " + ofToString(10));
/// 
/// // the legacy printf style
/// ofLog(OF_LOG_NOTICE, "the number is %d", 10); 
/// ~~~~
///
/// #### Stream: as a stream using the << stream operator
///
/// ~~~~{.cpp}
/// // the stream style, setting the log level
/// ofLog(OF_LOG_NOTICE) << "the number is " << 10;
/// 
/// // this is the same as the last line, but only sends at log level notice
/// ofLog() << "the number is " << 10;
/// 
/// // there are also log level specific stream objects,
/// // one for each level except OF_LOG_SILENT
/// ofLogVerbose() << "a verbose print"
/// ofLogNotice() << "a regular notice print";
/// ofLogWarning() << "uh oh, a warning";
/// ofLogError() << "oh no, an error occurred!";
/// ofLogFatalError() << "accckkk, a fatal error!!";
/// ~~~~
/// 
/// **Note**: The log level specific stream objects also take a string argument 
/// for the "module". A module is a string that is added to the beginning of 
/// the log line and can be used to separate logging messages by setting an 
/// independent log level for **that module only**. 
/// This module-specific log level has no effect on other modules. 
/// 
/// See ofSetLogLevel(string module, ofLogLevel level) for more details.
/// 
/// Example of logging to a specific module:
/// ~~~~{.cpp}
/// // log to a module called "Hello"
/// ofLogWarning("Hello") << "a warning print";
/// ~~~~
/// **Warning**: It is important to understand that the log level specific stream objects 
/// take the module name as an argument and the log messages via the << operator. 
/// Putting your message as a string argument inside the parentheses uses that message as 
/// a *module* and so nothing will be printed:
/// 
/// ~~~~{.cpp}
/// // this prints a warning message
/// ofLogWarning() << "a warning print";
/// 
/// // !!! this does not print a message as the string "a warning print" is the module argument !!!
/// ofLogWarning("a warning print");
/// 
/// // this prints a warning message to the "Hello" module
/// ofLogWarning("Hello") << "a warning print";
/// ~~~~
/// 
/// ####Log Message Redirection
/// 
/// It's useful to be able to record log messages to a file or send them to a 
/// custom destination.
///
/// For log redirection see
/// - ofLogToFile()
/// - ofLogToConsole()
/// - ofSetLoggerChannel()


/// \cond INTERNAL
/// printf annotations for automatic format checking in GCC
#ifdef __GNUC__
#define OF_PRINTF_ATTR(x, y) __attribute__ ((format (printf, x, y)))
#else
#define OF_PRINTF_ATTR(x, y)
#endif
/// \endcond


//--------------------------------------------------
/// \name Global logging level
/// \{

/// The supported logging levels. Default is `OF_LOG_NOTICE`
enum ofLogLevel{
	OF_LOG_VERBOSE,
	OF_LOG_NOTICE,
	OF_LOG_WARNING,
	OF_LOG_ERROR,
	OF_LOG_FATAL_ERROR,
	OF_LOG_SILENT	// this one is special and should always be last,
					// set ofSetLogLevel to OF_SILENT to not receive any messages
};

/// \brief Sets the logging level so only messages above a certain priority are shown.
///
/// This is useful if you want see lots of messages when debugging,
/// but then set a higher level so only warnings and errors appear for users.
///
/// ofLogLevel values are (in order of priority):
/// - `OF_LOG_VERBOSE`
/// - `OF_LOG_NOTICE`
/// - `OF_LOG_WARNING`
/// - `OF_LOG_ERROR`
/// - `OF_LOG_FATAL_ERROR`
/// - `OF_LOG_SILENT`
///
/// Following priority, setting a log level of `OF_LOG_ERROR`, means only error and fatal
/// error messages will be printed. Conversely, setting OF_LOG_VERBOSE means all log level
/// messages will be printed.
///
/// Default ofLogLevel is `OF_LOG_NOTICE`
/// \param level the ofLogLevel (and below) you want to show
void ofSetLogLevel(ofLogLevel level);


/// \brief set the logging level for only a specific module
/// 
/// Example of logging to a specific module:
/// ~~~~{.cpp}
/// // log to a module called "Hello"
/// ofLogWarning("Hello") << "a warning print";
/// ~~~~
void ofSetLogLevel(string module, ofLogLevel level);


/// \brief Get the currently set global logging level
ofLogLevel ofGetLogLevel();

/// \brief Get log level name as a string
/// \param level The ofLogLevel you want the name of
/// \param pad set to true if you want all log level names to be the same length
string ofGetLogLevelName(ofLogLevel level, bool pad=false);

/// \}

//--------------------------------------------------
/// \name Global logger channel
/// \{

class ofBaseLoggerChannel;

/// \brief Set the looging to output to a file instead of the console
/// \param path The path to the file to save the logging to
/// \param append Set to true if you want to append to the existing file
void ofLogToFile(const string & path, bool append=false);

/// \brief Set the logging to ouptut to the console
/// 
/// This is the default state, and is only needed to be called in case 
/// ofLogToFile() has been called
void ofLogToConsole();

/// \brief Set the logger to use a custom logger channel
void ofSetLoggerChannel(shared_ptr<ofBaseLoggerChannel> loggerChannel);

/// \}

//------------------------------------------------------------------------------
/// \class ofLog
/// \brief a streaming log interface
///
/// ofLog accepts variables via the ostream operator << and builds a string
/// and logs it when the stream is finished (via the destructor). A newline is
/// printed automatically and all the stream controls (endl, flush, hex, etc)
/// work normally. The log level is explicitly `OF_LOG_NOTICE` unless set.
///
/// Basic usage:
/// ~~~~~~~~~~~~~~
/// ofLog() << "a string" << 100 << 20.234f;
/// ~~~~~~~~~~~~~~
///
/// It also accepts the legacy ofLog interface: ofLog(ofLogLevel level, string message):
/// ~~~~~~~~~~~~~~
/// ofLog(OF_LOG_ERROR, "another string");
/// ~~~~~~~~~~~~~~
///
/// \author Dan Wilcox <danomatika@gmail.com> danomatika.com
///

// Class idea from http://www.gamedev.net/community/forums/topic.asp?topic_id=525405&whichpage=1&#3406418
// How to catch std::endl (which is actually a func pointer) http://yvan.seth.id.au/Entries/Technology/Code/std__endl.html

class ofLog{
	public:
	
		/// \name Logging
		/// \{

		/// \brief Start logging on notice level
		/// 
		/// ofLog provides a streaming log interface by accepting variables via the 
		/// ostream operator `<<` similar to cout and cerr.
		/// 
		/// It builds a string and logs it when the stream is finished. A newline 
		/// is printed automatically and all the stream controls (`endl`, `flush`, `hex`, etc) 
		/// work normally.
		/// 
		/// ~~~~{.cpp}
		/// 
		/// // converts incoming primitive types (int, float, etc) to strings automatically
		/// ofLog() << "a string" << 100 << 20.234f;
		/// 
		/// ~~~~
		/// 
		/// The log level is explicitly `OF_LOG_NOTICE`.
		ofLog();
		
		/// Start logging on a specific ofLogLevel
		/// 
		/// Example:
		/// ~~~~{.cpp}
		/// 
		/// // set the log level
		/// ofLog(OF_LOG_WARNING) << "a string" << 100 << 20.234f;
		/// 
		/// ~~~~
		/// 
		/// You can use the derived convenience classes as an alternative for specific log levels: 
		/// 
		/// 	ofLogVerbose()
		/// 	ofLogNotice()
		/// 	ofLogWarning()
		/// 	ofLogError()
		/// 	ofLogFatalError()
		/// 
		/// ~~~~{.cpp}
		/// 
		/// // set the log level
		/// ofLog(OF_LOG_WARNING) << "a string" << 100 << 20.234f;
		/// 
		/// // this is the same as above
		/// ofLogWarning() << "a string" << 100 << 20.234f;
		/// 
		/// ~~~~
		/// \param level The ofLogLevel to log on
		ofLog(ofLogLevel level);
	
	
		/// \brief Log a string at a specific log level.
		/// 
		/// Supply the logging message as a parameter to the function
		/// instead of as a stream.
		/// 
		/// The string message can be concatenated using the ofToString(const T& value) 
		/// conversion function:
		/// 
		/// ~~~~{.cpp}
		/// 
		/// // build a single string message
		/// ofLog(OF_LOG_NOTICE, "the number is " 
		/// + ofToString(10) + " and I have a float too " + ofToString(123.45f));
		/// 
		/// ~~~~
		ofLog(ofLogLevel level, const string & message);

		/// \brief Logs a message at a specific log level using the printf interface.
		///
		/// The message is built using the formatting from the C printf function and can be 
		/// used as a direct replacement. Essentially, the second argument is a string with 
		/// special formatting specifiers starting with '%' that specify where the following 
		/// variables go in the message. You can have as many variables as you want following 
		/// the logLevel and format string, but there must be a % specifier for each subsequent 
		/// variable.
		/// 
		/// For quick reference, here are a few of the most useful formatting specifiers:
		/// 
		/// * `%d`: integer number, `123`
		/// * `%f`: floating point number, `123.45`
		/// * `%s`: a C string ([null terminated](http://en.wikipedia.org/wiki/Null-terminated_string)); 
		/// this is not a C++ string, use [string::c_str()](http://www.cplusplus.com/reference/string/string/c_str/) 
		/// to get a C string from a C++ string
		/// * `%c`: a single character
		/// * `%x`: unsigned integer as a [hexidecimal](http://en.wikipedia.org/wiki/Hexadecimal) 
		/// number; `x` uses lower-case letters and `X` uses upper-case
		/// * `%%`: prints a `%` character
		/// 
		/// The specifier should match the variable type as it is used to tell the function 
		/// how to convert that primitive type (int, float, character, etc) into a string.
		/// 
		/// For instance, let's say we want to print two messages, a salutation and the 
		/// value of an int, a float, and a string variable:
		/// 
		/// ~~~~{.cpp}
		/// 
		/// // print a simple message with no variables
		/// ofLog(OF_LOG_WARNING, "welcome to the jungle");
		/// 
		/// // our variables
		/// float fun = 11.11;
		/// int games = 100;
		/// string theNames = "Dan, Kyle, & Golan";
		/// 
		/// // print a message with variables, sets the message format in the format string
		/// ofLog(OF_LOG_NOTICE, "we've got %d & %f, we got everything you want honey, we know %s", fun, games, theNames.c_str());
		/// 
		/// ~~~~
		/// 
		/// Note: `theNames.c_str()` returns a C string from theNames which is a C++ string object.
		/// 
		/// There are other formatting options such as setting the decimal precision of float objects 
		/// and the forward padding of numbers (i.e. 0001 instead of 1). 
		/// See the [Wikipedia printf format string article](http://en.wikipedia.org/wiki/Printf_format_string) 
		/// for more detailed information.
		ofLog(ofLogLevel level, const char* format, ...) OF_PRINTF_ATTR(3, 4);
		
		/// \}
	
		//--------------------------------------------------
		/// \name Logging configuration
		/// \{

		/// Let the logger automaticly add spaces between messages. Default is `false`.
		/// \param autoSpace Set to true to add spaces between messages
		static void setAutoSpace(bool autoSpace);
	
		/// Set the logging channel destinations for messages.
		/// This can be used to output to files instead of stdout.
		/// \sa ofFileLoggerChannel ofConsoleLoggerChannel
		static void setChannel(shared_ptr<ofBaseLoggerChannel> channel);
	
		/// \}

	
		/// \cond INTERNAL
	
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
	
		/// \endcond
	
	

	
	protected:
		/// \cond INTERNAL

		ofLogLevel level;			///< log level
		bool bPrinted;				///< has the msg been printed in the constructor? 
		string module;				///< the destination module for this message
		
		/// print a log line
		void _log(ofLogLevel level, const string & module, const string & message);
		bool checkLog(ofLogLevel level, const string & module);
	
		static shared_ptr<ofBaseLoggerChannel> channel;	///< the message destination
		/// \endcond
	
	private:
		std::ostringstream message;	///< temp buffer
		
		static bool bAutoSpace;		///< add a space between messages? 
		
		ofLog(ofLog const&) {}        					// not defined, not copyable
		ofLog& operator=(ofLog& from) {return *this;}	// not defined, not assignable
		
		static string padding;						///< the padding between ostream calls
};

//--------------------------------------------------------------
// Log Aliases

/// Derived log class for easy verbose logging.
/// Example: `ofLogVerbose("Log message")`
class ofLogVerbose : public ofLog{
	public:
		ofLogVerbose(const string &module="");
		ofLogVerbose(const string & module, const string & message);
		ofLogVerbose(const string & module, const char* format, ...) OF_PRINTF_ATTR(3, 4);
};

/// Derived log class for easy notice logging.
/// Example: `ofLogNotice("Log message")`
class ofLogNotice : public ofLog{
	public:
		ofLogNotice(const string & module="");
		ofLogNotice(const string & module, const string & message);
		ofLogNotice(const string & module, const char* format, ...) OF_PRINTF_ATTR(3, 4);
};

/// Derived log class for easy warning logging.
/// Example: `ofLogWarning("Log message")`
class ofLogWarning : public ofLog{
	public:
		ofLogWarning(const string & module="");
		ofLogWarning(const string & module, const string & message);
		ofLogWarning(const string & module, const char* format, ...) OF_PRINTF_ATTR(3, 4);
};

/// Derived log class for easy error logging.
/// Example: `ofLogError("Log message")`
class ofLogError : public ofLog{
	public:
		ofLogError(const string & module="");
		ofLogError(const string & module, const string & message);
		ofLogError(const string & module, const char* format, ...) OF_PRINTF_ATTR(3, 4);
};

/// Derived log class for easy fatal error logging.
/// Example: `ofLogFatalError("Log message")`
class ofLogFatalError : public ofLog{
	public:
		ofLogFatalError(const string & module="");
		ofLogFatalError(const string & module, const string & message);
		ofLogFatalError(const string & module, const char* format, ...) OF_PRINTF_ATTR(3, 4);
};


/// \cond INTERNAL

//--------------------------------------------------------------
// Logger Channels

/// swappable logging destinations, derive your own for custom logging
///
class ofBaseLoggerChannel{
public:
	virtual ~ofBaseLoggerChannel(){};
	virtual void log(ofLogLevel level, const string & module, const string & message)=0;
	virtual void log(ofLogLevel level, const string & module, const char* format, ...)  OF_PRINTF_ATTR(4, 5) =0;
	virtual void log(ofLogLevel level, const string & module, const char* format, va_list args)=0;
};

class ofConsoleLoggerChannel: public ofBaseLoggerChannel{
public:
	virtual ~ofConsoleLoggerChannel(){};
	void log(ofLogLevel level, const string & module, const string & message);
	void log(ofLogLevel level, const string & module, const char* format, ...) OF_PRINTF_ATTR(4, 5);
	void log(ofLogLevel level, const string & module, const char* format, va_list args);
};

class ofFileLoggerChannel: public ofBaseLoggerChannel{
public:
	ofFileLoggerChannel();
	ofFileLoggerChannel(const string & path, bool append);
	virtual ~ofFileLoggerChannel();

	void setFile(const string & path,bool append=false);

	void log(ofLogLevel level, const string & module, const string & message);
	void log(ofLogLevel level, const string & module, const char* format, ...) OF_PRINTF_ATTR(4, 5);
	void log(ofLogLevel level, const string & module, const char* format, va_list args);

	void close();

private:
	ofFile file;
};


class ofThreadErrorLogger: public Poco::ErrorHandler{
public:
    virtual ~ofThreadErrorLogger(){}

    virtual void exception(const Poco::Exception& exc){
        ofLogFatalError("ofThreadErrorLogger::exception") << exc.displayText();
    }

    virtual void exception(const std::exception& exc){
        ofLogFatalError("ofThreadErrorLogger::exception") << exc.what();
    }

    virtual void exception(){
        ofLogFatalError("ofThreadErrorLogger::exception") << "Unknown exception.";
    }
};
/// \endcond
