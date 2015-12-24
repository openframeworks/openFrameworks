#include "ofUtils.h"
#include "ofImage.h"
#include "ofFileUtils.h"

#include <chrono>
#include <numeric>
#include <locale>

#if !defined(TARGET_EMSCRIPTEN)
#include "Poco/URI.h"
#endif


#ifdef TARGET_WIN32
    #ifndef _MSC_VER
        #include <unistd.h> // this if for MINGW / _getcwd
	#include <sys/param.h> // for MAXPATHLEN
    #endif
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

#ifdef TARGET_WIN32
    #include <mmsystem.h>
	#ifdef _MSC_VER
		#include <direct.h>
	#endif

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

namespace{
    bool enableDataPath = true;
    uint64_t startTimeSeconds;   //  better at the first frame ?? (currently, there is some delay from static init, to running.
    uint64_t startTimeNanos;
#ifdef TARGET_OSX
    clock_serv_t cs;
#endif

    //--------------------------------------------------
    string defaultDataPath(){
    #if defined TARGET_OSX
        try{
            return std::filesystem::canonical(ofFilePath::join(ofFilePath::getCurrentExeDir(),  "../../../data/")).string();
        }catch(...){
            return ofFilePath::join(ofFilePath::getCurrentExeDir(),  "../../../data/");
        }
    #elif defined TARGET_ANDROID
            return string("sdcard/");
    #else
            try{
                return std::filesystem::canonical(ofFilePath::join(ofFilePath::getCurrentExeDir(),  "data/")).string();
            }catch(...){
                return ofFilePath::join(ofFilePath::getCurrentExeDir(),  "data/");
            }
    #endif
    }

    //--------------------------------------------------
    std::filesystem::path & defaultWorkingDirectory(){
            static auto * defaultWorkingDirectory = new std::filesystem::path();
            return * defaultWorkingDirectory;
    }

    //--------------------------------------------------
    std::filesystem::path & dataPathRoot(){
            static auto * dataPathRoot = new std::filesystem::path(defaultDataPath());
            return *dataPathRoot;
    }
}

namespace of{
namespace priv{
    void initutils(){
#ifdef TARGET_OSX
        host_get_clock_service(mach_host_self(), SYSTEM_CLOCK, &cs);
#endif
        defaultWorkingDirectory() = std::filesystem::absolute(std::filesystem::current_path());
        ofResetElapsedTimeCounter();
        ofSeedRandom();
    }

    void endutils(){
#ifdef TARGET_OSX
        mach_port_deallocate(mach_task_self(), cs);
#endif
    }
}
}

//--------------------------------------
void ofGetMonotonicTime(uint64_t & seconds, uint64_t & nanoseconds){
#if (defined(TARGET_LINUX) && !defined(TARGET_RASPBERRY_PI)) || defined(TARGET_EMSCRIPTEN)
	struct timespec now;
	clock_gettime(CLOCK_MONOTONIC, &now);
	seconds = now.tv_sec;
	nanoseconds = now.tv_nsec;
#elif defined(TARGET_OSX)
        mach_timespec_t now;
        clock_get_time(cs, &now);
	seconds = now.tv_sec;
	nanoseconds = now.tv_nsec;
#elif defined( TARGET_WIN32 )
	LARGE_INTEGER freq;
	LARGE_INTEGER counter;
	QueryPerformanceFrequency(&freq);
	QueryPerformanceCounter(&counter);
	seconds = counter.QuadPart/freq.QuadPart;
	nanoseconds = (counter.QuadPart % freq.QuadPart)*1000000000/freq.QuadPart;
#else
	struct timeval now;
	gettimeofday( &now, nullptr );
	seconds = now.tv_sec;
	nanoseconds = now.tv_usec * 1000;
#endif
}


//--------------------------------------
uint64_t ofGetElapsedTimeMillis(){
    uint64_t seconds;
    uint64_t nanos;
	ofGetMonotonicTime(seconds,nanos);
	return (seconds - startTimeSeconds)*1000 + ((long long)(nanos - startTimeNanos))/1000000;
}

//--------------------------------------
uint64_t ofGetElapsedTimeMicros(){
    uint64_t seconds;
    uint64_t nanos;
	ofGetMonotonicTime(seconds,nanos);
	return (seconds - startTimeSeconds)*1000000 + ((long long)(nanos - startTimeNanos))/1000;
}

//--------------------------------------
float ofGetElapsedTimef(){
    uint64_t seconds;
    uint64_t nanos;
	ofGetMonotonicTime(seconds,nanos);
	return (seconds - startTimeSeconds) + ((long long)(nanos - startTimeNanos))/1000000000.;
}

//--------------------------------------
void ofResetElapsedTimeCounter(){
	ofGetMonotonicTime(startTimeSeconds,startTimeNanos);
}

//=======================================
// this is from freeglut, and used internally:
/* Platform-dependent time in milliseconds, as an unsigned 32-bit integer.
 * This value wraps every 49.7 days, but integer overflows cancel
 * when subtracting an initial start time, unless the total time exceeds
 * 32-bit, where the GLUT API return value is also overflowed.
 */
uint64_t ofGetSystemTime( ) {
	uint64_t seconds, nanoseconds;
	ofGetMonotonicTime(seconds,nanoseconds);
	return seconds * 1000 + nanoseconds / 1000000;
}

uint64_t ofGetSystemTimeMicros( ) {
    uint64_t seconds, nanoseconds;
	ofGetMonotonicTime(seconds,nanoseconds);
	return seconds * 1000000 + nanoseconds / 1000;
}

//--------------------------------------------------
unsigned int ofGetUnixTime(){
	return (unsigned int)time(nullptr);
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

//--------------------------------------------------
void ofEnableDataPath(){
	enableDataPath = true;
}

//--------------------------------------------------
void ofDisableDataPath(){
	enableDataPath = false;
}

//--------------------------------------------------
bool ofRestoreWorkingDirectoryToDefault(){
    try{
        std::filesystem::current_path(defaultWorkingDirectory());
        return true;
    }catch(...){
        return false;
    }
}

//--------------------------------------------------
void ofSetDataPathRoot(const string& newRoot){
	dataPathRoot() = newRoot;
}

//--------------------------------------------------
string ofToDataPath(const string& path, bool makeAbsolute){
	if (!enableDataPath)
		return path;

    bool hasTrailingSlash = !path.empty() && std::filesystem::path(path).generic_string().back()=='/';

	// if our Current Working Directory has changed (e.g. file open dialog)
#ifdef TARGET_WIN32
	if (defaultWorkingDirectory() != std::filesystem::current_path()) {
		// change our cwd back to where it was on app load
		bool ret = ofRestoreWorkingDirectoryToDefault();
		if(!ret){
			ofLogWarning("ofUtils") << "ofToDataPath: error while trying to change back to default working directory " << defaultWorkingDirectory();
		}
	}
#endif

	// this could be performed here, or wherever we might think we accidentally change the cwd, e.g. after file dialogs on windows
	const auto  & dataPath = dataPathRoot();
	std::filesystem::path inputPath(path);
	std::filesystem::path outputPath;

	// if path is already absolute, just return it
	if (inputPath.is_absolute()) {
		try {
            auto outpath = std::filesystem::canonical(inputPath);
            if(std::filesystem::is_directory(outpath) && hasTrailingSlash){
                return ofFilePath::addTrailingSlash(outpath.string());
            }else{
                return outpath.string();
            }
		}
		catch (...) {
            return inputPath.string();
		}
	}

	// here we check whether path already refers to the data folder by looking for common elements
	// if the path begins with the full contents of dataPathRoot then the data path has already been added
	// we compare inputPath.toString() rather that the input var path to ensure common formatting against dataPath.toString()
    auto dirDataPath = dataPath.string();
	// also, we strip the trailing slash from dataPath since `path` may be input as a file formatted path even if it is a folder (i.e. missing trailing slash)
    dirDataPath = ofFilePath::addTrailingSlash(dirDataPath);

    auto relativeDirDataPath = ofFilePath::makeRelative(std::filesystem::current_path().string(),dataPath.string());
    relativeDirDataPath  = ofFilePath::addTrailingSlash(relativeDirDataPath);

    if (inputPath.string().find(dirDataPath) != 0 && inputPath.string().find(relativeDirDataPath)!=0) {
		// inputPath doesn't contain data path already, so we build the output path as the inputPath relative to the dataPath
	    if(makeAbsolute){
            outputPath = dirDataPath / inputPath;
	    }else{
            outputPath = relativeDirDataPath / inputPath;
	    }
	} else {
		// inputPath already contains data path, so no need to change
		outputPath = inputPath;
	}

    // finally, if we do want an absolute path and we don't already have one
	if(makeAbsolute){
	    // then we return the absolute form of the path
	    try {
            auto outpath = std::filesystem::canonical(std::filesystem::absolute(outputPath));
            if(std::filesystem::is_directory(outpath) && hasTrailingSlash){
                return ofFilePath::addTrailingSlash(outpath.string());
            }else{
                return outpath.string();
            }
	    }
	    catch (std::exception &) {
            return std::filesystem::absolute(outputPath).string();
	    }
	}else{
		// or output the relative path
        return outputPath.string();
	}
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
	ostringstream out;
	// how many bytes are in the string
	std::size_t numBytes = value.size();
	for(std::size_t i = 0; i < numBytes; i++) {
		// print each byte as a 2-character wide hex value
		out << setfill('0') << setw(2) << hex << (unsigned int) ((unsigned char)value[i]);
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
	int x = 0;
	istringstream cur(intString);
	cur >> x;
	return x;
}

//----------------------------------------
int ofHexToInt(const string& intHexString) {
	int x = 0;
	istringstream cur(intHexString);
	cur >> hex >> x;
	return x;
}

//----------------------------------------
char ofHexToChar(const string& charHexString) {
	int x = 0;
	istringstream cur(charHexString);
	cur >> hex >> x;
	return (char) x;
}

//----------------------------------------
float ofHexToFloat(const string& floatHexString) {
	union intFloatUnion {
		int x;
		float f;
	} myUnion;
	myUnion.x = 0;
	istringstream cur(floatHexString);
	cur >> hex >> myUnion.x;
	return myUnion.f;
}

//----------------------------------------
string ofHexToString(const string& stringHexString) {
	stringstream out;
	stringstream stream(stringHexString);
	// a hex string has two characters per byte
	std::size_t numBytes = stringHexString.size() / 2;
	for(std::size_t i = 0; i < numBytes; i++) {
		string curByte;
		// grab two characters from the hex string
		stream >> setw(2) >> curByte;
		// prepare to parse the two characters
		stringstream curByteStream(curByte);
		int cur = 0;
		// parse the two characters as a hex-encoded int
		curByteStream >> hex >> cur;
		// add the int as a char to our output stream
		out << (char) cur;
	}
	return out.str();
}

//----------------------------------------
float ofToFloat(const string& floatString) {
	float x = 0;
	istringstream cur(floatString);
	cur >> x;
	return x;
}

//----------------------------------------
double ofToDouble(const string& doubleString) {
	double x = 0;
	istringstream cur(doubleString);
	cur >> x;
	return x;
}

//----------------------------------------
int64_t ofToInt64(const string& intString) {
	int64_t x = 0;
	istringstream cur(intString);
	cur >> x;
	return x;
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
	istringstream cur(lower);
	cur >> x;
	return x;
}

//----------------------------------------
char ofToChar(const string& charString) {
	char x = '\0';
	istringstream cur(charString);
	cur >> x;
	return x;
}

//----------------------------------------
template <> string ofToBinary(const string& value) {
	stringstream out;
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
	ostringstream out;
	stringstream stream(value);
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
	try {
		loc = std::locale(locale.c_str());
	}
	catch (...) {
		ofLogWarning("ofUtils") << "Couldn't create locale " << locale << " using default, " << loc.name();
	}
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
void ofAppendUTF8(string & str, int utf8){
	try{
		utf8::append(utf8, back_inserter(str));
	}catch(...){}
}

//--------------------------------------------------
string ofVAArgsToString(const char * format, ...){
	// variadic args to string:
	// http://www.codeproject.com/KB/string/string_format.aspx
	char aux_buffer[10000];
	string retStr("");
	if (nullptr != format){

		va_list marker;

		// initialize variable arguments
		va_start(marker, format);

		// Get formatted string length adding one for nullptr
		size_t len = vsprintf(aux_buffer, format, marker) + 1;

		// Reset variable arguments
		va_end(marker);

		if (len > 0)
		{
			va_list args;

			// initialize variable arguments
			va_start(args, format);

			// Create a char vector to hold the formatted string.
			vector<char> buffer(len, '\0');
			vsprintf(&buffer[0], format, args);
			retStr = &buffer[0];
			va_end(args);
		}

	}
	return retStr;
}

string ofVAArgsToString(const char * format, va_list args){
	// variadic args to string:
	// http://www.codeproject.com/KB/string/string_format.aspx
	char aux_buffer[10000];
	string retStr("");
	if (nullptr != format){

		// Get formatted string length adding one for nullptr
		vsprintf(aux_buffer, format, args);
		retStr = aux_buffer;

	}
	return retStr;
}

#ifndef TARGET_EMSCRIPTEN
//--------------------------------------------------
void ofLaunchBrowser(const string& url, bool uriEncodeQuery){
	Poco::URI uri;
	try {
		uri = Poco::URI(url);
	} catch(const std::exception & exc) {
		ofLogError("ofUtils") << "ofLaunchBrowser(): malformed url \"" << url << "\": " << exc.what();
		return;
	}

	if(uriEncodeQuery) {
		uri.setQuery(uri.getRawQuery()); // URI encodes during set
	}

	// http://support.microsoft.com/kb/224816
	// make sure it is a properly formatted url:
	//   some platforms, like Android, require urls to start with lower-case http/https
	//   Poco::URI automatically converts the scheme to lower case
	if(uri.getScheme() != "http" && uri.getScheme() != "https"){
		ofLogError("ofUtils") << "ofLaunchBrowser(): url does not begin with http:// or https://: \"" << uri.toString() << "\"";
		return;
	}

	#ifdef TARGET_WIN32
		ShellExecuteA(nullptr, "open", uri.toString().c_str(),
                nullptr, nullptr, SW_SHOWNORMAL);
	#endif

	#ifdef TARGET_OSX
        // could also do with LSOpenCFURLRef
		string commandStr = "open \"" + uri.toString() + "\"";
		int ret = system(commandStr.c_str());
        if(ret!=0) {
			ofLogError("ofUtils") << "ofLaunchBrowser(): couldn't open browser, commandStr \"" << commandStr << "\"";
		}
	#endif

	#ifdef TARGET_LINUX
		string commandStr = "xdg-open \"" + uri.toString() + "\"";
		int ret = system(commandStr.c_str());
		if(ret!=0) {
			ofLogError("ofUtils") << "ofLaunchBrowser(): couldn't open browser, commandStr \"" << commandStr << "\"";
		}
	#endif

	#ifdef TARGET_OF_IOS
		ofxiOSLaunchBrowser(uri.toString());
	#endif

	#ifdef TARGET_ANDROID
		ofxAndroidLaunchBrowser(uri.toString());
	#endif
}
#endif

//--------------------------------------------------
string ofGetVersionInfo(){
	stringstream sstr;
	sstr << OF_VERSION_MAJOR << "." << OF_VERSION_MINOR << "." << OF_VERSION_PATCH;

	if (!std::string(OF_VERSION_PRE_RELEASE).empty())
	{
		sstr << "-" << OF_VERSION_PRE_RELEASE;
	}

	sstr << std::endl;
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

std::string ofGetEnv(const std::string & var){
#ifdef TARGET_WIN32
	const size_t BUFSIZE = 4096;
	std::vector<char> pszOldVal(BUFSIZE, 0);
	auto size = GetEnvironmentVariableA(var.c_str(), pszOldVal.data(), BUFSIZE);
	if(size>0){
		return std::string(pszOldVal.begin(), pszOldVal.begin()+size);
	}else{
		return "";
	}
#else
	auto value = getenv(var.c_str());
	if(value){
		return value;
	}else{
		return "";
	}
#endif
}
