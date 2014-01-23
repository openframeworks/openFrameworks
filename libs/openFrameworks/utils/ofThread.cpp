#include "ofThread.h" 

#include "ofLog.h"
#include "ofUtils.h"
#ifdef TARGET_ANDROID
#include <jni.h>
#include "ofxAndroidUtils.h"
#endif

//------------------------------------------------- 
ofThread::ofThread():
    threadRunning(false),
    mutexesBlock(true)
{
   thread.setName("Thread " + ofToString(thread.id()));
} 

//------------------------------------------------- 
ofThread::~ofThread(){
}

//------------------------------------------------- 
bool ofThread::isThreadRunning()
{
    return threadRunning;
}

//------------------------------------------------- 
int ofThread::getThreadId() const
{
	return thread.id();
}

//------------------------------------------------- 
std::string ofThread::getThreadName() const
{
	return thread.name();
}

//-------------------------------------------------
void ofThread::startThread(bool _mutexesBlock){

	if(thread.isRunning()){
		ofLogWarning(thread.name()) << "Cannot start, thread already running.";
		return;
	}

    threadRunning = true;
    mutexesBlock = true;

	thread.start(*this);
}

//-------------------------------------------------
void ofThread::startThread(bool blocking, bool verbose){
    ofLogWarning("ofThread::startThread") << "Calling startThread with verbose is deprecated.";
    startThread(blocking);
}

//------------------------------------------------- 
bool ofThread::lock(){ 

	if(mutexesBlock){
        if(isCurrentThread()) {
            ofLogVerbose(thread.name()) << "ofThread waiting for its own mutex to be unlocked.";
        } else {
            ofLogVerbose(thread.name()) << "External thread waiting for ofThread mutex to be unlocked";
        }
		mutex.lock();
	} else {
		if(!mutex.tryLock())
        {
			ofLogVerbose(thread.name()) << "Mutex is already locked, tryLock failed.";
			return false; 
		}
	}

    if(isCurrentThread()) {
        ofLogVerbose(thread.name()) << "ofThread locked its own mutex.";
    } else {
        ofLogVerbose(thread.name()) << "External thread locked the ofThread mutex.";
    }

	return true; 
} 

//------------------------------------------------- 
void ofThread::unlock(){ 
	mutex.unlock();
	
    if(isCurrentThread()) {
        ofLogVerbose(thread.name()) << "ofThread unlocked its own mutex.";
    } else {
        ofLogVerbose(thread.name()) << "External thread unlocked the ofThread mutex.";
    }
}

//------------------------------------------------- 
void ofThread::stopThread(){
	if(thread.isRunning())
    {
        threadRunning = false;
	}
}

//-------------------------------------------------
void ofThread::waitForThread(bool stop, long milliseconds){
	if(thread.isRunning()){
		
		// tell thread to stop
		if(stop)
        {
            threadRunning = false;
			ofLogVerbose(thread.name()) << "Signaled to stop.";
		}
		
		// wait for the thread to finish
		ofLogVerbose(thread.name()) << "waiting to stop";

        if(isCurrentThread())
        {
			ofLogWarning(thread.name()) << "waitForThread should only be called from outside the thread.";
			return;
		}

        if (INFINITE_JOIN_TIMEOUT == milliseconds)
        {
            thread.join();
        }
        else
        {
            // Wait for "joinWaitMillis" milliseconds for thread to finish
            if(!thread.tryJoin(milliseconds)) {
                ofLogError( thread.name() ) << "unable to end/join thread " << endl;
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
bool ofThread::isCurrentThread() const {
    return ofThread::getCurrentPocoThread() == &getPocoThread();
}

//-------------------------------------------------
Poco::Thread& ofThread::getPocoThread(){
	return thread;
}

//-------------------------------------------------
const Poco::Thread& ofThread::getPocoThread() const {
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


// PROTECTED
//-------------------------------------------------
void ofThread::threadedFunction(){
	ofLogWarning(thread.name()) << "Override ofThread::threadedFunction() in your ofThread subclass.";
}


// PRIVATE
//-------------------------------------------------
void ofThread::run(){
	
	ofLogVerbose(thread.name()) << "started";
#ifdef TARGET_ANDROID
	JNIEnv * env;
	jint attachResult = ofGetJavaVMPtr()->AttachCurrentThread(&env,NULL);
#endif
	// user function
	threadedFunction();
	
#ifdef TARGET_ANDROID
	attachResult = ofGetJavaVMPtr()->DetachCurrentThread();
#endif

    threadRunning = false;

	ofLogVerbose(thread.name()) << "stopped";
}
