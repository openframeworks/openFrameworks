#include "ofThread.h" 

//------------------------------------------------- 
ofThread::ofThread(){ 
   threadRunning = false; 
   verbose = false;
   #ifdef TARGET_WIN32 
      InitializeCriticalSection(&critSec); 
   #else 
      pthread_mutex_init(&myMutex, NULL); 
   #endif 
} 

//------------------------------------------------- 
ofThread::~ofThread(){ 
   #ifndef TARGET_WIN32 
      pthread_mutex_destroy(&myMutex); 
   #endif 
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

   #ifdef TARGET_WIN32 
      if(blocking)EnterCriticalSection(&critSec); 
      else { 
         if(!TryEnterCriticalSection(&critSec)){ 
            if(verbose)printf("ofThread: mutext is busy \n"); 
            return false; 
         } 
      } 
      if(verbose)printf("ofThread: we are in -- mutext is now locked \n"); 
   #else 

      if(blocking){ 
         if(verbose)printf("ofThread: waiting till mutext is unlocked\n"); 
         pthread_mutex_lock(&myMutex); 
         if(verbose)printf("ofThread: we are in -- mutext is now locked \n"); 
      }else{ 
         int value = pthread_mutex_trylock(&myMutex); 
         if(value == 0){ 
            if(verbose)printf("ofThread: we are in -- mutext is now locked \n"); 
         } 
         else{ 
            if(verbose)printf("ofThread: mutext is busy - already locked\n"); 
            return false; 
         } 
      } 
   #endif 

   return true; 
} 

//------------------------------------------------- 
bool ofThread::unlock(){ 

   #ifdef TARGET_WIN32 
      LeaveCriticalSection(&critSec); 
   #else 
      pthread_mutex_unlock(&myMutex); 
   #endif 

   if(verbose)printf("ofThread: we are out -- mutext is now unlocked \n"); 

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
			if(pthread_self()==myThread) printf("ofThread: error, waitForThread should only be called from outside the thread");
		    pthread_join(myThread, NULL);
		#endif
		if(verbose)printf("ofThread: thread stopped\n");
		myThread = NULL;
   }else{
		if(verbose)printf("ofThread: thread already stopped\n");
	}
}
