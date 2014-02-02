#pragma once

#include "ofConstants.h"
#include <bitset> // for ofToBinary

#include "ofLog.h"

#ifdef TARGET_WIN32	 // for ofLaunchBrowser
	#include <shellapi.h>
#endif

#include "Poco/Path.h"

int 	ofNextPow2(int input);

void	ofResetElapsedTimeCounter();		// this happens on the first frame
float 	ofGetElapsedTimef();
unsigned long long ofGetElapsedTimeMillis();
unsigned long long ofGetElapsedTimeMicros();
int 	ofGetFrameNum();

int 	ofGetSeconds();
int 	ofGetMinutes();
int 	ofGetHours();

//number of seconds since 1970
unsigned int ofGetUnixTime();

unsigned long long ofGetSystemTime( );			// system time in milliseconds;
unsigned long long ofGetSystemTimeMicros( );			// system time in microseconds;

		//returns 
string ofGetTimestampString();
string ofGetTimestampString(string timestampFormat);


int     ofGetYear();
int     ofGetMonth();
int     ofGetDay();
int     ofGetWeekday();

void 	ofLaunchBrowser(string url, bool uriEncodeQuery=false);

void	ofEnableDataPath();
void	ofDisableDataPath();
string 	ofToDataPath(string path, bool absolute=false);

template<class T>
void ofRandomize(vector<T>& values) {
	random_shuffle(values.begin(), values.end());
}

template<class T, class BoolFunction>
void ofRemove(vector<T>& values, BoolFunction shouldErase) {
	values.erase(remove_if(values.begin(), values.end(), shouldErase), values.end());
}

template<class T>
void ofSort(vector<T>& values) {
	sort(values.begin(), values.end());
}
template<class T, class BoolFunction>
void ofSort(vector<T>& values, BoolFunction compare) {
	sort(values.begin(), values.end(), compare);
}

template <class T>
unsigned int ofFind(const vector<T>& values, const T& target) {
	return distance(values.begin(), find(values.begin(), values.end(), target));
}

template <class T>
bool ofContains(const vector<T>& values, const T& target) {
	return ofFind(values, target) != values.size();
}

void ofSetWorkingDirectoryToDefault();

//set the root path that ofToDataPath will use to search for files relative to the app
//the path must have a trailing slash (/) !!!!
void ofSetDataPathRoot( string root );

template <class T>
string ofToString(const T& value){
	ostringstream out;
	out << value;
	return out.str();
}

/// like sprintf "%4f" format, in this example precision=4
template <class T>
string ofToString(const T& value, int precision){
	ostringstream out;
	out << fixed << setprecision(precision) << value;
	return out.str();
}

/// like sprintf "% 4d" or "% 4f" format, in this example width=4, fill=' '
template <class T>
string ofToString(const T& value, int width, char fill ){
	ostringstream out;
	out << fixed << setfill(fill) << setw(width) << value;
	return out.str();
}

/// like sprintf "%04.2d" or "%04.2f" format, in this example precision=2, width=4, fill='0'
template <class T>
string ofToString(const T& value, int precision, int width, char fill ){
	ostringstream out;
	out << fixed << setfill(fill) << setw(width) << setprecision(precision) << value;
	return out.str();
}

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
template <>
string ofToHex(const string& value);
string ofToHex(const char* value);

int ofHexToInt(const string& intHexString);
char ofHexToChar(const string& charHexString);
float ofHexToFloat(const string& floatHexString);
string ofHexToString(const string& stringHexString);

int ofToInt(const string& intString);
char ofToChar(const string& charString);
float ofToFloat(const string& floatString);
double ofToDouble(const string& doubleString);
bool ofToBool(const string& boolString);

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
template <>
string ofToBinary(const string& value);
string ofToBinary(const char* value);

int ofBinaryToInt(const string& value);
char ofBinaryToChar(const string& value);
float ofBinaryToFloat(const string& value);
string ofBinaryToString(const string& value);


string 	ofGetVersionInfo();
unsigned int ofGetVersionMajor();
unsigned int ofGetVersionMinor();
unsigned int ofGetVersionPatch();

void	ofSaveScreen(string filename);
void	ofSaveFrame(bool bUseViewport = false);
void	ofSaveViewport(string filename);

//--------------------------------------------------
vector <string> ofSplitString(const string & source, const string & delimiter, bool ignoreEmpty = false, bool trim = false);
string ofJoinString(vector <string> stringElements, const string & delimiter);
void ofStringReplace(string& input, string searchStr, string replaceStr);
bool ofIsStringInString(string haystack, string needle);
int ofStringTimesInString(string haystack, string needle);

string ofToLower(const string & src);
string ofToUpper(const string & src);

string ofVAArgsToString(const char * format, ...);
string ofVAArgsToString(const char * format, va_list args);

string ofSystem(string command);

ofTargetPlatform ofGetTargetPlatform();


