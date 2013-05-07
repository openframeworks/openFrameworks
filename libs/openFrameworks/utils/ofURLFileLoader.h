#pragma once

#include <deque>
#include <queue>

#include "ofThread.h"
#include "ofEvents.h"
#include "ofFileUtils.h"

#include "Poco/Condition.h"


class ofHttpRequest{
public:
	ofHttpRequest(){};

	ofHttpRequest(string url,string name,bool saveTo=false)
	:url(url)
	,name(name)
	,saveTo(saveTo)
	,id(nextID++){}

	string				url;
	string				name;
	bool				saveTo;

	int getID(){return id;}
private:
	int					id;
	static int			nextID;
};

class ofHttpResponse{
public:
	ofHttpResponse(){}

	ofHttpResponse(ofHttpRequest request,const ofBuffer & data,int status, string error, int size)
	:request(request)
	,data(data)
	,status(status)
	,error(error)
    ,size(size)
    ,downloaded(data.size())
    ,downloading(false){}

	ofHttpResponse(ofHttpRequest request,int status,string error, int size)
	:request(request)
	,status(status)
	,error(error)
    ,size(size)
    ,downloaded(0)
    ,downloading(false){}

	operator ofBuffer&(){
		return data;
	}
    
    int getDownloaded(){
        if(request.saveTo){
            return downloaded;
        }else{
            return data.size();
        }
    }
    
    float getProgressPct(){
        if(size!=0){
            return getDownloaded()/float(size);
        }else{
            return -1;
        }
    }

	ofHttpRequest	    request;
	ofBuffer		    data;
	int					status;
	string				error;
    int                 size;
    bool                downloading;
private:
    friend class ofURLFileLoader;
    int                 downloaded;
};

ofHttpResponse ofLoadURL(string url);
int ofLoadURLAsync(string url, string name=""); // returns id
ofHttpResponse ofSaveURLTo(string url, string path);
int ofSaveURLAsync(string url, string path);
void ofRemoveURLRequest(int id);
void ofRemoveAllURLRequests();

ofEvent<ofHttpResponse> & ofURLResponseEvent();
ofEvent<ofHttpResponse> & ofURLProgressEvent();

template<class T>
void ofRegisterURLNotification(T * obj){
	ofAddListener(ofURLResponseEvent(),obj,&T::urlResponse);
}

template<class T>
void ofUnregisterURLNotification(T * obj){
	ofRemoveListener(ofURLResponseEvent(),obj,&T::urlResponse);
}


class ofURLFileLoader : public ofThread  {

    public:

        ofURLFileLoader();
        ofHttpResponse get(string url);
        int getAsync(string url, string name=""); // returns id
        ofHttpResponse saveTo(string url, string path);
        int saveAsync(string url, string path);
		void remove(int id);
		void clear();

    protected:

		// threading -----------------------------------------------
		void threadedFunction();
        void start();
        void stop();
        void update(ofEventArgs & args);  // notify in update so the notification is thread safe

    private:

		// perform the requests on the thread
        ofHttpResponse handleRequest(ofHttpRequest request);

		deque<ofHttpRequest> requests;
		queue<ofHttpResponse> responses;
        ofHttpResponse downloading;
        int prevProgress;

		Poco::Condition condition;

};
