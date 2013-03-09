#ifndef _THREADED_OBJECT
#define _THREADED_OBJECT

#include "ofMain.h"

// this is not a very exciting example yet
// but ofThread provides the basis for ofNetwork and other
// operations that require threading.
//
// please be careful - threading problems are notoriously hard
// to debug and working with threads can be quite difficult


class threadedObject : public ofThread{

	public:


	    int count;  // threaded fucntions that share data need to use lock (mutex)
	                // and unlock in order to write to that data
	                // otherwise it's possible to get crashes.
	                //
	                // also no opengl specific stuff will work in a thread...
	                // threads can't create textures, or draw stuff on the screen
	                // since opengl is single thread safe

		//--------------------------
		threadedObject(){
			count = 0;
            bTestThrowError = false;
		}

		void start(){
            startThread(true, false);   // blocking, verbose
        }

        void stop(){
            stopThread();
        }

		//--------------------------
		void threadedFunction(){

			while( isThreadRunning() != 0 ){
				if( lock() ){
					count++;
					if(count > 50000) count = 0;
					unlock();
					ofSleepMillis(1 * 1000);
				}
                
                if(bTestThrowError) {
                    mutex.lock();
                    bTestThrowError = false;
                    mutex.unlock();
                    
                    // this will stop the thread, but the exception will be caught
                    // by the built-in ofThreadErrorHandler.
                    throw Poco::ApplicationException("We just threw a test exception!");
                    
                }
			}
		}

		//--------------------------
		void draw(){

			string str = "I am a slowly increasing thread. \nmy current count is: ";

			if( lock() ){
				str += ofToString(count);
				unlock();
			}else{
				str = "can't lock!\neither an error\nor the thread has stopped";
			}
			ofDrawBitmapString(str, 50, 56);
		}

        // this fucntion simply demonstates that ofThreadErrorHandler
        // will catch exceptions thrown from within our threaded function.
        void throwErrorTest() {
            ofScopedLock lock(mutex); // here we use a scoped lock, instead of mutex.lock() / unlock()
            // when an ofScopedLock is constructed, it locks the mutex that is passed to it.
            bTestThrowError = true;
            // when ofScopedLock called "lock" goes out of scope (that is -- when it's destructor
            // is called), it will unlock the mutex that was locked in its constructor.
        }

        // an equivalent function would look like this
        /*
         void throwErrorTest() {
            mutex.lock();
            bTestThrowError = true;
            mutex.unlock();
         }
         */
    
    
protected:
    bool bTestThrowError;

};

#endif
