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
      if(verbose)printf("ofThread: thread already running\n"); 
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
		if(verbose)printf("ofThread: waiting till mutex is unlocked\n"); 
		mutex.lock();
	}
	else
	{
		if ( !mutex.tryLock() )
		{
            if(verbose)printf("ofThread: mutex is busy - already locked\n"); 
			return false; 
		}
	}
	if(verbose)printf("ofThread: we are in -- mutex is now locked \n"); 
	
	return true; 
} 

//------------------------------------------------- 
bool ofThread::unlock(){ 

	mutex.unlock();
	if(verbose)printf("ofThread: we are out -- mutex is now unlocked \n"); 

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
		if(verbose)printf("ofThread: thread stopped\n");
		threadRunning = false;
	}else{
		if(verbose)printf("ofThread: thread already stopped\n");
	}
}

//-------------------------------------------------
void ofThread::waitForThread(bool stop){
	if (threadRunning){
		// Reset the thread state
		if(stop){
			threadRunning = false;
			if(verbose)printf("ofThread: stopping thread\n");
		}
		if(verbose)printf("ofThread: waiting for thread to stop\n");
		// Wait for the thread to finish
		#ifdef TARGET_WIN32
			WaitForSingleObject(myThread, INFINITE);
			CloseHandle(myThread);
		#else
			if(pthread_self()==myThread) ofLog(OF_LOG_ERROR,"ofThread: error, waitForThread should only be called from outside the thread");
		    pthread_join(myThread, NULL);
		#endif
		if(verbose)printf("ofThread: thread stopped\n");
		//myThread = NULL;
   }else{
		if(verbose)printf("ofThread: thread already stopped\n");
	}
}
