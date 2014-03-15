#include "Poco/Net/HTTPSession.h"
#include "Poco/Net/HTTPClientSession.h"
#include "Poco/Net/HTTPSClientSession.h"
#include "Poco/Net/HTTPRequest.h"
#include "Poco/Net/HTTPResponse.h"
#include "Poco/StreamCopier.h"
#include "Poco/Path.h"
#include "Poco/URI.h"
#include "Poco/Exception.h"
#include "Poco/URIStreamOpener.h"
#include "Poco/Net/HTTPStreamFactory.h"
#include "Poco/Net/HTTPSStreamFactory.h"
#include "Poco/Net/SSLManager.h"
#include "Poco/Net/KeyConsoleHandler.h"
#include "Poco/Net/ConsoleCertificateHandler.h"

#include "ofURLFileLoader.h"
#include "ofAppRunner.h"
#include "ofUtils.h"

using namespace Poco::Net;

using namespace Poco;

#include "ofConstants.h"

static bool factoryLoaded = false;
int	ofHttpRequest::nextID = 0;
ofEvent<ofHttpResponse> & ofURLResponseEvent(){
	static ofEvent<ofHttpResponse> * event = new ofEvent<ofHttpResponse>;
	return *event;
}

ofURLFileLoader::ofURLFileLoader() {
	if(!factoryLoaded){
		try {
			HTTPStreamFactory::registerFactory();
			HTTPSStreamFactory::registerFactory();
			SharedPtr<PrivateKeyPassphraseHandler> pConsoleHandler = new KeyConsoleHandler(false);
			SharedPtr<InvalidCertificateHandler> pInvalidCertHandler = new ConsoleCertificateHandler(true);
			Context::Ptr pContext = new Context(Context::CLIENT_USE, "", Context::VERIFY_NONE);
			SSLManager::instance().initializeClient(pConsoleHandler, pInvalidCertHandler, pContext);
			factoryLoaded = true;
		}
		catch (Poco::SystemException & PS) {
			ofLogError("ofURLFileLoader") << "couldn't create factory: " << PS.displayText();
		}
		catch (Poco::ExistsException & PS) {
			ofLogError("ofURLFileLoader") << "couldn't create factory: " << PS.displayText();
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
	Poco::ScopedLock<ofMutex> lock(mutex);
	for(int i=0;i<(int)requests.size();i++){
		if(requests[i].getID()==id){
			requests.erase(requests.begin()+i);
			return;
		}
	}
	ofLogError("ofURLFileLoader") << "remove(): request " <<  id << " not found";
}

void ofURLFileLoader::clear(){
	Poco::ScopedLock<ofMutex> lock(mutex);
	requests.clear();
	while(!responses.empty()) responses.pop();
}

void ofURLFileLoader::start() {
     if (isThreadRunning() == false){
		ofAddListener(ofEvents().update,this,&ofURLFileLoader::update);
        startThread(true, false);   // blocking, verbose
    }else{
    	ofLogVerbose("ofURLFileLoader") << "start(): signaling new request condition";
    	condition.signal();
    }
}

void ofURLFileLoader::stop() {
    lock();
    stopThread();
    condition.signal();
    unlock();
}

void ofURLFileLoader::threadedFunction() {
	ofLogVerbose("ofURLFileLoader") << "threadedFunction(): starting thread";
	lock();
	while( isThreadRunning() == true ){
		if(requests.size()>0){
	    	ofLogVerbose("ofURLFileLoader") << "threadedFunction(): querying request " << requests.front().name;
			ofHttpRequest request(requests.front());
			unlock();

			ofHttpResponse response(handleRequest(request));

			lock();
			if(response.status!=-1){
				// double-check that the request hasn't been removed from the queue
				if( (requests.size()==0) || (requests.front().getID()!=request.getID()) ){
					// this request has been removed from the queue
					ofLogVerbose("ofURLFileLoader") << "threadedFunction(): request " << requests.front().name
					<< " is missing from the queue, must have been removed/cancelled";
				}
				else{
					ofLogVerbose("ofURLFileLoader") << "threadedFunction(): got response to request "
					<< requests.front().name << " status " <<response.status;
					responses.push(response);
					requests.pop_front();
				}
			}else{
				responses.push(response);
		    	ofLogVerbose("ofURLFileLoader") << "threadedFunction(): failed getting request " << requests.front().name;
			}
		}else{
			ofLogVerbose("ofURLFileLoader") << "threadedFunction(): stopping on no requests condition";
			condition.wait(mutex);
		}
	}
}

ofHttpResponse ofURLFileLoader::handleRequest(ofHttpRequest request) {
	try {
		URI uri(request.url);
		std::string path(uri.getPathAndQuery());
		if (path.empty()) path = "/";

		HTTPRequest req(HTTPRequest::HTTP_GET, path, HTTPMessage::HTTP_1_1);
		HTTPResponse res;
		ofPtr<HTTPSession> session;
		istream * rs;
		if(uri.getScheme()=="https"){
			 //const Poco::Net::Context::Ptr context( new Poco::Net::Context( Poco::Net::Context::CLIENT_USE, "", "", "rootcert.pem" ) );
			HTTPSClientSession * httpsSession = new HTTPSClientSession(uri.getHost(), uri.getPort());//,context);
			httpsSession->setTimeout(Poco::Timespan(20,0));
			httpsSession->sendRequest(req);
			rs = &httpsSession->receiveResponse(res);
			session = ofPtr<HTTPSession>(httpsSession);
		}else{
			HTTPClientSession * httpSession = new HTTPClientSession(uri.getHost(), uri.getPort());
			httpSession->setTimeout(Poco::Timespan(20,0));
			httpSession->sendRequest(req);
			rs = &httpSession->receiveResponse(res);
			session = ofPtr<HTTPSession>(httpSession);
		}
		if(!request.saveTo){
			return ofHttpResponse(request,*rs,res.getStatus(),res.getReason());
		}else{
			ofFile saveTo(request.name,ofFile::WriteOnly,true);
			char aux_buffer[1024];
			rs->read(aux_buffer, 1024);
			std::streamsize n = rs->gcount();
			while (n > 0){
				// we resize to size+1 initialized to 0 to have a 0 at the end for strings
				saveTo.write(aux_buffer,n);
				if (rs->good()){
					rs->read(aux_buffer, 1024);
					n = rs->gcount();
				}
				else n = 0;
			}
			return ofHttpResponse(request,res.getStatus(),res.getReason());
		}

	} catch (const Exception& exc) {
        ofLogError("ofURLFileLoader") << "handleRequest(): "+ exc.displayText();

        return ofHttpResponse(request,-1,exc.displayText());

    } catch (...) {
    	return ofHttpResponse(request,-1,"ofURLFileLoader: fatal error, couldn't catch Exception");
    }

	return ofHttpResponse(request,-1,"ofURLFileLoader: fatal error, couldn't catch Exception");
	
}	

void ofURLFileLoader::update(ofEventArgs & args){
	lock();
	while(!responses.empty()){
		ofHttpResponse response(responses.front());
		ofLogVerbose("ofURLLoader") << "update(): new response " << response.request.name;
		responses.pop();
		unlock();
		ofNotifyEvent(ofURLResponseEvent(),response);
		lock();
	}
	unlock();

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

void ofStopURLLoader(){
	getFileLoader().stop();
}
