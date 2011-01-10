#pragma once

#include "ofThread.h"

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


enum requestType {
	OF_URL_FILE_LOADER_TEXT_REQUEST,
	OF_URL_FILE_LOADER_BYTES_REQUEST
};

enum resuestStatus {
	OF_URL_FILE_LOADER_RESTING,
	OF_URL_FILE_LOADER_LOADING,
	OF_URL_FILE_LOADER_LOADING_FAILED,
	OF_URL_FILE_LOADER_LOADING_SUCCEEDED
};


class binaryData {
	
public:
	unsigned char * buffer;
	int	numBytes;
	
};

class ofURLFileLoader : public ofThread  {

    public:

        ofURLFileLoader();

        void getText(string url_, bool bAsync = true);
		void getBytes(string url_, bool bAsync = true);
		
		
		void resetStatus(){
			status = OF_URL_FILE_LOADER_RESTING;
		}
	
		// threading -----------------------------------------------
		void threadedFunction();
        void start();
        void stop();
	
		int	getStatus(){
			return status;
		}
		
		binaryData * getData(){
			return &args;
		}

    private:
		
		// perform the requests on the thread
		void handleTextRequest(string url_);
		void handleStreamRequest(string url_);
        
		string			url;
        string			response;        
		bool			bResponseReady;		
		int				requestType;		
		binaryData		args;
		int				status;
	


};
