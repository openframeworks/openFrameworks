/*
 * ofxEmscriptenURLFileLoader.cpp
 *
 *  Created on: May 30, 2014
 *      Author: arturo
 */

#include "ofxEmscriptenURLFileLoader.h"
#include "ofLog.h"
#include <emscripten/emscripten.h>

using namespace std;

ofxEmscriptenURLFileLoader::ofxEmscriptenURLFileLoader() {
}

ofxEmscriptenURLFileLoader::~ofxEmscriptenURLFileLoader() {
}

ofHttpResponse ofxEmscriptenURLFileLoader::get(const string & url){
	getAsync(url,url);
	return ofHttpResponse();
}

int ofxEmscriptenURLFileLoader::getAsync(const string & url, const string & name){
	ofHttpRequest * req = new ofHttpRequest(url,name,false);
#if __EMSCRIPTEN_major__>1 || (__EMSCRIPTEN_major__==1 && __EMSCRIPTEN_minor__>22)
	emscripten_async_wget2_data(url.c_str(), "GET", "", req, true, &onload_cb, &onerror_cb, NULL);
#endif
	return req->getId();
}

ofHttpResponse ofxEmscriptenURLFileLoader::saveTo(const string & url, const of::filesystem::path & path){
	saveAsync(url,path);
	return ofHttpResponse();
}

int ofxEmscriptenURLFileLoader::saveAsync(const string & url, const of::filesystem::path & path){
	ofHttpRequest * req = new ofHttpRequest(url,url,true);
#if __EMSCRIPTEN_major__>1 || (__EMSCRIPTEN_major__==1 && __EMSCRIPTEN_minor__>22)
	emscripten_async_wget2(url.c_str(), path.c_str(), "GET", "", req, &onload_file_cb, &onerror_file_cb, NULL);
#endif
	return 0;
}

ofHttpResponse ofxEmscriptenURLFileLoader::handleRequest(const ofHttpRequest & request){
	ofLogWarning() << "handleRequest is still not implemented on emscripten";
	return ofHttpResponse();
}

int ofxEmscriptenURLFileLoader::handleRequestAsync(const ofHttpRequest & request){
	ofLogWarning() << "handleRequest is still not implemented on emscripten";
	return -1;
}

void ofxEmscriptenURLFileLoader::remove(int id){

}

void ofxEmscriptenURLFileLoader::clear(){

}

void ofxEmscriptenURLFileLoader::stop(){

}

void ofxEmscriptenURLFileLoader::onload_cb(unsigned, void* request, void* data, unsigned int size){
	ofHttpResponse response;
	response.data.set((const char*)data,size);
	response.status = 200;
	response.request = *(ofHttpRequest*)request;
	ofNotifyEvent(ofURLResponseEvent(),response);
	delete (ofHttpRequest*)request;
}

void ofxEmscriptenURLFileLoader::onerror_cb(unsigned, void* request, int status, const char* message){
	ofHttpResponse response;
	response.status = status;
	response.error = message;
	response.request = *(ofHttpRequest*)request;
	ofNotifyEvent(ofURLResponseEvent(),response);
	delete (ofHttpRequest*)request;
}

void ofxEmscriptenURLFileLoader::onload_file_cb(unsigned, void* request, const char* file){
	ofHttpResponse response;
	response.status = 200;
	response.request = *(ofHttpRequest*)request;
	ofNotifyEvent(ofURLResponseEvent(),response);
	delete (ofHttpRequest*)request;
}

void ofxEmscriptenURLFileLoader::onerror_file_cb(unsigned, void* request, int status){
	ofHttpResponse response;
	response.status = status;
	response.request = *(ofHttpRequest*)request;
	ofNotifyEvent(ofURLResponseEvent(),response);
	delete (ofHttpRequest*)request;
}
