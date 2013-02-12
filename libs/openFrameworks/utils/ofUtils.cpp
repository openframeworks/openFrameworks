#include "ofUtils.h"
#include "ofImage.h"
#include "ofTypes.h"
#include "ofGraphics.h"
#include "ofAppRunner.h"

#include "Poco/String.h"
#include "Poco/LocalDateTime.h"
#include "Poco/DateTimeFormatter.h"

#include <cctype> // for toupper



#ifdef TARGET_WIN32
    #ifndef _MSC_VER
        #include <unistd.h> // this if for MINGW / _getcwd
	#include <sys/param.h> // for MAXPATHLEN
    #endif
#endif


#if defined(TARGET_OF_IPHONE) || defined(TARGET_OSX ) || defined(TARGET_LINUX)
	#include <sys/time.h>
#endif

#ifdef TARGET_OSX
	#ifndef TARGET_OF_IPHONE
		#include <mach-o/dyld.h>
		#include <sys/param.h> // for MAXPATHLEN
	#endif
#endif

#ifdef TARGET_WIN32
    #include <mmsystem.h>
	#ifdef _MSC_VER
		#include <direct.h>
	#endif

#endif

#ifdef TARGET_OF_IPHONE
#include "ofxiPhoneExtras.h"
#endif

#ifdef TARGET_ANDROID
#include "ofxAndroidUtils.h"
#endif

#ifndef MAXPATHLEN
	#define MAXPATHLEN 1024
#endif

static bool enableDataPath = true;
static unsigned long long startTime = ofGetSystemTime();   //  better at the first frame ?? (currently, there is some delay from static init, to running.
static unsigned long long startTimeMicros = ofGetSystemTimeMicros();

//--------------------------------------
unsigned long long ofGetElapsedTimeMillis(){
	return ofGetSystemTime() - startTime;
}

//--------------------------------------
unsigned long long ofGetElapsedTimeMicros(){
	return ofGetSystemTimeMicros() - startTimeMicros;
}

//--------------------------------------
float ofGetElapsedTimef(){
	return ofGetElapsedTimeMicros() / 1000000.0f;
}

//--------------------------------------
void ofResetElapsedTimeCounter(){
	startTime = ofGetSystemTime();
	startTimeMicros = ofGetSystemTimeMicros();
}

//=======================================
// this is from freeglut, and used internally:
/* Platform-dependent time in milliseconds, as an unsigned 32-bit integer.
 * This value wraps every 49.7 days, but integer overflows cancel
 * when subtracting an initial start time, unless the total time exceeds
 * 32-bit, where the GLUT API return value is also overflowed.
 */
unsigned long long ofGetSystemTime( ) {
	#ifndef TARGET_WIN32
		struct timeval now;
		gettimeofday( &now, NULL );
		return 
			(unsigned long long) now.tv_usec/1000 + 
			(unsigned long long) now.tv_sec*1000;
	#else
		#if defined(_WIN32_WCE)
			return GetTickCount();
		#else
			return timeGetTime();
		#endif
	#endif
}

unsigned long long ofGetSystemTimeMicros( ) {
	#ifndef TARGET_WIN32
		struct timeval now;
		gettimeofday( &now, NULL );
		return 
			(unsigned long long) now.tv_usec +
			(unsigned long long) now.tv_sec*1000000;
	#else
		#if defined(_WIN32_WCE)
			return ((unsigned long long)GetTickCount()) * 1000;
		#else
			return ((unsigned long long)timeGetTime()) * 1000;
		#endif
	#endif
}

//--------------------------------------------------
unsigned int ofGetUnixTime(){
	return (unsigned int)time(NULL);
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
string ofGetTimestampString(string timestampFormat){
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
//use ofSetDataPathRoot() to override this
static string & dataPathRoot(){
#if defined TARGET_OSX
	static string * dataPathRoot = new string("../../../data/");
#elif defined TARGET_ANDROID
	static string * dataPathRoot = new string("sdcard/");
#elif defined(TARGET_LINUX)
	static string * dataPathRoot = new string(ofFilePath::join(ofFilePath::getCurrentExeDir(),  "data/"));
#else
	static string * dataPathRoot = new string("data/");
#endif
	return *dataPathRoot;
}

static bool & isDataPathSet(){
	static bool * dataPathSet = new bool(false);
	return * dataPathSet;
}

//--------------------------------------------------
void ofSetDataPathRoot(string newRoot){
	string newPath = "";

	#ifdef TARGET_OSX
		#ifndef TARGET_OF_IPHONE
			char path[MAXPATHLEN];
			uint32_t size = sizeof(path);

			if (_NSGetExecutablePath(path, &size) == 0){
				//printf("executable path is %s\n", path);
				string pathStr = string(path);

				//theo: check this with having '/' as a character in a folder name - OSX treats the '/' as a ':'
				//checked with spaces too!

				vector < string> pathBrokenUp = ofSplitString( pathStr, "/");

				newPath = "";

				for(int i = 0; i < pathBrokenUp.size()-1; i++){
					newPath += pathBrokenUp[i];
					newPath += "/";
				}

				//cout << newPath << endl;   // some sanity checks here
				//system( "pwd" );

				chdir ( newPath.c_str() );
				//system("pwd");
			}else{
				ofLog(OF_LOG_FATAL_ERROR, "buffer too small; need size %u\n", size);
			}
		#endif
	#endif
	
	dataPathRoot() = newRoot;
	isDataPathSet() = true;
}

//--------------------------------------------------
string ofToDataPath(string path, bool makeAbsolute){
	
	if (!isDataPathSet())
		ofSetDataPathRoot(dataPathRoot());
	
	if( enableDataPath ){

        //we create dataPath as a string for the check, on windows we modify it to check both types of slashes
        //however we use the original value from dataPathRoot() to prepend the string if needed.  
        string dataPath = dataPathRoot(); 
        string enclosingFolder = path.substr(0,dataPath.length());
        
        #ifdef TARGET_WIN32
            //this is so we can check both "data\" and "data/" on windows
            std::replace( enclosingFolder.begin(), enclosingFolder.end(), '\\', '/' );
            std::replace( dataPath.begin(), dataPath.end(), '\\', '/' );
        #endif // TARGET_WIN32

		//check if absolute path has been passed or if data path has already been applied
		//do we want to check for C: D: etc ?? like  substr(1, 2) == ':' ??
		if( path.length()==0 || (path.substr(0,1) != "/" &&  path.substr(1,1) != ":" && enclosingFolder != dataPath)){
			path = dataPathRoot()+path;
		}

		if(makeAbsolute && (path.length()==0 || path.substr(0,1) != "/")){
			#if !defined( TARGET_OF_IPHONE) & !defined(TARGET_ANDROID)

			#ifndef TARGET_WIN32
				char currDir[1024];
				path = "/"+path;
                path = getcwd(currDir, 1024)+path;

			#else

				char currDir[1024];
				path = "\\"+path;
				path = _getcwd(currDir, 1024)+path;
				std::replace( path.begin(), path.end(), '/', '\\' ); // fix any unixy paths...


			#endif


			#else
				//do we need iphone specific code here?
			#endif
		}

	}
	return path;
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
string ofJoinString(vector <string> stringElements, const string & delimiter){
	string resultString = "";
	int numElements = stringElements.size();

	for(int k = 0; k < numElements; k++){
		if( k < numElements-1 ){
			resultString += stringElements[k] + delimiter;
		} else {
			resultString += stringElements[k];
		}
	}

	return resultString;
}

//--------------------------------------------------
void ofStringReplace(string& input, string searchStr, string replaceStr){
	size_t uPos = 0; 
	size_t uFindLen = searchStr.length(); 
	size_t uReplaceLen = replaceStr.length();
		
	if( uFindLen == 0 ){
		return;
	}

	for( ;(uPos = input.find( searchStr, uPos )) != std::string::npos; ){
		input.replace( uPos, uFindLen, replaceStr );
		uPos += uReplaceLen;
	}	
}

//--------------------------------------------------
bool ofIsStringInString(string haystack, string needle){
	return ( strstr(haystack.c_str(), needle.c_str() ) != NULL );
}

//--------------------------------------------------
string ofToLower(const string & src){
	string dst(src);
	transform(src.begin(),src.end(),dst.begin(),::tolower);
	return dst;
}

//--------------------------------------------------
string ofToUpper(const string & src){
	string dst(src);
	transform(src.begin(),src.end(),dst.begin(),::toupper);
	return dst;
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
void ofLaunchBrowser(string url){

	// http://support.microsoft.com/kb/224816

	// make sure it is a properly formatted url:
	//   some platforms, like Android, require urls to start with lower-case http/https
	if(Poco::icompare(url.substr(0,8), "https://") == 0){
		url.replace(0,5,"https");
	}
	else if(Poco::icompare(url.substr(0,7), "http://") == 0){
		url.replace(0,4,"http");
	}
	else{
		ofLog(OF_LOG_WARNING, "ofLaunchBrowser: url must begin with http:// or https://");
		return;
	}

	#ifdef TARGET_WIN32
		#if (_MSC_VER)
		// microsoft visual studio yaks about strings, wide chars, unicode, etc
		ShellExecuteA(NULL, "open", url.c_str(),
                NULL, NULL, SW_SHOWNORMAL);
		#else
		ShellExecute(NULL, "open", url.c_str(),
                NULL, NULL, SW_SHOWNORMAL);
		#endif
	#endif

	#ifdef TARGET_OSX
		// ok gotta be a better way then this,
		// this is what I found...
		string commandStr = "open "+url;
		system(commandStr.c_str());
	#endif

	#ifdef TARGET_LINUX
		string commandStr = "xdg-open "+url;
		int ret = system(commandStr.c_str());
		if(ret!=0) ofLog(OF_LOG_ERROR,"ofLaunchBrowser: couldn't open browser");
	#endif

	#ifdef TARGET_OF_IPHONE
		ofxiPhoneLaunchBrowser(url);
	#endif

	#ifdef TARGET_ANDROID
		ofxAndroidLaunchBrowser(url);
	#endif
}

//--------------------------------------------------
string ofGetVersionInfo(){
	stringstream sstr;
	sstr << OF_VERSION_MAJOR << "." << OF_VERSION_MINOR << "." << OF_VERSION_PATCH << endl;
	return sstr.str();
}

//---- new to 006
//from the forums http://www.openframeworks.cc/forum/viewtopic.php?t=1413

//--------------------------------------------------
void ofSaveScreen(string filename) {
   ofImage screen;
   screen.allocate(ofGetWidth(), ofGetHeight(), OF_IMAGE_COLOR);
   screen.grabScreen(0, 0, ofGetWidth(), ofGetHeight());
   screen.saveImage(filename);
}

//--------------------------------------------------
void ofSaveViewport(string filename) {
	// because ofSaveScreen doesn't related to viewports
	ofImage screen;
	ofRectangle view = ofGetCurrentViewport();
	screen.allocate(view.width, view.height, OF_IMAGE_COLOR);
	screen.grabScreen(0, 0, view.width, view.height);
	screen.saveImage(filename);
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
string ofSystem(string command){
	FILE * ret = NULL;
#ifdef TARGET_WIN32
	 ret = _popen(command.c_str(),"r");
#else 
	ret = popen(command.c_str(),"r");
#endif
	
	string strret;
	char c;

	if (ret == NULL){
		ofLogError() << "ofSystem: error opening return file";
	}else{
		do {
		      c = fgetc (ret);
		      strret += c;
		} while (c != EOF);
		fclose (ret);
	}

	return strret;
}

//--------------------------------------------------
ofTargetPlatform ofGetTargetPlatform(){
#ifdef TARGET_LINUX
	if(ofSystem("uname -m").find("x86_64")==0)
		return OF_TARGET_LINUX64;
	else
		return OF_TARGET_LINUX;
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
#elif defined(TARGET_OF_IPHONE)
	return OF_TARGET_IPHONE;
#endif
}
