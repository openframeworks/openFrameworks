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
        ofLogFatalError("ofErrorHandler::exception") << "Uncaught exception: " << exc.displayText();
    }
    
    virtual void exception(const std::exception& exc) {
        ofLogFatalError("ofErrorHandler::exception") << "Uncaught exception: " << exc.what();
    }
    virtual void exception() {
        ofLogFatalError("ofErrorHandler::exception") << "Uncaught exception: Unknown exception.";
    }
};
