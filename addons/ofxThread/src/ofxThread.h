#ifndef _OFX_THREAD_H_
#define _OFX_THREAD_H_

#include "ofConstants.h"

#ifdef TARGET_WIN32
	#include <process.h>
#else
    #include <pthread.h>
#endif

class ofxThread{

	public:
		ofxThread();
		virtual ~ofxThread();
		bool isThreadRunning();
		void startThread(bool _blocking = true, bool _verbose = true);
		bool lock();
		bool unlock();
		void stopThread(bool close = true);
		void waitForThread(bool stop = true);

	protected:

		//-------------------------------------------------
		//you need to overide this with the function you want to thread
		virtual void threadedFunction(){
			if(verbose)printf("ofxThread: overide threadedFunction with your own\n");
		}

		//-------------------------------------------------

		#ifdef TARGET_WIN32
			static unsigned int __stdcall thread(void * objPtr){
				ofxThread* me	= (ofxThread*)objPtr;
				me->threadedFunction();
				me->stopThread(false);
				return 0;
			}

		#else
			static void * thread(void * objPtr){
				ofxThread* me	= (ofxThread*)objPtr;
				me->threadedFunction();
				me->stopThread(false);
				pthread_exit(NULL);
				return 0;
			}
		#endif


	#ifdef TARGET_WIN32
			HANDLE            myThread;
			CRITICAL_SECTION  critSec;  	//same as a mutex
	#else
			pthread_t        myThread;
			pthread_mutex_t  myMutex;
	#endif

	bool threadRunning;
	bool blocking;
	bool verbose;
};

#endif
