#pragma once

#include "ofConstants.h"
#include <bitset> // for ofToBinary

#include "ofLog.h"

#ifdef TARGET_WIN32	 // for ofLaunchBrowser
	#include <shellapi.h>
#endif

#include "Poco/Path.h"

/// \name Elapsed time
/// \{

/// \brief Reset the elapsed time counter.
///
/// This method resets the times returned by ofGetElapsedTimef(),
/// ofGetElapsedTimeMillis() and ofGetElapsedTimeMicros() to zero.
///
/// \note This is called on the first frame during app startup.
void ofResetElapsedTimeCounter();

/// \brief Return the elapsed time in seconds.
///
/// This returns the elapsed time since ofResetElapsedTimeCounter() was called.
/// Usually ofResetElapsedTimeCounter() is called automatically once during
/// program startup.
///
/// \returns the floating point elapsed time in seconds.
float ofGetElapsedTimef();


/// \brief Return the elapsed time in milliseconds.
///
/// This returns the elapsed time since ofResetElapsedTimeCounter() was called.
/// Usually ofResetElapsedTimeCounter() is called automatically once during
/// program startup.
///
/// \returns the elapsed time in milliseconds (1000 milliseconds = 1 second).
unsigned long long ofGetElapsedTimeMillis();

/// \brief Return the elapsed time in microseconds.
///
/// This returns the elapsed time since ofResetElapsedTimeCounter() was called.
/// Usually ofResetElapsedTimeCounter() is called automatically upon program
/// startup.
///
/// \returns the elapsed time in microseconds (1000000 microseconds = 1 second).
unsigned long long ofGetElapsedTimeMicros();

/// \brief Get the number of frames rendered since the program started.
/// \returns the number of frames rendered since the program started.
int ofGetFrameNum();

/// \}

// --------------------------------------------
/// \name System time
/// \{

/// \brief Get the seconds after the minute.
/// \returns the seconds after the minute [0-59].
int ofGetSeconds();

/// \brief Get minutes after the hour.
/// \returns the minutes after the hour [0-59].
int ofGetMinutes();

/// \brief Get the hour of the day.
/// \returns the hour of the day [0-23].
int ofGetHours();

/// \brief Get the number of seconds since 1970.
/// \returns the number of seconds since 1970 (aka epoch time).
unsigned int ofGetUnixTime();

/// \brief Get the system time in milliseconds.
/// \returns the system time in milliseconds.
unsigned long long ofGetSystemTime();

/// \brief Get the system time in microseconds.
/// \returns the system time in microseconds.
unsigned long long ofGetSystemTimeMicros();

/// \brief Sleeps the current thread for the specified amount of ms
void ofSleepMillis(int millis);

/// \brief Formats the current system time according to the given format.
///
/// The default timestamp format is "%Y-%m-%d-%H-%M-%S-%i" (e.g.
/// 2011-01-15-18-29-35-299).
///
/// \returns the current time with the given format.
string ofGetTimestampString();

/// \brief Formats the current system time according to the given format.
///
/// The format string is used as a template to format the date and
/// is copied character by character except for the following special
/// characters, which are replaced by the corresponding value.
///
///   * %w - abbreviated weekday (Mon, Tue, ...)
///   * %W - full weekday (Monday, Tuesday, ...)
///   * %b - abbreviated month (Jan, Feb, ...)
///   * %B - full month (January, February, ...)
///   * %d - zero-padded day of month (01 .. 31)
///   * %e - day of month (1 .. 31)
///   * %f - space-padded day of month ( 1 .. 31)
///   * %m - zero-padded month (01 .. 12)
///   * %n - month (1 .. 12)
///   * %o - space-padded month ( 1 .. 12)
///   * %y - year without century (70)
///   * %Y - year with century (1970)
///   * %H - hour (00 .. 23)
///   * %h - hour (00 .. 12)
///   * %a - am/pm
///   * %A - AM/PM
///   * %M - minute (00 .. 59)
///   * %S - second (00 .. 59)
///   * %s - seconds and microseconds (equivalent to %S.%F)
///   * %i - millisecond (000 .. 999)
///   * %c - centisecond (0 .. 9)
///   * %F - fractional seconds/microseconds (000000 - 999999)
///   * %z - time zone differential in ISO 8601 format (Z or +NN.NN)
///   * %Z - time zone differential in RFC format (GMT or +NNNN)
///   * %% - percent sign
///
/// \param timestampFormat The formatting pattern.
/// \returns the formatted timestamp as a std::string.
string ofGetTimestampString(const string& timestampFormat);

/// \brief Get the current year.
/// \returns the current year.
int ofGetYear();

/// \brief Get the current month.
/// \returns the current month [1-12].
int ofGetMonth();

/// \brief Get the current day within the month.
/// \returns the day witin the month [1-31].
int ofGetDay();

/// \brief Get the current weekday.
///
/// Values 0 = Sunday, 1 = Monday, ..., 6 = Saturday.
///
/// \returns the current weekday [0-6].
int ofGetWeekday();

/// \}

// --------------------------------------------
/// \name Data path
/// \{

/// \brief Enable the use of the data path.
///
/// This function causes ofToDataPath() to respect the relative path set
/// with ofSetDataPathRoot().  This is enabled by default.
void ofEnableDataPath();

/// \brief Disable the use of the data path.
///
/// This function causes ofToDataPath() to ignore the relative path set
/// with ofSetDataPathRoot().
void ofDisableDataPath();

/// \brief Convert a given path to a path relative to the location of the data/ folder.
///
/// This funtion returns path unchanged if ofDisableDataPath() was called first.
/// Users might want absolute paths for non-openFrameworks functions that
/// are unaware of the data/ path's location.
///
/// \param path The path to prepend.
/// \param absolute Set to true to return an absolute path.
/// \returns the converted path.
string ofToDataPath(const string& path, bool absolute=false);

/// \brief Reset the working directory to the platform default.
void ofSetWorkingDirectoryToDefault();

/// \brief Set the relative path to the data/ folder from the executable.
/// \warning The provided path must have a trailing slash (/).
/// \param root The path to the data/ folder relative to the app executable.
void ofSetDataPathRoot(const string& root);



/// \}

// --------------------------------------------
/// \name Vectors
/// \{

/// \brief Randomly reorder the values in a vector.
/// \param values The vector of values to modify.
template<class T>
void ofRandomize(vector<T>& values) {
	random_shuffle(values.begin(), values.end());
}

/// \brief Conditionally remove values from a vector.
///
/// Values are removed if, when passed to the BoolFunction, the BoolFunction
/// function returns true.  The give BoolFunction can be a custom function
/// or a built-in function.
///
/// Example of a custom function to remove odd numbers from a vector<int>
/// of integers:
///
/// ~~~~{.cpp}
/// bool IsOdd(int i) {
///     return ((i % 2) == 1);
/// }
/// ~~~~
///
/// To call the function, one might use:
///
/// ~~~~{.cpp}
/// vector<int> myInts;
///
/// // Fill the vector with integers.
/// for (int i = 0; i < 10; ++i)
/// {
///     myInts.push_back(i);
/// }
///
/// ofRemove(myInts, IsOdd);
/// ~~~~
///
/// The resulting vector will contain the following values:
///
///    0, 2, 4, 6, 8
///
/// \tparam T the type contained by the vector.
/// \tparam BoolFunction the boolean function used to erase.
/// \param values The vector of values to modify.
/// \param shouldErase a boolean function as described above.
/// \sa http://www.cplusplus.com/reference/algorithm/remove_if/
template<class T, class BoolFunction>
void ofRemove(vector<T>& values, BoolFunction shouldErase) {
	values.erase(remove_if(values.begin(), values.end(), shouldErase), values.end());
}

/// \brief Sort a vector of values into ascending order.
///
/// The elements are compared using operator< for the first version.
/// Equivalent elements are not guaranteed to keep their original relative
/// order.
///
/// \tparam T the type contained by the vector.
/// \param values The vector of values to be sorted.
/// \sa http://www.cplusplus.com/reference/algorithm/sort/
template<class T>
void ofSort(vector<T>& values) {
	sort(values.begin(), values.end());
}

/// \brief Sort a vector of values into an order defined by a comparator.
///
/// Example of a custom function to sort descending of integers:
///
/// ~~~~{.cpp}
/// bool sortDescending(int i,int j)
/// {
///     return (j < i);
/// }
/// ~~~~
///
/// This binary function must accept two
/// elements in the range as arguments and return a value convertible to bool.
/// The value returned indicates whether the element passed as first argument is
/// considered to go before the second in the specific strict weak ordering it
/// defines.  The function shall not modify any of its arguments.  This can
/// either be a function pointer or a function object.
///
/// To call the function, one might use:
///
/// ~~~~{.cpp}
/// std::vector<int> myInts;
///
/// // Fill the vector with integers.
/// for (int i = 0; i < 10; ++i)
/// {
///     myInts.push_back(i);
/// }
///
/// ofSort(myInts, sortDescending);
/// ~~~~
///
/// The resulting vector will contain the following values:
///
///    9, 8, 7, 6, 5, 4, 3, 2, 1, 0.
///
/// \tparam T the type contained by the vector.
/// \param The vector of values to be sorted.
/// \param The comparison function.
/// \sa http://www.cplusplus.com/reference/algorithm/sort/
template<class T, class BoolFunction>
void ofSort(vector<T>& values, BoolFunction compare) {
	sort(values.begin(), values.end(), compare);
}

/// \brief Search for a target value in a vector of values.
/// \tparam T the type contained by the vector.
/// \param values The vector of values to be searched.
/// \param target The target value to be found.
/// \returns true the index of the first target value found.
template <class T>
unsigned int ofFind(const vector<T>& values, const T& target) {
	return distance(values.begin(), find(values.begin(), values.end(), target));
}

/// \brief Search for a target value in a vector of values.
/// \tparam T the type contained by the vector.
/// \param values The vector of values to be searched.
/// \param target The target value to be found.
/// \returns true iff at least one value equal to the target value is found.
template <class T>
bool ofContains(const vector<T>& values, const T& target) {
	return ofFind(values, target) != values.size();
}



/// \}

// --------------------------------------------
/// \name String manipulation
/// \{

/// \brief Splits a given string using delimiter and returns 
/// the resulting parts of the string as vector.
///
/// A useful application is to use delimiter `" "`:
/// ~~~~{.cpp}
/// result = ofSplitString("of rocks", " ");
/// // result[0] = "of"
/// // result[1] = "rocks"
/// ~~~~
/// Like this a big string can be "cut" and turned it into 
/// a vector of words.
///
/// \param source The string to split
/// \param delimiter The string to split on
/// \param ignoreEmpty Set to true to remove empty results
/// \param trim Set to true to trim the resulting strings
/// \returns A vector of string split on the delimiter
vector<string> ofSplitString(const string& source, const string& delimiter, bool ignoreEmpty = false, bool trim = false);

/// \brief Join a vector of string together to one string
/// 
/// \param stringElements The vector of strings to join
/// \param delimiter The delimiter to put betweeen each string
string ofJoinString(const vector<string>& stringElements, const string& delimiter);

/// \brief Replace occurrences of a string with another string
/// \param input The string to run the replacement on
/// \param searchStr The string to be replaced
/// \param replaceStr The string to put in place 
void ofStringReplace(string& input, const string& searchStr, const string& replaceStr);

/// \brief Check if string contains another string
/// 
/// Example:
/// ~~~{.cpp}
///     string haystack = "foobar";
///     ofLog() << ofIsStringInString(haystack, "foo"); // outputs 1
///     ofLog() << ofIsStringInString(haystack,"bar"); // outputs 1
///     ofLog() << ofIsStringInString(haystack, "something else"); // outputs 0
/// ~~~
/// \param haystack The string to check for occurrence in 
/// \param needle The string to check for
bool ofIsStringInString(const string& haystack, const string& needle);

/// \brief Check how many times a string contains another string
/// \param haystack The string to check for occurrence in 
/// \param needle The string to check for
int ofStringTimesInString(const string& haystack, const string& needle);

/// \brief Converts all characters in the string to lowercase.
string ofToLower(const string& src);

/// \brief Converts all characters in the string to uppercase.
///
/// ~~~~{.cpp}
///     string politeGreeting = "Hello, World!";
///     string enthusiasticGreeting = ofToUpper(politeGreeting); 
///     ofLog() << enthusiasticGreeting; // returns "HELLO, WORLD!"
/// ~~~~
string ofToUpper(const string& src);

string ofVAArgsToString(const char * format, ...);
string ofVAArgsToString(const char * format, va_list args);

/// \}

// --------------------------------------------
/// \name String conversion
/// \{


/// \brief Convert a value to a string
/// 
/// Example:
/// ~~~~~{.cpp}
/// string str = "framerate is "; 						
/// str += ofToString(ofGetFrameRate())+"fps"; 
/// // returns something like "framerate is 60 fps"
/// ~~~~~
template <class T>
string ofToString(const T& value){
	ostringstream out;
	out << value;
	return out.str();
}

/// \brief Convert a value to a string with a specific precision
///
/// Like sprintf "%4f" format, in this example precision=4
template <class T>
string ofToString(const T& value, int precision){
	ostringstream out;
	out << fixed << setprecision(precision) << value;
	return out.str();
}

/// \brief Convert a value to a string with a specific width and fill
///
/// Like sprintf "% 4d" or "% 4f" format, in this example width=4, fill=' '
template <class T>
string ofToString(const T& value, int width, char fill ){
	ostringstream out;
	out << fixed << setfill(fill) << setw(width) << value;
	return out.str();
}

/// \brief Convert a value to a string with a specific precision, width and filll
///
/// Like sprintf "%04.2d" or "%04.2f" format, in this example precision=2, width=4, fill='0'
template <class T>
string ofToString(const T& value, int precision, int width, char fill ){
	ostringstream out;
	out << fixed << setfill(fill) << setw(width) << setprecision(precision) << value;
	return out.str();
}

/// \brief Convert a vector of values to a string
template<class T>
string ofToString(const vector<T>& values) {
	stringstream out;
	int n = values.size();
	out << "{";
	if(n > 0) {
		for(int i = 0; i < n - 1; i++) {
			out << values[i] << ", ";
		}
		out << values[n - 1];
	}
	out << "}";
	return out.str();
}

template<class T>
T ofFromString(const string & value){
	T data;
    stringstream ss;
    ss << value;
    ss >> data;
    return data;
}

template<>
string ofFromString(const string & value);

template<>
const char * ofFromString(const string & value);

/// \}

// --------------------------------------------
/// \name Number conversion
/// \{

/// \brief Converts a string representation of an int (e.g., `"3"`) to an actual int
int ofToInt(const string& intString);

/// \brief Converts a string representation of a float (e.g., `"3.14"`) to an actual float
float ofToFloat(const string& floatString);

/// \brief Converts a string representation of a double (e.g., `"3.14"`) to an actual double
double ofToDouble(const string& doubleString);

/// \brief Converts a string representation of a boolean (e.g., `"TRUE"`) 
/// to an actual bool using a case-insensitive comparison against the words `"true"` and `"false"`.
bool ofToBool(const string& boolString);

/// \brief Converts any value to its equivalent hexadecimal 
/// representation corresponding to the way it is stored in memory.
template <class T>
string ofToHex(const T& value) {
	ostringstream out;
	// pretend that the value is a bunch of bytes
	unsigned char* valuePtr = (unsigned char*) &value;
	// the number of bytes is determined by the datatype
	int numBytes = sizeof(T);
	// the bytes are stored backwards (least significant first)
	for(int i = numBytes - 1; i >= 0; i--) {
		// print each byte out as a 2-character wide hex value
		out << setfill('0') << setw(2) << hex << (int) valuePtr[i];
	}
	return out.str();
}

/// \brief Converts a string (e.g., `"abc"`) to its equivalent 
/// hexadecimal representation (e.g., `"616263"`).
template <>
string ofToHex(const string& value);

/// \brief Converts a c-style string (e.g., `"abc"`) to its equivalent hexadecimal 
/// representation (e.g., `"616263"`).
string ofToHex(const char* value);

/// \brief Converts a hexadecimal representation of an int 
/// (little-endian, 32-bit, e.g., `"0xbadf00d"` or `"badf00d"`) to an actual int.
int ofHexToInt(const string& intHexString);

/// \brief Converts a hexadecimal representation of an char 
/// (e.g., `"61"`) to an actual char (e.g., `a`).
char ofHexToChar(const string& charHexString);

/// \brief Converts a hexadecimal representation of an float 
/// (little-endian, 32-bit IEEE 754, e.g., `"4060000000000000"`) 
/// to an actual float (e.g., `128.f`).
float ofHexToFloat(const string& floatHexString);

/// \brief Converts a hexadecimal representation of an string 
/// (e.g., `"61626364656667"`) to an actual string (`"abcdefg"`)
string ofHexToString(const string& stringHexString);


/// \brief Converts a string representation of a single char (e.g., `"c"`) to an actual char.
char ofToChar(const string& charString);


/// \brief Converts any datatype value to a string of only 1s and 0s 
/// corresponding to the way value is stored in memory.
template <class T>
string ofToBinary(const T& value) {
	ostringstream out;
	const char* data = (const char*) &value;
	// the number of bytes is determined by the datatype
	int numBytes = sizeof(T);
	// the bytes are stored backwards (least significant first)
	for(int i = numBytes - 1; i >= 0; i--) {
		bitset<8> cur(data[i]);
		out << cur;
	}
	return out.str();
}

/// \brief Converts a string value to a string of only 1s and 0s 
/// corresponding to the way value is stored in memory.
template <>
string ofToBinary(const string& value);

/// \brief Converts any C-style string value to a string of only 1s and 0s 
/// corresponding to the way value is stored in memory.
string ofToBinary(const char* value);

/// \brief Interprets a string consisting only of 1s and 0s as an int 
/// (little-endian, 32-bit), and returns the corresponding int value.
int ofBinaryToInt(const string& value);

/// \brief Interprets a string consisting only of 1s and 0s as a char, 
/// and returns the corresponding char value.
char ofBinaryToChar(const string& value);

/// \brief Interprets a string consisting only of 1s and 0s as a float 
/// (little-endian, 32-bit IEEE 754), and returns the corresponding float value.
float ofBinaryToFloat(const string& value);

/// \brief Interprets a string consisting only of 1s and 0s as 8-bit 
/// ASCII characters, and returns the corresponding string.
string ofBinaryToString(const string& value);

/// \}

// --------------------------------------------
/// \name openFrameworks version
/// \{

/// \brief Get the current version of openFrameworks with dot delimeter.
/// For example `0.9.0`.
string 	ofGetVersionInfo();

/// \brief Get the major of the current version number of openFrameworks (the first digit)
unsigned int ofGetVersionMajor();

/// \brief Get the minor of the current version number of openFrameworks (the middle digit)
unsigned int ofGetVersionMinor();

/// \brief Get the patch number of the current version number of openFrameworks (the last digit)
unsigned int ofGetVersionPatch();

/// \}

// --------------------------------------------
/// \name Frame saving
/// \{


/// \brief Saves the current screen image into a given file
///
/// Example:
/// ~~~~{.cpp}
/// ofSaveScreen("screen1.png"); // Will save screen1.png in the /data folder
/// ~~~~
/// 
/// \param filename The filename to save the image in
void	ofSaveScreen(const string& filename);

/// \brief Saves the current frame as a PNG image.
void	ofSaveFrame(bool bUseViewport = false);
void	ofSaveViewport(const string& filename);


/// \}


// --------------------------------------------
/// \name System
/// \{


/// \brief Launch the given URL in the default browser.
/// \param url the URL to open.
/// \param uriEncodeQuery true if the query parameters in the given URL have
/// already been URL encoded.
void ofLaunchBrowser(const string& url, bool uriEncodeQuery=false);

/// \brief Executes a system command. Similar to run a command in terminal.
/// \note Will block until the executed program/command has finished.
/// \returns the system command output as string. 
string ofSystem(const string& command);

/// \brief Get the target platform of the current system.
ofTargetPlatform ofGetTargetPlatform();

/// \}


