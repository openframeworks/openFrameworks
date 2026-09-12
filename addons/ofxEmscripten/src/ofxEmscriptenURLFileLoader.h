/*
 * ofxEmscriptenURLFileLoader.h
 *
 *  Created on: May 30, 2014
 *      Author: arturo
 *  Updated for Emscripten 3.1+ fetch + ASYNCIFY (6.0.6)
 */
#pragma once
#include "ofURLFileLoader.h"
struct emscripten_fetch_t;

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
	static void onload_fetch(emscripten_fetch_t *fetch);
	static void onerror_fetch(emscripten_fetch_t *fetch);
	static void onload_file_fetch(emscripten_fetch_t *fetch);
	static void onerror_file_fetch(emscripten_fetch_t *fetch);
};
