#include "ofErrors.h"

static ofErrorHandler currentErrorHandler;
static deque<ofErrorHandler> errorHandlerHistory;

//----------------------------------------------------------
void ofInitErrorHandler() {
    ofSetErrorHandler(currentErrorHandler);
}

//----------------------------------------------------------
void ofSetErrorHandler(ofErrorHandler errorHandler){
    currentErrorHandler = errorHandler;
    Poco::ErrorHandler::set(&currentErrorHandler);
    // Poco::ErrorHandler::set returns Poco::ErrorHandler*,
    // but we don't need to get that pointer, because we are
    // keeping a record in the errorHandlerHistory;
}

//----------------------------------------------------------
ofErrorHandler ofGetErrorHandler(){
	return currentErrorHandler;
}

//----------------------------------------------------------
void ofPushErrorHandler(){
	errorHandlerHistory.push_front(currentErrorHandler);
    
	//if we are over the max number of styles we have set, then delete the oldest styles.
	if( errorHandlerHistory.size() > OF_MAX_STYLE_HISTORY ){
		errorHandlerHistory.pop_back();
		//should we warn here?
		//ofLog(OF_LOG_WARNING, "ofPushErrorHandler - warning: you have used ofPushErrorHandler more than %i times without calling ofPopStyle - check your code!", OF_MAX_STYLE_HISTORY);
	}
}

//----------------------------------------------------------
void ofPopErrorHandler(){
	if( errorHandlerHistory.size() ){
		ofSetErrorHandler(errorHandlerHistory.front());
		errorHandlerHistory.pop_front();
	}
}



