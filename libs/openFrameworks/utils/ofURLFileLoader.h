#pragma once

#include "ofThread.h"


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
		
		ofBuffer & getData(){
			return args;
		}

    private:
		
		// perform the requests on the thread
		void handleTextRequest(string url_);
		void handleStreamRequest(string url_);
        
		string			url;
        string			response;        
		bool			bResponseReady;		
		int				requestType;		
		ofBuffer		args;
		int				status;
	


};
