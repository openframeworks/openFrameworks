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
	,id(nextID++){};
    
	ofHttpRequest(string url,string name,bool saveTo=false)
	:url(url)
	,name(name)
	,saveTo(saveTo)
	,id(nextID++){};
    
    
	string				url;
	string				name;
    string              method;
    string              host;
	bool				saveTo;
    
    int getID(){return id;}
    void addCookie(string key, string value){cookies.insert(make_pair(key, value));}
    void addHeader(string key, string value){header.insert(make_pair(key, value));}
    void addData(string key, string value){data.insert(make_pair(key, value));}
    void addFile(string key, string path){files.insert(make_pair(key, path));}
    void setUserAgent(string value){header.insert(make_pair("User-Agent", value));}
    static string OF_HTTP_GET;
	static string OF_HTTP_HEAD;
	static string OF_HTTP_PUT;
	static string OF_HTTP_POST;
	static string OF_HTTP_OPTIONS;
	static string OF_HTTP_DELETE;
	static string OF_HTTP_TRACE;
	static string OF_HTTP_CONNECT;
    

    map<string, string> cookies;
    map<string, string> header;
    map<string, string> files;
    map<string, string> data;
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
ofHttpResponse ofSaveURLTo(string url, string path);
int ofSaveURLAsync(string url, string path);
void ofRemoveURLRequest(int id);
void ofRemoveAllURLRequests();

ofEvent<ofHttpResponse> & ofURLResponseEvent();

template<class T>
void ofRegisterURLNotification(T * obj){
	ofAddListener(ofURLResponseEvent(),obj,&T::urlResponse);
}

template<class T>
void ofUnregisterURLNotification(T * obj){
	ofRemoveListener(ofURLResponseEvent(),obj,&T::urlResponse);
}


class ofURLFileLoader : public ofThread  {
    
public:
    
    ofURLFileLoader();
    ofHttpResponse get(string url);
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
