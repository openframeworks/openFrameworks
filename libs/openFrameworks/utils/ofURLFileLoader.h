#pragma once

#include "ofThread.h"
#include "ofEvents.h"
#include <queue>


bool ofLoadURL(string url, ofBuffer & buffer, bool bAsync = true, string name="");


class ofHttpRequest{
public:
	ofHttpRequest(string url,string name, ofBuffer & buffer)
	:url(url)
	,name(name)
	,response(buffer)
	,status(-1){}

	string				url;
	string				name;
	ofBuffer		&   response;
	string				error;
	int					status;
};


extern ofEvent<ofHttpRequest> ofURLResponseEvent;

template<class T>
void ofRegisterURLNotification(T * obj){
	ofAddListener(ofURLResponseEvent,obj,&T::urlResponse);
}

template<class T>
void ofUnregisterURLNotification(T * obj){
	ofRemoveListener(ofURLResponseEvent,obj,&T::urlResponse);
}


class ofURLFileLoader : public ofThread  {

    public:

        ofURLFileLoader();
		bool get(string url, ofBuffer & buffer, bool bAsync = true, string name="");

    protected:

		// threading -----------------------------------------------
		void threadedFunction();
        void start();
        void stop();
        void update(ofEventArgs & args);  // notify in update so the notification is thread safe

    private:

		// perform the requests on the thread
		bool handleRequest(ofHttpRequest & request);

		queue<ofHttpRequest> requests;
		queue<ofHttpRequest> attendedRequests;

};
