
#pragma once

#include "ofURLFileLoader.h"
#include "ofxiOSConstants.h"
#if defined(TARGET_OF_IOS) && defined(TARGET_OF_TVOS) && !defined(TARGET_OF_WATCHOS) && !defined(TARGET_OF_XROS)

class ofxtvOSURLFileLoader: public ofBaseURLFileLoader {
public:
	ofxtvOSURLFileLoader();
	virtual ~ofxtvOSURLFileLoader();
	ofHttpResponse get(const std::string & url);
	int getAsync(const std::string & url, const std::string & name=""); // returns id
	ofHttpResponse saveTo(const std::string & url, const of::filesystem::path & path);
	int saveAsync(const std::string & url, const of::filesystem::path & path);
	ofHttpResponse handleRequest(const ofHttpRequest & request);
	int handleRequestAsync(const ofHttpRequest & request);
	void remove(int id);
	void clear();
	void stop();

private:
	static void onload_cb(unsigned, void* request, void* data, unsigned int size);
	static void onerror_cb(unsigned, void* request, int status, const char* msg);
	static void onload_file_cb(unsigned, void* request, const char* file);
	static void onerror_file_cb(unsigned, void* request, int status);
};

#endif
