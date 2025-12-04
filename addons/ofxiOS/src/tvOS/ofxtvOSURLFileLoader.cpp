
#include "ofxtvOSURLFileLoader.h"
#if defined(TARGET_OF_IOS) && defined(TARGET_OF_TVOS) && !defined(TARGET_OF_WATCHOS) && !defined(TARGET_OF_XROS)
#include "ofLog.h"

using namespace std;

ofxtvOSURLFileLoader::ofxtvOSURLFileLoader() {
}

ofxtvOSURLFileLoader::~ofxtvOSURLFileLoader() {
}

ofHttpResponse ofxtvOSURLFileLoader::get(const string & url){
	getAsync(url,url);
	return ofHttpResponse();
}

int ofxtvOSURLFileLoader::getAsync(const string & url, const string & name){
	ofHttpRequest * req = new ofHttpRequest(url,name,false);
	return req->getId();
}

ofHttpResponse ofxtvOSURLFileLoader::saveTo(const string & url, const of::filesystem::path & path){
	saveAsync(url,path);
	return ofHttpResponse();
}

int ofxtvOSURLFileLoader::saveAsync(const string & url, const of::filesystem::path & path){
	ofHttpRequest * req = new ofHttpRequest(url,url,true);
	return 0;
}

ofHttpResponse ofxtvOSURLFileLoader::handleRequest(const ofHttpRequest & request){
	ofLogWarning() << "handleRequest is still not implemented on emscripten";
	return ofHttpResponse();
}

int ofxtvOSURLFileLoader::handleRequestAsync(const ofHttpRequest & request){
	ofLogWarning() << "handleRequest is still not implemented on emscripten";
	return -1;
}

void ofxtvOSURLFileLoader::remove(int id){

}

void ofxtvOSURLFileLoader::clear(){

}

void ofxtvOSURLFileLoader::stop(){

}

void ofxtvOSURLFileLoader::onload_cb(unsigned, void* request, void* data, unsigned int size){
	ofHttpResponse response;
	response.data.set((const char*)data,size);
	response.status = 200;
	response.request = *(ofHttpRequest*)request;
	ofNotifyEvent(ofURLResponseEvent(),response);
	delete (ofHttpRequest*)request;
}

void ofxtvOSURLFileLoader::onerror_cb(unsigned, void* request, int status, const char* message){
	ofHttpResponse response;
	response.status = status;
	response.error = message;
	response.request = *(ofHttpRequest*)request;
	ofNotifyEvent(ofURLResponseEvent(),response);
	delete (ofHttpRequest*)request;
}

void ofxtvOSURLFileLoader::onload_file_cb(unsigned, void* request, const char* file){
	ofHttpResponse response;
	response.status = 200;
	response.request = *(ofHttpRequest*)request;
	ofNotifyEvent(ofURLResponseEvent(),response);
	delete (ofHttpRequest*)request;
}

void ofxtvOSURLFileLoader::onerror_file_cb(unsigned, void* request, int status){
	ofHttpResponse response;
	response.status = status;
	response.request = *(ofHttpRequest*)request;
	ofNotifyEvent(ofURLResponseEvent(),response);
	delete (ofHttpRequest*)request;
}
#endif
