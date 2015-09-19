#pragma once

#include "ofConstants.h"
#include "ofFileUtils.h"
#include "ofTypes.h"

/// \file
/// ofLog provides an interface for writing text output from your app.
/// It's basically a more useful version of `std::cout` or `printf` where
/// the output can be filtered and written to the console a file, or even a
/// custom logging module.
///
/// Sometimes you want to be able to see when something has happened inside
/// the code, but don't need to draw something visually. Oftentimes it's
/// more then enough to print out the state of a few variables when debugging.
/// Other times you need to know if a crash happened while your app was
/// running somewhere, so you log messages and variables to a file you can
/// read after the program crashes.
///
/// ### Log Levels
/// You can set the logging level so only messages above a certain level
/// are shown. This is useful if you want see lots of messages when debugging,
/// but then set a higher level so only warnings and errors appear for users.
///
/// See ofSetLogLevel(ofLogLevel level) for more details.
///
/// ### Usage
/// There are 2 ways you can use ofLog:
///
/// #### Functional: as a function taking a message
///
/// ~~~~{.cpp}
/// // Send a single string message, setting the log level.
/// ofLog(OF_LOG_NOTICE, "the number is " + ofToString(10));
/// 
/// // The legacy printf style.
/// ofLog(OF_LOG_NOTICE, "the number is %d", 10); 
/// ~~~~
///
/// #### Stream: as a stream using the << stream operator
///
/// ~~~~{.cpp}
/// // The stream style, setting the log level to OF_LOG_WARNING.
/// ofLog(OF_LOG_WARNING) << "the number is " << 10;
///
/// // This is the same as the last line, except it uses the default OF_LOG_NOTICE.
/// ofLog() << "the number is " << 10;
/// 
/// // There are also log level-specific stream objects, one for each level
/// // except OF_LOG_SILENT.
/// ofLogVerbose() << "A verbose message."
/// ofLogNotice() << "A regular notice message.";
/// ofLogWarning() << "Uh oh, a warning!";
/// ofLogError() << "Oh no, an error occurred!";
/// ofLogFatalError() << "Accckkk, a fatal error!!";
/// ~~~~
/// 
/// **Note**: The log level specific stream objects also take a string argument 
/// for the "module". A module is a string that is added to the beginning of 
/// the log line and can be used to separate logging messages by setting an 
/// independent log level for **that module only**. This module-specific log
/// level has no effect on other modules.
/// 
/// See ofSetLogLevel(string module, ofLogLevel level) for more details.
/// 
/// Example of logging to a specific module:
/// ~~~~{.cpp}
/// // log to a module called "Hello"
/// ofLogWarning("Hello") << "A warning message.";
/// ~~~~
///
/// **Warning**: It is important to understand that the log level specific
/// stream objects take the module name as an argument and the log messages via
/// the << operator. Putting your message as a string argument inside the
/// parentheses uses that message as a *module* and so nothing will be printed:
/// 
/// ~~~~{.cpp}
/// // This prints a warning message.
/// ofLogWarning() << "A warning message.";
///
/// // !!! This does not print a message because the string "a warning print"
/// // is the module argument !!!
/// ofLogWarning("A warning print");
/// 
/// // This prints a warning message to the "Hello" module.
/// ofLogWarning("Hello") << "A warning message.";
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
/// printf annotations for automatic format checking in GCC.
#ifdef __GNUC__
#define OF_PRINTF_ATTR(x, y) __attribute__ ((format (printf, x, y)))
#else
#define OF_PRINTF_ATTR(x, y)
#endif
/// \endcond


//--------------------------------------------------
/// \name Global logging level
/// \{

/// \brief The supported logging levels. Default is `OF_LOG_NOTICE`.
enum ofLogLevel{
	OF_LOG_VERBOSE,
	OF_LOG_NOTICE,
	OF_LOG_WARNING,
	OF_LOG_ERROR,
	OF_LOG_FATAL_ERROR,
	OF_LOG_SILENT	// OF_LOG_SILENT can be used to disable _all_ log messages.
					// All logging can be disabled by calling
					/// ofSetLogLevel(OF_LOG_SILENT).
};

/// \brief Sets the logging level to selectively show log messages.
///
/// This is useful if you want see lots of messages when debugging,
/// but then set a higher level so only warnings and errors appear for users.
///
/// ofLogLevel values in order from lowest to highest level are:
/// - `OF_LOG_VERBOSE` (lowest level)
/// - `OF_LOG_NOTICE`
/// - `OF_LOG_WARNING`
/// - `OF_LOG_ERROR`
/// - `OF_LOG_FATAL_ERROR`
/// - `OF_LOG_SILENT` (highest level)
///
/// Thus, setting a log level of `OF_LOG_ERROR`, means only logging messages
/// marked OF_LOG_ERROR and OF_LOG_FATAL_ERROR will be printed. Conversely,
/// setting OF_LOG_VERBOSE means all log level messages, including
/// OF_LOG_VERBOSE, will be printed.  Finally, setting a log level of
/// OF_LOG_SILENT will prevent any messages from being printed.
///
/// The default ofLogLevel is `OF_LOG_NOTICE`.
///
/// \param level the ofLogLevel (and below) you want to show
void ofSetLogLevel(ofLogLevel level);


/// \brief Set the logging level for a specific module.
///
/// When a module name is supplied to ofSetLogLevel, the provided ofLogLevel
/// is selectively applied only to ofLog messages marked with the specified
/// module.
///
/// This is particularly useful when the user desires to, for example, log at
/// an OF_LOG_VERBOSE level for one module and then log at OF_LOG_ERROR for
/// another module.
///
/// Example of logging to a specific module:
///
/// ~~~~{.cpp}
/// // Set the default log level for all logging.
/// ofSetLogLevel(OF_LOG_ERROR);
///
/// // Selectively enable verbose logging for the MyClass module.
/// ofSetLogLevel("MyClass", OF_LOG_VERBOSE);
///
/// // If we then log the following ...
///
/// // Log a vermose message to a module called "MyClass".
/// ofLogVerbose("MyClass") << "A verbose message from MyClass.";
///
/// // Log a verbose message to a module called "MyOtherClass".
/// ofLogVerbose("MyOtherClass") << "A verbose message from MyOtherClass.";
///
/// // In this case, we will see the verbose message from "MyClass", but not
/// // the message from "MyOtherClass".
/// ~~~~
void ofSetLogLevel(string module, ofLogLevel level);

/// \brief Get the currently set global logging level.
/// \returns The currently set global logging level.
ofLogLevel ofGetLogLevel();

/// \brief Get log level name as a string.
/// \param level The ofLogLevel you want as a string.
/// \param pad True if you want all log level names to be the same length.
/// \returns The log level name as a string.
string ofGetLogLevelName(ofLogLevel level, bool pad=false);

/// \}

//--------------------------------------------------
/// \name Global logger channel
/// \{

class ofBaseLoggerChannel;

/// \brief Set the logging to output to a file instead of the console.
/// \param path The path to the log file to use.
/// \param append True if you want to append to the existing file.
void ofLogToFile(const string & path, bool append=false);

/// \brief Set the logging to ouptut to the console.
/// 
/// This is the default state and can be called to reset console logging
/// after ofLogToFile or ofSetLoggerChannel has been called.
void ofLogToConsole();

/// \brief Set the logger to use a custom logger channel.
///
/// Custom logger channels must extend ofBaseLoggerChannel. Custom log channels
/// can be useful for combining logging methods, logging to a server, logging
/// to email or even Twitter.
///
/// \param loggerChannel A shared pointer to the logger channel.
void ofSetLoggerChannel(shared_ptr<ofBaseLoggerChannel> loggerChannel);

/// \}

/// \class ofLog
/// \brief A C++ stream-style logging interface.
///
/// ofLog accepts variables via the std::ostream operator << and builds a string
/// and logs it when the stream is finished (via the destructor). A newline is
/// printed automatically and all the stream controls (std::endl, std::flush,
/// std::hex, etc) work normally. The default log level is `OF_LOG_NOTICE`.
///
/// Basic usage:
///
/// ~~~~{.cpp}
///
/// ofLog() << "My integer is " << 100 << " and my float is " << 20.234f;
///
/// ~~~~
///
/// It also accepts the legacy ofLog interface:
/// ofLog(ofLogLevel level, string message):
///
/// ~~~~{.cpp}
///
/// ofLog(OF_LOG_ERROR, "Another string.");
///
/// ~~~~
///
/// \author Dan Wilcox <danomatika@gmail.com> danomatika.com

// Class idea from http://www.gamedev.net/community/forums/topic.asp?topic_id=525405&whichpage=1&#3406418
// How to catch std::endl (which is actually a func pointer) http://yvan.seth.id.au/Entries/Technology/Code/std__endl.html

class ofLog{
	public:
	
		/// \name Logging
		/// \{

		/// \brief Start logging on notice level.
		/// 
		/// ofLog provides a streaming log interface by accepting variables via
		/// the `std::ostream` operator `<<` similar to `std::cout` and
		/// `std::cerr`.
		/// 
		/// It builds a string and logs it when the stream is finished. A
		/// newline is printed automatically and all the stream controls
		/// (`std::endl`, `std::flush`, `std::hex`, etc)
		/// work normally.
		/// 
		/// ~~~~{.cpp}
		/// 
		/// // Converts primitive types (int, float, etc) to strings automatically.
		/// ofLog() << "a string " << 100 << 20.234f;
		/// 
		/// ~~~~
		/// 
		/// The log level is `OF_LOG_NOTICE` by default.
		ofLog();
		
		/// \brief Start logging on a specific ofLogLevel.
		/// 
		/// Example:
		/// ~~~~{.cpp}
		/// 
		/// // Set the log level.
		/// ofLog(OF_LOG_WARNING) << "a string " << 100 << 20.234f;
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
		/// // Set the log level.
		/// ofLog(OF_LOG_WARNING) << "a string " << 100 << 20.234f;
		/// 
		/// // This is the same as above.
		/// ofLogWarning() << "a string " << 100 << 20.234f;
		/// 
		/// ~~~~
		///
		/// \param level The ofLogLevel for this log message.
		ofLog(ofLogLevel level);
	
	
		/// \brief Log a string at a specific log level.
		/// 
		/// Supply the logging message as a parameter to the function
		/// instead of as a stream.
		/// 
		/// The string message can be concatenated using the
		/// ofToString(const T& value) conversion function:
		/// 
		/// ~~~~{.cpp}
		/// 
		/// // Build a single string message.
		/// ofLog(OF_LOG_NOTICE, "the number is " 
		/// + ofToString(10) + " and I have a float too " + ofToString(123.45f));
		/// 
		/// ~~~~
		///
		/// \param level The ofLogLevel for this log message.
		/// \param message The log message.
		ofLog(ofLogLevel level, const string & message);

		/// \brief Logs a message at a specific log level using the printf interface.
		///
		/// The message is built using the formatting from the C printf function
		/// and can be used as a direct replacement. Essentially, the second
		/// argument is a string with special formatting specifiers starting
		/// with '%' that specify where the following variables go in the
		/// message. You can have as many variables as you want following the
		/// logLevel and format string, but there must be a % specifier for each
		/// subsequent variable.
		/// 
		/// For quick reference, here are a few of the most useful formatting
		/// specifiers:
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
		/// The specifier should match the variable type as it is used to tell
		/// the function how to convert that primitive type (int, float,
		/// character, etc) into a string.
		/// 
		/// For instance, let's say we want to print two messages, a salutation
		/// and the value of an int, a float, and a string variable:
		/// 
		/// ~~~~{.cpp}
		/// 
		/// // Print a simple message with no variables.
		/// ofLog(OF_LOG_WARNING, "Welcome to the jungle.");
		/// 
		/// // Our variables.
		/// float fun = 11.11;
		/// int games = 100;
		/// string theNames = "Dan, Kyle, & Golan";
		/// 
		/// // Print a message with variables, sets the message format in the
		/// // format string.
		/// ofLog(OF_LOG_NOTICE, "we've got %d & %f, we got everything you want honey, we know %s", fun, games, theNames.c_str());
		/// 
		/// ~~~~
		/// 
		/// Note: `theNames.c_str()` returns a C string from theNames which is
		/// a C++ string object.
		/// 
		/// There are other formatting options such as setting the decimal
		/// precision of float objects and the forward padding of numbers
		/// (i.e. 0001 instead of 1). See the [Wikipedia printf format string
		/// article](http://en.wikipedia.org/wiki/Printf_format_string) for more
		/// detailed information.
		///
		/// \param level The ofLogLevel for this log message.
		/// \param format The printf-style format string.
		ofLog(ofLogLevel level, const char* format, ...) OF_PRINTF_ATTR(3, 4);
		
		/// \}
	
		//--------------------------------------------------
		/// \name Logging configuration
		/// \{

		/// \brief Let the logger automaticly add spaces between messages.
		///
		/// Default is `false`.
		///
		/// \param autoSpace Set to true to add spaces between messages
		static void setAutoSpace(bool autoSpace);
	
		/// \brief Set the logging channel destinations for messages.
		///
		/// This can be used to output to files instead of stdout.
		///
		/// \sa ofFileLoggerChannel ofConsoleLoggerChannel
		/// \param channel The channel to log to.
		static void setChannel(shared_ptr<ofBaseLoggerChannel> channel);
	
		/// \}

	
		/// \cond INTERNAL
	
		/// \brief Destroy the ofLog.
		///
		/// This destructor does the actual printing via std::ostream.
		virtual ~ofLog();
		
		/// \brief Define flexible stream operator.
		///
		/// This allows the class to use the << std::ostream to read data of
		/// almost any type.
		///
		/// \tparam T the data type to be streamed.
		/// \param value the data to be streamed.
		/// \returns A reference to itself.
		template <class T> 
			ofLog& operator<<(const T& value){
			message << value << padding;
			return *this;
		}
	
		/// \brief Define flexible stream operator.
		///
		/// This allows the class to use the << std::ostream to catch function
		/// pointers such as std::endl and std::hex.
		///
		/// \param func A function pointer that takes a std::ostream as an argument.
		/// \returns A reference to itself.
		ofLog& operator<<(std::ostream& (*func)(std::ostream&)){
			func(message);
			return *this;
		}
	
		/// \endcond
	
	

	
	protected:
		/// \cond INTERNAL

		ofLogLevel level; ///< Log level.
		bool bPrinted;	  ///< Has the message been printed in the constructor?
		string module;    ///< The destination module for this message.
		
		/// \brief Print a log line.
		/// \param level The log level.
		/// \param module The target module.
		/// \param message The log message.
		void _log(ofLogLevel level, const string & module, const string & message);
	
		/// \brief Determine if the given module is active at the given log level.
		/// \param level The log level.
		/// \param module The target module.
		/// \returns true if the given module is active at the given log level.
		bool checkLog(ofLogLevel level, const string & module);
	
		static shared_ptr<ofBaseLoggerChannel> channel;	///< The target channel.
	
		/// \endcond
	
	private:
		std::ostringstream message;	///< Temporary buffer.
		
		static bool bAutoSpace; ///< Should space be added between messages?
		
		ofLog(ofLog const&) {}        					// not defined, not copyable
		ofLog& operator=(ofLog& from) {return *this;}	// not defined, not assignable
		
		static string padding; ///< The padding between std::ostream calls.
};


/// \brief Derived log class for easy verbose logging.
///
/// Example: `ofLogVerbose("Log message")`.
class ofLogVerbose : public ofLog{
	public:
		/// \brief Create a verbose log message.
		/// \param module The target module.
		ofLogVerbose(const string &module="");

		/// \brief Create a verbose log message.
		/// \param module The target module.
		/// \param message The log message.
		ofLogVerbose(const string & module, const string & message);

		/// \brief Create a verbose log message.
		/// \param module The target module.
		/// \param format The printf-style format string.
		ofLogVerbose(const string & module, const char* format, ...) OF_PRINTF_ATTR(3, 4);
};

/// \brief Derived log class for easy notice logging.
///
/// Example: `ofLogNotice("Log message")`.
class ofLogNotice : public ofLog{
	public:
		/// \brief Create a notice log message.
		/// \param module The target module.
		ofLogNotice(const string & module="");

		/// \brief Create a notice log message.
		/// \param module The target module.
		/// \param message The log message.
		ofLogNotice(const string & module, const string & message);

		/// \brief Create a notice log message.
		/// \param module The target module.
		/// \param format The printf-style format string.
		ofLogNotice(const string & module, const char* format, ...) OF_PRINTF_ATTR(3, 4);
};

/// \brief Derived log class for easy warning logging.
///
/// Example: `ofLogWarning("Log message")`.
class ofLogWarning : public ofLog{
	public:
	/// \brief Create a verbose log message.
	/// \param module The target module.
		ofLogWarning(const string & module="");
	/// \brief Create a verbose log message.
	/// \param module The target module.
	/// \param message The log message.
		ofLogWarning(const string & module, const string & message);
	
	/// \brief Create a verbose log message.
	/// \param module The target module.
	/// \param format The printf-style format string.
		ofLogWarning(const string & module, const char* format, ...) OF_PRINTF_ATTR(3, 4);
};

/// \brief Derived log class for easy error logging.
///
/// Example: `ofLogError("Log message")`.
class ofLogError : public ofLog{
	public:
		/// \brief Create a error log message.
		/// \param module The target module.
		ofLogError(const string & module="");
	
		/// \brief Create a error log message.
		/// \param module The target module.
		/// \param message The log message.
		ofLogError(const string & module, const string & message);
	
		/// \brief Create a error log message.
		/// \param module The target module.
		/// \param format The printf-style format string.
		ofLogError(const string & module, const char* format, ...) OF_PRINTF_ATTR(3, 4);
};

/// \brief Derived log class for easy fatal error logging.
///
/// Example: `ofLogFatalError("Log message")`.
class ofLogFatalError : public ofLog{
	public:
		/// \brief Create a fatal error log message.
		/// \param module The target module.
		ofLogFatalError(const string & module="");

		/// \brief Create a fatal error log message.
		/// \param module The target module.
		/// \param message The log message.
		ofLogFatalError(const string & module, const string & message);
	
		/// \brief Create a fatal error log message.
		/// \param module The target module.
		/// \param format The printf-style format string.
		ofLogFatalError(const string & module, const char* format, ...) OF_PRINTF_ATTR(3, 4);
};


/// \cond INTERNAL

//--------------------------------------------------------------
// Logger Channels

/// \brief The base class representing a logger channel.
///
/// Users can derive their own logging channels from ofBaseLoggerChannel or use
/// default channels.
class ofBaseLoggerChannel{
public:
	/// \brief Destroy the channel.
	virtual ~ofBaseLoggerChannel(){};
	
	/// \brief Log a message.
	/// \param level The log level.
	/// \param module The target module.
	/// \param message The log message.
	virtual void log(ofLogLevel level, const string & module, const string & message)=0;

	/// \brief Log a message.
	/// \param level The log level.
	/// \param module The target module.
	/// \param format The printf-style format string.
	virtual void log(ofLogLevel level, const string & module, const char* format, ...)  OF_PRINTF_ATTR(4, 5) =0;

	/// \brief Log a message.
	/// \param level The log level.
	/// \param module The target module.
	/// \param format The printf-style format string.
	/// \param args the list of printf-style arguments.
	virtual void log(ofLogLevel level, const string & module, const char* format, va_list args)=0;
};

/// \brief A logger channel that logs its messages to the console.
class ofConsoleLoggerChannel: public ofBaseLoggerChannel{
public:
	/// \brief Destroy the console logger channel.
	virtual ~ofConsoleLoggerChannel(){};
	void log(ofLogLevel level, const string & module, const string & message);
	void log(ofLogLevel level, const string & module, const char* format, ...) OF_PRINTF_ATTR(4, 5);
	void log(ofLogLevel level, const string & module, const char* format, va_list args);
};

/// \brief A logger channel that logs its messages to a log file.
class ofFileLoggerChannel: public ofBaseLoggerChannel{
public:
	/// \brief Create an ofFileLoggerChannel.
	ofFileLoggerChannel();
	
	/// \brief Create an ofFileLoggerChannel with parameters.
	/// \param path The file path for the log file.
	/// \param append True if the log data should be added to an existing file.
	ofFileLoggerChannel(const string & path, bool append);

	/// \brief Destroy the file logger channel.
	virtual ~ofFileLoggerChannel();

	/// \brief Set the log file.
	/// \param path The file path for the log file.
	/// \param append True if the log data should be added to an existing file.
	void setFile(const string & path,bool append=false);

	void log(ofLogLevel level, const string & module, const string & message);
	void log(ofLogLevel level, const string & module, const char* format, ...) OF_PRINTF_ATTR(4, 5);
	void log(ofLogLevel level, const string & module, const char* format, va_list args);

	/// \brief CLose the log file.
	void close();

private:
	ofFile file; ///< The location of the log file.
	
};

/// \endcond
