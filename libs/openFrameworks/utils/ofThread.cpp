#include "ofThread.h"
#include "ofLog.h"

#ifdef TARGET_ANDROID
#include <jni.h>
#include "ofxAndroidUtils.h"
#endif

//-------------------------------------------------
ofThread::ofThread()
:threadRunning(false)
,threadDone(true)
,mutexBlocks(true)
,name(""){
}

//-------------------------------------------------
bool ofThread::isThreadRunning() const{
    return threadRunning;
}

//-------------------------------------------------
std::thread::id ofThread::getThreadId() const{
	return thread.get_id();
}

//-------------------------------------------------
std::string ofThread::getThreadName() const{
	return name;
}

//-------------------------------------------------
void ofThread::setThreadName(const std::string & name){
	this->name = name;
}

//-------------------------------------------------
void ofThread::startThread(){
	std::unique_lock<std::mutex> lck(mutex);
	if(threadRunning || !threadDone){
		ofLogWarning("ofThread") << "- name: " << getThreadName() << " - Cannot start, thread already running.";
		return;
	}

	threadDone = false;
	threadRunning = true;
	this->mutexBlocks = true;

	thread = std::thread(std::bind(&ofThread::run,this));
}

//-------------------------------------------------
void ofThread::startThread(bool mutexBlocks){
    std::unique_lock<std::mutex> lck(mutex);
	if(threadRunning || !threadDone){
		ofLogWarning("ofThread") << "- name: " << getThreadName() << " - Cannot start, thread already running.";
		return;
	}

    threadDone = false;
    threadRunning = true;
    this->mutexBlocks = mutexBlocks;

	thread = std::thread(std::bind(&ofThread::run,this));
}

//-------------------------------------------------
bool ofThread::lock(){
	if(mutexBlocks){
		mutex.lock();
	}else{
		if(!mutex.try_lock()){
			return false; // mutex is locked, tryLock failed
		}
	}
	return true;
}

//-------------------------------------------------
bool ofThread::tryLock(){
	return mutex.try_lock();
}

//-------------------------------------------------
void ofThread::unlock(){
	mutex.unlock();
}

//-------------------------------------------------
void ofThread::stopThread(){
    threadRunning = false;
}

//-------------------------------------------------
void ofThread::waitForThread(bool callStopThread, long milliseconds){
	if(!threadDone){
		// tell thread to stop
        if(callStopThread){
            stopThread();
		}

		// wait for the thread to finish
        if(isCurrentThread()){
			return; // waitForThread should only be called outside thread
		}

        if (INFINITE_JOIN_TIMEOUT == milliseconds){
            std::unique_lock<std::mutex> lck(mutex);
            if(!threadDone){
                condition.wait(lck);
            }
        }else{
            // Wait for "joinWaitMillis" milliseconds for thread to finish
            std::unique_lock<std::mutex> lck(mutex);
            if(!threadDone && condition.wait_for(lck,std::chrono::milliseconds(milliseconds))==std::cv_status::timeout){
				// unable to completely wait for thread
            }
        }
    }
}

//-------------------------------------------------
void ofThread::sleep(long milliseconds){
	std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds));
}

//-------------------------------------------------
void ofThread::yield(){
	std::this_thread::yield();
}

//-------------------------------------------------
bool ofThread::isCurrentThread() const{
    return std::this_thread::get_id() == thread.get_id();
}

//-------------------------------------------------
std::thread & ofThread::getNativeThread(){
	return thread;
}

//-------------------------------------------------
const std::thread & ofThread::getNativeThread() const{
	return thread;
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
	}catch(const std::exception& exc){
		ofLogFatalError("ofThreadErrorLogger::exception") << exc.what();
	}catch(...){
		ofLogFatalError("ofThreadErrorLogger::exception") << "Unknown exception.";
	}
	try{
		thread.detach();
	}catch(...){}
#ifdef TARGET_ANDROID
	attachResult = ofGetJavaVMPtr()->DetachCurrentThread();
#endif

    std::unique_lock<std::mutex> lck(mutex);
	threadRunning = false;
	threadDone = true;
    condition.notify_all();
}
