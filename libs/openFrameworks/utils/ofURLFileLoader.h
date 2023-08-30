#pragma once

#include "ofEvents.h"
// ofBuffer only
#include "ofFileUtils.h"
#include "ofConstants.h"
#include <map>

class ofHttpResponse;

/// \class ofHttpRequest
/// \brief an HTTP GET or POST request
class ofHttpRequest{
public:
	ofHttpRequest();
	ofHttpRequest(const std::string& url, const std::string& name,bool saveTo=false);

	std::string				url; ///< request url
	std::string				name; ///< optional name key for sorting
	bool				saveTo; ///< save to a file once the request is finised?
	std::map<std::string,std::string>	headers; ///< HTTP header keys & values
	std::string				body; ///< POST body data
	std::string				contentType; ///< POST data mime type
	std::function<void(const ofHttpResponse&)> done;
    size_t              timeoutSeconds = 0;

	/// \return the unique id for this request
	int getId() const;
	OF_DEPRECATED_MSG("Use getId().", int getID());

	/// HTTP request type
	enum Method{
		GET, ///< request data from a specified resource (via url)
		POST ///< submit data to be processed to a specified resource (via url)
	} method;

private:
	int					id; ///< unique id for this request
	static int			nextID; ///< global for computing next unique id
};

/// \class ofHttpResponse
/// \brief an HTTP response to a GET or POST request
class ofHttpResponse{
public:
	ofHttpResponse();
	ofHttpResponse(const ofHttpRequest& request, const ofBuffer& data, int status, const std::string& error);
	ofHttpResponse(const ofHttpRequest& request, int status, const std::string& error);

	operator ofBuffer&();

	ofHttpRequest	    request; ///< matching HTTP request for this response
	ofBuffer		    data; ///< response raw data
	int					status; ///< HTTP response status (200: OK, 404: Not Found, etc)
	std::string				error; ///< HTTP error string, if any (OK, Not Found, etc)
};

/// \brief make an HTTP GET request
/// blocks until a response is returned or the request times out
/// \param url HTTP url to request, ie. "http://somewebsite.com/someapi/someimage.jpg"
/// \returns HTTP response
ofHttpResponse ofLoadURL(const std::string& url);

/// \brief make an asynchronous HTTP GET request
/// will not block, placed in a queue and run using a background thread
/// \param url HTTP url to request, ie. "http://somewebsite.com/someapi/someimage.jpg"
/// \param name optional key to use when sorting requests
/// \return unique id for the active HTTP request
int ofLoadURLAsync(const std::string& url, const std::string& name=""); // returns id

/// \brief make an HTTP GET request and save the response data to a file
/// blocks until a response is returned or the request times out
/// \param url HTTP url to request, ie. "http://somewebsite.com/someapi/someimage.jpg"
/// \param path file path to save to
/// \return HTTP response on success or failure
ofHttpResponse ofSaveURLTo(const std::string& url, const of::filesystem::path& path);

/// make an asynchronous HTTP request for a url and save the response to a file at path
/// \returns unique request id for the active HTTP request

/// \brief make an asynchronous HTTP request and save the response data to a file
/// will not block, placed in a queue and run using a background thread
/// \param url HTTP url to request, ie. "http://somewebsite.com/someapi/someimage.jpg"
/// \param path file path to save to
/// \returns unique id for the active HTTP request
int ofSaveURLAsync(const std::string& url, const of::filesystem::path& path);

/// \brief remove an active HTTP request from the queue
/// \param id HTTP request id
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
		ofHttpResponse get(const std::string& url);
	
		/// \brief make an asynchronous HTTP request
		/// will not block, placed in a queue and run using a background thread
		/// \param url HTTP url to request, ie. "http://somewebsite.com/someapi/someimage.jpg"
		/// \param name optional key to use when sorting requests
		/// \return unique id for the active HTTP request
        int getAsync(const std::string& url, const std::string& name="");
	
		/// \brief make an HTTP request and save the response data to a file
		/// blocks until a response is returned or the request times out
		/// \param url HTTP url to request, ie. "http://somewebsite.com/someapi/someimage.jpg"
		/// \param path file path to save to
		/// \return HTTP response on success or failure
        ofHttpResponse saveTo(const std::string& url, const of::filesystem::path& path);
	
		/// \brief make an asynchronous HTTP request and save the response data to a file
		/// will not block, placed in a queue and run using a background thread
		/// \param url HTTP url to request, ie. "http://somewebsite.com/someapi/someimage.jpg"
		/// \param path file path to save to
		/// \returns unique id for the active HTTP request
        int saveAsync(const std::string& url, const of::filesystem::path& path);
	
		/// \brief remove an active HTTP request from the queue
		/// \param id HTTP request id
		void remove(int id);
	
		/// \brief clear all active HTTP requests from the queue
		void clear();
	
		/// \brief stop & remove all active and waiting HTTP requests
		void stop();
	
		// \brief low level HTTP request implementation
		/// blocks until a response is returned or the request times out
		/// \return HTTP response on success or failure
        ofHttpResponse handleRequest(const ofHttpRequest & request);
	
		// \brief low level HTTP request implementation
		/// this is a non-blocking version of handleRequest that will return a response in the urlResponse callback
		/// \return unique id of the active HTTP request
        int handleRequestAsync(const ofHttpRequest& request);

    private:
	std::shared_ptr<ofBaseURLFileLoader> impl;
};


/// \class ofBaseURLFileLoader
/// \brief loads a file from a URL using an HTTP request
class ofBaseURLFileLoader{
public:

	virtual ~ofBaseURLFileLoader(){};

	/// \brief make an HTTP request
	/// blocks until a response is returned or the request times out
	/// \param url HTTP url to request, ie. "http://somewebsite.com/someapi/someimage.jpg"
	/// \return HTTP response on success or failure
	virtual ofHttpResponse get(const std::string& url)=0;

	/// \brief make an asynchronous HTTP request
	/// will not block, placed in a queue and run using a background thread
	/// \param url HTTP url to request, ie. "http://somewebsite.com/someapi/someimage.jpg"
	/// \param name optional key to use when sorting requests
	/// \return unique id for the active HTTP request
	virtual int getAsync(const std::string& url, const std::string& name="")=0;

	/// \brief make an HTTP request and save the response data to a file
	/// blocks until a response is returned or the request times out
	/// \param url HTTP url to request, ie. "http://somewebsite.com/someapi/someimage.jpg"
	/// \param path file path to save to
	/// \return HTTP response on success or failure
	virtual ofHttpResponse saveTo(const std::string& url, const of::filesystem::path& path)=0;

	/// \brief make an asynchronous HTTP request and save the response data to a file
	/// will not block, placed in a queue and run using a background thread
	/// \param url HTTP url to request, ie. "http://somewebsite.com/someapi/someimage.jpg"
	/// \param path file path to save to
	/// \returns unique id for the active HTTP request
	virtual int saveAsync(const std::string& url, const of::filesystem::path& path)=0;

	/// \brief remove an active HTTP request from the queue
	/// \param unique HTTP request id
	virtual void remove(int id)=0;

	/// \brief clear all active HTTP requests from the queue
	virtual void clear()=0;

	/// \brief stop & remove all active and waiting HTTP requests
	virtual void stop()=0;

	/// \brief low level HTTP request implementation
	/// blocks until a response is returned or the request times out
	/// \return HTTP response on success or failure
	virtual ofHttpResponse handleRequest(const ofHttpRequest & request) = 0;
	virtual int handleRequestAsync(const ofHttpRequest& request)=0; // returns id

};
