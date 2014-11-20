#include "ofUtils.h"
#include "ofImage.h"
#include "ofTypes.h"
#include "ofGraphics.h"
#include "ofAppRunner.h"

#include "Poco/String.h"
#include "Poco/UTF8String.h"
#include "Poco/LocalDateTime.h"
#include "Poco/DateTimeFormatter.h"
#include "Poco/URI.h"

#include <cctype> // for toupper



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

static bool enableDataPath = true;
static unsigned long long startTimeSeconds;   //  better at the first frame ?? (currently, there is some delay from static init, to running.
static unsigned long long startTimeNanos;


//--------------------------------------
void ofGetMonotonicTime(unsigned long long & seconds, unsigned long long & nanoseconds){
#if (defined(TARGET_LINUX) && !defined(TARGET_RASPBERRY_PI)) || defined(TARGET_EMSCRIPTEN)
	struct timespec now;
	clock_gettime(CLOCK_MONOTONIC, &now);
	seconds = now.tv_sec;
	nanoseconds = now.tv_nsec;
#elif defined(TARGET_OSX)
	clock_serv_t cs;
	mach_timespec_t now;
	host_get_clock_service(mach_host_self(), SYSTEM_CLOCK, &cs);
	clock_get_time(cs, &now);
	mach_port_deallocate(mach_task_self(), cs);
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
	gettimeofday( &now, NULL );
	seconds = now.tv_sec;
	nanoseconds = now.tv_usec * 1000;
#endif
}


//--------------------------------------
unsigned long long ofGetElapsedTimeMillis(){
	unsigned long long seconds;
	unsigned long long nanos;
	ofGetMonotonicTime(seconds,nanos);
	return (seconds - startTimeSeconds)*1000 + ((long long)(nanos - startTimeNanos))/1000000;
}

//--------------------------------------
unsigned long long ofGetElapsedTimeMicros(){
	unsigned long long seconds;
	unsigned long long nanos;
	ofGetMonotonicTime(seconds,nanos);
	return (seconds - startTimeSeconds)*1000000 + ((long long)(nanos - startTimeNanos))/1000;
}

//--------------------------------------
float ofGetElapsedTimef(){
	unsigned long long seconds;
	unsigned long long nanos;
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
unsigned long long ofGetSystemTime( ) {
	unsigned long long seconds, nanoseconds;
	ofGetMonotonicTime(seconds,nanoseconds);
	return seconds * 1000 + nanoseconds / 1000000;
}

unsigned long long ofGetSystemTimeMicros( ) {
	unsigned long long seconds, nanoseconds;
	ofGetMonotonicTime(seconds,nanoseconds);
	return seconds * 1000000 + nanoseconds / 1000;
}

//--------------------------------------------------
unsigned int ofGetUnixTime(){
	return (unsigned int)time(NULL);
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
	Poco::LocalDateTime now;
	return Poco::DateTimeFormatter::format(now, timeFormat);
}

//specify the string format - eg: %Y-%m-%d-%H-%M-%S-%i ( 2011-01-15-18-29-35-299 )
//--------------------------------------------------
string ofGetTimestampString(const string& timestampFormat){
	Poco::LocalDateTime now;
	return Poco::DateTimeFormatter::format(now, timestampFormat);
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
string defaultDataPath(){
#if defined TARGET_OSX
	return string("../../../data/");
#elif defined TARGET_ANDROID
	return string("sdcard/");
#elif defined(TARGET_LINUX) || defined(TARGET_WIN32)
	return string(ofFilePath::join(ofFilePath::getCurrentExeDir(),  "data/"));
#else
	return string("data/");
#endif
}

//--------------------------------------------------
static Poco::Path & defaultWorkingDirectory(){
	static Poco::Path * defaultWorkingDirectory = new Poco::Path();
	return * defaultWorkingDirectory;
}

//--------------------------------------------------
static Poco::Path & dataPathRoot(){
	static Poco::Path * dataPathRoot = new Poco::Path(defaultDataPath());
	return *dataPathRoot;
}

//--------------------------------------------------
Poco::Path getWorkingDir(){
#ifndef TARGET_EMSCRIPTEN
	char charWorkingDir[MAXPATHLEN];
	char* ret = getcwd(charWorkingDir, MAXPATHLEN);
	if(ret)
		return Poco::Path(charWorkingDir);
	else
		return Poco::Path();
#else
	return Poco::Path();
#endif
}

//--------------------------------------------------
void ofSetWorkingDirectoryToDefault(){
#ifdef TARGET_OSX
	#ifndef TARGET_OF_IOS
		string newPath = "";
		char path[MAXPATHLEN];
		uint32_t size = sizeof(path);
		
		if (_NSGetExecutablePath(path, &size) == 0){
			Poco::Path classPath(path);
			classPath.makeParent();
			chdir( classPath.toString().c_str() );
		}else{
			ofLogFatalError("ofUtils") << "ofSetDataPathRoot(): path buffer too small, need size " << (unsigned int) size;
		}
	#endif
#endif

	defaultWorkingDirectory() = getWorkingDir();
#ifndef TARGET_EMSCRIPTEN
	defaultWorkingDirectory().makeAbsolute();
#endif
}
	
//--------------------------------------------------
void ofSetDataPathRoot(const string& newRoot){
	dataPathRoot() = Poco::Path(newRoot);
}

//--------------------------------------------------
string ofToDataPath(const string& path, bool makeAbsolute){
	if (!enableDataPath)
		return path;
	
	// if our Current Working Directory has changed (e.g. file open dialog)
#ifdef TARGET_WIN32
	if (defaultWorkingDirectory().toString() != getWorkingDir().toString()) {
		// change our cwd back to where it was on app load
		int ret = chdir(defaultWorkingDirectory().toString().c_str());
		if(ret==-1){
			ofLogWarning("ofUtils") << "ofToDataPath: error while trying to change back to default working directory " << defaultWorkingDirectory().toString();
		}
	}
#endif
	// this could be performed here, or wherever we might think we accidentally change the cwd, e.g. after file dialogs on windows
	
	Poco::Path const  & dataPath(dataPathRoot());
	Poco::Path inputPath(path);
	Poco::Path outputPath;
	
	// if path is already absolute, just return it
	if (inputPath.isAbsolute()) {
		return path;
	}
	
	// here we check whether path already refers to the data folder by looking for common elements
	// if the path begins with the full contents of dataPathRoot then the data path has already been added
	// we compare inputPath.toString() rather that the input var path to ensure common formatting against dataPath.toString()
	string strippedDataPath = dataPath.toString();
	// also, we strip the trailing slash from dataPath since `path` may be input as a file formatted path even if it is a folder (i.e. missing trailing slash)
	strippedDataPath = ofFilePath::removeTrailingSlash(strippedDataPath);
	
	if (inputPath.toString().find(strippedDataPath) != 0) {
		// inputPath doesn't contain data path already, so we build the output path as the inputPath relative to the dataPath
		outputPath = dataPath;
		outputPath.resolve(inputPath);
	} else {
		// inputPath already contains data path, so no need to change
		outputPath = inputPath;
	}
	
	// finally, if we do want an absolute path and we don't already have one
	if (makeAbsolute) {
		// then we return the absolute form of the path
		return outputPath.absolute().toString();
	} else {
		// or output the relative path
		return outputPath.toString();
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
	int numBytes = value.size();
	for(int i = 0; i < numBytes; i++) {
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
	int numBytes = stringHexString.size() / 2;
	for(int i = 0; i < numBytes; i++) {
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
bool ofToBool(const string& boolString) {
	static const string trueString = "true";
	static const string falseString = "false";
	string lower = Poco::toLower(boolString);
	if(lower == trueString) {
		return true;
	}
	if(lower == falseString) {
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
	int numBytes = value.size();
	for(int i = 0; i < numBytes; i++) {
		bitset<8> bitBuffer(value[i]);
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
	bitset<intSize> binaryString(value);
	return (int) binaryString.to_ulong();
}

//----------------------------------------
char ofBinaryToChar(const string& value) {
	const int charSize = sizeof(char) * 8;
	bitset<charSize> binaryString(value);
	return (char) binaryString.to_ulong();
}

//----------------------------------------
float ofBinaryToFloat(const string& value) {
	const int floatSize = sizeof(float) * 8;
	bitset<floatSize> binaryString(value);
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
	bitset<8> byteString;
	int numBytes = value.size() / 8;
	for(int i = 0; i < numBytes; i++) {
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
			Poco::trimInPlace(sub);
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
    return Poco::cat(delimiter, stringElements.begin(), stringElements.end());
}

//--------------------------------------------------
void ofStringReplace(string& input, const string& searchStr, const string& replaceStr){
    input = Poco::replace(input, searchStr, replaceStr);
}

//--------------------------------------------------
bool ofIsStringInString(const string& haystack, const string& needle){
    return haystack.find(needle) != std::string::npos;
}

//--------------------------------------------------
int ofStringTimesInString(const string& haystack, const string& needle){
	const size_t step = needle.size();

	size_t count(0);
	size_t pos(0) ;

	while( (pos=haystack.find(needle, pos)) != std::string::npos) {
		pos +=step;
		++count ;
	}

	return count;
}

//--------------------------------------------------
string ofToLower(const string & src){
    return Poco::UTF8::toLower(src);
}

//--------------------------------------------------
string ofToUpper(const string & src){
    return Poco::UTF8::toUpper(src);
}

//--------------------------------------------------
string ofVAArgsToString(const char * format, ...){
	// variadic args to string:
	// http://www.codeproject.com/KB/string/string_format.aspx
	static char aux_buffer[10000];
	string retStr("");
	if (NULL != format){

		va_list marker;

		// initialize variable arguments
		va_start(marker, format);

		// Get formatted string length adding one for NULL
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
	if (NULL != format){

		// Get formatted string length adding one for NULL
		vsprintf(aux_buffer, format, args);
		retStr = aux_buffer;

	}
	return retStr;
}

//--------------------------------------------------
void ofLaunchBrowser(const string& _url, bool uriEncodeQuery){

    Poco::URI uri;
    
    try {
        uri = Poco::URI(_url);
    } catch(const Poco::SyntaxException& exc) {
        ofLogError("ofUtils") << "ofLaunchBrowser(): malformed url \"" << _url << "\": " << exc.displayText();
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
		#if (_MSC_VER)
		// microsoft visual studio yaks about strings, wide chars, unicode, etc
		ShellExecuteA(NULL, "open", uri.toString().c_str(),
                NULL, NULL, SW_SHOWNORMAL);
		#else
		ShellExecute(NULL, "open", uri.toString().c_str(),
                NULL, NULL, SW_SHOWNORMAL);
		#endif
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

//--------------------------------------------------
string ofGetVersionInfo(){
	stringstream sstr;
	sstr << OF_VERSION_MAJOR << "." << OF_VERSION_MINOR << "." << OF_VERSION_PATCH << endl;
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

//---- new to 006
//from the forums http://www.openframeworks.cc/forum/viewtopic.php?t=1413

//--------------------------------------------------
void ofSaveScreen(const string& filename) {
   ofImage screen;
   screen.allocate(ofGetWidth(), ofGetHeight(), OF_IMAGE_COLOR);
   screen.grabScreen(0, 0, ofGetWidth(), ofGetHeight());
   screen.save(filename);
}

//--------------------------------------------------
void ofSaveViewport(const string& filename) {
	// because ofSaveScreen doesn't related to viewports
	ofImage screen;
	ofRectangle view = ofGetCurrentViewport();
	screen.allocate(view.width, view.height, OF_IMAGE_COLOR);
	screen.grabScreen(0, 0, view.width, view.height);
	screen.save(filename);
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
	FILE * ret = NULL;
#ifdef TARGET_WIN32
	ret = _popen(command.c_str(),"r");
#else 
	ret = popen(command.c_str(),"r");
#endif
	
	string strret;
	int c;

	if (ret == NULL){
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
        return OF_TARGET_WINGCC;
    #endif
#elif defined(TARGET_ANDROID)
    return OF_TARGET_ANDROID;
#elif defined(TARGET_OF_IOS)
    return OF_TARGET_IOS;
#elif defined(TARGET_EMSCRIPTEN)
    return OF_TARGET_EMSCRIPTEN;
#endif
}
