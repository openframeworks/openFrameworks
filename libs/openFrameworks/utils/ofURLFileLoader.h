#pragma once
#include "ofEvents.h"
#include "ofFileUtils.h"
#include "ofTypes.h"

/// an HTTP request, can be a GET or POST
class ofHttpRequest{
public:
	ofHttpRequest();
	ofHttpRequest(const string& url, const string& name,bool saveTo=false);

	string				url; //< request url
	string				name; //< optional name key for sorting
	bool				saveTo; //< save to a file once the request is finised?
	map<string,string>	headers; //< HTTP header keys & values
	string				body; //< POST body data
	string				contentType; //< POST data mime type

	/// \returns the unique id for this request
	int getId() const;
	OF_DEPRECATED_MSG("Use ofGetId().", int getID());

	/// HTTP request type
	enum Method{
		GET, //< requests data from a specified resource
		POST //< submits data to be processed to a specified resource
	} method;

private:
	int					id; //< unique id for this request
	static int			nextID; //< global for computing next unique id
};

/// an HTTP response to a GET or POST request
class ofHttpResponse{
public:
	ofHttpResponse();
	ofHttpResponse(const ofHttpRequest& request, const ofBuffer& data, int status, const string& error);
	ofHttpResponse(const ofHttpRequest& request, int status, const string& error);

	operator ofBuffer&();

	ofHttpRequest	    request; //< matching HTTP request for this response
	ofBuffer		    data; //< response raw data
	int					status; //< HTTP response status
	string				error; //< HTTP error string, if any
};

/// make an HTTP request for a url,
/// \returns HTTP response
ofHttpResponse ofLoadURL(const string& url);

/// make an asynchronous HTTP request for a url,
/// set optional name as a key when sorting requests later
/// \returns unique request id for the active HTTP request
int ofLoadURLAsync(const string& url, const string& name=""); // returns id

/// make an HTTP request for a url and save the response to a file at path
ofHttpResponse ofSaveURLTo(const string& url, const string& path);

/// make an asynchronous HTTP request for a url and save the response to a file at path
/// \returns unique request id for the active HTTP request
int ofSaveURLAsync(const string& url, const string& path);

/// remove an active HTTP request using it's unique id
void ofRemoveURLRequest(int id);

/// remove all active HTTP requests
void ofRemoveAllURLRequests();

/// stops & removs all active and waiting HTTP requests
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

/// loads a file from a URL using an HTTP request
class ofURLFileLoader  {
    public:
	
        ofURLFileLoader();	
	
		/// make a HTTP request for a url
		ofHttpResponse get(const string& url);
	
		/// make an asynchronous HTTP request for a url
		/// set optional name as a key when sorting requests later
		/// \returns unique request id for the finished HTTP request
        int getAsync(const string& url, const string& name=""); // returns id
	
		/// make an HTTP request for a url and save the response to a file at path
		ofHttpResponse saveTo(const string& url, const string& path);
	
		/// make an asynchronous HTTP request for a url and save the response to a file at path
		/// \returns unique request id for the finished HTTP request
		int saveAsync(const string& url, const string& path);
	
		/// remove an active HTTP request using it's unique id
		void remove(int id);
	
		/// clear all active HTTP requests
		void clear();
	
		/// stops & removes all active and waiting HTTP requests
		void stop();
	
		/// makes an HTTP request and returns an HTTP response
        ofHttpResponse handleRequest(ofHttpRequest & request);

    private:
        shared_ptr<ofBaseURLFileLoader> impl;
};
