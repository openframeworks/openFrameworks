#pragma once

#include <deque>

#include "ofConstants.h"

#include "Poco/Exception.h"
#include "Poco/ErrorHandler.h"

#include "ofLog.h"

using std::deque;

class ofErrorHandler;

//----------------------------------------------------------
void ofInitErrorHandler();
void ofSetErrorHandler(ofErrorHandler errorHandler);
ofErrorHandler ofGetErrorHandler();
void ofPushErrorHandler();
void ofPopErrorHandler();

//----------------------------------------------------------
class ofErrorHandler : public Poco::ErrorHandler {
public:
    ofErrorHandler(){};
    
    virtual ~ofErrorHandler(){}
    
    virtual void exception(const Poco::Exception& exc) {
        ofLogError("ofErrorHandler::exception") << "Uncaught exception: " << exc.displayText();
    }
    
    virtual void exception(const std::exception& exc) {
        ofLogError("ofErrorHandler::exception") << "Uncaught exception: " << exc.what();
    }
    virtual void exception() {
        ofLogError("ofErrorHandler::exception") << "Uncaught exception: Unknown exception.";
    }
};

class ofThreadErrorHandler : public ofErrorHandler {
public:

    ofThreadErrorHandler(const string& _threadName = "NONE") {
        threadName = _threadName;
    }

    virtual ~ofThreadErrorHandler() { }

    virtual void exception(const Poco::Exception& exc) {
        ofLogError("ofThreadErrorHandler::exception") << "Thread: Uncaught thread exception: " << exc.displayText();
    }

    virtual void exception(const std::exception& exc) {
        ofLogError("ofThreadErrorHandler::exception") << "Uncaught thread exception: " << exc.what();
    }
    virtual void exception() {
        ofLogError("ofThreadErrorHandler::exception") << "Uncaught thread exception: Unknown exception.";
    }

    virtual void setName(const string& name) {
        threadName = name;
    }

    string getName() const {
        return threadName;
    }

private:
    string threadName;
};

