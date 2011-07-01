#pragma once

#include "ofConstants.h"
#include "ofTypes.h"

#ifdef TARGET_WIN32
	#include <process.h>
#else
    #include <pthread.h>
#endif

class ofThread{

	public:
		ofThread();
		virtual ~ofThread();
		//virtual ofThread();
	
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
			if(verbose)printf("ofThread: overide threadedFunction with your own\n");
		}

		//-------------------------------------------------

		#ifdef TARGET_WIN32
			static unsigned int __stdcall thread(void * objPtr){
				ofThread* me	= (ofThread*)objPtr;
				me->threadedFunction();
				me->stopThread(false);
				return 0;
			}

		#else
			static void * thread(void * objPtr){
				ofThread* me	= (ofThread*)objPtr;
				me->threadedFunction();
				me->stopThread(false);
				pthread_exit(NULL);
				return 0;
			}
		#endif


	#ifdef TARGET_WIN32
			HANDLE            myThread;
	#else
			pthread_t        myThread;
	#endif
	ofMutex mutex;

	bool threadRunning;
	bool blocking;
	bool verbose;
};
