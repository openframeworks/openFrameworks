#include "ofMain.h"
#include "ofAppNoWindow.h"
#include "ofxUnitTests.h"

//
class ofStringLoggerChannel: public ofBaseLoggerChannel{
	std::stringstream stdOut;
    std::stringstream stdErr;
public:
    void log(ofLogLevel level, const std::string & module, const std::string & message){
		std::stringstream& ss = level < OF_LOG_ERROR ? stdOut : stdErr;

		ss << "[ " << ofGetLogLevelName(level, true)  << "] ";
		if(module != ""){
            ss << module << ": ";
		}

		ss << message << std::endl;
    }

    std::string getStdOut(){
        return stdOut.str();
    }

    std::string getStdErr(){
        return stdErr.str();
    }

	void reset(){
		stdOut.str("");
		stdOut.clear();
		stdErr.str("");
		stdErr.clear();
	}
};

class ofApp: public ofxUnitTestsApp{
	void run(){
		auto testLogger = ofGetLoggerChannel();
		// switch to our custom logger
		ofSetLoggerChannel(stringLogger);
		ofLogNotice("logging","Hello %s!","World");
		ofLogError("logging","error %d : %s",0,"error description");
		//restore logger
		ofSetLoggerChannel(testLogger);
		ofxTestEq(stringLogger->getStdOut(),"[ notice ] logging: Hello World!\n","logging a notice message with printf syntax");
		ofxTestEq(stringLogger->getStdErr(),"[  error ] logging: error 0 : error description\n","logging an error message with printf syntax");
	}
	std::shared_ptr<ofStringLoggerChannel> stringLogger{new ofStringLoggerChannel};
};

//========================================================================
int main( ){
	ofInit();
	auto window = std::make_shared<ofAppNoWindow>();
	auto app = std::make_shared<ofApp>();
	// this kicks off the running of my app
	// can be OF_WINDOW or OF_FULLSCREEN
	// pass in width and height too:
	ofRunApp(window, app);
	return ofRunMainLoop();

}
