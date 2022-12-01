#pragma once

#pragma clang diagnostic ignored "-Wformat-security"

#include "ofConstants.h"
#if !defined(TARGET_MINGW) 
	#include "utf8.h"
#else
	#include "utf8cpp/utf8.h" // MSYS2 : use of system-installed include
#endif
#include <bitset> // For ofToBinary.
#include <chrono>
#include <iomanip>  //for setprecision
#include <algorithm>
#include <sstream>
#include <type_traits>

/// \section Elapsed Time
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

/// \section System Time
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
uint64_t ofGetUnixTime();

/// \brief Get the system time in milliseconds.
/// \returns the system time in milliseconds.
OF_DEPRECATED_MSG("Use ofGetSystemTimeMillis() instead", uint64_t ofGetSystemTime());

/// \brief Get the system time in milliseconds.
/// \returns the system time in milliseconds.
uint64_t ofGetSystemTimeMillis();

/// \brief Get the system time in microseconds.
/// \returns the system time in microseconds.
uint64_t ofGetSystemTimeMicros();


struct ofTime{
	uint64_t seconds = 0;
	uint64_t nanoseconds = 0;

	enum Mode{
		System,
		FixedRate,
	} mode = System;

	uint64_t getAsMilliseconds() const;
	uint64_t getAsMicroseconds() const;
	uint64_t getAsNanoseconds() const;
	double getAsSeconds() const;
#ifndef TARGET_WIN32
	timespec getAsTimespec() const;
#endif

	std::chrono::time_point<std::chrono::nanoseconds> getAsTimePoint() const;
	std::chrono::nanoseconds operator-(const ofTime&) const;
	bool operator<(const ofTime&) const;
	bool operator>(const ofTime&) const;
	bool operator<=(const ofTime&) const;
	bool operator>=(const ofTime&) const;

	template<typename rep, typename ratio>
	ofTime operator+(const std::chrono::duration<rep,ratio> & duration) const{
		constexpr uint64_t NANOS_PER_SEC = 1000000000ll;
		auto ns = std::chrono::duration_cast<std::chrono::nanoseconds>(duration);
		ofTime t = *this;
		t.nanoseconds += ns.count();
		if(t.nanoseconds>=NANOS_PER_SEC){
			uint64_t secs = t.nanoseconds / NANOS_PER_SEC;
			t.nanoseconds -= NANOS_PER_SEC*secs;
			t.seconds+=secs;
		}
		return t;
	}

	template<typename rep, typename ratio>
	ofTime &operator+=(const std::chrono::duration<rep,ratio> & duration){
		constexpr uint64_t NANOS_PER_SEC = 1000000000ll;
		auto ns = std::chrono::duration_cast<std::chrono::nanoseconds>(duration);
		this->nanoseconds += ns.count();
		if(this->nanoseconds>=NANOS_PER_SEC){
			uint64_t secs = this->nanoseconds / NANOS_PER_SEC;
			this->nanoseconds -= NANOS_PER_SEC*secs;
			this->seconds+=secs;
		}
		return *this;
	}
};

/// \brief Get the system time.
/// \returns the system time.
ofTime ofGetCurrentTime();

/// \brief Sleeps the current thread for the specified amount of milliseconds.
/// \param millis The number of millseconds to sleep.
void ofSleepMillis(int millis);

/// \brief Formats the current system time according to the given format.
///
/// The default timestamp format is "%Y-%m-%d-%H-%M-%S-%i" (e.g.
/// 2011-01-15-18-29-35-299).
///
/// \returns the current time as a string with the default format.
std::string ofGetTimestampString();

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
std::string ofGetTimestampString(const std::string& timestampFormat);

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

/// \section Vectors
/// \brief Randomly reorder the values in a vector.
/// \tparam T the type contained by the vector.
/// \param values The vector of values to modify.
/// \sa http://www.cplusplus.com/reference/algorithm/random_shuffle/
template<class T>
void ofRandomize(std::vector<T>& values) {
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
void ofRemove(std::vector<T>& values, BoolFunction shouldErase) {
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
void ofSort(std::vector<T>& values) {
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
void ofSort(std::vector<T>& values, BoolFunction compare) {
	std::sort(values.begin(), values.end(), compare);
}

/// \brief Search for a target value in a vector of values.
/// \tparam T the type contained by the vector.
/// \param values The vector of values to be searched.
/// \param target The target value to be found.
/// \returns true the index of the first target value found.
/// \sa http://www.cplusplus.com/reference/iterator/distance/
template <class T>
std::size_t ofFind(const std::vector<T>& values, const T& target) {
	return std::distance(values.begin(), find(values.begin(), values.end(), target));
}

/// \brief Search for a target value in a vector of values.
/// \tparam T the type contained by the vector.
/// \param values The vector of values to be searched.
/// \param target The target value to be found.
/// \returns true if at least one value equal to the target value is found.
template <class T>
bool ofContains(const std::vector<T>& values, const T& target) {
	return ofFind(values, target) != values.size();
}



/// \section String Manipulation
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
std::vector<std::string> ofSplitString(const std::string& source, const std::string& delimiter, bool ignoreEmpty = false, bool trim = false);

/// \brief Join a vector of strings together into one string.
/// \param stringElements The vector of strings to join.
/// \param delimiter The delimiter to put betweeen each string.
std::string ofJoinString(const std::vector<std::string>& stringElements, const std::string& delimiter);

/// \brief Replace all occurrences of a string with another string.
/// \note The input string is passed by reference, so it will be modified.
/// \param input The string to run the replacement on.
/// \param searchStr The string to be replaced.
/// \param replaceStr The string to put in place.
void ofStringReplace(std::string& input, const std::string& searchStr, const std::string& replaceStr);

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
bool ofIsStringInString(const std::string& haystack, const std::string& needle);

/// \brief Check how many times a string contains another string.
/// \param haystack The string to check for occurrence in .
/// \param needle The string to check for.
std::size_t ofStringTimesInString(const std::string& haystack, const std::string& needle);

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
std::string ofToLower(const std::string& src, const std::string & locale="");

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
std::string ofToUpper(const std::string& src, const std::string & locale="");

/// \brief Remove locale-defined whitespace from the beginning of a string.
///
/// With the default locale, the following white spaces will be removed:
///
///    space (0x20, ' ')
///    form feed (0x0c, '\f')
///    line feed (0x0a, '\n')
///    carriage return (0x0d, '\r')
///    horizontal tab (0x09, '\t')
///    vertical tab (0x0b, '\v')
///
/// ~~~~{.cpp}
///     std::string original = "     Hello, World!";
///     std::string trimmed = ofTrimFront(original);
///     ofLog() << trimmed; // Returns "Hello, World"
/// ~~~~
///
/// \sa https://en.cppreference.com/w/cpp/string/byte/isspace
///
/// \param src The original un-trimmed string.
/// \param locale The locale for determining the definition of "spaces".
/// \returns a front-trimmed std::string.
std::string ofTrimFront(const std::string & src, const std::string & locale = "");

/// \brief Remove locale-defined whitespace from the end of a string.
///
/// With the default locale, the following white spaces will be removed:
///
///    space (0x20, ' ')
///    form feed (0x0c, '\f')
///    line feed (0x0a, '\n')
///    carriage return (0x0d, '\r')
///    horizontal tab (0x09, '\t')
///    vertical tab (0x0b, '\v')
///
/// ~~~~{.cpp}
///     std::string original = "Hello, World!     ";
///     std::string trimmed = ofTrimBack(original);
///     ofLog() << trimmed; // Returns "Hello, World"
/// ~~~~
///
/// \sa https://en.cppreference.com/w/cpp/string/byte/isspace
///
/// \param src The original un-trimmed string.
/// \param locale The locale for determining the definition of "spaces".
/// \returns a front-trimmed std::string.
std::string ofTrimBack(const std::string & src, const std::string & locale = "");

/// \brief Remove locale-defined whitespace from the beginning and end of a string.
///
/// With the default locale, the following white spaces will be removed:
///
///    space (0x20, ' ')
///    form feed (0x0c, '\f')
///    line feed (0x0a, '\n')
///    carriage return (0x0d, '\r')
///    horizontal tab (0x09, '\t')
///    vertical tab (0x0b, '\v')
///
/// ~~~~{.cpp}
///     std::string original = "     Hello, World!     ";
///     std::string trimmed = ofTrim(original);
///     ofLog() << trimmed; // Returns "Hello, World"
/// ~~~~
///
/// \sa https://en.cppreference.com/w/cpp/string/byte/isspace
///
/// \param src The original un-trimmed string.
/// \param locale The locale for determining the definition of "spaces".
/// \returns a front-trimmed std::string.
std::string ofTrim(const std::string & src, const std::string & locale = "");

OF_DEPRECATED_MSG("Use ofUTF8Append instead", void ofAppendUTF8(std::string & str, uint32_t utf8));

/// \brief Append a Unicode codepoint to a UTF8-encoded std::string.
///
/// ~~~~{.cpp}
///    uint32_t original = 0x0001F603; // "😃"
///    std::string utf8String = "Hello ";
///    ofUTF8Append(utf8String, original);
///    ofLog() << utf8String; // Prints "Hello 😃".
/// ~~~~
///
/// \param utf8 The UTF8-encoded std::string to be modified.
/// \param codepoint The Unicode codepoint that will be converted to UTF8 and appended to \p utf8.
void ofUTF8Append(std::string & utf8, uint32_t codepoint);

/// \brief Insert a Unicode codepoint into a UTF8-encoded string at a position.
///
/// ~~~~{.cpp}
///    std::string utf8String = "Helloworld!";
///    uint32_t codepoint = 0x0001F603; // "😃"
///    ofUTF8Insert(utf8String, 5, codepoint); // Insert "Hello😃world!"
///    ofLog() << utf8String; // Prints "Hello😃world!".
/// ~~~~
///
/// \param utf8 The string to insert a codepoint into.
/// \param pos The codepoint position in the UTF8-encoded string.
/// \param codepoint The codepoint to insert.
void ofUTF8Insert(std::string & utf8, size_t pos, uint32_t codepoint);

/// \brief Erase a range of codepoints from a UTF8-encoded substring.
///
/// ~~~~{.cpp}
///    std::string utf8String = "Hello world! 😃";
///    ofUTF8Erase(utf8String, 0, 13); // Erase "Hello world! "
///    ofLog() << utf8String; // Prints "😃".
/// ~~~~
///
/// \param utf8 The string to extract from.
/// \param pos The codepoint position in the UTF8-encoded string.
/// \param len The number of codepoints starting at \pos to extract.
/// \returns a UTF8-encoded substring.
void ofUTF8Erase(std::string & utf8, size_t pos, size_t len);

/// \brief Extract a range of codepoints from as a std::string.
///
/// ~~~~{.cpp}
///    std::string utf8String = "Hello world! 😃";
///    ofLog() << ofUTF8Substring(utf8String, 13, 1); // Prints "😃".
/// ~~~~
///
/// \param utf8 The string to extract from.
/// \param pos The codepoint position in the UTF8-encoded string.
/// \param len The number of codepoints starting at \pos to extract.
/// \returns a UTF8-encoded substring.
std::string ofUTF8Substring(const std::string & utf8, size_t pos, size_t len);

/// \brief Convert a Unicode codepoint to a UTF8-encoded std::string.
///
/// ~~~~{.cpp}
///    uint32_t original = 0x0001F603; // "😃"
///    std::string utf8String = ofUTF8ToString(original);
///    ofLog() << utf8String; // Prints 😃.
/// ~~~~
///
/// \param codepoint The Unicode codepoint.
/// \returns a UTF8-encoded string.
std::string ofUTF8ToString(uint32_t codepoint);

/// \brief Get the number of Unicode code points in a UTF8-encoded string.
///
/// ~~~~{.cpp}
///    std::string original = "😃";
///    ofLog() << original.size(); // Returns 4.
///    ofLog() << ofUTF8Length(original); // Returns 1.
/// ~~~~
///
/// \param utf8 The UTF8-encoded std::string.
/// \returns the number of Unicode codepoints in the given string, or 0 if the
///          string is an invalid UTF8 string.
size_t ofUTF8Length(const std::string & utf8);


/// \brief Convert a variable length argument to a string.
/// \param format A printf-style format string.
/// \param args A variable argument list.
/// \returns A string representation of the argument list.
///
template <typename ... Args>
//__attribute__((__format__ (__printf__, 2, 0)))
std::string ofVAArgsToString(const char * format, Args&& ... args){
	char buf[256];
	size_t n = std::snprintf(buf, sizeof(buf), format, std::forward<Args>(args)...);
	
//	std::string str = format;
//	size_t n = std::snprintf(buf, sizeof(buf), str, std::forward<Args>(args)...);

	// Static buffer large enough?
	if (n < sizeof(buf)) {
		return{ buf, n };
	}

	// Static buffer too small
	std::string s(n + 1, 0);
	std::snprintf(const_cast<char*>(s.data()), s.size(), format, std::forward<Args>(args)...);
	
	return s;

}


/// \section String Conversion
/// \brief Convert a value to a string.
///
/// ofToString does its best to convert any value to a string. If the data type
/// implements a stream << operator, then it will be converted.
///
/// Example:
/// ~~~~{.cpp}
///    std::string str = "framerate is ";
///    str += ofToString(ofGetFrameRate()) + " fps";
///    // The string now contains something like "framerate is 60 fps".
/// ~~~~
///
/// \tparam T The data type of the value to convert to a string.
/// \param value The value to convert to a string.
/// \returns A string representing the value or an empty string on failure.
template <class T>
std::string ofToString(const T& value){
	std::ostringstream out;
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
std::string ofToString(const T& value, int precision){
	std::ostringstream out;
	out << std::fixed << std::setprecision(precision) << value;
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
std::string ofToString(const T& value, int width, char fill ){
	std::ostringstream out;
	out << std::fixed << std::setfill(fill) << std::setw(width) << value;
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
std::string ofToString(const T& value, int precision, int width, char fill ){
	std::ostringstream out;
	out << std::fixed << std::setfill(fill) << std::setw(width) << std::setprecision(precision) << value;
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
std::string ofToString(const std::vector<T>& values) {
	std::stringstream out;
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
T ofFromString(const std::string & value){
	T data;
	std::stringstream ss;
	ss << value;
	ss >> data;
	return data;
}

/// \brief Convert a string represetnation to another string.
/// \param value The string value to convert to another string.
/// \returns the string converted to another string.
template<>
std::string ofFromString(const std::string & value);

/// \brief Convert a string represetnation to another string.
///
/// Equivalent to calling the `.c_str()` on a std::string.
///
/// \param value The string value to convert to another string.
/// \returns the string converted to a c-style string.
template<>
const char * ofFromString(const std::string & value);

/// \brief Convert a string to a given data type.
/// \tparam T The return type.
/// \param value The string value to convert to a give type.
/// \returns the string converted to the type.
template<typename T>
T ofTo(const std::string & str){
	T x;
	std::istringstream cur(str);
	cur >> x;
	return x;
}

/// \section Number Conversion
/// \brief Convert a string to an integer.
///
/// Converts a `std::string` representation of an int (e.g., `"3"`) to an actual
/// `int`.
///
/// \param intString The string representation of the integer.
/// \returns the integer represented by the string or 0 on failure.
int ofToInt(const std::string& intString);

/// \brief Convert a string to a int64_t.
///
/// Converts a `std::string` representation of a long integer
/// (e.g., `"9223372036854775807"`) to an actual `int64_t`.
///
/// \param intString The string representation of the long integer.
/// \returns the long integer represented by the string or 0 on failure.
int64_t ofToInt64(const std::string& intString);

/// \brief Convert a string to a float.
///
/// Converts a std::string representation of a float (e.g., `"3.14"`) to an
/// actual `float`.
///
/// \param floatString string representation of the float.
/// \returns the float represented by the string or 0 on failure.
float ofToFloat(const std::string& floatString);

/// \brief Convert a string to a double.
///
/// Converts a std::string representation of a double (e.g., `"3.14"`) to an
/// actual `double`.
///
/// \param doubleString The string representation of the double.
/// \returns the double represented by the string or 0 on failure.
double ofToDouble(const std::string& doubleString);

/// \brief Convert a string to a boolean.
///
/// Converts a std::string representation of a boolean (e.g., `"TRUE"`) to an
/// actual `bool` using a case-insensitive comparison against the words `"true"`
/// and `"false"`.
///
/// \param boolString The string representation of the boolean.
/// \returns the boolean represented by the string or 0 on failure.
bool ofToBool(const std::string& boolString);

/// \brief Converts any value to its equivalent hexadecimal representation.
///
/// The hexadecimal representation corresponds to the way a number is stored in
/// memory.
///
/// \tparam T The data type of the value to convert to a hexadecimal string.
/// \param value The value to convert to a hexadecimal string.
/// \returns the hexadecimal string representation of the value.
template <class T>
std::string ofToHex(const T& value) {
	std::ostringstream out;
	// pretend that the value is a bunch of bytes
	unsigned char* valuePtr = (unsigned char*) &value;
	// the number of bytes is determined by the datatype
	int numBytes = sizeof(T);
	// the bytes are stored backwards (least significant first)
	for(int i = numBytes - 1; i >= 0; i--) {
		// print each byte out as a 2-character wide hex value
		out << std::setfill('0') << std::setw(2) << std::hex << (int) valuePtr[i];
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
std::string ofToHex(const std::string& value);

/// \brief Convert a c-style string to a hexadecimal string.
///
/// Converts a string (e.g., `"abc"`) to its equivalent hexadecimal
/// representation (e.g., `"616263"`).
///
/// \param value The value to convert to a hexadecimal string.
/// \returns a hexadecimal string.
std::string ofToHex(const char* value);

/// \brief Convert a string representing an integer in hexadecimal to a string.
///
/// Converts a hexadecimal representation of an int (little-endian, 32-bit,
/// e.g., `"0xbadf00d"` or `"badf00d"`) to an actual `int`.
///
/// \param intHexString The string representing an integer in hexadecimal.
/// \returns the integer represented by the string.
int ofHexToInt(const std::string& intHexString);

/// \brief Convert a string representing an char in hexadecimal to a char.
///
/// Converts a hexadecimal representation of an char (e.g., `"61"`) to an actual
/// `char` (e.g., `a`).
///
/// \param charHexString The string representing an char in hexadecimal.
/// \returns the char represented by the string.
char ofHexToChar(const std::string& charHexString);

/// \brief Convert a string representing an float in hexadecimal to a float.
///
/// Converts a hexadecimal representation of an float (little-endian, 32-bit
/// IEEE 754, e.g., `"43000000"`) to an actual float (e.g., `128.f`).
///
/// \param floatHexString The string representing an float in hexadecimal.
/// \returns the float represented by the string.
float ofHexToFloat(const std::string& floatHexString);

/// \brief Convert a string representing an string in hexadecimal to a string.
///
/// Converts a hexadecimal representation of an string (e.g.,
/// `"61626364656667"`) to an actual string (`"abcdefg"`).
///
/// \param stringHexString The string representing an string in hexadecimal.
/// \returns the string represented by the string.
std::string ofHexToString(const std::string& stringHexString);

/// \brief Convert a string representation of a char to a actual char.
///
/// Converts a string representation of a single char (e.g., `"c"`) to an
/// actual `char`.
///
/// \note Multi-byte chars and multi-char strings are not supported.
///
/// \param charString The char string to convert.
/// \returns The string as a char or 0 on failure.
char ofToChar(const std::string& charString);

/// \brief Converts any datatype value to a string of only 1s and 0s.
///
/// The corresponding string of 1s and 0s corresponds to the way value is stored
/// in memory.
///
/// \tparam T The data type of the value to convert to a binary string.
/// \param value The data to convert to a binary string.
/// \returns a binary string.
template <class T>
std::string ofToBinary(const T& value) {
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
std::string ofToBinary(const std::string& value);

/// \brief Converts a c-style string to a string of only 1s and 0s.
///
/// The corresponding string of 1s and 0s corresponds to the way value is stored
/// in memory.
///
/// \param value The c-style string to convert to a binary string.
/// \returns a binary string.
std::string ofToBinary(const char* value);

/// \brief Convert a binary string to an int.
///
/// Interprets a string consisting only of 1s and 0s as an int (little-endian,
/// 32-bit), and returns the corresponding int value.
///
/// \value The binary string.
/// \returns the integer represented by the string or 0 on failure.
int ofBinaryToInt(const std::string& value);

/// \brief Convert a binary string to an char.
///
/// Interprets a string consisting only of 1s and 0s as an char and returns
/// the corresponding int value.
///
/// \value The binary string.
/// \returns the char represented by the string or 0 on failure.
char ofBinaryToChar(const std::string& value);

/// \brief Convert a binary string to a float.
///
/// Interprets a string consisting only of 1s and 0s as an a float
/// (little-endian, 32-bit IEEE 754), and returns the corresponding float value.
///
/// \value The binary string.
/// \returns the float represented by the string or 0 on failure.
float ofBinaryToFloat(const std::string& value);

/// \brief Convert a binary string to ASCII characters.
///
/// Interprets a string consisting only of 1s and 0s as 8-bit ASCII characters
/// and returns the corresponding string.
///
/// \value The binary string.
/// \returns the ASCII string represented by the string.
std::string ofBinaryToString(const std::string& value);

/// \section openFrameworks Version
/// \brief Get the current version of openFrameworks as a string.
///
/// openFrameworks uses the semantic versioning system.
///
/// \sa http://semver.org/
/// \returns The string representation of the version (e.g. `0.9.0`).
std::string ofGetVersionInfo();

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


/// \section Frame Saving
/// \brief Saves the current screen image to a file on disk.
///
/// Example:
/// ~~~~{.cpp}
///    ofSaveScreen("screen.png"); // Will save screen.png in the /data folder.
/// ~~~~
///
/// The output file type will be deduced from the given file name.
///
/// \param filename The image output file.
void ofSaveScreen(const std::string& filename);

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
void ofSaveViewport(const std::string& filename);


/// \section System

/// \brief Launch the given URL in the default browser.
///
/// \param url the URL to open.
/// \param uriEncodeQuery true if the query parameters in the given URL have
/// already been URL encoded.
#ifndef TARGET_EMSCRIPTEN
void ofLaunchBrowser(const std::string& url, bool uriEncodeQuery=false);
#endif

/// \brief Executes a system command. Similar to run a command in terminal.
///
/// \note Will block until the executed program/command has finished.
/// \returns the system command output as string.
std::string ofSystem(const std::string& command);

/// \brief Get the target platform of the current system.
///
/// \returns the current ofTargetPlatform.
ofTargetPlatform ofGetTargetPlatform();

/// \brief Get the value of a given environment variable.
///
/// \note The available environment variables differ between operating systems.
/// \returns the environmnt variable's value or an empty string if not found.
std::string ofGetEnv(const std::string & var);

/// \brief Iterate through each Unicode codepoint in a UTF8-encoded std::string.
///
/// For UTF8-encoded strings each Unicode codepoint is comprised of between one
/// and four bytes. Thus to access individual Unicode codepoints, we must step
/// through a std::string in a UTF8-aware way.
///
/// The easiest way to use it is with a C++11 range style for loop like:
///
/// ~~~~{.cpp}
///     for(uint32_t codePoint: ofUTF8Iterator(str)){
///         // ...
///     }
/// ~~~~
///
/// which will iterate through all the utf8 codepoints in the string.
///
/// If the passed UTF8-encoded std::string is invalid, the iterator yield 0
/// elements e.g.:
///
/// ~~~~{.cpp}
///     std::string invalidUTF8String = "...";
///     ofLog() << (ofUTF8Iterator(invalidUTF8String).begin() == ofUTF8Iterator(invalidUTF8String).end()); // Returns true.
/// ~~~~
class ofUTF8Iterator{
public:
	/// \brief Create a ofUTF8Iterator for a given UTF8-encoded string.
	///
	/// \param str A UTF8-encoded string to iterate through.
	ofUTF8Iterator(const std::string & str);

	/// \returns A forward iterator that points to the first codepoint in the UTF8 string.
	utf8::iterator<std::string::const_iterator> begin() const;

	/// \returns An forward iterator that points to the end of the UTF8 string.
	utf8::iterator<std::string::const_iterator> end() const;

	/// \returns An reverse iterator that points to the last codepoint in the UTF8 string.
	utf8::iterator<std::string::const_reverse_iterator> rbegin() const;

	/// \returns An reverse iterator that points to the reverse end of the UTF8 string.
	utf8::iterator<std::string::const_reverse_iterator> rend() const;

private:
	/// \brief A copy of the validated UTF8-encoded std::string for validation.
	///
	/// If the UTF8-encoded std::string passed to the constructor
	/// `ofUTF8Iterator(...)` is invalid, this variable will be empty.
	std::string src_valid;
};


/*! \cond PRIVATE */
namespace of{
namespace priv{
    void initutils();
    void endutils();
}
}
/*! \endcond */
