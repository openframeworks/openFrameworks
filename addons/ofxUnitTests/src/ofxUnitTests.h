/*
 * ofxUnitTestsApp.h
 *
 *  Created on: Jul 8, 2015
 *      Author: arturo
 */
#pragma once

#include "ofConstants.h"
#include "ofLog.h"
#include "ofBaseApp.h"
#include "ofAppRunner.h"
#include <string>

class ofColorsLoggerChannel: public ofBaseLoggerChannel{
	std::string CON_DEFAULT="\033[0m";
	std::string CON_BOLD="\033[1m";
	std::string CON_RED="\033[31m";
	std::string CON_YELLOW="\033[33m";
	std::string CON_GREEN="\033[32m";
	std::string getColor(ofLogLevel level) const{
		switch(level){
		case OF_LOG_FATAL_ERROR:
		case OF_LOG_ERROR:
			return CON_RED;
		case OF_LOG_WARNING:
			return CON_YELLOW;
		case OF_LOG_NOTICE:
			return CON_GREEN;
		default:
			return CON_DEFAULT;
		}
	}
public:
	void log(ofLogLevel level, const std::string & module, const std::string & message){
		// print to cerr for OF_LOG_ERROR and OF_LOG_FATAL_ERROR, everything else to cout
		std::cout << "[" << ofGetLogLevelName(level, true)  << "] ";
		// only print the module name if it's not ""
		if(module != ""){
			std::cout << module << ": ";
		}
		std::cout << CON_BOLD << getColor(level) << message << CON_DEFAULT << std::endl;
	}

	void log(ofLogLevel level, const std::string & module, const char* format, ...){
		va_list args;
		va_start(args, format);
		log(level, module, format, args);
		va_end(args);
	}

	void log(ofLogLevel level, const std::string & module, const char* format, va_list args){
		//thanks stefan!
		//http://www.ozzu.com/cpp-tutorials/tutorial-writing-custom-printf-wrapper-function-t89166.html
		fprintf(stdout, (CON_BOLD + getColor(level) + "[%s] " + CON_DEFAULT).c_str(), ofGetLogLevelName(level, true).c_str());
		if(module != ""){
			fprintf(stdout, "%s: ", module.c_str());
		}
		vfprintf(stdout, format, args);
		fprintf(stdout, "\n");
	}
};

class ofxUnitTestsApp: public ofBaseApp{
	void setup(){
		ofSetLoggerChannel(std::shared_ptr<ofBaseLoggerChannel>(new ofColorsLoggerChannel));
		run();

		if(numTestsFailed == 0){
			ofLogNotice() << numTestsPassed << "/" << numTestsTotal << " tests passed";
		}else{
			ofLogError() <<  numTestsFailed << "/" << numTestsTotal << " tests failed";
		}
		ofExit(numTestsFailed);
	}
	virtual void run() = 0;

protected:
	void test(bool test, const std::string & testName, const std::string & msg, const std::string & file, int line){
		numTestsTotal++;
		if(test){
			ofLogNotice() << testName << " passed";
			numTestsPassed++;
		}else{
			ofLogError() << testName << " failed " << msg;
			ofLogError() << file << ": " << line;
			numTestsFailed++;
		}
	}

	void test(bool test, const std::string & testName, const std::string & file, int line){
		this->test(test,testName,"",file,line);
	}

	template<typename T1, typename T2>
	void test_eq(T1 t1, T2 t2, const std::string & testName, const std::string & msg, const std::string & file, int line){
		numTestsTotal++;
		if(t1==t2){
			ofLogNotice() << testName << " passed";
			numTestsPassed++;
		}else{
			ofLogError() << testName << " failed " << msg;
			ofLogError() << "value1 " << t1;
			ofLogError() << "value2 " << t2;
			ofLogError() << file << ": " << line;
			numTestsFailed++;
		}
	}

	template<typename T1, typename T2>
	void test_eq(T1 t1, T2 t2, const std::string & testName, const std::string & file, int line){
		test_eq(t1,t2,testName,"",file,line);
	}

private:
	int numTestsTotal = 0;
	int numTestsPassed = 0;
	int numTestsFailed = 0;
};

#define test(x, ...) this->test(x,__VA_ARGS__,__FILE__,__LINE__)
#define test_eq(x, ...) this->test_eq(x,__VA_ARGS__,__FILE__,__LINE__)
