/*
 * ofxEmscriptenURLFileLoader.cpp
 *
 *  Created on: May 30, 2014
 *      Author: arturo
 *  Updated for Emscripten 3.1+ fetch + ASYNCIFY (6.0.6)
 */

#include "ofxEmscriptenURLFileLoader.h"
#include "ofLog.h"
#include <emscripten/emscripten.h>
#include <emscripten/fetch.h>
#include <cstring>

using namespace std;

ofxEmscriptenURLFileLoader::ofxEmscriptenURLFileLoader() {
}

ofxEmscriptenURLFileLoader::~ofxEmscriptenURLFileLoader() {
}

ofHttpResponse ofxEmscriptenURLFileLoader::get(const string & url){
	// Sync version via ASYNCIFY fetch (blocks with emscripten_fetch_wait if needed)
	// For now, delegate to async and return empty (maintains old API)
	getAsync(url,url);
	return ofHttpResponse();
}

int ofxEmscriptenURLFileLoader::getAsync(const string & url, const string & name){
	ofHttpRequest * req = new ofHttpRequest(url,name,false);
#if defined(__EMSCRIPTEN_MAJOR__) && __EMSCRIPTEN_MAJOR__ >= 3
	emscripten_fetch_attr_t attr;
	emscripten_fetch_attr_init(&attr);
	strcpy(attr.requestMethod, "GET");
	attr.attributes = EMSCRIPTEN_FETCH_LOAD_TO_MEMORY;
	attr.onsuccess = onload_fetch;
	attr.onerror = onerror_fetch;
	attr.userData = req;
	emscripten_fetch(&attr, url.c_str());
#elif __EMSCRIPTEN_major__>1 || (__EMSCRIPTEN_major__==1 && __EMSCRIPTEN_minor__>22)
	emscripten_async_wget2_data(url.c_str(), "GET", "", req, true, &onload_cb, &onerror_cb, NULL);
#else
	ofLogWarning("ofxEmscriptenURLFileLoader") << "emscripten_async_wget2 not available on this Emscripten version";
	delete req;
	return -1;
#endif
	return req->getId();
}

ofHttpResponse ofxEmscriptenURLFileLoader::saveTo(const string & url, const of::filesystem::path & path){
	saveAsync(url,path);
	return ofHttpResponse();
}

int ofxEmscriptenURLFileLoader::saveAsync(const string & url, const of::filesystem::path & path){
	ofHttpRequest * req = new ofHttpRequest(url,url,true);
#if defined(__EMSCRIPTEN_MAJOR__) && __EMSCRIPTEN_MAJOR__ >= 3
	emscripten_fetch_attr_t attr;
	emscripten_fetch_attr_init(&attr);
	strcpy(attr.requestMethod, "GET");
	attr.attributes = EMSCRIPTEN_FETCH_LOAD_TO_MEMORY | EMSCRIPTEN_FETCH_PERSIST_FILE;
	attr.onsuccess = onload_file_fetch;
	attr.onerror = onerror_file_fetch;
	attr.userData = req;
	// For fetch persist, set destination
	attr.destinationPath = strdup(path.string().c_str());
	emscripten_fetch(&attr, url.c_str());
#elif __EMSCRIPTEN_major__>1 || (__EMSCRIPTEN_major__==1 && __EMSCRIPTEN_minor__>22)
	emscripten_async_wget2(url.c_str(), path.c_str(), "GET", "", req, &onload_file_cb, &onerror_file_cb, NULL);
#else
	ofLogWarning("ofxEmscriptenURLFileLoader") << "emscripten_async_wget2 not available";
	delete req;
	return -1;
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

void ofxEmscriptenURLFileLoader::onload_fetch(emscripten_fetch_t *fetch){
	ofHttpRequest * req = (ofHttpRequest*)fetch->userData;
	ofHttpResponse response;
	response.data.set((const char*)fetch->data, fetch->numBytes);
	response.status = 200;
	response.request = *req;
	ofNotifyEvent(ofURLResponseEvent(),response);
	delete req;
	emscripten_fetch_close(fetch);
}

void ofxEmscriptenURLFileLoader::onerror_fetch(emscripten_fetch_t *fetch){
	ofHttpRequest * req = (ofHttpRequest*)fetch->userData;
	ofHttpResponse response;
	response.status = fetch->status;
	response.error = fetch->statusText;
	response.request = *req;
	ofNotifyEvent(ofURLResponseEvent(),response);
	delete req;
	emscripten_fetch_close(fetch);
}

void ofxEmscriptenURLFileLoader::onload_file_fetch(emscripten_fetch_t *fetch){
	ofHttpRequest * req = (ofHttpRequest*)fetch->userData;
	ofHttpResponse response;
	response.status = 200;
	response.request = *req;
	ofNotifyEvent(ofURLResponseEvent(),response);
	delete req;
	emscripten_fetch_close(fetch);
}

void ofxEmscriptenURLFileLoader::onerror_file_fetch(emscripten_fetch_t *fetch){
	ofHttpRequest * req = (ofHttpRequest*)fetch->userData;
	ofHttpResponse response;
	response.status = fetch->status;
	response.request = *req;
	ofNotifyEvent(ofURLResponseEvent(),response);
	delete req;
	emscripten_fetch_close(fetch);
}
