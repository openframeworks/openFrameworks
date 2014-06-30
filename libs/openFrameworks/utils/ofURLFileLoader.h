#pragma once

#include <deque>
#include <queue>
#include <map>


#include "ofThread.h"
#include "ofEvents.h"
#include "ofFileUtils.h"

#include "Poco/Condition.h"


class ofHttpRequest{
    
public:
    ofHttpRequest()
	:saveTo(false)
	,id(nextID++)
    ,redirectCount(0)
    ,redirect(false)
    {};
    
	ofHttpRequest(string url,string name, bool saveTo=false, ofHttpRequestType type = OF_HTTP_GET)
	:url(url)
	,name(name)
	,saveTo(saveTo)
    ,redirectCount(0)
    ,redirect(false)
	,id(nextID++)
    {

    };
    
	string				url;
    string              redirecturl;
	string				name;
    bool                redirect;
    ofHttpRequestType   type;
	bool				saveTo;
    int                 redirectCount;
    
    int getID(){return id;}
    void addCookie(string key, string value){cookies.insert(make_pair(key, value));}
    void addHeader(string key, string value){header.insert(make_pair(key, value));}
    void addData(string key, string value){data.insert(make_pair(key, value));}
    void addFile(string key, string path){files.insert(make_pair(key, path));}
    void setUserAgent(string value){header.insert(make_pair("User-Agent", value));}

    
    map<string, string> cookies;
    map<string, string> header;
    map<string, string> files;
    map<string, string> data;
private:
	int					id;
	static int			nextID;
    
};


class ofHttpPut : public ofHttpRequest{
    
public:
    ofHttpPut()
    :ofHttpRequest()
    {};
    
	ofHttpPut(string url,string name, bool saveTo=false)
	:ofHttpRequest(url, name, saveTo, OF_HTTP_PUT)
	{};

private:
	int					id;
	static int			nextID;
};

class ofHttpGet : public ofHttpRequest{
    
public:
    ofHttpGet()
    :ofHttpRequest()
    {};
    
	ofHttpGet(string url,string name, bool saveTo=false)
	:ofHttpRequest(url, name, saveTo, OF_HTTP_GET)
	{};
    
private:
	int					id;
	static int			nextID;
};


class ofHttpPost : public ofHttpRequest{
    
public:
    ofHttpPost()
    :ofHttpRequest()
    {};
    
	ofHttpPost(string url,string name, bool saveTo=false)
	:ofHttpRequest(url, name, saveTo, OF_HTTP_POST)
	{};
    
private:
	int					id;
	static int			nextID;
};

class ofHttpDelete : public ofHttpRequest{
    
public:
    ofHttpDelete()
    :ofHttpRequest()
    {};
    
	ofHttpDelete(string url,string name, bool saveTo=false)
	:ofHttpRequest(url, name, saveTo, OF_HTTP_DELETE)
	{};
    
private:
	int					id;
	static int			nextID;
};


class ofHttpResponse{
public:
	ofHttpResponse(){}
    
	ofHttpResponse(ofHttpRequest request,const ofBuffer & data,int status, string error)
	:request(request)
	,data(data)
	,status(status)
	,error(error){}
    
	ofHttpResponse(ofHttpRequest request,int status,string error)
	:request(request)
	,status(status)
	,error(error){}
    
	operator ofBuffer&(){
		return data;
	}
    
	ofHttpRequest	    request;
	ofBuffer		    data;
	int					status;
	string				error;
};

ofHttpResponse ofLoadURL(string url);
int ofLoadURLAsync(string url, string name=""); // returns id
int ofLoadRequestAsync(ofHttpRequest req);
ofHttpResponse ofLoadRequest(ofHttpRequest req);
ofHttpResponse ofSaveURLTo(string url, string path);
int ofSaveURLAsync(string url, string path);
void ofRemoveURLRequest(int id);
void ofRemoveAllURLRequests();

void ofStopURLLoader();

ofEvent<ofHttpResponse> & ofURLResponseEvent();
ofEvent<ofHttpResponse> & ofHTTPSuccessEvent();
ofEvent<ofHttpResponse> & ofHTTPErrorEvent();
ofEvent<ofHttpResponse> & ofHTTPRedirectEvent();


template<class T>
void ofRegisterURLNotification(T * obj){
	ofAddListener(ofURLResponseEvent(),obj,&T::urlResponse);
}

template<class T>
void ofUnregisterURLNotification(T * obj){
	ofRemoveListener(ofURLResponseEvent(),obj,&T::urlResponse);
}

template <class T>
void ofRegisterHTTPEvents(T * obj){
    ofAddListener(ofHTTPSuccessEvent(), obj, &T::httpSuccess);
    ofAddListener(ofHTTPErrorEvent(), obj, &T::httpError);
    ofAddListener(ofHTTPRedirectEvent(), obj, &T::httpRedirect);
}

template <class T>
void ofUnregisterHTTPEvents(T * obj){
    ofRemoveListener(ofHTTPSuccessEvent(), obj, &T::httpSuccess);
    ofRemoveListener(ofHTTPErrorEvent(), obj, &T::httpError);
    ofRemoveListener(ofHTTPRedirectEvent(), obj, &T::httpRedirect);
}

class ofURLFileLoader : public ofThread  {
    
public:
    
    ofURLFileLoader();
    ofHttpResponse get(string url);
    ofHttpResponse get(ofHttpRequest request);
    int getAsync(string url, string name=""); // returns id
    int getAsync(ofHttpRequest req);
    ofHttpResponse saveTo(string url, string path);
    int saveAsync(string url, string path);
    void remove(int id);
    void clear();
    void stop();
    
protected:
    
    // threading -----------------------------------------------
    void threadedFunction();
    void start();
    void update(ofEventArgs & args);  // notify in update so the notification is thread safe
    
private:
    
    // perform the requests on the thread
    ofHttpResponse handleRequest(ofHttpRequest request);
    
    deque<ofHttpRequest> requests;
    queue<ofHttpResponse> responses;
    
    Poco::Condition condition;
    
};
