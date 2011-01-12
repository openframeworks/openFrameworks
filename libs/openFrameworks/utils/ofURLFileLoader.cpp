#include "ofURLFileLoader.h"


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

#ifdef TARGET_LINUX
	#include <auto_ptr.h>
#endif

#include "ofConstants.h"


//TODO: is this needed for linux?
#ifdef TARGET_LINUX
	#include <auto_ptr.h>
#endif


static bool factoryLoaded = false;



ofURLFileLoader::ofURLFileLoader() {
	
	status		= OF_URL_FILE_LOADER_RESTING;
	
}

void ofURLFileLoader::getText(string url_, bool bAsync) {
    url = url_;
	requestType = OF_URL_FILE_LOADER_TEXT_REQUEST;
	status = OF_URL_FILE_LOADER_LOADING;
	start();
}

//void ofURLFileLoader::getStreamUnthreaded(string url_, bool bAsync) {
//    
//	url	= url_;
//
//	if(!factoryLoaded){
//		HTTPStreamFactory::registerFactory();
//		factoryLoaded = true;
//	}
//	
//	string str;
//	
//	try {
//		//specify out url and open stream
//		URI uri(url_);      
//		std::auto_ptr<std::istream> pStr(URIStreamOpener::defaultOpener().open(uri));
//		//copy to our string
//		StreamCopier::copyToString(*pStr.get(), str);
//	} catch (Exception& exc) {
//        cerr << exc.displayText() << std::endl;
//
//    }	
//	
//	//figure out how many bytes the image is and allocate
//	int bytesToRead = str.size();
//	unsigned char buff[bytesToRead];
//
//	memset(buff, 0, bytesToRead);
//
//	for(int i = 0; i < bytesToRead; i++){
//		buff[i] = str[i];
//	}
//	
//	args.buff = buff;
//	args.bytesToRead = bytesToRead;
//	
//	notifyUnthreadedStreamReceived(args);
//	
//}

void ofURLFileLoader::getBytes(string url_, bool bAsync) {
	args.clear();
	
    url					= url_;
	requestType			= OF_URL_FILE_LOADER_BYTES_REQUEST;
	status = OF_URL_FILE_LOADER_LOADING;
	
	if(!factoryLoaded){
		HTTPStreamFactory::registerFactory();
		factoryLoaded = true;
	}
	
    start();
}



void ofURLFileLoader::start() {
     if (isThreadRunning() == false){
        bResponseReady = false;
        startThread(false, false);   // blocking, verbose
    }
}

void ofURLFileLoader::stop() {
    stopThread();
}

void ofURLFileLoader::threadedFunction() {

    switch (requestType) {
		
		case OF_URL_FILE_LOADER_TEXT_REQUEST: 
			handleTextRequest(url);
			//notifyTextReceived(response);
			break;
		
		case OF_URL_FILE_LOADER_BYTES_REQUEST: 
			handleStreamRequest(url); 
			//notifyThreadedStreamReceived(args);
			break;
	}
	
    stop();
    response.clear();
    bResponseReady = true;

}

void ofURLFileLoader::handleTextRequest(string url_) {
	
	try {
		URI uri(url_);
        string path(uri.getPathAndQuery());
        if (path.empty()) path = "/";

        HTTPClientSession session(uri.getHost(), uri.getPort());
        HTTPRequest req(HTTPRequest::HTTP_GET, path, HTTPMessage::HTTP_1_1);
        session.sendRequest(req);

        HTTPResponse res; 
        std::istream& rs = session.receiveResponse(res);
        std::cout << res.getStatus() << " " << res.getReason() << std::endl;
        
		StreamCopier::copyToString(rs, response);
		
		status = OF_URL_FILE_LOADER_LOADING_SUCCEEDED;		// ? is this right?
		
	} catch (Exception& exc) {
		
		
		status = OF_URL_FILE_LOADER_LOADING_FAILED;
        cerr << exc.displayText() << std::endl;
    }		
	
}

void ofURLFileLoader::handleStreamRequest(string url_) {
	
	string str;
	
	try {
		//specify out url and open stream
		URI uri(url_);      
		std::auto_ptr<std::istream> pStr(URIStreamOpener::defaultOpener().open(uri));
		//copy to our string
		//StreamCopier::copyToString(*pStr.get(), str);
		
		if(pStr->good() && args.set(*pStr.get())){
			status = OF_URL_FILE_LOADER_LOADING_SUCCEEDED;
		}else{
			status = OF_URL_FILE_LOADER_LOADING_FAILED;
		}
		
	} catch (Exception& exc) {
		
		status = OF_URL_FILE_LOADER_LOADING_FAILED;
        ofLog(OF_LOG_ERROR, exc.displayText());

    }	
	
}	


//
//void ofURLFileLoader::notifyTextReceived (string response) {
//	textReady.notify(this, response);
//}
//void ofURLFileLoader::notifyUnthreadedStreamReceived (StreamEventArgs args) {
//	unthreadedStreamReady.notify(this, args);
//}
//
//void ofURLFileLoader::notifyThreadedStreamReceived (StreamEventArgs args) {
//	threadedStreamReady.notify(this, args);
//}
//       


