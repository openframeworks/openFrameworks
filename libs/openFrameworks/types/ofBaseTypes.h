#pragma once
#include "ofConstants.h"
#include "ofTypes.h"
#include "ofRectangle.h"
#include "ofURLFileLoader.h"
#include "ofGLUtils.h"
#include "ofGraphicsBaseTypes.h"
#include "ofGLBaseTypes.h"

class ofAbstractParameter;

template<typename T>
class ofImage_;

typedef ofImage_<unsigned char> ofImage;
typedef ofImage_<float> ofFloatImage;
typedef ofImage_<unsigned short> ofShortImage;

template<typename T>
class ofPixels_;

typedef ofPixels_<unsigned char> ofPixels;
typedef ofPixels_<float> ofFloatPixels;
typedef ofPixels_<unsigned short> ofShortPixels;
typedef ofPixels& ofPixelsRef;

template<typename T>
class ofColor_;

typedef ofColor_<unsigned char> ofColor;

class ofSoundBuffer;
enum ofLoopType: short;


bool ofIsVFlipped();








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
	virtual ofHttpResponse saveTo(const std::string& url, const std::filesystem::path& path)=0;

	/// \brief make an asynchronous HTTP request and save the response data to a file
	/// will not block, placed in a queue and run using a background thread
	/// \param url HTTP url to request, ie. "http://somewebsite.com/someapi/someimage.jpg"
	/// \param path file path to save to
	/// \returns unique id for the active HTTP request
	virtual int saveAsync(const std::string& url, const std::filesystem::path& path)=0;

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
