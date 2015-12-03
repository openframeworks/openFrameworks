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
		fprintf(stdout, (CON_BOLD + getColor(level) + "[%s] " + CON_DEFAULT).c_str(), ofGetLogLevelName(level, true).c_str());
		if(module != ""){
			fprintf(stdout, "%s: ", module.c_str());
		}
		vfprintf(stdout, format, args);
		fprintf(stdout, "\n");
	}
};




#ifdef TARGET_WIN32

std::string ofxAppveyorAPIURL(){
	const size_t BUFSIZE = 4096;
	std::vector<char> pszOldVal(BUFSIZE, 0);
	GetEnvironmentVariableA("APPVEYOR_API_URL", pszOldVal.data(), BUFSIZE);
	return std::string(pszOldVal.begin(), pszOldVal.end());
}
#endif

class ofAppveyorSystemChannel: public ofBaseLoggerChannel{

	std::string category(ofLogLevel level){
		std::string category;
		switch(level){
			case OF_LOG_VERBOSE:
			case OF_LOG_NOTICE:
				return "Information";
			case OF_LOG_WARNING:
				return "Warning";
			default:
				return "Error";
			break;
		}
	}

	std::string totalOut;

public:
	std::string getTotalOut(){
		return totalOut;
	}

	void log(ofLogLevel level, const std::string & module, const std::string & message){
		auto msg = message;
		if(module!=""){
			msg = module + ": " + msg;
		}
		totalOut += "[" + ofGetLogLevelName(level) + "]\t\t" + msg + "\n";
		ofSystem("appveyor AddMessage \"" + msg + "\" -Category " + category(level));
	}

	void log(ofLogLevel level, const std::string & module, const char* format, ...){
		va_list args;
		va_start(args, format);
		log(level, module, format, args);
		va_end(args);
	}

	void log(ofLogLevel level, const std::string & module, const char* format, va_list args){
		auto msg = ofVAArgsToString(format,args);
		log(level, module, msg);
	}
};

class ofxUnitTestsApp: public ofBaseApp{

	void setup(){
/*#ifdef TARGET_WIN32
		std::shared_ptr<ofAppveyorSystemChannel> appveyorLogger;
		if(ofxAppveyorAPIURL()!=""){
			appveyorLogger.reset(new ofAppveyorSystemChannel);
			ofSetLoggerChannel(appveyorLogger);
		}else{
			ofSetLoggerChannel(std::shared_ptr<ofBaseLoggerChannel>(new ofColorsLoggerChannel));
			ofLogNotice() << "Not running in Appveyor";
		}
#else*/
		ofSetLoggerChannel(std::shared_ptr<ofBaseLoggerChannel>(new ofColorsLoggerChannel));
//#endif
		auto then = ofGetElapsedTimeMillis();
		run();
		auto now = ofGetElapsedTimeMillis();

		bool passed = numTestsFailed==0;
		if(passed){
			ofLogNotice() << numTestsPassed << "/" << numTestsTotal << " tests passed";
		}else{
			ofLogError() <<  numTestsFailed << "/" << numTestsTotal << " tests failed";
		}
		ofLogNotice() << "took " << ofToString(now-then) << "ms";
#ifdef TARGET_WIN32
        if(ofxAppveyorAPIURL()!=""){
            //ofSystem("appveyor AddTest -Name " + projectName.string() + " -Framework ofxUnitTests -FileName " + exeName.string() + " -Outcome " + (passed?"Passed":"Failed") + " -Duration " + ofToString(now-then));
            auto projectDir = std::filesystem::canonical(std::filesystem::path(ofFilePath::getCurrentExeDir()) / "..");
            auto projectName = projectDir.stem();
            auto exeName = std::filesystem::path(ofFilePath::getCurrentExePath()).filename();
            ofHttpRequest req;
            req.headers["Accept"] = "application/json";
            req.headers["Content-type"] = "application/json";
            req.method = ofHttpRequest::POST;
            req.url = ofxAppveyorAPIURL() + "api/tests";
            req.body =
                    "{ " +
                        json_var_value("testName", projectName.string()) + ", " +
                        json_var_value("testFramework", "ofxUnitTests") + ", " +
                        json_var_value("fileName", exeName.string()) + ", " +
                        json_var_value("outcome", passed?"Passed":"Failed") + ", " +
                        json_var_value("durationMilliseconds", ofToString(now-then)) +
                    "}";
            ofURLFileLoader http;
            auto res = http.handleRequest(req);
            if(res.status!=200){
                ofLogError() << res.status << ", " << res.error;
                cout << res.data.getText() << endl;
            }
        }
#endif

		ofExit(numTestsFailed);
    }

protected:

    virtual void run() = 0;

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
    std::string json_var_value(const std::string & var, const std::string & value){
        return "\"" + var + "\": \"" + value + "\"";
    }

	int numTestsTotal = 0;
	int numTestsPassed = 0;
	int numTestsFailed = 0;
};

#define test(x, ...) this->test(x,__VA_ARGS__,__FILE__,__LINE__)
#define test_eq(x, ...) this->test_eq(x,__VA_ARGS__,__FILE__,__LINE__)
