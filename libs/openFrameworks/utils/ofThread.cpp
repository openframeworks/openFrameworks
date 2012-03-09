#include "ofThread.h" 

#include "ofLog.h"
#include "ofUtils.h"

//------------------------------------------------- 
ofThread::ofThread(){ 
   threadRunning = false;
   verbose = false;
   thread.setName("Thread "+ofToString(thread.id()));
} 

//------------------------------------------------- 
ofThread::~ofThread(){ 
   stopThread(true);
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
} 

//------------------------------------------------- 
bool ofThread::lock(){ 

	if(blocking){
		if(verbose){
			if(Poco::Thread::current() == &thread){
				ofLogVerbose(thread.name()) << "thread waiting for mutex to be unlocked";
			}
			else{
				ofLogVerbose(thread.name()) << "waiting for mutex to be unlocked";
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
			ofLogVerbose(thread.name()) << "thread locked mutex";
		}
		else{
			ofLogVerbose(thread.name()) << "mutex locked";
		}
	}
	
	return true; 
} 

//------------------------------------------------- 
void ofThread::unlock(){ 
	mutex.unlock();
	
	if(verbose){
		if(Poco::Thread::current() == &thread){
			ofLogVerbose(thread.name()) << "thread unlocked mutex";
		}
		else{
			ofLogVerbose(thread.name()) << "mutex unlocked";
		}
	}
	return; 
} 

//------------------------------------------------- 
void ofThread::stopThread(bool close){
	if(thread.isRunning()) {
		threadRunning = false;
		if(close && thread.isRunning()){
			thread.tryJoin(0);
		}
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
		thread.join();
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
	
	// user function
	threadedFunction();
	
	threadRunning = false;
	ofLogVerbose(thread.name()) << "stopped";
}
