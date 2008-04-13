#ifndef _OF_UTIL
#define _OF_UTIL

#include "ofConstants.h"

#ifdef TARGET_WIN32	 // for ofLaunchBrowser
	#include <shellapi.h>
#endif


int 	ofNextPow2( int input);

float 	ofGetElapsedTimef();
int		ofGetElapsedTimeMillis();
int 	ofGetFrameNum();

int 	ofGetSeconds();
int 	ofGetMinutes();
int 	ofGetHours();

int     ofGetYear();
int     ofGetMonth();
int     ofGetDay();
int     ofGetWeekDay();

void 	ofLaunchBrowser(string url);

string 	ofToDataPath(string path);
string  ofToString(double value, int precision = 7);
string  ofToString(int  value);

string 	ofGetVersionInfo();

//----------------------- not for the public:
void	setFrameNum(int num);

#endif


