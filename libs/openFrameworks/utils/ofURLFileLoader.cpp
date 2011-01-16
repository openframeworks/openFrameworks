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
	for(int i=0;i<requests.size();i++){
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
			ofHttpResponse response(handleRequest(request));
			if(response.status!=-1){
				lock();
				responses.push(response);
				requests.pop_front();
				unlock();
			}
		}else{
			unlock();
			stop();
		}
		ofSleepMillis(10);
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
	if(responses.size()){
		ofHttpResponse response(responses.front());
		ofNotifyEvent(ofURLResponseEvent,response);
		responses.pop();
		if(!isThreadRunning() && !responses.size())
			ofRemoveListener(ofEvents.update,this,&ofURLFileLoader::update);
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
