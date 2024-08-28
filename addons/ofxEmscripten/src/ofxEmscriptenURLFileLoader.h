/*
 * ofxEmscriptenURLFileLoader.h
 *
 *  Created on: May 30, 2014
 *      Author: arturo
 */
#pragma once
#include "ofURLFileLoader.h"

class ofxEmscriptenURLFileLoader: public ofBaseURLFileLoader {
public:
	ofxEmscriptenURLFileLoader();
	virtual ~ofxEmscriptenURLFileLoader();
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
