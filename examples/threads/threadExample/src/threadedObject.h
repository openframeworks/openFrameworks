#pragma once

#include "ofMain.h"
#include <atomic>

/// This is a simple example of a ThreadedObject created by extending ofThread.
/// It contains data (count) that will be accessed from within and outside the
/// thread and demonstrates several of the data protection mechanisms (aka
/// mutexes).
class ThreadedObject: public ofThread
{
public:
	/// On destruction wait for the thread to finish
	/// so we don't destroy the pixels while they are
	/// being used. Otherwise we would crash
	~ThreadedObject(){
		stop();
		waitForThread(false);
	}

	void setup(){
		pixels.allocate(640,480,OF_IMAGE_COLOR_ALPHA);
		tex.allocate(pixels);
		start();
	}

	/// Start the thread.
	void start(){
		startThread();
	}

	/// Signal the thread to stop.  After calling this method,
	/// isThreadRunning() will return false and the while loop will stop
	/// next time it has the chance to.
	/// In order for the thread to actually go out of the while loop
	/// we need to notify the condition, otherwise the thread will
	/// sleep there forever.
	/// We also lock the mutex so the notify_all call only happens
	/// once the thread is waiting. We lock the mutex during the
	/// whole while loop but when we call condition.wait, that
	/// unlocks the mutex which ensures that we'll only call
	/// stop and notify here once the condition is waiting
	void stop(){
		std::unique_lock<std::mutex> lck(mutex);
		stopThread();
		condition.notify_all();
	}

	/// Everything in this function will happen in a different
	/// thread which leaves the main thread completelty free for
	/// other tasks.
	void threadedFunction(){
		while(isThreadRunning()){
			if (ofIsCurrentThreadTheMainThread()) {
				// will never happen but to document the branch:
				ofLogNotice("ThreadedObject::threadedFunction") << "processed in main thread";
			} else {
				ofLogNotice("ThreadedObject::threadedFunction") << "processed in other thread";
			}
			// since we are only writting to the frame number from one thread
			// and there's no calculations that depend on it we can just write to
			// it without locking
			threadFrameNum++;

			// Lock the mutex until the end of the block, until the closing }
			// in which this variable is contained or we unlock it explicitly
			std::unique_lock<std::mutex> lock(mutex);

			// The mutex is now locked so we can modify
			// the shared memory without problem
			auto t = ofGetElapsedTimef();
			for(auto line: pixels.getLines()){
				auto x = 0;
				for(auto pixel: line.getPixels()){
					auto ux = x/float(pixels.getWidth());
					auto uy = line.getLineNum()/float(pixels.getHeight());
					pixel[0] = ofNoise(ux, uy, t);
					pixel[1] = ofNoise(ux, uy, t);
					pixel[2] = ofNoise(ux, uy, t);
					pixel[3] = 1;
					x++;
				}
			}

			// Now we wait for the main thread to finish
			// with this frame until we can generate a new one
			// This sleeps the thread until the condition is signaled
			// and unlocks the mutex so the main thread can lock it
			condition.wait(lock);
		}
	}

	void update(){
		// if we didn't lock here we would see
		// tearing as the thread would be updating
		// the pixels while we upload them to the texture
		std::unique_lock<std::mutex> lock(mutex);
		tex.loadData(pixels);
		condition.notify_all();
	}

	void updateNoLock(){
		// we don't lock here so we will see
		// tearing as the thread will update
		// the pixels while we upload them to the texture
		tex.loadData(pixels);
		condition.notify_all();
	}

	/// This drawing function cannot be called from the thread itself because
	/// it includes OpenGL calls
	void draw(){
		tex.draw(0,0);
	}

	int getThreadFrameNum(){
		return threadFrameNum;
	}

protected:
	// pixels represents shared data that we aim to always access from both the
	// main thread AND this threaded object and at least from one of them for
	// writing. Therefore, we need to protect it with the mutex.
	// Otherwise it wouldn't make sense to lock.
	ofFloatPixels pixels;

	ofTexture tex;
	std::condition_variable condition;
	int threadFrameNum = 0;
};
