#include "ofThread.h" 

//------------------------------------------------- 
ofThread::ofThread(){ 
   threadRunning = false; 
   verbose = false;
} 

//------------------------------------------------- 
ofThread::~ofThread(){ 
   stopThread(); 
} 

//------------------------------------------------- 
bool ofThread::isThreadRunning(){ 
   //should be thread safe - no writing of vars here 
   return threadRunning; 
} 

//------------------------------------------------- 
void ofThread::startThread(bool _blocking, bool _verbose){ 
   if( threadRunning ){ 
      if(verbose){
    	  ofLog(OF_LOG_NOTICE, "ofThread: thread already running");
      }
      return; 
   } 

   //have to put this here because the thread can be running 
   //before the call to create it returns 
   threadRunning   = true; 

   #ifdef TARGET_WIN32 
      //InitializeCriticalSection(&critSec); 
      myThread = (HANDLE)_beginthreadex(NULL, 0, this->thread,  (void *)this, 0, NULL); 
   #else 
      //pthread_mutex_init(&myMutex, NULL); 
      pthread_create(&myThread, NULL, thread, (void *)this); 
   #endif 

   blocking      =   _blocking; 
   verbose         = _verbose; 
} 

//------------------------------------------------- 
//returns false if it can't lock 
bool ofThread::lock(){ 

	if ( blocking )
	{
		if(verbose){
			ofLog(OF_LOG_NOTICE, "ofThread: waiting till mutex is unlocked");
		}
		mutex.lock();
	}
	else
	{
		if ( !mutex.tryLock() )
		{
            if(verbose){
            	ofLog(OF_LOG_NOTICE, "ofThread: mutex is busy - already locked");
            }
			return false; 
		}
	}
	if(verbose){
		ofLog(OF_LOG_NOTICE, "ofThread: we are in -- mutex is now locked");
	}
	
	return true; 
} 

//------------------------------------------------- 
bool ofThread::unlock(){ 

	mutex.unlock();
	if(verbose){
		ofLog(OF_LOG_NOTICE, "ofThread: we are out -- mutex is now unlocked");
	}

   return true; 
} 

//------------------------------------------------- 
void ofThread::stopThread(bool close){
	if(threadRunning){
		if(close){
			#ifdef TARGET_WIN32
				CloseHandle(myThread);
			#else
				//pthread_mutex_destroy(&myMutex);
				pthread_detach(myThread);
			#endif
		}
		if(verbose){
			ofLog(OF_LOG_NOTICE, "ofThread: thread stopped");
		}
		threadRunning = false;
	}else{
		if(verbose){
			ofLog(OF_LOG_NOTICE, "ofThread: thread already stopped");
		}
	}
}

//-------------------------------------------------
void ofThread::waitForThread(bool stop){
	if (threadRunning){
		// Reset the thread state
		if(stop){
			threadRunning = false;
			if(verbose){
				ofLog(OF_LOG_NOTICE, "ofThread: stopping thread");
			}
		}
		if(verbose){
			ofLog(OF_LOG_NOTICE, "ofThread: waiting for thread to stop");
		}
		// Wait for the thread to finish
		#ifdef TARGET_WIN32
			WaitForSingleObject(myThread, INFINITE);
			CloseHandle(myThread);
		#else
			if(pthread_self()==myThread) ofLog(OF_LOG_ERROR,"ofThread: error, waitForThread should only be called from outside the thread");
		    pthread_join(myThread, NULL);
		#endif
		if(verbose){
			ofLog(OF_LOG_NOTICE, "ofThread: thread stopped");
		}
		//myThread = NULL;
   }else{
		if(verbose){
			ofLog(OF_LOG_NOTICE, "ofThread: thread already stopped");
		}
	}
}
