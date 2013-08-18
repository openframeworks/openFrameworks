#include "ofApp.h"



string HOST = "http://httpbin.org";

// /user-agent Returns user-agent.
string USER_AGENT = "/user-agent";
// /headers Returns header dict.
string HEADER = "/headers";
// /get Returns GET data.
string GET = "/get";
// /post Returns POST data.
string POST = "/post";
// /put Returns PUT data.
string PUT = "/put";
// /delete Returns DELETE data
string DELETE = "/delete";

ofHttpRequest httpPut, httpPost, httpGet, httpDelete, httpUserAgent, httpHeaders;
ofHttpResponse currentResponse;
bool loadAsync;
//--------------------------------------------------------------
void ofApp::setup(){
    ofSetFrameRate(60);
    ofSetVerticalSync(true);
    ofBackground(4, 5, 6);
    ofRegisterURLNotification(this);
    ofSetLogLevel(OF_LOG_VERBOSE);
    

    httpPut.url = HOST+PUT;
    httpPut.method = ofHttpRequest::OF_HTTP_PUT;
    httpPut.addData("data 1", "foobar data 1");
    httpPut.addData("data 2", "foobar data 2");
    httpPut.name = "put";
    

    httpGet.method = ofHttpRequest::OF_HTTP_GET;
    httpGet.url = HOST+GET;
    httpGet.addData("data 1", "foobar data 1");
    httpGet.addData("data 2", "foobar data 2");
    httpGet.name = "get";
    

    httpPost.method = ofHttpRequest::OF_HTTP_POST;
    httpPost.url = HOST+POST;
    httpPost.addData("data 1", "foobar data 1");
    httpPost.addData("data 2", "foobar data 2");
    httpPost.name = "post";
    

    httpUserAgent.method = ofHttpRequest::OF_HTTP_GET;
    httpUserAgent.url = HOST+USER_AGENT;
    httpUserAgent.setUserAgent("Mozilla/5.0 (Macintosh; Intel Mac OS X 10_8_2) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/28.0.1500.95 Safari/537.36");
    httpUserAgent.name = "user-agent";
    

    httpDelete.url = HOST+DELETE;
    httpDelete.method = ofHttpRequest::OF_HTTP_DELETE;
    httpDelete.addData("data 1", "foobar data 1");
    httpDelete.addData("data 2", "foobar data 2");
    httpDelete.name = "delete";
    
    httpHeaders.url = HOST+HEADER;
    httpHeaders.method = ofHttpRequest::OF_HTTP_GET;
    httpHeaders.addCookie("_gauges_unique_hour", "1");
    httpHeaders.addCookie("_gauges_unique_day", "1");
    httpHeaders.addCookie("_gauges_unique_month", "1");
    httpHeaders.addCookie("_gauges_unique_year", "1");
    httpHeaders.addCookie("_gauges_unique", "1");
    httpHeaders.addHeader("Accept-Encoding", "gzip,deflate,sdch");
    httpUserAgent.setUserAgent("Mozilla/5.0 (Macintosh; Intel Mac OS X 10_8_2) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/28.0.1500.95 Safari/537.36");
    httpHeaders.name = "headers";
    
    loadAsync = true;
}


void ofApp::urlResponse(ofHttpResponse & response)
{
    if(response.status > 300){
        ofLogError()<<"status: "<<response.status<<" name: "<<response.request.name<<endl;
    }
    currentResponse = response;
    
}


//--------------------------------------------------------------
void ofApp::update(){

}


//--------------------------------------------------------------
void ofApp::draw(){
    ofSetColor(255, 255, 0);
    ofDrawBitmapString("name: "+currentResponse.request.name, ofPoint(50, 50));
    ofDrawBitmapString("status: "+ofToString(currentResponse.status), ofPoint(50, 150));
    ofDrawBitmapString("data: "+ofToString(currentResponse.data), ofPoint(50, 250));
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

    
    if(key == '0'){
        loadAsync = !loadAsync;
    }
    if(key == '1'){
        if(loadAsync)
            async(httpGet);
        else
            sync(httpGet);
    }
    if(key == '2'){
        if(loadAsync)
            async(httpPut);
        else
            sync(httpPut);
        
    }
    if(key == '3'){
        if(loadAsync)
            async(httpDelete);
        else
            sync(httpDelete);
      
    }
    if(key == '4'){
        if(loadAsync)
            async(httpPut);
        else
            sync(httpPut);
       
    }
    if(key == '5'){
        if(loadAsync)
            async(httpPost);
        else
            sync(httpPost);
      
    }
    if(key == '6'){
        if(loadAsync)
            async(httpHeaders);
        else
            sync(httpHeaders);
        
    }
    if(key == '7'){
        if(loadAsync)
            async(httpUserAgent);
        else
            sync(httpUserAgent);
    }
    

}
void ofApp::async(ofHttpRequest req){
    ofLoadRequestAsync(req);
    
}
void ofApp::sync(ofHttpRequest req){
    currentResponse = ofLoadRequest(req);
}