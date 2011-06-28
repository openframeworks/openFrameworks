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


int ofURLFileLoader::getAsync(string url, string name){
	if(name=="") name=url;
	ofHttpRequest request(url,name);
	lock();
	requests.push_back(request);
	unlock();
	start();
	return request.getID();
}


ofHttpResponse ofURLFileLoader::saveTo(string url, string path){
    ofHttpRequest request(url,path,true);
    return handleRequest(request);
}

int ofURLFileLoader::saveAsync(string url, string path){
	ofHttpRequest request(url,path,true);
	lock();
	requests.push_back(request);
	unlock();
	start();
	return request.getID();
}

void ofURLFileLoader::remove(int id){
	lock();
	for(int i=0;i<(int)requests.size();i++){
		if(requests[i].getID()==id){
			requests.erase(requests.begin()+i);
			return;
		}
	}
	unlock();
}

void ofURLFileLoader::clear(){
	lock();
	requests.clear();
	while(!responses.empty()) responses.pop();
	unlock();
}

void ofURLFileLoader::start() {
     if (isThreadRunning() == false){
        startThread(false, false);   // blocking, verbose
    }else{
    	ofLog(OF_LOG_VERBOSE,"signaling new request condition");
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
		    	ofAddListener(ofEvents.update,this,&ofURLFileLoader::update);
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
		if(!request.saveTo){
			return ofHttpResponse(request,rs,res.getStatus(),res.getReason());
		}else{
			ofFile saveTo(request.name,ofFile::WriteOnly);
			char aux_buffer[1024];
			rs.read(aux_buffer, 1024);
			std::streamsize n = rs.gcount();
			while (n > 0){
				// we resize to size+1 initialized to 0 to have a 0 at the end for strings
				saveTo.write(aux_buffer,n);
				if (rs){
					rs.read(aux_buffer, 1024);
					n = rs.gcount();
				}
				else n = 0;
			}
			return ofHttpResponse(request,res.getStatus(),res.getReason());
		}

	} catch (Exception& exc) {
        ofLog(OF_LOG_ERROR, "ofURLFileLoader " + exc.displayText());

        return ofHttpResponse(request,-1,exc.displayText());
    }	
	
}	

void ofURLFileLoader::update(ofEventArgs & args){
	lock();
	if(responses.size()){
		ofHttpResponse response(responses.front());
		ofLog(OF_LOG_VERBOSE,"ofURLLoader::update: new response " +response.request.name);
		responses.pop();
		unlock();
		ofNotifyEvent(ofURLResponseEvent,response);
	}else{
		ofRemoveListener(ofEvents.update,this,&ofURLFileLoader::update);
		unlock();
	}

}

static ofURLFileLoader & getFileLoader(){
	static ofURLFileLoader * fileLoader = new ofURLFileLoader;
	return *fileLoader;
}

ofHttpResponse ofLoadURL(string url){
	return getFileLoader().get(url);
}

int ofLoadURLAsync(string url, string name){
	return getFileLoader().getAsync(url,name);
}

ofHttpResponse ofSaveURLTo(string url, string path){
	return getFileLoader().saveTo(url,path);
}

int ofSaveURLAsync(string url, string path){
	return getFileLoader().saveAsync(url,path);
}

void ofRemoveURLRequest(int id){
	getFileLoader().remove(id);
}

void ofRemoveAllURLRequests(){
	getFileLoader().clear();
}
