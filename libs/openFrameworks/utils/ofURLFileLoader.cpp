#include "ofURLFileLoader.h"
#include "ofAppRunner.h"
#include "ofUtils.h"

#include "Poco/Net/HTTPClientSession.h"
#include "Poco/Net/HTTPRequest.h"
#include "Poco/Net/HTTPResponse.h"
#include "Poco/StreamCopier.h"
#include "Poco/Path.h"
#include "Poco/URI.h"
#include "Poco/Exception.h"
#include "Poco/URIStreamOpener.h"
#include "Poco/Net/HTTPStreamFactory.h"

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

static bool factoryLoaded = false;
int	ofHttpRequest::nextID = 0;
ofEvent<ofHttpResponse> ofURLResponseEvent;

ofURLFileLoader::ofURLFileLoader() {
	if(!factoryLoaded){
		try {
			HTTPStreamFactory::registerFactory();
			factoryLoaded = true;
		}
		catch (Poco::SystemException PS) {
			ofLog(OF_LOG_ERROR, "Got exception in url ofURLFileloader");
		}
	}
}

ofHttpResponse ofURLFileLoader::get(string url) {
    ofHttpRequest request(url,url);
    return handleRequest(request);
}


void ofURLFileLoader::getAsync(string url, string name){
	if(name=="") name=url;
	ofHttpRequest request(url,name);
	lock();
	requests.push_back(request);
	unlock();
	start();
}

void ofURLFileLoader::remove(ofHttpRequest httpRequest){
	lock();
	for(int i=0;i<(int)requests.size();i++){
		if(requests[i].getID()==httpRequest.getID()){
			requests.erase(requests.begin()+i);
			return;
		}
	}
	unlock();
}

void ofURLFileLoader::start() {
     if (isThreadRunning() == false){
    	ofAddListener(ofEvents.update,this,&ofURLFileLoader::update);
        startThread(false, false);   // blocking, verbose
    }else{
    	ofLog(OF_LOG_VERBOSE,"signaling new request condition");
    	ofAddListener(ofEvents.update,this,&ofURLFileLoader::update);
    	condition.signal();
    }
}

void ofURLFileLoader::stop() {
    stopThread();
}

void ofURLFileLoader::threadedFunction() {
	while( isThreadRunning() == true ){
		lock();
    	ofLog(OF_LOG_VERBOSE,"starting thread loop ");
		if(requests.size()>0){
	    	ofLog(OF_LOG_VERBOSE,"querying request " + requests.front().name);
			ofHttpRequest request(requests.front());
			unlock();

			ofHttpResponse response(handleRequest(request));

			if(response.status!=-1){
				lock();
		    	ofLog(OF_LOG_VERBOSE,"got request " + requests.front().name);
				responses.push(response);
				requests.pop_front();
				unlock();
			}else{
		    	ofLog(OF_LOG_VERBOSE,"failed getting request " + requests.front().name);
			}
			ofSleepMillis(10);
		}else{
			ofLog(OF_LOG_VERBOSE,"stopping on no requests condition");
			condition.wait(mutex);
		}
	}
}

ofHttpResponse ofURLFileLoader::handleRequest(ofHttpRequest request) {
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
		return ofHttpResponse(request,rs,res.getStatus(),res.getReason());
	} catch (Exception& exc) {
        ofLog(OF_LOG_ERROR, "ofURLFileLoader " + exc.displayText());

        return ofHttpResponse(request,-1,exc.displayText());
    }	
	
}	

void ofURLFileLoader::update(ofEventArgs & args){
	lock();
	if(responses.size()){
		ofHttpResponse response(responses.front());
		responses.pop();
		if(!responses.size())
			ofRemoveListener(ofEvents.update,this,&ofURLFileLoader::update);
		unlock();
		ofNotifyEvent(ofURLResponseEvent,response);
	}else{
		unlock();
	}

}

static ofURLFileLoader fileLoader;

ofHttpResponse ofLoadURL(string url){
	return fileLoader.get(url);
}

void ofLoadURLAsync(string url, string name){
	fileLoader.getAsync(url,name);
}

void ofRemoveURLRequest(ofHttpRequest request){
	fileLoader.remove(request);
}
