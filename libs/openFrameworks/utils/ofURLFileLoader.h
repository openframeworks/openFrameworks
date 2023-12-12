#pragma once

#include "ofEvents.h"
// ofBuffer only
#include "ofFileUtils.h"
#include <map>

class ofHttpResponse;

/// \class ofHttpRequest
/// \brief An HTTP GET or POST request.
class ofHttpRequest {
public:
	ofHttpRequest();
	ofHttpRequest(const std::string & url, const std::string & name, bool saveTo = false);

	std::string url; ///< request url
	std::string name; ///< optional name key for sorting
	bool saveTo; ///< save to a file once the request is finished?
	std::map<std::string, std::string> headers; ///< HTTP header keys & values
	std::string body; ///< POST body data
	std::string contentType; ///< POST data mime type
	std::function<void(const ofHttpResponse &)> done;
	size_t timeoutSeconds = 0;

	/// \return the unique id for this request
	int getId() const;
	[[deprecated("Use getId().")]]
	int getID();

	/// HTTP request type
	enum Method {
		GET, ///< request data from a specified resource (via url)
		POST ///< submit data to be processed to a specified resource (via url)
	} method;

private:
	int id; ///< unique id for this request
	static int nextID; ///< global for computing next unique id
};

/// \class ofHttpResponse
/// \brief An HTTP response to a GET or POST request.
class ofHttpResponse {
public:
	ofHttpResponse();
	ofHttpResponse(const ofHttpRequest & request, const ofBuffer & data, int status, const std::string & error);
	ofHttpResponse(const ofHttpRequest & request, int status, const std::string & error);

	operator ofBuffer &();

	ofHttpRequest request; ///< matching HTTP request for this response
	ofBuffer data; ///< response raw data
	int status; ///< HTTP response status (200: OK, 404: Not Found, etc)
	std::string error; ///< HTTP error string, if any (OK, Not Found, etc)
};

/// \brief Make an HTTP GET request.
///
/// Blocks until a response is returned or the request times out.
///
/// \param url HTTP url to request, ie. "http://somewebsite.com/someapi/someimage.jpg"
/// \return HTTP response
ofHttpResponse ofLoadURL(const std::string & url);

/// \brief Make an asynchronous HTTP GET request.
///
/// Asynchronous requests will not block as they are placed in a queue and run
/// using a background thread. This also means, however, that the response may
/// not be available after the this function returns.
///
/// \param url HTTP url to request, ie. "http://somewebsite.com/someapi/someimage.jpg"
/// \param name optional key to use when sorting requests
/// \return unique id for the active HTTP request
int ofLoadURLAsync(const std::string & url, const std::string & name = ""); // returns id

/// \brief Make an HTTP GET request and save the response data to a file.
///
/// Blocks until a response is returned or the request times out.
///
/// \param url HTTP url to request, ie. "http://somewebsite.com/someapi/someimage.jpg"
/// \param path file path to save to
/// \return HTTP response on success or failure
ofHttpResponse ofSaveURLTo(const std::string & url, const of::filesystem::path & path);

/// \brief Make an asynchronous HTTP request and save the response data to a
/// file.
///
/// Asynchronous requests will not block as they are placed in a queue and run
/// using a background thread. This also means, however, that the response may
/// not be available after the this function returns.
///
/// \param url HTTP url to request, ie. "http://somewebsite.com/someapi/someimage.jpg"
/// \param path file path to save to
/// \return unique id for the active HTTP request
int ofSaveURLAsync(const std::string & url, const of::filesystem::path & path);

/// \brief Remove an active HTTP request from the queue.
/// \param id HTTP request id
void ofRemoveURLRequest(int id);

/// \brief Remove all active HTTP requests from the queue.
void ofRemoveAllURLRequests();

/// \brief Stop & remove all active and waiting HTTP requests.
void ofStopURLLoader();

ofEvent<ofHttpResponse> & ofURLResponseEvent();

template <class T>
void ofRegisterURLNotification(T * obj) {
	ofAddListener(ofURLResponseEvent(), obj, &T::urlResponse);
}

template <class T>
void ofUnregisterURLNotification(T * obj) {
	ofRemoveListener(ofURLResponseEvent(), obj, &T::urlResponse);
}

class ofBaseURLFileLoader;

/// \class ofURLFileLoader
/// \brief Loads a file from a URL using an HTTP request.
class ofURLFileLoader {
public:
	ofURLFileLoader();

	/// \brief Make an HTTP request.
	///
	/// Blocks until a response is returned or the request times out.
	///
	/// \param url HTTP url to request, ie. "http://somewebsite.com/someapi/someimage.jpg"
	/// \return HTTP response on success or failure
	ofHttpResponse get(const std::string & url);

	/// \brief Make an asynchronous HTTP request.
	///
	/// Will not block, placed in a queue and run using a background thread.
	///
	/// \param url HTTP url to request, ie. "http://somewebsite.com/someapi/someimage.jpg"
	/// \param name optional key to use when sorting requests
	/// \return unique id for the active HTTP request
	int getAsync(const std::string & url, const std::string & name = "");

	/// \brief Make an HTTP request and save the response data to a file.
	///
	/// Blocks until a response is returned or the request times out.
	///
	/// \param url HTTP url to request, ie. "http://somewebsite.com/someapi/someimage.jpg"
	/// \param path file path to save to
	/// \return HTTP response on success or failure
	ofHttpResponse saveTo(const std::string & url, const of::filesystem::path & path);

	/// \brief Make an asynchronous HTTP request and save the response data to a file.
	///
	/// Will not block, placed in a queue and run using a background thread.
	///
	/// \param url HTTP url to request, ie. "http://somewebsite.com/someapi/someimage.jpg"
	/// \param path file path to save to
	/// \returns unique id for the active HTTP request
	int saveAsync(const std::string & url, const of::filesystem::path & path);

	/// \brief Remove an active HTTP request from the queue.
	/// \param id HTTP request id
	void remove(int id);

	/// \brief Clear all active HTTP requests from the queue.
	void clear();

	/// \brief Stop & remove all active and waiting HTTP requests.
	void stop();

	/// \brief Low level HTTP request implementation.
	///
	/// Blocks until a response is returned or the request times out.
	///
	/// \return HTTP response on success or failure
	ofHttpResponse handleRequest(const ofHttpRequest & request);

	/// \brief Low level HTTP request asynchronous implementation.
	///
	/// This is a non-blocking version of handleRequest that will return a
	/// response in the urlResponse callback.
	///
	/// \return unique id of the active HTTP request
	int handleRequestAsync(const ofHttpRequest & request);

private:
	std::shared_ptr<ofBaseURLFileLoader> impl;
};

/// \class ofBaseURLFileLoader
/// \brief Loads a file from a URL using an HTTP request.
class ofBaseURLFileLoader {
public:
	virtual ~ofBaseURLFileLoader() {};

	/// \brief Make an HTTP request.
	///
	/// Blocks until a response is returned or the request times out.
	///
	/// \param url HTTP url to request, ie. "http://somewebsite.com/someapi/someimage.jpg"
	/// \return HTTP response on success or failure
	virtual ofHttpResponse get(const std::string & url) = 0;

	/// \brief Make an asynchronous HTTP request.
	///
	/// Will not block, placed in a queue and run using a background thread.
	///
	/// \param url HTTP url to request, ie. "http://somewebsite.com/someapi/someimage.jpg"
	/// \param name optional key to use when sorting requests
	/// \return unique id for the active HTTP request
	virtual int getAsync(const std::string & url, const std::string & name = "") = 0;

	/// \brief Make an HTTP request and save the response data to a file.
	///
	/// Blocks until a response is returned or the request times out.
	///
	/// \param url HTTP url to request, ie. "http://somewebsite.com/someapi/someimage.jpg"
	/// \param path file path to save to
	/// \return HTTP response on success or failure
	virtual ofHttpResponse saveTo(const std::string & url, const of::filesystem::path & path) = 0;

	/// \brief Make an asynchronous HTTP request and save the response data to a file.
	///
	/// Will not block, placed in a queue and run using a background thread.
	///
	/// \param url HTTP url to request, ie. "http://somewebsite.com/someapi/someimage.jpg"
	/// \param path file path to save to
	/// \return unique id for the active HTTP request
	virtual int saveAsync(const std::string & url, const of::filesystem::path & path) = 0;

	/// \brief Remove an active HTTP request from the queue.
	/// \param id HTTP request id
	virtual void remove(int id) = 0;

	/// \brief Clear all active HTTP requests from the queue.
	virtual void clear() = 0;

	/// \brief Stop & remove all active and waiting HTTP requests.
	virtual void stop() = 0;

	/// \brief Low level HTTP request implementation.
	///
	/// Blocks until a response is returned or the request times out.
	///
	/// \return HTTP response on success or failure
	virtual ofHttpResponse handleRequest(const ofHttpRequest & request) = 0;

	/// \brief Low level HTTP request asynchronous implementation.
	///
	/// This is a non-blocking version of handleRequest that will return a
	/// response in the urlResponse callback.
	///
	/// \return unique id of the active HTTP request
	virtual int handleRequestAsync(const ofHttpRequest & request) = 0;
};
