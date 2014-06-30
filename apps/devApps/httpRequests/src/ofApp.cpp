#include "ofApp.h"



//string HOST = "";

// /user-agent Returns user-agent.
string USER_AGENT = "http://httpbin.org/user-agent";
// /headers Returns header dict.
string HEADER = "http://httpbin.org/headers";
// /get Returns GET data.
string GET = "http://httpbin.org/get";
// /post Returns POST data.
string POST = "http://httpbin.org/post";
// /put Returns PUT data.
string PUT = "http://httpbin.org/put";
// /delete Returns DELETE data
string DELETE = "http://httpbin.org/delete";
// /re
string REDIRECT_30 = "http://httpbin.org/redirect/30";
string REDIRECT_15 = "http://httpbin.org/redirect/15";
string REDIRECT_10 = "http://httpbin.org/redirect/10";
string REDIRECT_5 = "http://httpbin.org/redirect/5";

ofHttpRequest httpPut, httpPost, httpGet, httpDelete, httpUserAgent, httpHeaders;
ofHttpResponse currentResponse;
bool loadAsync;
//--------------------------------------------------------------
void ofApp::setup(){
    ofSetFrameRate(60);
    ofSetVerticalSync(true);
    ofBackground(4, 5, 6);
    ofRegisterURLNotification(this);
    ofSetLogLevel(OF_LOG_ERROR);
    

    httpPut.url = PUT;
    httpPut.type = OF_HTTP_PUT;
    httpPut.addData("data 1", "foobar data 1");
    httpPut.addData("data 2", "foobar data 2");
    httpPut.name = "put";
    

    
    httpGet.url = "http://www.google.com";
    httpGet.type = OF_HTTP_GET;
    httpGet.name = "get";
    
    

    httpPost.type = OF_HTTP_POST;
    httpPost.url = "http://127.0.0.1:5800/upload";
    httpPost.addFile("upload-file", ofToDataPath("2014-05-16-14-14-24-575.gif"));
    httpPost.name = "post";
    

    httpUserAgent.type = OF_HTTP_GET;
    httpUserAgent.url = USER_AGENT;
    httpUserAgent.setUserAgent("Mozilla/5.0 (Macintosh; Intel Mac OS X 10_8_2) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/28.0.1500.95 Safari/537.36");
    httpUserAgent.name = "user-agent";
    

    httpDelete.url = DELETE;
    httpDelete.type = OF_HTTP_DELETE;
    httpDelete.addData("data 1", "foobar data 1");
    httpDelete.addData("data 2", "foobar data 2");
    httpDelete.name = "delete";
    
    httpHeaders.url = HEADER;
    httpHeaders.type = OF_HTTP_GET;
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
    if(response.status >  300){
        ofLogError()<<"status: "<<response.status<<" name: "<<response.request.name<<endl;
        ofLogError()<<"data: "<<response.data<<endl;
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