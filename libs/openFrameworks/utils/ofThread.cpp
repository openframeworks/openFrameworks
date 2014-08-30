#include "ofThread.h"
#include "ofLog.h"
#include "ofUtils.h"

#ifdef TARGET_ANDROID
#include <jni.h>
#include "ofxAndroidUtils.h"
#endif


//-------------------------------------------------
ofThread::ofThread()
:_threadRunning(false)
,_mutexBlocks(true)
,threadBeingWaitedFor(false){
   thread.setName("Thread " + ofToString(thread.id()));
}


//-------------------------------------------------
ofThread::~ofThread(){
}


//-------------------------------------------------
bool ofThread::isThreadRunning() const{
    return _threadRunning;
}


//-------------------------------------------------
int ofThread::getThreadId() const{
	return thread.id();
}


//-------------------------------------------------
std::string ofThread::getThreadName() const{
	return thread.name();
}


//-------------------------------------------------
void ofThread::startThread(bool mutexBlocks){
	if(thread.isRunning()){
		ofLogWarning("ofThread") << "- name: " << getThreadName() << " - Cannot start, thread already running.";
		return;
	}

    _threadRunning = true;
    _mutexBlocks = mutexBlocks;

	thread.start(*this);
}


//-------------------------------------------------
void ofThread::startThread(bool mutexBlocks, bool verbose){
    ofLogWarning("ofThread") << "- name: " << getThreadName() << " - Calling startThread with verbose is deprecated.";
    startThread(mutexBlocks);
}


//-------------------------------------------------
bool ofThread::lock(){
	if(_mutexBlocks){
        if(isCurrentThread()){
            ofLogVerbose("ofThread") << "- name: " << getThreadName() << " - ofThread waiting for its own mutex to be unlocked.";
        }else{
            ofLogVerbose("ofThread") << "- name: " << getThreadName() << " - External thread waiting for ofThread mutex to be unlocked";
        }
		mutex.lock();

	}else{
		if(!mutex.tryLock()){
			ofLogVerbose("ofThread") << "- name: " << getThreadName() << " - Mutex is already locked, tryLock failed.";
			return false;
		}
	}

    if(isCurrentThread()){
        ofLogVerbose("ofThread") << "- name: " << getThreadName() << " - ofThread locked its own mutex.";
    }else{
        ofLogVerbose("ofThread") << "- name: " << getThreadName() << " - External thread locked the ofThread mutex.";
    }

	return true;
}


//-------------------------------------------------
void ofThread::unlock(){
	mutex.unlock();

    if(isCurrentThread()){
        ofLogVerbose("ofThread") << "- name: " << getThreadName() << " - ofThread unlocked its own mutex.";
    } else {
        ofLogVerbose("ofThread") << "- name: " << getThreadName() << " - External thread unlocked the ofThread mutex.";
    }
}


//-------------------------------------------------
void ofThread::stopThread(){
    _threadRunning = false;
}


//-------------------------------------------------
void ofThread::waitForThread(bool callStopThread, long milliseconds){
	if(thread.isRunning()){
		threadBeingWaitedFor = true;

		// tell thread to stop
		if(callStopThread){
            stopThread();
			ofLogVerbose("ofThread") << "- name: " << getThreadName() << " - Signaled to stop.";
		}

		// wait for the thread to finish
		ofLogVerbose("ofThread") << "- name: " << getThreadName() << " - waiting to stop";

        if(isCurrentThread()){
			ofLogWarning("ofThread") << "- name: " << getThreadName() << " - waitForThread should only be called from outside the this ofThread.";
			return;
		}

        if (INFINITE_JOIN_TIMEOUT == milliseconds){
            thread.join();
        }else{
            // Wait for "joinWaitMillis" milliseconds for thread to finish
            if(!thread.tryJoin(milliseconds)){
                ofLogError("ofThread") << "- name: " << getThreadName() << " - Unable to completely waitForThread.";
            }
        }
    }
}


//-------------------------------------------------
void ofThread::sleep(long milliseconds){
	Poco::Thread::sleep(milliseconds);
}


//-------------------------------------------------
void ofThread::yield(){
	Poco::Thread::yield();
}


//-------------------------------------------------
bool ofThread::isCurrentThread() const{
    return ofThread::getCurrentPocoThread() == &getPocoThread();
}


//-------------------------------------------------
Poco::Thread& ofThread::getPocoThread(){
	return thread;
}


//-------------------------------------------------
const Poco::Thread& ofThread::getPocoThread() const{
	return thread;
}


//-------------------------------------------------
bool ofThread::isMainThread(){
    return !Poco::Thread::current();
}


//-------------------------------------------------
ofThread* ofThread::getCurrentThread(){
	// assumes all created threads are ofThreads ...
	// might be dangerous if people are using Poco::Threads directly
	return (ofThread*) Poco::Thread::current();
}


//-------------------------------------------------
Poco::Thread* ofThread::getCurrentPocoThread(){
	return Poco::Thread::current();
}


//-------------------------------------------------
void ofThread::threadedFunction(){
	ofLogWarning("ofThread") << "- name: " << getThreadName() << " - Override ofThread::threadedFunction() in your ofThread subclass.";
}


//-------------------------------------------------
void ofThread::run(){
	ofLogVerbose("ofThread") << "- name: " << getThreadName() << " - Started Thread.";
#ifdef TARGET_ANDROID
	JNIEnv * env;
	jint attachResult = ofGetJavaVMPtr()->AttachCurrentThread(&env,NULL);
#endif
	// user function
    // should loop endlessly.
	threadedFunction();

    _threadRunning = false;

#if !defined(TARGET_WIN32) && !defined(TARGET_ANDROID)
	// FIXME: this won't be needed once we update POCO https://github.com/pocoproject/poco/issues/79
	if(!threadBeingWaitedFor){ //if threadedFunction() ended and the thread is not being waited for, detach it before exiting.
		pthread_detach(pthread_self());
	}
#endif
#ifdef TARGET_ANDROID
	attachResult = ofGetJavaVMPtr()->DetachCurrentThread();
#endif

	ofLogVerbose("ofThread") << "- name: " << getThreadName() << " - Thread Finished.";
}
