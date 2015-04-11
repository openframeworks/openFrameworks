/*
 * ofxEmscriptenURLFileLoader.h
 *
 *  Created on: May 30, 2014
 *      Author: arturo
 */
#pragma once
#include "ofURLFileLoader.h"
#include "ofBaseTypes.h"

class ofxEmscriptenURLFileLoader: public ofBaseURLFileLoader {
public:
	ofxEmscriptenURLFileLoader();
	virtual ~ofxEmscriptenURLFileLoader();
	ofHttpResponse get(string url);
	int getAsync(string url, string name=""); // returns id
	ofHttpResponse saveTo(string url, string path);
	int saveAsync(string url, string path);
	ofHttpResponse handleRequest(ofHttpRequest request);
	void remove(int id);
	void clear();
	void stop();

private:
	static void onload_cb(unsigned, void* request, void* data, unsigned int size);
	static void onerror_cb(unsigned, void* request, int status, const char* msg);
	static void onload_file_cb(unsigned, void* request, const char* file);
	static void onerror_file_cb(unsigned, void* request, int status);
};
