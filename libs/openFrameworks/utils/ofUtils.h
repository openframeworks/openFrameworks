#ifndef _OF_UTIL
#define _OF_UTIL

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

string  ofToString(double value, int precision = 7);
string  ofToString(int  value);

int ofToInt(const string& intString);
float ofToFloat(const string& floatString);

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

#endif


