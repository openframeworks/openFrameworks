#include "ofURLFileLoader.h"
#include "ofAppRunner.h"

#include "Poco/Net/HTTPClientSession.h"
#include "Poco/Net/HTTPRequest.h"
#include "Poco/Net/HTTPResponse.h"
#include "Poco/StreamCopier.h"
#include "Poco/Path.h"
#include "Poco/URI.h"
#include "Poco/Exception.h"
#include "Poco/URIStreamOpener.h"
#include "Poco/Net/HTTPStreamFactory.h"
#include <queue>

using Poco::Net::HTTPClientSession;
using Poco::Net::HTTPRequest;
using Poco::Net::HTTPResponse;
using Poco::Net::HTTPMessage;
using Poco::Net::HTTPStreamFactory;

using Poco::StreamCopier;
using Poco::Path;
using Poco::URI;
using Poco::URIStreamOpener;
using Poco::Exception;

#include "ofConstants.h"

#ifdef TARGET_LINUX
	#include <auto_ptr.h>
#endif

static bool factoryLoaded = false;
ofEvent<ofHttpRequest> ofURLResponseEvent;

ofURLFileLoader::ofURLFileLoader() {
	if(!factoryLoaded){
		HTTPStreamFactory::registerFactory();
		factoryLoaded = true;
	}
}

bool ofURLFileLoader::get(string url, ofBuffer & buffer, bool bAsync, string name) {
	if(name=="") name=url;
    ofHttpRequest request(url,name,buffer);
    if(bAsync){
    	lock();
    	requests.push(request);
    	unlock();
    	start();
    	return true;
    }else{
    	return handleRequest(request);
    }
}

/*void ofURLFileLoader::remove(ofHttpRequest & httpRequest){
	lock();
	for(int i=0;i<requests.size();i++){
		if(&requests[i]==&httpRequest){
			requests.erase(requests.begin()+i);
			return;
		}
	}
	unlock();
}*/



void ofURLFileLoader::start() {
     if (isThreadRunning() == false){
    	 ofAddListener(ofEvents.update,this,&ofURLFileLoader::update);
         startThread(false, false);   // blocking, verbose
    }
}

void ofURLFileLoader::stop() {
    stopThread();
}

void ofURLFileLoader::threadedFunction() {
	while( isThreadRunning() == true ){
		lock();
		if(requests.size()>0){
			ofHttpRequest request(requests.front());
			unlock();
			if(handleRequest(request) || request.status!=-1){
				lock();
				attendedRequests.push(request);
				requests.pop();
				unlock();
			}
		}else{
			unlock();
			stop();
		}
		ofSleepMillis(10);
	}
}

bool ofURLFileLoader::handleRequest(ofHttpRequest & request) {
	try {
		URI uri(request.url);
		std::string path(uri.getPathAndQuery());
		if (path.empty()) path = "/";

		HTTPClientSession session(uri.getHost(), uri.getPort());
		HTTPRequest req(HTTPRequest::HTTP_GET, path, HTTPMessage::HTTP_1_1);
		session.setTimeout(Poco::Timespan(20,0));
		session.sendRequest(req);
		HTTPResponse res;
		istream& rs = session.receiveResponse(res);
		request.status = res.getStatus();
		request.error = res.getReason();
		if(request.response.set(rs)){
			return true;
		}else{
			return false;
		}
	} catch (Exception& exc) {
        ofLog(OF_LOG_ERROR, "ofURLFileLoader " + exc.displayText());
        request.error = exc.displayText();
        return false;
    }	
	
}	

void ofURLFileLoader::update(ofEventArgs & args){
	if(attendedRequests.size()){
		ofNotifyEvent(ofURLResponseEvent,attendedRequests.front());
		attendedRequests.pop();
		if(!isThreadRunning() && !attendedRequests.size())
			ofRemoveListener(ofEvents.update,this,&ofURLFileLoader::update);
	}

}

static ofURLFileLoader fileLoader;

bool ofLoadURL(string url, ofBuffer & buffer, bool bAsync, string name){
	return fileLoader.get(url,buffer,bAsync,name);
}

/*
void ofRemoveRequest(ofHttpRequest & request){
	fileLoader.remove(request);
}*/
