#pragma once

#include <deque>
#include <queue>
#include <map>

#include "ofThread.h"
#include "ofEvents.h"
#include "ofFileUtils.h"

#include "Poco/Condition.h"
#include "Poco/Net/HTTPSession.h"
#include "Poco/Net/HTTPClientSession.h"
#include "Poco/Net/HTTPSClientSession.h"
#include "Poco/Net/HTTPRequest.h"
#include "Poco/Net/HTTPResponse.h"
#include "Poco/Net/HTMLForm.h"
#include "Poco/Net/HTTPBasicCredentials.h"
#include "Poco/Net/FilePartSource.h"
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
    map<string, string> header;
    map<string, string> files;
    map<string, string> data;
	bool				saveTo;
    
    int getID(){return id;}
    void setMethod(string method){this->method = method;}
    void addKeyValues(string key, string value){
        string & foo = header[key];
        foo = value;
    }
    void addData(string key, string value){
        string & foo = data[key];
        foo = value;
    }
    void addFile(string key, string path){
        string & foo = files[key];
        foo = path;
    }
    string HTTP_GET = Poco::Net::HTTPRequest::HTTP_GET;
	string HTTP_HEAD = Poco::Net::HTTPRequest::HTTP_HEAD;
	string HTTP_PUT = Poco::Net::HTTPRequest::HTTP_PUT;
    string HTTP_POST = Poco::Net::HTTPRequest::HTTP_POST;
    string HTTP_OPTIONS = Poco::Net::HTTPRequest::HTTP_OPTIONS;
    string HTTP_DELETE = Poco::Net::HTTPRequest::HTTP_DELETE;
    string HTTP_TRACE = Poco::Net::HTTPRequest::HTTP_TRACE;
    string HTTP_CONNECT = Poco::Net::HTTPRequest::HTTP_CONNECT;
    
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
