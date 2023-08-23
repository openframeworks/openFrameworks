#include "ofUtils.h"
// FIXME: split ofUtils in two files, one which uses urlparser / ofImage, other without for smaller apps.
#include "ofImage.h"
#include "ofLog.h"
#include "ofAppBaseWindow.h"
#include "ofMainLoop.h"
#include "ofAppRunner.h"
#include "ofEvents.h"
#include "ofGLUtils.h"
#include "ofMath.h"
#include "ofPixels.h"

#include <chrono>
#include <numeric>
#include <locale>
#include "uriparser/Uri.h"

#ifdef TARGET_WIN32	 // For ofLaunchBrowser.
	#include <shellapi.h>
    #ifndef _MSC_VER
        #include <unistd.h> // this if for MINGW / _getcwd
		#include <sys/param.h> // for MAXPATHLEN
	// FIXME: else
    #endif
	#ifdef _MSC_VER
		#include <direct.h>
	#endif
	#include <mmsystem.h>
#endif

#if defined(TARGET_OF_IOS) || defined(TARGET_OSX ) || defined(TARGET_LINUX) || defined(TARGET_EMSCRIPTEN)
	#include <sys/time.h>
#endif

#ifdef TARGET_OSX
	#ifndef TARGET_OF_IOS
		#include <mach-o/dyld.h>
		#include <sys/param.h> // for MAXPATHLEN
	#endif
	#include <mach/clock.h>
	#include <mach/mach.h>
#endif

#ifdef TARGET_OF_IOS
#include "ofxiOSExtras.h"
#endif

#ifdef TARGET_ANDROID
#include "ofxAndroidUtils.h"
#endif

#ifndef MAXPATHLEN
	#define MAXPATHLEN 1024
#endif

using std::vector;
using std::string;
using std::setfill;

namespace of{
namespace priv{
	void initutils(){
        ofResetElapsedTimeCounter();
        of::random::Engine::construct();
    }

	void endutils(){
//#ifdef TARGET_OSX
//        mach_port_deallocate(mach_task_self(), cs);
//#endif
    }

	class Clock{
	public:
		Clock(){
		#ifdef TARGET_OSX
			host_get_clock_service(mach_host_self(), SYSTEM_CLOCK, &cs);
		#endif
		}

		//--------------------------------------
		void setTimeModeSystem(){
			mode = ofTime::System;
			loopListener.unsubscribe();
		}

		//--------------------------------------
		void setTimeModeFixedRate(uint64_t stepNanos, ofMainLoop & mainLoop){
			fixedRateTime = getMonotonicTimeForMode(ofTime::System);
			mode = ofTime::FixedRate;
			fixedRateStep = stepNanos;
			loopListener = mainLoop.loopEvent.newListener([this]{
				fixedRateTime.nanoseconds += fixedRateStep;
				while(fixedRateTime.nanoseconds>1000000000){
					fixedRateTime.nanoseconds -= 1000000000;
					fixedRateTime.seconds += 1;
				}
			});
		}

		//--------------------------------------
		ofTime getCurrentTime(){
			return getMonotonicTimeForMode(mode);
		}

		//--------------------------------------
		std::chrono::nanoseconds getElapsedTime(){
			return getCurrentTime() - startTime;
		}

		//--------------------------------------
		void resetElapsedTimeCounter(){
			startTime = getMonotonicTimeForMode(ofTime::System);
		}

	private:

		//--------------------------------------
		ofTime getMonotonicTimeForMode(ofTime::Mode mode){
			ofTime t;
			t.mode = mode;
			if(mode == ofTime::System){
			#if (defined(TARGET_LINUX) && !defined(TARGET_RASPBERRY_PI_LEGACY)) || defined(TARGET_EMSCRIPTEN)
				struct timespec now;
				clock_gettime(CLOCK_MONOTONIC, &now);
				t.seconds = now.tv_sec;
				t.nanoseconds = now.tv_nsec;
			#elif defined(TARGET_OSX)
				mach_timespec_t now;
				clock_get_time(cs, &now);
				t.seconds = now.tv_sec;
				t.nanoseconds = now.tv_nsec;
			#elif defined( TARGET_WIN32 )
				LARGE_INTEGER freq;
				LARGE_INTEGER counter;
				QueryPerformanceFrequency(&freq);
				QueryPerformanceCounter(&counter);
				t.seconds = counter.QuadPart/freq.QuadPart;
				t.nanoseconds = (counter.QuadPart % freq.QuadPart)*1000000000/freq.QuadPart;
			#else
				struct timeval now;
				gettimeofday( &now, nullptr );
				t.seconds = now.tv_sec;
				t.nanoseconds = now.tv_usec * 1000;
			#endif
			}else{
				t = fixedRateTime;
			}
			return t;
		}
		uint64_t fixedRateStep = 1666667;
		ofTime fixedRateTime;
		ofTime startTime;
		ofTime::Mode mode = ofTime::System;
		ofEventListener loopListener;
	#ifdef TARGET_OSX
		clock_serv_t cs;
	#endif
	};

	Clock & getClock(){
		static Clock * clock = new Clock;
		return *clock;
	}
}
}


//--------------------------------------
uint64_t ofTime::getAsMilliseconds() const{
	auto seconds = std::chrono::seconds(this->seconds);
	auto nanoseconds = std::chrono::nanoseconds(this->nanoseconds);
	return (std::chrono::duration_cast<std::chrono::milliseconds>(seconds) +
			std::chrono::duration_cast<std::chrono::milliseconds>(nanoseconds)).count();
}

//--------------------------------------
uint64_t ofTime::getAsMicroseconds() const{
	auto seconds = std::chrono::seconds(this->seconds);
	auto nanoseconds = std::chrono::nanoseconds(this->nanoseconds);
	return (std::chrono::duration_cast<std::chrono::microseconds>(seconds) +
			std::chrono::duration_cast<std::chrono::microseconds>(nanoseconds)).count();
}

//--------------------------------------
uint64_t ofTime::getAsNanoseconds() const{
	auto seconds = std::chrono::seconds(this->seconds);
	auto nanoseconds = std::chrono::nanoseconds(this->nanoseconds);
	return (std::chrono::duration_cast<std::chrono::nanoseconds>(seconds) + nanoseconds).count();
}

//--------------------------------------
double ofTime::getAsSeconds() const{
	return seconds + nanoseconds / 1000000000.;
}

#ifndef TARGET_WIN32
timespec ofTime::getAsTimespec() const{
	timespec ret;
	ret.tv_sec = seconds;
	ret.tv_nsec = nanoseconds;
	return ret;
}
#endif

//--------------------------------------
std::chrono::time_point<std::chrono::nanoseconds> ofTime::getAsTimePoint() const{
	auto seconds = std::chrono::seconds(this->seconds);
	auto nanoseconds = std::chrono::nanoseconds(this->nanoseconds);
	return std::chrono::time_point<std::chrono::nanoseconds>(
				std::chrono::duration_cast<std::chrono::nanoseconds>(seconds) + nanoseconds);
}

//--------------------------------------
std::chrono::nanoseconds ofTime::operator-(const ofTime& other) const{
	auto seconds = std::chrono::seconds(this->seconds) - std::chrono::seconds(other.seconds);
	auto nanoseconds = std::chrono::nanoseconds(this->nanoseconds) - std::chrono::nanoseconds(other.nanoseconds);
	return std::chrono::duration_cast<std::chrono::nanoseconds>(seconds) + nanoseconds;
}

//--------------------------------------
bool ofTime::operator<(const ofTime & other) const{
	return seconds < other.seconds || (seconds == other.seconds && nanoseconds < other.nanoseconds);
}

//--------------------------------------
bool ofTime::operator>(const ofTime & other) const{
	return seconds > other.seconds || (seconds == other.seconds && nanoseconds > other.nanoseconds);
}

//--------------------------------------
bool ofTime::operator<=(const ofTime & other) const{
	return seconds <= other.seconds || (seconds == other.seconds && nanoseconds <= other.nanoseconds);
}

//--------------------------------------
bool ofTime::operator>=(const ofTime & other) const{
	return seconds >= other.seconds || (seconds == other.seconds && nanoseconds >= other.nanoseconds);
}

//--------------------------------------
uint64_t ofGetFixedStepForFps(double fps){
	return 1000000000 / fps;
}

//--------------------------------------
void ofSetTimeModeSystem(){
	auto mainLoop = ofGetMainLoop();
	if(!mainLoop){
		ofLogError("ofSetSystemTimeMode") << "ofMainLoop is not initialized yet, can't set time mode";
		return;
	}
	auto window = mainLoop->getCurrentWindow();
	if(!window){
		ofLogError("ofSetSystemTimeMode") << "No window setup yet can't set time mode";
		return;
	}
	window->events().setTimeModeSystem();
	of::priv::getClock().setTimeModeSystem();
}

//--------------------------------------
void ofSetTimeModeFixedRate(uint64_t stepNanos){
	auto mainLoop = ofGetMainLoop();
	if(!mainLoop){
		ofLogError("ofSetSystemTimeMode") << "ofMainLoop is not initialized yet, can't set time mode";
		return;
	}
	auto window = mainLoop->getCurrentWindow();
	if(!window){
		ofLogError("ofSetSystemTimeMode") << "No window setup yet can't set time mode";
		return;
	}
	window->events().setTimeModeFixedRate(stepNanos);
	of::priv::getClock().setTimeModeFixedRate(stepNanos, *mainLoop);
}

//--------------------------------------
void ofSetTimeModeFiltered(float alpha){
	auto mainLoop = ofGetMainLoop();
	if(!mainLoop){
		ofLogError("ofSetSystemTimeMode") << "ofMainLoop is not initialized yet, can't set time mode";
		return;
	}
	auto window = mainLoop->getCurrentWindow();
	if(!window){
		ofLogError("ofSetSystemTimeMode") << "No window setup yet can't set time mode";
		return;
	}
	window->events().setTimeModeFiltered(alpha);
	of::priv::getClock().setTimeModeSystem();
}

//--------------------------------------
ofTime ofGetCurrentTime(){
	return of::priv::getClock().getCurrentTime();
}


//--------------------------------------
uint64_t ofGetElapsedTimeMillis(){
	return std::chrono::duration_cast<std::chrono::milliseconds>(of::priv::getClock().getElapsedTime()).count();
}

//--------------------------------------
uint64_t ofGetElapsedTimeMicros(){
	return std::chrono::duration_cast<std::chrono::microseconds>(of::priv::getClock().getElapsedTime()).count();
}

//--------------------------------------
float ofGetElapsedTimef(){
	return std::chrono::duration<double>(of::priv::getClock().getElapsedTime()).count();
}

//--------------------------------------
void ofResetElapsedTimeCounter(){
	of::priv::getClock().resetElapsedTimeCounter();
}

//--------------------------------------
uint64_t ofGetSystemTime( ) {
	return of::priv::getClock().getCurrentTime().getAsMilliseconds();
}

//--------------------------------------
uint64_t ofGetSystemTimeMillis( ) {
	return of::priv::getClock().getCurrentTime().getAsMilliseconds();
}

//--------------------------------------
uint64_t ofGetSystemTimeMicros( ) {
	return of::priv::getClock().getCurrentTime().getAsMicroseconds();
}

//--------------------------------------------------
uint64_t ofGetUnixTime(){
	return static_cast<uint64_t>(time(nullptr));
}

uint64_t ofGetUnixTimeMillis() {
    auto elapsed = std::chrono::system_clock::now().time_since_epoch();
    return std::chrono::duration_cast<std::chrono::milliseconds>(elapsed).count();
}

//--------------------------------------
void ofSleepMillis(int millis){
	#ifdef TARGET_WIN32
		Sleep(millis);
	#elif defined(TARGET_LINUX)
		timespec interval = {millis/1000, millis%1000*1000000};
		timespec rem = {0,0};
		clock_nanosleep(CLOCK_MONOTONIC,0,&interval,&rem);
	#elif !defined(TARGET_EMSCRIPTEN)
		usleep(millis * 1000);
	#endif
}

//default ofGetTimestampString returns in this format: 2011-01-15-18-29-35-299
//--------------------------------------------------
string ofGetTimestampString(){

	string timeFormat = "%Y-%m-%d-%H-%M-%S-%i";

	return ofGetTimestampString(timeFormat);
}

//specify the string format - eg: %Y-%m-%d-%H-%M-%S-%i ( 2011-01-15-18-29-35-299 )
//--------------------------------------------------
string ofGetTimestampString(const string& timestampFormat){
	std::stringstream str;
	auto now = std::chrono::system_clock::now();
	auto t = std::chrono::system_clock::to_time_t(now);    std::chrono::duration<double> s = now - std::chrono::system_clock::from_time_t(t);
    int ms = s.count() * 1000;
	auto tm = *std::localtime(&t);
	constexpr int bufsize = 256;
	char buf[bufsize];

	// Beware! an invalid timestamp string crashes windows apps.
	// so we have to filter out %i (which is not supported by vs)
	// earlier.
	auto tmpTimestampFormat = timestampFormat;
	ofStringReplace(tmpTimestampFormat, "%i", ofToString(ms, 3, '0'));

	if (strftime(buf,bufsize, tmpTimestampFormat.c_str(),&tm) != 0){
		str << buf;
	}
	auto ret = str.str();


    return ret;
}

//--------------------------------------------------
int ofGetSeconds(){
	time_t 	curr;
	tm 		local;
	time(&curr);
	local	=*(localtime(&curr));
	return local.tm_sec;
}

//--------------------------------------------------
int ofGetMinutes(){
	time_t 	curr;
	tm 		local;
	time(&curr);
	local	=*(localtime(&curr));
	return local.tm_min;
}

//--------------------------------------------------
int ofGetHours(){
	time_t 	curr;
	tm 		local;
	time(&curr);
	local	=*(localtime(&curr));
	return local.tm_hour;
}

//--------------------------------------------------
int ofGetYear(){
	time_t    curr;
	tm       local;
	time(&curr);
	local   =*(localtime(&curr));
	int year = local.tm_year + 1900;
	return year;
}

//--------------------------------------------------
int ofGetMonth(){
	time_t    curr;
	tm       local;
	time(&curr);
	local   =*(localtime(&curr));
	int month = local.tm_mon + 1;
	return month;
}

//--------------------------------------------------
int ofGetDay(){
	time_t    curr;
	tm       local;
	time(&curr);
	local   =*(localtime(&curr));
	return local.tm_mday;
}

//--------------------------------------------------
int ofGetWeekday(){
	time_t    curr;
	tm       local;
	time(&curr);
	local   =*(localtime(&curr));
	return local.tm_wday;
}

//----------------------------------------
template<>
string ofFromString(const string& value){
	return value;
}

//----------------------------------------
template<>
const char * ofFromString(const string& value){
	return value.c_str();
}

//----------------------------------------
template <>
string ofToHex(const string& value) {
	std::ostringstream out;
	// how many bytes are in the string
	std::size_t numBytes = value.size();
	for(std::size_t i = 0; i < numBytes; i++) {
		// print each byte as a 2-character wide hex value
		out << setfill('0') << std::setw(2) << std::hex << (unsigned int) ((unsigned char)value[i]);
	}
	return out.str();
}

//----------------------------------------
string ofToHex(const char* value) {
	// this function is necessary if you want to print a string
	// using a syntax like ofToHex("test")
	return ofToHex((string) value);
}

//----------------------------------------
int ofToInt(const string& intString) {
	return ofTo<int>(intString);
}

//----------------------------------------
int ofHexToInt(const string& intHexString) {
	int x = 0;
	std::istringstream cur(intHexString);
	cur >> std::hex >> x;
	return x;
}

//----------------------------------------
char ofHexToChar(const string& charHexString) {
	int x = 0;
	std::istringstream cur(charHexString);
	cur >> std::hex >> x;
	return (char) x;
}

//----------------------------------------
float ofHexToFloat(const string& floatHexString) {
	union intFloatUnion {
		uint32_t i;
		float f;
	} myUnion;
	myUnion.i = 0;
	std::istringstream cur(floatHexString);
	cur >> std::hex >> myUnion.i;
	return myUnion.f;
}

//----------------------------------------
string ofHexToString(const string& stringHexString) {
	std::stringstream out;
	std::stringstream stream(stringHexString);
	// a hex string has two characters per byte
	std::size_t numBytes = stringHexString.size() / 2;
	for(std::size_t i = 0; i < numBytes; i++) {
		string curByte;
		// grab two characters from the hex string
		stream >> std::setw(2) >> curByte;
		// prepare to parse the two characters
		std::stringstream curByteStream(curByte);
		int cur = 0;
		// parse the two characters as a hex-encoded int
		curByteStream >> std::hex >> cur;
		// add the int as a char to our output stream
		out << (char) cur;
	}
	return out.str();
}

//----------------------------------------
float ofToFloat(const string& floatString) {
	return ofTo<float>(floatString);
}

//----------------------------------------
double ofToDouble(const string& doubleString) {
	return ofTo<double>(doubleString);
}

//----------------------------------------
int64_t ofToInt64(const string& intString) {
	return ofTo<int64_t>(intString);
}

//----------------------------------------
bool ofToBool(const string& boolString) {
	auto lower = ofToLower(boolString);
	if(lower == "true") {
		return true;
	}
	if(lower == "false") {
		return false;
	}
	bool x = false;
	std::istringstream cur(lower);
	cur >> x;
	return x;
}

//----------------------------------------
char ofToChar(const string& charString) {
	return ofTo<char>(charString);
}

//----------------------------------------
template <> string ofToBinary(const string& value) {
	std::stringstream out;
	std::size_t numBytes = value.size();
	for(std::size_t i = 0; i < numBytes; i++) {
		std::bitset<8> bitBuffer(value[i]);
		out << bitBuffer;
	}
	return out.str();
}

//----------------------------------------
string ofToBinary(const char* value) {
	// this function is necessary if you want to print a string
	// using a syntax like ofToBinary("test")
	return ofToBinary((string) value);
}

//----------------------------------------
int ofBinaryToInt(const string& value) {
	const int intSize = sizeof(int) * 8;
	std::bitset<intSize> binaryString(value);
	return (int) binaryString.to_ulong();
}

//----------------------------------------
char ofBinaryToChar(const string& value) {
	const int charSize = sizeof(char) * 8;
	std::bitset<charSize> binaryString(value);
	return (char) binaryString.to_ulong();
}

//----------------------------------------
float ofBinaryToFloat(const string& value) {
	const int floatSize = sizeof(float) * 8;
	std::bitset<floatSize> binaryString(value);
	union ulongFloatUnion {
			unsigned long result;
			float f;
	} myUFUnion;
	myUFUnion.result = binaryString.to_ulong();
	return myUFUnion.f;
}
//----------------------------------------
string ofBinaryToString(const string& value) {
	std::ostringstream out;
	std::stringstream stream(value);
	std::bitset<8> byteString;
	std::size_t numBytes = value.size() / 8;
	for(std::size_t i = 0; i < numBytes; i++) {
		stream >> byteString;
		out << (char) byteString.to_ulong();
	}
	return out.str();
}

//--------------------------------------------------
vector <string> ofSplitString(const string & source, const string & delimiter, bool ignoreEmpty, bool trim) {
	vector<string> result;
	if (delimiter.empty()) {
		result.push_back(source);
		return result;
	}
	string::const_iterator substart = source.begin(), subend;
	while (true) {
		subend = search(substart, source.end(), delimiter.begin(), delimiter.end());
		string sub(substart, subend);
		if(trim) {
			sub = ofTrim(sub);
		}
		if (!ignoreEmpty || !sub.empty()) {
			result.push_back(sub);
		}
		if (subend == source.end()) {
			break;
		}
		substart = subend + delimiter.size();
	}
	return result;
}

//--------------------------------------------------
string ofJoinString(const vector<string>& stringElements, const string& delimiter){
	string str;
	if(stringElements.empty()){
		return str;
	}
	auto numStrings = stringElements.size();
	string::size_type strSize = delimiter.size() * (numStrings - 1);
	for (const string &s : stringElements) {
		strSize += s.size();
	}
	str.reserve(strSize);
	str += stringElements[0];
	for (decltype(numStrings) i = 1; i < numStrings; ++i) {
		str += delimiter;
		str += stringElements[i];
	}
	return str;
}

//--------------------------------------------------
void ofStringReplace(string& input, const string& searchStr, const string& replaceStr){
	auto pos = input.find(searchStr);
	while(pos != std::string::npos){
		input.replace(pos, searchStr.size(), replaceStr);
		pos += replaceStr.size();
		std::string nextfind(input.begin() + pos, input.end());
		auto nextpos = nextfind.find(searchStr);
		if(nextpos==std::string::npos){
			break;
		}
		pos += nextpos;
	}
}

//--------------------------------------------------
bool ofIsStringInString(const string& haystack, const string& needle){
    return haystack.find(needle) != std::string::npos;
}

//--------------------------------------------------
std::size_t ofStringTimesInString(const string& haystack, const string& needle){
	const size_t step = needle.size();

	size_t count(0);
	size_t pos(0) ;

	while( (pos=haystack.find(needle, pos)) != std::string::npos) {
		pos +=step;
		++count ;
	}

	return count;
}


ofUTF8Iterator::ofUTF8Iterator(const string & str){
	try{
		utf8::replace_invalid(str.begin(),str.end(),back_inserter(src_valid));
	}catch(...){
	}
}

utf8::iterator<std::string::const_iterator> ofUTF8Iterator::begin() const{
	try {
		return utf8::iterator<std::string::const_iterator>(src_valid.begin(), src_valid.begin(), src_valid.end());
	}
	catch (...) {
		return utf8::iterator<std::string::const_iterator>();
	}
}

utf8::iterator<std::string::const_iterator> ofUTF8Iterator::end() const{
	try {
		return utf8::iterator<std::string::const_iterator>(src_valid.end(), src_valid.begin(), src_valid.end());
	}
	catch (...) {
		return utf8::iterator<std::string::const_iterator>();
	}
}

utf8::iterator<std::string::const_reverse_iterator> ofUTF8Iterator::rbegin() const {
	try {
		return utf8::iterator<std::string::const_reverse_iterator>(src_valid.rbegin(), src_valid.rbegin(), src_valid.rend());
	}
	catch (...) {
		return utf8::iterator<std::string::const_reverse_iterator>();
	}
}

utf8::iterator<std::string::const_reverse_iterator> ofUTF8Iterator::rend() const {
	try {
		return utf8::iterator<std::string::const_reverse_iterator>(src_valid.rbegin(), src_valid.rbegin(), src_valid.rend());
	}
	catch (...) {
		return utf8::iterator<std::string::const_reverse_iterator>();
	}
}


//--------------------------------------------------
// helper method to get locale from name
static std::locale getLocale(const string & locale) {
std::locale loc;
#if defined(TARGET_WIN32) && !_MSC_VER
	static bool printonce = true;
	if( printonce ){
		std::string current( setlocale(LC_ALL,NULL) );
		setlocale (LC_ALL,"");
		ofLogWarning("ofUtils") << "std::locale not supported. Using C locale: " << current ;
		printonce = false;
	}
#else
	try {
		loc = std::locale(locale.c_str());
	}
	catch (...) {
		ofLogWarning("ofUtils") << "Couldn't create locale " << locale << " using default, " << loc.name();
	}
#endif
	return loc;
}

//--------------------------------------------------
string ofToLower(const string & src, const string & locale){
	std::string dst;
	std::locale loc = getLocale(locale);
	try{
		for(auto c: ofUTF8Iterator(src)){
			utf8::append(std::tolower<wchar_t>(c, loc), back_inserter(dst));
		}
	}catch(...){
	}
	return dst;
}

//--------------------------------------------------
string ofToUpper(const string & src, const string & locale){
	std::string dst;
	std::locale loc = getLocale(locale);
	try{
		for(auto c: ofUTF8Iterator(src)){
			utf8::append(std::toupper<wchar_t>(c, loc), back_inserter(dst));
		}
	}catch(...){
	}
	return dst;
}

//--------------------------------------------------
string ofTrimFront(const string & src, const string& locale){
    auto dst = src;
    std::locale loc = getLocale(locale);
    dst.erase(dst.begin(),std::find_if_not(dst.begin(),dst.end(),[&](char & c){return std::isspace<char>(c,loc);}));
    return dst;
}

//--------------------------------------------------
string ofTrimBack(const string & src, const string& locale){
    auto dst = src;
    std::locale loc = getLocale(locale);
	dst.erase(std::find_if_not(dst.rbegin(),dst.rend(),[&](char & c){return std::isspace<char>(c,loc);}).base(), dst.end());
	return dst;
}

//--------------------------------------------------
string ofTrim(const string & src, const string& locale){
    return ofTrimFront(ofTrimBack(src));
}

//--------------------------------------------------
void ofAppendUTF8(string & str, uint32_t utf8){
	try{
		utf8::append(utf8, back_inserter(str));
	}catch(...){}
}

//--------------------------------------------------
void ofUTF8Append(string & str, uint32_t utf8){
	try{
		utf8::append(utf8, back_inserter(str));
	}catch(...){}
}

//--------------------------------------------------
void ofUTF8Insert(string & str, size_t pos, uint32_t utf8){
	std::string newText;
	size_t i = 0;
	for(auto c: ofUTF8Iterator(str)){
		if(i==pos){
			ofUTF8Append(newText, utf8);
		}
		ofUTF8Append(newText, c);
		i+=1;
	}
	if(i==pos){
		ofUTF8Append(newText, utf8);
	}
	str = newText;
}

//--------------------------------------------------
void ofUTF8Erase(string & str, size_t start, size_t len){
	std::string newText;
	size_t i = 0;
	for(auto c: ofUTF8Iterator(str)){
		if(i<start || i>=start + len){
			ofUTF8Append(newText, c);
		}
		i+=1;
	}
	str = newText;
}

//--------------------------------------------------
std::string ofUTF8Substring(const string & str, size_t start, size_t len){
	size_t i=0;
	std::string newText;
	for(auto c: ofUTF8Iterator(str)){
		if(i>=start){
			ofUTF8Append(newText, c);
		}
		i += 1;
		if(i==start + len){
			break;
		}
	}
	return newText;
}

//--------------------------------------------------
std::string ofUTF8ToString(uint32_t utf8){
	std::string str;
	ofUTF8Append(str, utf8);
	return str;
}

//--------------------------------------------------
size_t ofUTF8Length(const std::string & str){
	try{
		return utf8::distance(str.begin(), str.end());
	}catch(...){
		return 0;
	}
}

//--------------------------------------------------
void ofLaunchBrowser(const string& url, bool uriEncodeQuery){
	UriParserStateA state;
	UriUriA uri;
	state.uri = &uri;
	if(uriParseUriA(&state, url.c_str())!=URI_SUCCESS){
		ofLogError("ofUtils") << "ofLaunchBrowser(): malformed url \"" << url << "\"";
		uriFreeUriMembersA(&uri);
		return;
	}
	if(uriEncodeQuery) {
		uriNormalizeSyntaxA(&uri); // URI encodes during set
	}
	std::string scheme(uri.scheme.first, uri.scheme.afterLast);
	int size;
	uriToStringCharsRequiredA(&uri, &size);
	std::vector<char> buffer(size+1, 0);
	int written;
	uriToStringA(buffer.data(), &uri, url.size()*2, &written);
	std::string uriStr(buffer.data(), written-1);
	uriFreeUriMembersA(&uri);


	// http://support.microsoft.com/kb/224816
	// make sure it is a properly formatted url:
	//   some platforms, like Android, require urls to start with lower-case http/https
	//   Poco::URI automatically converts the scheme to lower case
	if(scheme != "http" && scheme != "https"){
		ofLogError("ofUtils") << "ofLaunchBrowser(): url does not begin with http:// or https://: \"" << uriStr << "\"";
		return;
	}

	#ifdef TARGET_WIN32
		ShellExecuteA(nullptr, "open", uriStr.c_str(),
                nullptr, nullptr, SW_SHOWNORMAL);
	#endif

	#ifdef TARGET_OSX
        // could also do with LSOpenCFURLRef
		string commandStr = "open \"" + uriStr + "\"";
		int ret = system(commandStr.c_str());
        if(ret!=0) {
			ofLogError("ofUtils") << "ofLaunchBrowser(): couldn't open browser, commandStr \"" << commandStr << "\"";
		}
	#endif

	#ifdef TARGET_LINUX
		string commandStr = "xdg-open \"" + uriStr + "\"";
		int ret = system(commandStr.c_str());
		if(ret!=0) {
			ofLogError("ofUtils") << "ofLaunchBrowser(): couldn't open browser, commandStr \"" << commandStr << "\"";
		}
	#endif

	#ifdef TARGET_OF_IOS
		ofxiOSLaunchBrowser(uriStr);
	#endif

	#ifdef TARGET_ANDROID
		ofxAndroidLaunchBrowser(uriStr);
	#endif

	#ifdef TARGET_EMSCRIPTEN
		ofLogError("ofUtils") << "ofLaunchBrowser() not implementeed in emscripten";
	#endif
}

//--------------------------------------------------
string ofGetVersionInfo(){
	std::stringstream sstr;
	sstr << OF_VERSION_MAJOR << "." << OF_VERSION_MINOR << "." << OF_VERSION_PATCH;

	if (!std::string(OF_VERSION_PRE_RELEASE).empty())
	{
		sstr << "-" << OF_VERSION_PRE_RELEASE;
	}

	return sstr.str();
}

unsigned int ofGetVersionMajor() {
	return OF_VERSION_MAJOR;
}

unsigned int ofGetVersionMinor() {
	return OF_VERSION_MINOR;
}

unsigned int ofGetVersionPatch() {
	return OF_VERSION_PATCH;
}

std::string ofGetVersionPreRelease() {
	return OF_VERSION_PRE_RELEASE;
}


//---- new to 006
//from the forums http://www.openframeworks.cc/forum/viewtopic.php?t=1413

//--------------------------------------------------
void ofSaveScreen(const string& filename) {
   /*ofImage screen;
   screen.allocate(ofGetWidth(), ofGetHeight(), OF_IMAGE_COLOR);
   screen.grabScreen(0, 0, ofGetWidth(), ofGetHeight());
   screen.save(filename);*/
	ofPixels pixels;
	ofGetGLRenderer()->saveFullViewport(pixels);
	ofSaveImage(pixels,filename);
}

//--------------------------------------------------
void ofSaveViewport(const string& filename) {
	// because ofSaveScreen doesn't related to viewports
	/*ofImage screen;
	ofRectangle view = ofGetCurrentViewport();
	screen.allocate(view.width, view.height, OF_IMAGE_COLOR);
	screen.grabScreen(0, 0, view.width, view.height);
	screen.save(filename);*/

	ofPixels pixels;
	ofGetGLRenderer()->saveFullViewport(pixels);
	ofSaveImage(pixels,filename);
}

//--------------------------------------------------
int saveImageCounter = 0;
void ofSaveFrame(bool bUseViewport){
   string fileName = ofToString(saveImageCounter) + ".png";
	if (bUseViewport){
		ofSaveViewport(fileName);
	} else {
		ofSaveScreen(fileName);
	}
	saveImageCounter++;
}

//--------------------------------------------------
string ofSystem(const string& command){
	FILE * ret = nullptr;
#ifdef TARGET_WIN32
	ret = _popen(command.c_str(),"r");
#else
	ret = popen(command.c_str(),"r");
#endif

	string strret;
	int c;

	if (ret == nullptr){
		ofLogError("ofUtils") << "ofSystem(): error opening return file for command \"" << command  << "\"";
	}else{
		c = fgetc (ret);
		while (c != EOF) {
			strret += c;
			c = fgetc (ret);
		}
#ifdef TARGET_WIN32
		_pclose (ret);
#else
		pclose (ret);
#endif
	}

	return strret;
}

//--------------------------------------------------
ofTargetPlatform ofGetTargetPlatform(){
#ifdef TARGET_LINUX
    string arch = ofSystem("uname -m");
    if(ofIsStringInString(arch,"x86_64")) {
        return OF_TARGET_LINUX64;
    } else if(ofIsStringInString(arch,"armv6l")) {
        return OF_TARGET_LINUXARMV6L;
    } else if(ofIsStringInString(arch,"armv7l")) {
        return OF_TARGET_LINUXARMV7L;
	} else if(ofIsStringInString(arch,"aarch64")) {
		return OF_TARGET_LINUXAARCH64;		
    } else {
        return OF_TARGET_LINUX;
    }
#elif defined(TARGET_OSX)
    return OF_TARGET_OSX;
#elif defined(TARGET_WIN32)
    #if (_MSC_VER)
        return OF_TARGET_WINVS;
    #else
        return OF_TARGET_MINGW;
    #endif
#elif defined(TARGET_ANDROID)
    return OF_TARGET_ANDROID;
#elif defined(TARGET_OF_IOS)
    return OF_TARGET_IOS;
#elif defined(TARGET_EMSCRIPTEN)
    return OF_TARGET_EMSCRIPTEN;
#endif
}

std::string ofGetEnv(const std::string & var, const std::string defaultValue){
#ifdef TARGET_WIN32
	const size_t BUFSIZE = 4096;
	std::vector<char> pszOldVal(BUFSIZE, 0);
	auto size = GetEnvironmentVariableA(var.c_str(), pszOldVal.data(), BUFSIZE);
	if(size>0){
		return std::string(pszOldVal.begin(), pszOldVal.begin()+size);
	}else{
		return defaultValue;
	}
#else
	auto value = getenv(var.c_str());
	if(value){
		return value;
	}else{
		return defaultValue;
	}
#endif
}
