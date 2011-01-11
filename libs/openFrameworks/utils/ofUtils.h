#pragma once

#include "ofConstants.h"
#include <bitset> // for ofToBinary

#ifdef TARGET_WIN32	 // for ofLaunchBrowser
	#include <shellapi.h>
#endif


int 	ofNextPow2(int input);

void	ofResetElapsedTimeCounter();		// this happens on the first frame
float 	ofGetElapsedTimef();
int		ofGetElapsedTimeMillis();
int 	ofGetFrameNum();

int 	ofGetSeconds();
int 	ofGetMinutes();
int 	ofGetHours();

unsigned long ofGetSystemTime( );			// system time in milliseconds;

int     ofGetYear();
int     ofGetMonth();
int     ofGetDay();
int     ofGetWeekday();

void 	ofLaunchBrowser(string url);

void	ofEnableDataPath();
void	ofDisableDataPath();
string 	ofToDataPath(string path, bool absolute=false);


//set the root path that ofToDataPath will use to search for files relative to the app
//the path must have a trailing slash (/) !!!!
void	ofSetDataPathRoot( string root );

template <class T> string ofToString(const T& value){
	ostringstream out;
	out << value;
	return out.str();
}

template <class T> string ofToString(const T& value, int precision){
	ostringstream out;
	out << fixed << setprecision(precision) << value;
	return out.str();
}

template <class T> string ofToHex(const T& value) {
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
template <> string ofToHex(const string& value);
string ofToHex(const char* value);

int ofToInt(const string& intString);
int ofHexToInt(const string& intHexString);
float ofToFloat(const string& floatString);
bool ofToBool(const string& boolString);
char ofToChar(const string& charString);

template <class T> string ofToBinary(const T& value) {
	const int numBits = 8 * sizeof(value);
	bitset<numBits> bitBuffer(value);
	return bitBuffer.to_string();
}
template <> string ofToBinary(const string& value);
string ofToBinary(const char* value);

string 	ofGetVersionInfo();

void	ofSaveScreen(string filename);
void	ofSaveFrame();

vector<string>	ofSplitString(const string & text, const string & delimiter);

//--------------------------------------------------
void ofSetLogLevel(int logLevel);
void ofLog(int logLevel, string message);
void ofLog(int logLevel, const char* format, ...);
void ofSetConsoleColor(int color);
void ofRestoreConsoleColor();

//--------------------------------------------------
class ofBuffer;
bool ofReadFile(const string & path, ofBuffer & file, bool binary=false);
