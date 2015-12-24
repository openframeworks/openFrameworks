#pragma once

#include "ofConstants.h"
#include "utf8.h"
#include <bitset> // For ofToBinary.

#include "ofLog.h"

#ifdef TARGET_WIN32	 // For ofLaunchBrowser.
	#include <shellapi.h>
#endif

/// \name Elapsed Time
/// \{

/// \brief Reset the elapsed time counter.
///
/// This method resets the times returned by ofGetElapsedTimef(),
/// ofGetElapsedTimeMillis() and ofGetElapsedTimeMicros() to zero.
///
/// \note This is called on the first frame during app startup.
void ofResetElapsedTimeCounter();

/// \brief Get the elapsed time in seconds.
///
/// This returns the elapsed time since ofResetElapsedTimeCounter() was called.
/// Usually ofResetElapsedTimeCounter() is called automatically once during
/// program startup.
///
/// \returns the floating point elapsed time in seconds.
float ofGetElapsedTimef();


/// \brief Get the elapsed time in milliseconds.
///
/// This returns the elapsed time since ofResetElapsedTimeCounter() was called.
/// Usually ofResetElapsedTimeCounter() is called automatically once during
/// program startup.
///
/// \returns the elapsed time in milliseconds (1000 milliseconds = 1 second).
uint64_t ofGetElapsedTimeMillis();

/// \brief Get the elapsed time in microseconds.
///
/// This returns the elapsed time since ofResetElapsedTimeCounter() was called.
/// Usually ofResetElapsedTimeCounter() is called automatically upon program
/// startup.
///
/// \returns the elapsed time in microseconds (1000000 microseconds = 1 second).
uint64_t ofGetElapsedTimeMicros();

/// \brief Get the number of frames rendered since the program started.
/// \returns the number of frames rendered since the program started.
uint64_t ofGetFrameNum();

/// \}

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

/// \brief Get the number of seconds since Midnight, January 1, 1970.
///
/// Resolution is in seconds.
///
/// \returns the number of seconds since Midnight, January 1, 1970 (epoch time).
unsigned int ofGetUnixTime();

/// \brief Get the system time in milliseconds.
/// \returns the system time in milliseconds.
uint64_t ofGetSystemTime();

/// \brief Get the system time in microseconds.
/// \returns the system time in microseconds.
uint64_t ofGetSystemTimeMicros();

/// \brief Sleeps the current thread for the specified amount of milliseconds.
/// \param millis The number of millseconds to sleep.
void ofSleepMillis(int millis);

/// \brief Formats the current system time according to the given format.
///
/// The default timestamp format is "%Y-%m-%d-%H-%M-%S-%i" (e.g.
/// 2011-01-15-18-29-35-299).
///
/// \returns the current time as a string with the default format.
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
/// \returns the formatted timestamp as a string.
/// \warning an invalid timestampFormat may crash windows apps.
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

/// \name Data Path
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

/// \brief Make a path relative to the location of the data/ folder.
///
/// This funtion returns path unchanged if ofDisableDataPath() was called first.
///
/// By default, a relative path is returned. Users requiring absolute paths for
/// (e.g. for non-openFrameworks functions), can specify that an absolute path
/// be returned.
///
/// \param path The path to make relative to the data/ folder.
/// \param absolute Set to true to return an absolute path.
/// \returns the new path, unless paths were disabled with ofDisableDataPath().
string ofToDataPath(const string& path, bool absolute=false);

/// \brief Reset the working directory to the platform default.
///
/// The default working directory is where the application was started from
/// or the exe directory in case of osx bundles. GLUT might change the default
/// working directory to the resources directory in the bundle in osx. This
/// will restore it to the exe dir or whatever was the current dir when the
/// application was started
bool ofRestoreWorkingDirectoryToDefault();

/// \brief Set the relative path to the data/ folder from the executable.
///
/// This method can be useful when users want to embed the data as a resource
/// folder within an *.app bundle on OSX or perhaps work from a shared data
/// folder in the user's Documents directory.
///
/// \warning The provided path must have a trailing slash (/).
/// \param root The path to the data/ folder relative to the app executable.
void ofSetDataPathRoot(const string& root);


/// \}

/// \name Vectors
/// \{

/// \brief Randomly reorder the values in a vector.
/// \tparam T the type contained by the vector.
/// \param values The vector of values to modify.
/// \sa http://www.cplusplus.com/reference/algorithm/random_shuffle/
template<class T>
void ofRandomize(vector<T>& values) {
	random_shuffle(values.begin(), values.end());
}

/// \brief Conditionally remove values from a vector.
///
/// Values are removed if, when passed to the BoolFunction, the BoolFunction
/// function returns true.  The given BoolFunction can be a custom function
/// or a built-in function.
///
/// Example of a custom function to remove odd numbers from a std::vector<int>
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
/// std::vector<int> myInts;
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
/// \param shouldErase A boolean function as described above.
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
/// bool sortDescending(int i, int j)
/// {
///     return (j < i);
/// }
/// ~~~~
///
/// This binary function must accept two elements in the range as arguments and
/// return a value convertible to bool. The value returned indicates whether the
/// element passed as first argument is considered to go before the second in
/// the specific strict weak ordering it defines.  The function shall not modify
/// any of its arguments.  This can either be a function pointer or a function
/// object.
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
/// \param values The vector of values to be sorted.
/// \param compare The comparison function.
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
/// \sa http://www.cplusplus.com/reference/iterator/distance/
template <class T>
std::size_t ofFind(const vector<T>& values, const T& target) {
	return distance(values.begin(), find(values.begin(), values.end(), target));
}

/// \brief Search for a target value in a vector of values.
/// \tparam T the type contained by the vector.
/// \param values The vector of values to be searched.
/// \param target The target value to be found.
/// \returns true if at least one value equal to the target value is found.
template <class T>
bool ofContains(const vector<T>& values, const T& target) {
	return ofFind(values, target) != values.size();
}



/// \}


/// \name String Manipulation
/// \{

/// \brief Splits a string using a delimiter.
///
/// ofSplitString splits a string and returns the collection of string
/// tokens inside of a std::vector<std::string>.
///
/// A useful application is to use delimiter `" "`:
///
/// ~~~~{.cpp}
/// result = ofSplitString("of rocks", " ");
/// // result[0] = "of"
/// // result[1] = "rocks"
/// ~~~~
///
/// Like this a big string can be "cut" and turned it into a collection of
/// words. The user can also ignore empty tokens.  Empty tokens occur when a
/// there are multiple delimiters in a row (e.g. "of,rocks,,!").  The user can
/// also "trim" the tokens. This will remove whitespace from before and after
/// each token.  This might be useful when a delimted string is padded with
/// with spaces (e.g. "of    ,   rocks   ,!").
///
/// \param source The string to split.
/// \param delimiter The delimiter string.
/// \param ignoreEmpty Set to true to remove empty tokens.
/// \param trim Set to true to trim the resulting tokens.
/// \returns A vector of strings split with the delimiter.
vector<string> ofSplitString(const string& source, const string& delimiter, bool ignoreEmpty = false, bool trim = false);

/// \brief Join a vector of strings together into one string.
/// \param stringElements The vector of strings to join.
/// \param delimiter The delimiter to put betweeen each string.
string ofJoinString(const vector<string>& stringElements, const string& delimiter);

/// \brief Replace all occurrences of a string with another string.
/// \note The input string is passed by reference, so it will be modified.
/// \param input The string to run the replacement on.
/// \param searchStr The string to be replaced.
/// \param replaceStr The string to put in place.
void ofStringReplace(string& input, const string& searchStr, const string& replaceStr);

/// \brief Check if string contains another string.
/// 
/// Example:
/// ~~~{.cpp}
///     std::string haystack = "foobar";
///     ofLog() << ofIsStringInString(haystack, "foo"); // Outputs 1.
///     ofLog() << ofIsStringInString(haystack,"bar"); // Outputs 1.
///     ofLog() << ofIsStringInString(haystack, "something else"); // Outputs 0.
/// ~~~
/// \param haystack The string to check for occurrence in.
/// \param needle The string to check for.
bool ofIsStringInString(const string& haystack, const string& needle);

/// \brief Check how many times a string contains another string.
/// \param haystack The string to check for occurrence in .
/// \param needle The string to check for.
std::size_t ofStringTimesInString(const string& haystack, const string& needle);

/// \brief Converts all characters in a string to lowercase.
///
/// This function expects a UTF-8 encoded multi-byte string to be passed.
/// Resulting strings will be returned as UTF-8 encoded multi-byte strings.
///
/// ~~~~{.cpp}
///     std::string enthusiasticGreeting = "HELLO, WORLD!";
///     std::string politeGreeting = ofToLower(enthusiasticGreeting);
///     ofLog() << politeGreeting; // Returns "hello, world!"
/// ~~~~
///
/// \param src The UTF-8 encoded string to convert to lowercase.
/// \returns the UTF-8 encoded string as all lowercase characters.
string ofToLower(const string& src, const string & locale="");

/// \brief Converts all characters in the string to uppercase.
///
/// This function expects a UTF-8 encoded multi-byte string to be passed.
/// Resulting strings will be returned as UTF-8 encoded multi-byte strings.
///
/// ~~~~{.cpp}
///     std::string politeGreeting = "Hello, World!";
///     std::string enthusiasticGreeting = ofToUpper(politeGreeting);
///     ofLog() << enthusiasticGreeting; // Returns "HELLO, WORLD!"
/// ~~~~
///
/// \param src The UTF-8 encoded string to convert to uppercase.
/// \returns the UTF-8 encoded string as all uppercase characters.
string ofToUpper(const string& src, const string & locale="");

string ofTrimFront(const string & src, const string & locale = "");
string ofTrimBack(const string & src, const string & locale = "");
string ofTrim(const string & src, const string & locale = "");

void ofAppendUTF8(string & str, int utf8);

/// \brief Convert a variable length argument to a string.
/// \param format a printf-style format string.
/// \returns A string representation of the argument list.
string ofVAArgsToString(const char * format, ...);

/// \brief Convert a variable length argument to a string.
/// \param format A printf-style format string.
/// \param args A variable argument list.
/// \returns A string representation of the argument list.
string ofVAArgsToString(const char * format, va_list args);

/// \}

/// \name String conversion
/// \{


/// \brief Convert a value to a string.
///
/// ofToString does its best to convert any value to a string. If the data type
/// implements a stream << operator, then it will be converted.
///
/// Example:
/// ~~~~{.cpp}
///		std::string str = "framerate is ";
///		str += ofToString(ofGetFrameRate()) + " fps";
///		// The string now containes something like "framerate is 60 fps".
/// ~~~~
///
/// \tparam T The data type of the value to convert to a string.
/// \param value The value to convert to a string.
/// \returns A string representing the value or an empty string on failure.
template <class T>
string ofToString(const T& value){
	ostringstream out;
	out << value;
	return out.str();
}

/// \brief Convert a value to a string with a specific precision.
///
/// Like sprintf "%4f" format, in this example precision=4
///
/// \tparam T The data type of the value to convert to a string.
/// \param value The value to convert to a string.
/// \param precision The precision to use when converting to a string.
/// \returns The string representation of the value.
template <class T>
string ofToString(const T& value, int precision){
	ostringstream out;
	out << fixed << setprecision(precision) << value;
	return out.str();
}

/// \brief Convert a value to a string with a specific width and fill
///
/// Like sprintf "% 4d" or "% 4f" format, in this example width=4, fill=' '
///
/// \tparam T The data type of the value to convert to a string.
/// \param value The value to convert to a string.
/// \param width The width of the value to use when converting to a string.
/// \param fill The character to use when padding the converted string.
/// \returns The string representation of the value.
template <class T>
string ofToString(const T& value, int width, char fill ){
	ostringstream out;
	out << fixed << setfill(fill) << setw(width) << value;
	return out.str();
}

/// \brief Convert a value to a string with a specific precision, width and filll
///
/// Like sprintf "%04.2d" or "%04.2f" format, in this example precision=2, width=4, fill='0'
///
/// \tparam T The data type of the value to convert to a string.
/// \param value The value to convert to a string.
/// \param precision The precision to use when converting to a string.
/// \param width The width of the value to use when converting to a string.
/// \param fill The character to use when padding the converted string.
/// \returns The string representation of the value.
template <class T>
string ofToString(const T& value, int precision, int width, char fill ){
	ostringstream out;
	out << fixed << setfill(fill) << setw(width) << setprecision(precision) << value;
	return out.str();
}

/// \brief Convert a vector of values to a comma-delimited string.
///
/// This method will take any vector of values and output a list of the values
/// as a comma-delimited string.
///
/// \tparam T The data type held by the vector.
/// \param values The vector of values to be converted to a string.
/// \returns a comma-delimited string representation of the intput values.
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

/// \brief Convert a string represetnation to another type.
///
/// Often, this method will be used to convert a string to a numerical type.
///
/// \tparam T The target data type.
/// \param value The string value to convert to type T.
/// \returns the string converted to the target data type T.
template<class T>
T ofFromString(const string & value){
	T data;
    stringstream ss;
    ss << value;
    ss >> data;
    return data;
}

/// \brief Convert a string represetnation to another string.
/// \param value The string value to convert to another string.
/// \returns the string converted to another string.
template<>
string ofFromString(const string & value);

/// \brief Convert a string represetnation to another string.
///
/// Equivalent to calling the `.c_str()` on a std::string.
///
/// \param value The string value to convert to another string.
/// \returns the string converted to a c-style string.
template<>
const char * ofFromString(const string & value);

/// \}

// --------------------------------------------
/// \name Number conversion
/// \{

/// \brief Convert a string to an integer.
///
/// Converts a `std::string` representation of an int (e.g., `"3"`) to an actual
/// `int`.
///
/// \param intString The string representation of the integer.
/// \returns the integer represented by the string or 0 on failure.
int ofToInt(const string& intString);

// --------------------------------------------
/// \name Number conversion
/// \{

/// \brief Convert a string to a int64_t.
///
/// Converts a `std::string` representation of a long integer
/// (e.g., `"9223372036854775807"`) to an actual `int64_t`.
///
/// \param intString The string representation of the long integer.
/// \returns the long integer represented by the string or 0 on failure.
int64_t ofToInt64(const string& intString);

/// \brief Convert a string to a float.
///
/// Converts a std::string representation of a float (e.g., `"3.14"`) to an
/// actual `float`.
///
/// \param floatString string representation of the float.
/// \returns the float represented by the string or 0 on failure.
float ofToFloat(const string& floatString);

/// \brief Convert a string to a double.
///
/// Converts a std::string representation of a double (e.g., `"3.14"`) to an
/// actual `double`.
///
/// \param doubleString The string representation of the double.
/// \returns the double represented by the string or 0 on failure.
double ofToDouble(const string& doubleString);

/// \brief Convert a string to a boolean.
///
/// Converts a std::string representation of a boolean (e.g., `"TRUE"`) to an
/// actual `bool` using a case-insensitive comparison against the words `"true"`
/// and `"false"`.
///
/// \param boolString The string representation of the boolean.
/// \returns the boolean represented by the string or 0 on failure.
bool ofToBool(const string& boolString);

/// \brief Converts any value to its equivalent hexadecimal representation.
///
/// The hexadecimal representation corresponds to the way a number is stored in
/// memory.
///
/// \tparam T The data type of the value to convert to a hexadecimal string.
/// \param value The value to convert to a hexadecimal string.
/// \returns the hexadecimal string representation of the value.
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

/// \brief Convert a string to a hexadecimal string.
///
/// Converts a string (e.g., `"abc"`) to its equivalent hexadecimal
/// representation (e.g., `"616263"`).
///
/// \param value The value to convert to a hexadecimal string.
/// \returns a hexadecimal string.
template <>
string ofToHex(const string& value);

/// \brief Convert a c-style string to a hexadecimal string.
///
/// Converts a string (e.g., `"abc"`) to its equivalent hexadecimal
/// representation (e.g., `"616263"`).
///
/// \param value The value to convert to a hexadecimal string.
/// \returns a hexadecimal string.
string ofToHex(const char* value);

/// \brief Convert a string representing an integer in hexadecimal to a string.
///
/// Converts a hexadecimal representation of an int (little-endian, 32-bit,
/// e.g., `"0xbadf00d"` or `"badf00d"`) to an actual `int`.
///
/// \param intHexString The string representing an integer in hexadecimal.
/// \returns the integer represented by the string.
int ofHexToInt(const string& intHexString);

/// \brief Convert a string representing an char in hexadecimal to a char.
///
/// Converts a hexadecimal representation of an char (e.g., `"61"`) to an actual
/// `char` (e.g., `a`).
///
/// \param charHexString The string representing an char in hexadecimal.
/// \returns the char represented by the string.
char ofHexToChar(const string& charHexString);

/// \brief Convert a string representing an float in hexadecimal to a float.
///
/// Converts a hexadecimal representation of an float (little-endian, 32-bit
/// IEEE 754, e.g., `"4060000000000000"`) to an actual float (e.g., `128.f`).
///
/// \param floatHexString The string representing an float in hexadecimal.
/// \returns the float represented by the string.
float ofHexToFloat(const string& floatHexString);

/// \brief Convert a string representing an string in hexadecimal to a string.
///
/// Converts a hexadecimal representation of an string (e.g.,
/// `"61626364656667"`) to an actual string (`"abcdefg"`).
///
/// \param stringHexString The string representing an string in hexadecimal.
/// \returns the string represented by the string.
string ofHexToString(const string& stringHexString);

/// \brief Convert a string representation of a char to a actual char.
///
/// Converts a string representation of a single char (e.g., `"c"`) to an
/// actual `char`.
///
/// \note Multi-byte chars and multi-char strings are not supported.
///
/// \param charString The char string to convert.
/// \returns The string as a char or 0 on failure.
char ofToChar(const string& charString);

/// \brief Converts any datatype value to a string of only 1s and 0s.
///
/// The corresponding string of 1s and 0s corresponds to the way value is stored
/// in memory.
///
/// \tparam T The data type of the value to convert to a binary string.
/// \param value The data to convert to a binary string.
/// \returns a binary string.
template <class T>
string ofToBinary(const T& value) {
	return std::bitset<8 * sizeof(T)>(*reinterpret_cast<const uint64_t*>(&value)).to_string();
}

/// \brief Converts a string value to a string of only 1s and 0s.
///
/// The corresponding string of 1s and 0s corresponds to the way value is stored
/// in memory.
///
/// \param value The string to convert to a binary string.
/// \returns a binary string.
template <>
string ofToBinary(const string& value);

/// \brief Converts a c-style string to a string of only 1s and 0s.
///
/// The corresponding string of 1s and 0s corresponds to the way value is stored
/// in memory.
///
/// \param value The c-style string to convert to a binary string.
/// \returns a binary string.
string ofToBinary(const char* value);

/// \brief Convert a binary string to an int.
///
/// Interprets a string consisting only of 1s and 0s as an int (little-endian,
/// 32-bit), and returns the corresponding int value.
///
/// \value The binary string.
/// \returns the integer represented by the string or 0 on failure.
int ofBinaryToInt(const string& value);

/// \brief Convert a binary string to an char.
///
/// Interprets a string consisting only of 1s and 0s as an char and returns
/// the corresponding int value.
///
/// \value The binary string.
/// \returns the char represented by the string or 0 on failure.
char ofBinaryToChar(const string& value);

/// \brief Convert a binary string to a float.
///
/// Interprets a string consisting only of 1s and 0s as an a float
/// (little-endian, 32-bit IEEE 754), and returns the corresponding float value.
///
/// \value The binary string.
/// \returns the float represented by the string or 0 on failure.
float ofBinaryToFloat(const string& value);

/// \brief Convert a binary string to ASCII characters.
///
/// Interprets a string consisting only of 1s and 0s as 8-bit ASCII characters
/// and returns the corresponding string.
///
/// \value The binary string.
/// \returns the ASCII string represented by the string.
string ofBinaryToString(const string& value);

/// \}

// --------------------------------------------
/// \name openFrameworks version
/// \{

/// \brief Get the current version of openFrameworks as a string.
///
/// openFrameworks uses the semantic versioning system.
///
/// \sa http://semver.org/
/// \returns The string representation of the version (e.g. `0.9.0`).
string 	ofGetVersionInfo();

/// \brief Get the major version number of openFrameworks.
///
/// openFrameworks uses the semantic versioning system.
///
/// \sa http://semver.org/
/// \returns The major version number.
unsigned int ofGetVersionMajor();

/// \brief Get the minor version number of openFrameworks.
///
/// openFrameworks uses the semantic versioning system.
///
/// \sa http://semver.org/
/// \returns The minor version number.
unsigned int ofGetVersionMinor();

/// \brief Get the patch version number of openFrameworks.
///
/// openFrameworks uses the semantic versioning system.
///
/// \sa http://semver.org/
/// \returns The patch version number.
unsigned int ofGetVersionPatch();

/// \brief Get the pre-release version of openFrameworks.
///
/// openFrameworks uses the semantic versioning system.
///
/// For pre-release versions of openFrameworks, including development versions,
/// this string will describe the pre-release state. Examples might include
/// "master", "rc1", "rc2", etc.  For all stable releases, this string will be
/// empty.
///
/// \sa http://semver.org/
/// \returns The pre-release version string.
std::string ofGetVersionPreRelease();


/// \}

// --------------------------------------------
/// \name Frame saving
/// \{


/// \brief Saves the current screen image to a file on disk.
///
/// Example:
/// ~~~~{.cpp}
///		ofSaveScreen("screen.png"); // Will save screen.png in the /data folder.
/// ~~~~
///
/// The output file type will be deduced from the given file name.
///
/// \param filename The image output file.
void ofSaveScreen(const string& filename);

/// \brief Saves the current frame as a PNG image.
///
/// The PNG image will be named according to an internal counter in sequence.
/// The count will be restarted each time the program is restarted.
///
/// \param bUseViewport Set to true if the current viewport should be used.
void ofSaveFrame(bool bUseViewport = false);

/// \brief Saves the current viewport as an image.
///
/// The output file type will be deduced from the given file name.
///
/// \param filename The image output file.
void ofSaveViewport(const string& filename);


/// \}

/// \name System
/// \{

/// \brief Launch the given URL in the default browser.
/// \param url the URL to open.
/// \param uriEncodeQuery true if the query parameters in the given URL have
/// already been URL encoded.
#ifndef TARGET_EMSCRIPTEN
void ofLaunchBrowser(const string& url, bool uriEncodeQuery=false);
#endif

/// \brief Executes a system command. Similar to run a command in terminal.
/// \note Will block until the executed program/command has finished.
/// \returns the system command output as string. 
string ofSystem(const string& command);

/// \brief Get the target platform of the current system.
/// \returns the current ofTargetPlatform.
ofTargetPlatform ofGetTargetPlatform();


std::string ofGetEnv(const std::string & var);

/// Allows to iterate over a string's utf8 codepoints.
/// The easiest way to use it is with a c++11 range style
/// for loop like:
///
/// for(auto c: ofUTF8Iterator(str)){
/// ...
/// }
///
/// which will iterate through all the utf8 codepoints in the
/// string.
class ofUTF8Iterator{
public:
	ofUTF8Iterator(const string & str);
	utf8::iterator<std::string::const_iterator> begin() const;
	utf8::iterator<std::string::const_iterator> end() const;
	utf8::iterator<std::string::const_reverse_iterator> rbegin() const;
	utf8::iterator<std::string::const_reverse_iterator> rend() const;

private:
	std::string src_valid;
};

/// \}



/*! \cond PRIVATE */
namespace of{
namespace priv{
    void setWorkingDirectoryToDefault();
    void initutils();
    void endutils();
}
}
/*! \endcond */
