#pragma once
#include "ofEvents.h"
#include "ofFileUtils.h"
#include "ofTypes.h"

class ofHttpRequest{
public:
	ofHttpRequest();
	ofHttpRequest(const string& url, const string& name,bool saveTo=false);

	string				url;
	string				name;
	bool				saveTo;
	map<string,string>	headers;

	int getId() const;
	OF_DEPRECATED_MSG("Use ofGetId().", int getID());

private:
	int					id;
	static int			nextID;
};

class ofHttpResponse{
public:
	ofHttpResponse();
	ofHttpResponse(const ofHttpRequest& request, const ofBuffer& data, int status, const string& error);
	ofHttpResponse(const ofHttpRequest& request, int status, const string& error);

	operator ofBuffer&();

	ofHttpRequest	    request;
	ofBuffer		    data;
	int					status;
	string				error;
};

ofHttpResponse ofLoadURL(const string& url);
int ofLoadURLAsync(const string& url, const string& name=""); // returns id
ofHttpResponse ofSaveURLTo(const string& url, const string& path);
int ofSaveURLAsync(const string& url, const string& path);
void ofRemoveURLRequest(int id);
void ofRemoveAllURLRequests();

void ofStopURLLoader();

ofEvent<ofHttpResponse> & ofURLResponseEvent();

template<class T>
void ofRegisterURLNotification(T * obj){
	ofAddListener(ofURLResponseEvent(),obj,&T::urlResponse);
}

template<class T>
void ofUnregisterURLNotification(T * obj){
	ofRemoveListener(ofURLResponseEvent(),obj,&T::urlResponse);
}

class ofBaseURLFileLoader;

class ofURLFileLoader  {
    public:
        ofURLFileLoader();	
        ofHttpResponse get(const string& url);
        int getAsync(const string& url, const string& name=""); // returns id
        ofHttpResponse saveTo(const string& url, const string& path);
        int saveAsync(const string& url, const string& path);
		void remove(int id);
		void clear();
        void stop();
        ofHttpResponse handleRequest(ofHttpRequest & request);

    private:
        shared_ptr<ofBaseURLFileLoader> impl;
};
