#pragma once
#include "ofEvents.h"
#include "ofFileUtils.h"
#include "ofTypes.h"

/// \class ofHttpRequest
/// \brief an HTTP GET or POST request
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

	/// \return the unique id for this request
	int getId() const;
	OF_DEPRECATED_MSG("Use ofGetId().", int getID());

	/// HTTP request type
	enum Method{
		GET, //< request data from a specified resource (via url)
		POST //< submit data to be processed to a specified resource (via url)
	} method;

private:
	int					id; //< unique id for this request
	static int			nextID; //< global for computing next unique id
};

/// \class ofHttpResponse
/// \brief an HTTP response to a GET or POST request
class ofHttpResponse{
public:
	ofHttpResponse();
	ofHttpResponse(const ofHttpRequest& request, const ofBuffer& data, int status, const string& error);
	ofHttpResponse(const ofHttpRequest& request, int status, const string& error);

	operator ofBuffer&();

	ofHttpRequest	    request; //< matching HTTP request for this response
	ofBuffer		    data; //< response raw data
	int					status; //< HTTP response status (200: OK, 404: Not Found, etc)
	string				error; //< HTTP error string, if any (OK, Not Found, etc)
};

/// \brief make an HTTP GET request
/// blocks until a response is returned or the request times out
/// \param url HTTP url to request, ie. "http://somewebsite.com/someapi/someimage.jpg"
/// \returns HTTP response
ofHttpResponse ofLoadURL(const string& url);

/// \brief make an asynchronous HTTP GET request
/// will not block, placed in a queue and run using a background thread
/// \param url HTTP url to request, ie. "http://somewebsite.com/someapi/someimage.jpg"
/// \param name optional key to use when sorting requests
/// \return unique id for the active HTTP request
int ofLoadURLAsync(const string& url, const string& name=""); // returns id

/// \brief make an HTTP GET request and save the response data to a file
/// blocks until a response is returned or the request times out
/// \param url HTTP url to request, ie. "http://somewebsite.com/someapi/someimage.jpg"
/// \param path file path to save to
/// \return HTTP response on success or failure
ofHttpResponse ofSaveURLTo(const string& url, const string& path);

/// make an asynchronous HTTP request for a url and save the response to a file at path
/// \returns unique request id for the active HTTP request

/// \brief make an asynchronous HTTP request and save the response data to a file
/// will not block, placed in a queue and run using a background thread
/// \param url HTTP url to request, ie. "http://somewebsite.com/someapi/someimage.jpg"
/// \param path file path to save to
/// \returns unique id for the active HTTP request
int ofSaveURLAsync(const string& url, const string& path);

/// \brief remove an active HTTP request from the queue
/// \param unique HTTP request id
void ofRemoveURLRequest(int id);

/// \brief remove all active HTTP requests from the queue
void ofRemoveAllURLRequests();

/// \brief stop & remove all active and waiting HTTP requests
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

/// \class ofURLFileLoader
/// \brief loads a file from a URL using an HTTP request
class ofURLFileLoader  {
    public:
	
        ofURLFileLoader();	
	
		/// \brief make an HTTP request
		/// blocks until a response is returned or the request times out
		/// \param url HTTP url to request, ie. "http://somewebsite.com/someapi/someimage.jpg"
		/// \return HTTP response on success or failure
		ofHttpResponse get(const string& url);
	
		/// \brief make an asynchronous HTTP request
		/// will not block, placed in a queue and run using a background thread
		/// \param url HTTP url to request, ie. "http://somewebsite.com/someapi/someimage.jpg"
		/// \param name optional key to use when sorting requests
		/// \return unique id for the active HTTP request
        int getAsync(const string& url, const string& name="");
	
		/// \brief make an HTTP request and save the response data to a file
		/// blocks until a response is returned or the request times out
		/// \param url HTTP url to request, ie. "http://somewebsite.com/someapi/someimage.jpg"
		/// \param path file path to save to
		/// \return HTTP response on success or failure
		ofHttpResponse saveTo(const string& url, const string& path);
	
		/// \brief make an asynchronous HTTP request and save the response data to a file
		/// will not block, placed in a queue and run using a background thread
		/// \param url HTTP url to request, ie. "http://somewebsite.com/someapi/someimage.jpg"
		/// \param path file path to save to
		/// \returns unique id for the active HTTP request
		int saveAsync(const string& url, const string& path);
	
		/// \brief remove an active HTTP request from the queue
		/// \param unique HTTP request id
		void remove(int id);
	
		/// \brief clear all active HTTP requests from the queue
		void clear();
	
		/// \brief stop & remove all active and waiting HTTP requests
		void stop();
	
		// \brief low level HTTP request implementation
		/// blocks until a response is returned or the request times out
		/// \return HTTP response on success or failure
        ofHttpResponse handleRequest(ofHttpRequest & request);

    private:
        shared_ptr<ofBaseURLFileLoader> impl;
};
