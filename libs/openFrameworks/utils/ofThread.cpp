#include "ofThread.h" 

#include "ofLog.h"
#include "ofUtils.h"
#ifdef TARGET_ANDROID
#include <jni.h>
#include "ofxAndroidUtils.h"
#endif

//------------------------------------------------- 
ofThread::ofThread(const char* threadName){ 
   threadRunning = false;
   verbose = false;
   if ( threadName )
	setThreadName( threadName );
   else
	setThreadName("Thread "+ofToString(thread.id()));
   blocking = true;
} 

//------------------------------------------------- 
ofThread::~ofThread(){
   //by passing true we're also telling the thread to stop 
   waitForThread(true);
} 

//------------------------------------------------- 
bool ofThread::isThreadRunning(){ 
   return threadRunning;
}

//------------------------------------------------- 
int ofThread::getThreadId(){
	return thread.id();
}

//------------------------------------------------- 
string ofThread::getThreadName(){
	return thread.name();
}

void ofThread::setThreadName(const string& threadName){
	//	update the internal thread name
	thread.setName( threadName );

	//	then external(os)
	setOSThreadName( getPocoThread().getName().c_str() );
}

//------------------------------------------------- 
bool ofThread::setOSThreadName(const char* name){

	//	if the thread has been created, update the OS thread name
	//	in win32, this name will be reflected in visual studio debugger
	//	http://msdn.microsoft.com/en-gb/library/xcb2z8hs.aspx
#if defined(TARGET_WIN32)
	const DWORD MS_VC_EXCEPTION=0x406D1388;
	#pragma pack(push,8)
	typedef struct tagTHREADNAME_INFO
	{
		DWORD dwType; // Must be 0x1000.
		LPCSTR szName; // Pointer to name (in user addr space).
		DWORD dwThreadID; // Thread ID (-1=caller thread).
		DWORD dwFlags; // Reserved for future use, must be zero.
	} THREADNAME_INFO;
	#pragma pack(pop)

	THREADNAME_INFO info;
	info.dwType = 0x1000;
	info.szName = name;
	info.dwThreadID = getPocoThread().tid();
	info.dwFlags = 0;

	//	this will fail if the OS thread hasn't started yet
	if ( info.dwThreadID == 0 )
		return false;
	
	__try
	{
		RaiseException( MS_VC_EXCEPTION, 0, sizeof(info)/sizeof(ULONG_PTR), (ULONG_PTR*)&info );
	}
	__except(EXCEPTION_EXECUTE_HANDLER)
	{
		return false;
	}
	return true;
#else
	return false;
#endif
}


//------------------------------------------------- 
void ofThread::startThread(bool blocking, bool verbose){

	if(thread.isRunning()){ 
		ofLogWarning(thread.name()) << "cannot start, thread already running";
		return; 
	} 

	// have to put this here because the thread can be running 
	// before the call to create it returns 
	threadRunning = true; 

	this->blocking = blocking;
	this->verbose = verbose;
	
	if(verbose){
		ofSetLogLevel(thread.name(), OF_LOG_VERBOSE);
	}
	else{
		ofSetLogLevel(thread.name(), OF_LOG_NOTICE);
	}

	thread.start(*this);

	//	re-apply threadname after it's been created to set the OS threadname
	setThreadName( getThreadName() );
} 

//------------------------------------------------- 
bool ofThread::lock(){ 

	if(blocking){
		if(verbose){
			if(Poco::Thread::current() == &thread){
				ofLogVerbose(thread.name()) << "thread waiting for own mutex to be unlocked";
			}
			else{
				ofLogVerbose(thread.name()) << "external waiting for thread mutex to be unlocked";
			}
		}
		mutex.lock();
	}
	else{
		if(!mutex.tryLock()){
			ofLogVerbose(thread.name()) << "mutex is busy - already locked"; 
			return false; 
		}
	}
	
	if(verbose){
		if(Poco::Thread::current() == &thread){
			ofLogVerbose(thread.name()) << "thread locked own mutex";
		}
		else{
			ofLogVerbose(thread.name()) << "external locked thread mutex";
		}
	}
	
	return true; 
} 

//------------------------------------------------- 
void ofThread::unlock(){ 
	mutex.unlock();
	
	if(verbose){
		if(Poco::Thread::current() == &thread){
			ofLogVerbose(thread.name()) << "thread unlocked own mutex";
		}
		else{
			ofLogVerbose(thread.name()) << "external unlocked thread mutex";
		}
	}
	return; 
} 

//------------------------------------------------- 
void ofThread::stopThread(){
	if(thread.isRunning()) {
		threadRunning = false;
	}
}

//-------------------------------------------------
void ofThread::waitForThread(bool stop){
	if(thread.isRunning()){
		
		// tell thread to stop
		if(stop){
			threadRunning = false;
			ofLogVerbose(thread.name()) << "signaled to stop";
		}
		
		// wait for the thread to finish
		ofLogVerbose(thread.name()) << "waiting to stop";
		if(Poco::Thread::current() == &thread){
			ofLogWarning(thread.name()) << "waitForThread should only be called from outside the thread";
			return;
		}
        //wait for 10 seconds for thread to finish 
		if( !thread.tryJoin(10000) ){
            ofLogError( thread.name() ) << "unable to end/join thread " << endl; 
        }
   }
}

//-------------------------------------------------
void ofThread::sleep(int sleepMS){
	Poco::Thread::sleep(sleepMS);
}

//-------------------------------------------------
void ofThread::yield(){
	Poco::Thread::yield();
}

//-------------------------------------------------
bool ofThread::isCurrentThread(){
	if(ofThread::getCurrentThread() == this)
		return true;
	return false;
}

//-------------------------------------------------
Poco::Thread & ofThread::getPocoThread(){
	return thread;
}

//-------------------------------------------------
bool ofThread::isMainThread(){
	if(Poco::Thread::current() == NULL)
		return true;
	return false;
}

//-------------------------------------------------
ofThread * ofThread::getCurrentThread(){
	// assumes all created threads are ofThreads ...
	// might be dangerous if people are using Poco::Threads directly
	return (ofThread *) Poco::Thread::current();
}

// PROTECTED
//-------------------------------------------------
void ofThread::threadedFunction(){
	ofLogWarning(thread.name()) << "override threadedFunction with your own";
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
