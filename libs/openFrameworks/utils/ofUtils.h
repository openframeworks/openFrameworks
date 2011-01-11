#pragma once

#include "ofConstants.h"

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

template <class T> string ofToString(T value){
	ostringstream out;
	out << value;
	return out.str();
}

template <class T> string ofToString(T value, int precision){
	ostringstream out;
	out << fixed << setprecision(precision) << value;
	return out.str();
}

int ofToInt(const string& intString);
float ofToFloat(const string& floatString);
bool ofToBool(const string& boolString);
char ofToChar(const string& charString);

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
