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



};

#endif
