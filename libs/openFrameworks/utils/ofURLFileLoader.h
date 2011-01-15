#pragma once

#include "ofThread.h"
#include "ofEvents.h"
#include "ofFileUtils.h"

#include <deque>
#include <queue>

class ofHttpRequest{
public:
	ofHttpRequest(string url,string name,int id=0)
	:url(url)
	,name(name)
	,id(nextID++){}

	string				url;
	string				name;

	int getID(){return id;}
private:
	int					id;
	static int			nextID;
};

class ofHttpResponse{
public:
	ofHttpResponse(ofHttpRequest request,const ofBuffer & data,int status, string error)
	:request(request)
	,data(data)
	,status(status)
	,error(error){}

	ofHttpResponse(ofHttpRequest request,int status,string error)
	:request(request)
	,status(status)
	,error(error){}

	ofHttpRequest	    request;
	ofBuffer		    data;
	int					status;
	string				error;
};

ofHttpResponse ofLoadURL(string url);
void ofLoadURLAsync(string url, string name="");
void ofRemoveURLRequest(ofHttpRequest request);

extern ofEvent<ofHttpResponse> ofURLResponseEvent;

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
        ofHttpResponse get(string url);
		void getAsync(string url, string name="");
		void remove(ofHttpRequest httpRequest);

    protected:

		// threading -----------------------------------------------
		void threadedFunction();
        void start();
        void stop();
        void update(ofEventArgs & args);  // notify in update so the notification is thread safe

    private:

		// perform the requests on the thread
        ofHttpResponse handleRequest(ofHttpRequest request);

		deque<ofHttpRequest> requests;
		queue<ofHttpResponse> responses;

};
