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
#include "ofURLFileLoader.h"
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
    std::string stdOut;
    std::string stdErr;
public:
    void log(ofLogLevel level, const std::string & module, const std::string & message){
		std::stringstream str;

		str << "[ " << ofGetLogLevelName(level, true)  << "] ";
		std::cout << "[ " << ofGetLogLevelName(level, true)  << "] ";

		if(module != ""){
            str << module << ": ";
			std::cout << module << ": ";
		}

		str << message << std::endl;
		std::cout << CON_BOLD << getColor(level) << message << CON_DEFAULT << std::endl;

        if(level>OF_LOG_WARNING){
            stdErr += str.str();
        }else{
            stdOut += str.str();
        }
    }

    std::string getStdOut(){
        return stdOut;
    }

    std::string getStdErr(){
        return stdErr;
    }
};

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

    std::string stdOut;
    std::string stdErr;

public:
    std::string getStdOut(){
        return stdOut;
	}

    std::string getStdErr(){
        return stdErr;
    }

	void log(ofLogLevel level, const std::string & module, const std::string & message){
		auto msg = message;
		if(module!=""){
			msg = module + ": " + msg;
		}
        stdOut += "[" + ofGetLogLevelName(level) + "]\t\t" + msg + "\n";
		ofSystem("appveyor AddMessage \"" + msg + "\" -Category " + category(level));
	}
};

class ofxUnitTestsApp: public ofBaseApp{

    void setup(){
        ofSetLoggerChannel(logger);
		auto then = ofGetElapsedTimeMillis();
		run();
		auto now = ofGetElapsedTimeMillis();
        auto durationMs = now - then;
		bool passed = numTestsFailed==0;
		if(passed){
			ofLogNotice() << numTestsPassed << "/" << numTestsTotal << " tests passed";
		}else{
			ofLogError() <<  numTestsFailed << "/" << numTestsTotal << " tests failed";
		}

		ofLogNotice() << "took " << ofToString(durationMs) << "ms";
#if defined(TARGET_WIN32)
        if(!reportAppVeyor(passed, durationMs)){
            ++numTestsFailed;
        }
#endif
        ofExit(numTestsFailed);
    }

protected:

    virtual void run() = 0;

	bool do_test(bool test, const std::string & testName, const std::string & msg, const std::string & file, int line){
		numTestsTotal++;
		if(test){
			ofLogNotice() << testName << " passed";
			numTestsPassed++;
			return true;
		}else{
			ofLogError() << testName << " failed " << msg;
			ofLogError() << file << ": " << line;
			numTestsFailed++;
			return false;
		}
	}

	bool do_test(bool test, const std::string & testName, const std::string & file, int line){
		return this->do_test(test,testName,"",file,line);
	}

	template<typename T1, typename T2>
	bool do_test_eq(T1 t1, T2 t2, const std::string & v1, const std::string & v2, const std::string & testName, const std::string & msg, const std::string & file, int line){
		numTestsTotal++;
		if(t1==t2){
			ofLogNotice() << testName << " passed";
			numTestsPassed++;
			return true;
		}else{
			ofLogError() << testName << " failed " << msg;
			ofLogError() << "test_eq(" << v1 << ", " << v2 << ")";
			ofLogError() << "value1: " << v1 << " is " << ofToString(t1);
			ofLogError() << "value2: " << v2 << " is " << ofToString(t2);
			ofLogError() << file << ": " << line;
			numTestsFailed++;
			return false;
		}
	}

	template<typename T1, typename T2>
	bool do_test_eq(T1 t1, T2 t2, const std::string & v1, const std::string & v2, const std::string & testName, const std::string & file, int line){
		return do_test_eq(t1,t2,v1,v2,testName,"",file,line);
	}

	template<typename T1, typename T2>
	bool do_test_gt(T1 t1, T2 t2, const std::string & v1, const std::string & v2, const std::string & testName, const std::string & msg, const std::string & file, int line){
		numTestsTotal++;
		if(t1>t2){
			ofLogNotice() << testName << " passed";
			numTestsPassed++;
			return true;
		}else{
			ofLogError() << testName << " failed " << msg;
			ofLogError() << "test_gt(" << v1 << ", " << v2 << ")";
			ofLogError() << "value1: " << v1 << " is " << ofToString(t1);
			ofLogError() << "value2: " << v2 << " is " << ofToString(t2);
			ofLogError() << file << ": " << line;
			numTestsFailed++;
			return false;
		}
	}

	template<typename T1, typename T2>
	bool do_test_gt(T1 t1, T2 t2, const std::string & v1, const std::string & v2, const std::string & testName, const std::string & file, int line){
		return do_test_gt(t1,t2,v1,v2,testName,"",file,line);
	}

	template<typename T1, typename T2>
	bool do_test_lt(T1 t1, T2 t2, const std::string & v1, const std::string & v2, const std::string & testName, const std::string & msg, const std::string & file, int line){
		numTestsTotal++;
		if(t1<t2){
			ofLogNotice() << testName << " passed";
			numTestsPassed++;
			return true;
		}else{
			ofLogError() << testName << " failed " << msg;
			ofLogError() << "test_lt(" << v1 << ", " << v2 << ")";
			ofLogError() << "value1: " << v1 << " is " << ofToString(t1);
			ofLogError() << "value2: " << v2 << " is " << ofToString(t2);
			ofLogError() << file << ": " << line;
			numTestsFailed++;
			return false;
		}
	}

	template<typename T1, typename T2>
	bool do_test_lt(T1 t1, T2 t2, const std::string & v1, const std::string & v2, const std::string & testName, const std::string & file, int line){
		return do_test_lt(t1,t2,v1,v2,testName,"",file,line);
	}

private:
    std::string json_var_value(const std::string & var, const std::string & value){
        return "\"" + var + "\": \"" + value + "\"";
    }

#if defined(TARGET_WIN32)
    bool reportAppVeyor(bool passed, uint64_t durationMs){
        const std::string APPVEYOR_API_URL = "APPVEYOR_API_URL";
        if(ofGetEnv(APPVEYOR_API_URL)!=""){
            //ofSystem("appveyor AddTest -Name " + projectName.string() + " -Framework ofxUnitTests -FileName " + exeName.string() + " -Outcome " + (passed?"Passed":"Failed") + " -Duration " + ofToString(now-then));
            auto projectDir = of::filesystem::canonical(of::filesystem::path(ofFilePath::getCurrentExeDir()) / "..");
            auto projectName = projectDir.stem();
            auto exeName = of::filesystem::path(ofFilePath::getCurrentExePath()).filename();
            auto stdOut = logger->getStdOut();
            ofStringReplace(stdOut, "\\", "\\\\");
            ofStringReplace(stdOut, "\"", "\\\"");
            auto stdErr = logger->getStdErr();
            ofStringReplace(stdErr, "\\", "\\\\");
            ofStringReplace(stdErr, "\"", "\\\"");
            ofHttpRequest req;
            req.headers["Accept"] = "application/json";
            req.headers["Content-type"] = "application/json";
            req.method = ofHttpRequest::POST;
            req.url = ofGetEnv(APPVEYOR_API_URL) + "api/tests";
            req.body =
                    "{ " +
                        json_var_value("testName", projectName.string()) + ", " +
                        json_var_value("testFramework", "ofxUnitTests") + ", " +
                        json_var_value("fileName", exeName.string()) + ", " +
                        json_var_value("outcome", passed?"Passed":"Failed") + ", " +
                        json_var_value("durationMilliseconds", ofToString(durationMs)) + ", " +
                        json_var_value("StdOut", stdOut) + ", " +
                        json_var_value("StdErr", stdErr) +
                    "}";
            req.timeoutSeconds = 20;
            ofLogNotice() << "Sending appveyor test results to " << req.url;
            ofURLFileLoader http;
            auto res = http.handleRequest(req);
            if(res.status<200 || res.status>=300){
                ofLogError() << "sending to " << req.url;
                ofLogError() << res.status << ", " << res.error;
                std::cout << res.data.getText() << std::endl;
                ofLogError() << "for body:";
                std::cout << req.body << std::endl;
                return false;
            }else{
                ofLogNotice() << "Test results sent correctly";
                return true;
            }
        }else{
            return true;
        }
    }
#endif

	int numTestsTotal = 0;
	int numTestsPassed = 0;
	int numTestsFailed = 0;
    std::shared_ptr<ofColorsLoggerChannel> logger{new ofColorsLoggerChannel};
};

#define ofxTest(x, ...) this->do_test(x,__VA_ARGS__,__FILE__,__LINE__)
#define ofxTestEq(x,y, ...) this->do_test_eq(x,y,# x,# y,__VA_ARGS__,__FILE__,__LINE__)
#define ofxTestGt(x,y, ...) this->do_test_gt(x,y,# x,# y,__VA_ARGS__,__FILE__,__LINE__)
#define ofxTestLt(x,y, ...) this->do_test_lt(x,y,# x,# y,__VA_ARGS__,__FILE__,__LINE__)
