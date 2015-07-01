#pragma once


#include "ofThread.h"


/// This is a simple example of a ThreadedObject created by extending ofThread.
/// It contains data (count) that will be accessed from within and outside the
/// thread and demonstrates several of the data protection mechanisms (aka
/// mutexes).
class ThreadedObject: public ofThread
{
public:
    /// Create a ThreadedObject and initialize the member
    /// variable in an initialization list.
    ThreadedObject(): count(0), shouldThrowTestException(false)
    {
    }

    /// Start the thread.
    void start()
    {
        // Mutex blocking is set to true by default
        // It is rare that one would want to use startThread(false).
        startThread();
    }

    /// Signal the thread to stop.  After calling this method,
    /// isThreadRunning() will return false and the while loop will stop
    /// next time it has the chance to.
    void stop()
    {
        stopThread();
    }

    /// Our implementation of threadedFunction.
    void threadedFunction()
    {
        while(isThreadRunning())
        {
            // Attempt to lock the mutex.  If blocking is turned on,
            if(lock())
            {
                // The mutex is now locked and the "count"
                // variable is protected.  Time to modify it.
                count++;

                // Here, we simply cause it to reset to zero if it gets big.
                if(count > 50000) count = 0;

                // Unlock the mutex.  This is only
                // called if lock() returned true above.
                unlock();

                // Sleep for 1 second.
                sleep(1000);

                if(shouldThrowTestException > 0)
                {
                    shouldThrowTestException = 0;

                    // Throw an exception to test the global ofBaseThreadErrorHandler.
                    // Users that require more specialized exception handling,
                    // should make sure that their threaded objects catch all
                    // exceptions. ofBaseThreadErrorHandler is only used as a
                    // way to provide better debugging / logging information in
                    // the event of an uncaught exception.
                    throw Poco::ApplicationException("We just threw a test exception!");
                }
            }
            else
            {
                // If we reach this else statement, it means that we could not
                // lock our mutex, and so we do not need to call unlock().
                // Calling unlock without locking will lead to problems.
                ofLogWarning("threadedFunction()") << "Unable to lock mutex.";
            }
        }
    }

    /// This drawing function cannot be called from the thread itself because
    /// it includes OpenGL calls (ofDrawBitmapString).
    void draw()
    {
        stringstream ss;

        ss << "I am a slowly increasing thread. " << endl;
        ss << "My current count is: ";

        if(lock())
        {
            // The mutex is now locked and the "count"
            // variable is protected.  Time to read it.
            ss << count;

            // Unlock the mutex.  This is only
            // called if lock() returned true above.
            unlock();
        }
        else
        {
            // If we reach this else statement, it means that we could not
            // lock our mutex, and so we do not need to call unlock().
            // Calling unlock without locking will lead to problems.
            ofLogWarning("threadedFunction()") << "Unable to lock mutex.";
        }

        ofDrawBitmapString(ss.str(), 50, 56);
    }

    // Use unique_lock to protect a copy of count while getting a copy.
    int getCount()
    {
        unique_lock<std::mutex> lock(mutex);
        return count;
    }

    void throwTestException()
    {
        shouldThrowTestException = 1;
    }

protected:
    // A flag to check and see if we should throw a test exception.
    Poco::AtomicCounter shouldThrowTestException;

    // This is a simple variable that we aim to always access from both the
    // main thread AND this threaded object.  Therefore, we need to protect it
    // with the mutex.  In the case of simple numerical variables, some
    // garuntee thread safety for small integral types, but for the sake of
    // illustration, we use an int.  This int could represent ANY complex data
    // type that needs to be protected.
    //
    // Note, if we simply want to count in a thread-safe manner without worrying
    // about mutexes, we might use Poco::AtomicCounter instead.
    int count;

};
