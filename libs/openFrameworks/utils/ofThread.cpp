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
		mutex.lock();
	}else{
		if(!mutex.try_lock()){
			return false; // mutex is locked, tryLock failed
		}
	}
	return true;
}


//-------------------------------------------------
void ofThread::unlock(){
	mutex.unlock();
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
            stopThread(); // signalled to stop
		}

		// wait for the thread to finish
        if(isCurrentThread()){
			return; // waitForThread should only be called outside thread
		}

        try{
            if (INFINITE_JOIN_TIMEOUT == milliseconds){
                thread.join();
            }else{
                // Wait for "joinWaitMillis" milliseconds for thread to finish
                if(!thread.tryJoin(milliseconds)){
                    // unable to completely wait for thread
                }
            }
        }catch(...){
            
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
#ifdef TARGET_ANDROID
	JNIEnv * env;
	jint attachResult = ofGetJavaVMPtr()->AttachCurrentThread(&env,nullptr);
	if(attachResult!=0){
		ofLogWarning() << "couldn't attach new thread to java vm";
	}
#endif
	// user function
    // should loop endlessly.
	try{
		threadedFunction();
	}catch(const Poco::Exception& exc){
		ofLogFatalError("ofThreadErrorLogger::exception") << exc.displayText();
	}catch(const std::exception& exc){
		ofLogFatalError("ofThreadErrorLogger::exception") << exc.what();
	}catch(...){
		ofLogFatalError("ofThreadErrorLogger::exception") << "Unknown exception.";
	}

    _threadRunning = false;

#if !defined(TARGET_WIN32)
	// FIXME: this won't be needed once we update POCO https://github.com/pocoproject/poco/issues/79
	if(!threadBeingWaitedFor){ //if threadedFunction() ended and the thread is not being waited for, detach it before exiting.
		pthread_detach(pthread_self());
	}
#endif
#ifdef TARGET_ANDROID
	attachResult = ofGetJavaVMPtr()->DetachCurrentThread();
#endif
}
