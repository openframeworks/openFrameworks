#pragma once


#include "Poco/AtomicCounter.h"
#include "Poco/Thread.h"
#include "Poco/Runnable.h"
#include "ofConstants.h"
#include "ofTypes.h"


/// \class ofThread
/// \brief A threaded base class with a built in mutex for convenience.
///
/// Users can extend this base class by public inheritance like this:
///
///     class MyThreadedClass: public ofThread
///     {
///         public:
///         /// ...
///         void threadedFunction()
///         {
///             while(isThreadRunning())
///             {
///                 /// Threaded function here.
///             }
///         }
///     };
///
/// ofThread is a convenient wrapper for Poco::Thread, Poco::Runnable and
/// Poco::Mutex.  It represents a simplified (sometimes overly simplified -
/// or simplified in ways that might not make sense for your project)
/// pathway for quickly writing threaded classes.  Poco::Runnable represents
/// a class that can be "run" via its void run() method.  Poco::Thread is
/// able to spawn a thread and "run" the contents of a class that extends
/// the Poco::Runnable interface (which ofThread does).  Poco::FastMutex,
/// (aka ofMutex) is a "mutual exclusion" object that prevents two threads
/// from accessing the same data at the same time.  It is important to know
/// that Poco::FastMutex (aka ofMutex) is not "recursive" while Poco::Mutex
/// is. This means that if the same thread attempts to lock a thread while
/// it ALREADY has a lock on the mutex, the program will lock up and go
/// nowhere.  Thus, it is important that ofThread subclasses carefully
/// their use of the mutex.  Currently ofThread does not lock its own mutex
/// at any point (e.g. ofThread's internal variables are not thread safe).
/// This is a somewhat dangerous convenience that is (theoretically)
/// supposed to make it easier for subclasses to avoid the recursive mutex
/// "problem". The situation that arises from two threads simultanously
/// reading or writing from the same shared data (shared data
/// occupies the same physical location in memory) leads to something
/// called a "race condition", which can lead to deadlocks.
/// A deadlock is as bad as it sounds.  It means your program
/// just stops.  ofMutex prevents race conditions, deadlocks and crashes by
/// permitting only one thread access to shared data at a time.  When using
/// mutexes to protect data, the trick is to always be sure to unlock the
/// mutex when finished.  This problem can often be avoided by using
/// an Poco::FastMutex::ScopedLock (aka ofScopedLock).  See the
/// the documentation for more information.  Finally, there are many cases
/// where it might make more sense to use Poco::Thread, Poco::Runnable and
/// Poco::FastMutex directly rather than using ofThread.  Further, cross
/// platform thread management will be alleviated with the std::thread
/// support library included with C++11.
class ofThread: protected Poco::Runnable {
public:
    /// \brief Create an ofThread.
    ofThread();

    /// \brief Destroy the ofThread.
    /// \warning The destructor WILL NOT stop the thread or wait for
    ///     the underlying Poco::Thread to finish.  For threads that
    ///     require the correct deallocation of resources, the user
    ///     MUST call waitForThread(...); to ensure that the thread
    ///     is stopped and the thread's resources are released.
    ///     Improper release of resources or memory can lead to
    ///     segementation faults and other errors.
    virtual ~ofThread();

    /// \brief Check the running status of the thread.
    /// \returns true iff the thread is currently running.
    bool isThreadRunning() const;

    /// \brief Get the unique thread id.
    /// \note This is NOT the the same as the operating thread id!
    int getThreadId() const;

    /// \brief Get the unique thread name, in the form of "Thread id#"
    /// \returns the Thread ID string.
    std::string getThreadName() const;

    /// \deprecated
    /// \brief Start the thread with options.
    ///
    /// \param mutexesBlock Set blocking to true if you want the mutex to
    ///        block when lock() is called.
    /// \param verbose use verbose logging methods.
    OF_DEPRECATED_MSG("Use startThread(bool blocking = true) instead.",
                      void startThread(bool mutexesBlock, bool verbose) );

    /// \brief Start the thread with options.
    /// \param mutexBlocks Set blocking to true if you want the mutex to
    ///        block when lock() is called.
    /// \note Subclasses can directly access the mutex and employ thier
    ///       own locking strategy.
    void startThread(bool mutexBlocks = true);

    /// \brief Try to lock the mutex.
    ///
    /// If the thread was started startThread(true), then this call will wait
    /// until the mutex is available and return true.  If the thread was started
    /// startThread(false), this call will return true iff the mutex is
    /// was successfully acquired.
    ///
    /// \returns true iff the lock was successfully acquired.
    bool lock();

    /// \brief Unlock the mutex.
    ///
    /// This will only unlocks the mutex if it was previously by the same
    /// calling thread.
    void unlock();

    /// \brief Stop the thread.
    ///
    /// This does immediately stop the thread from processing, but
    /// will only set a flag that must be checked from within your
    /// threadedFunction() by calling isThreadRunning().  If the user wants
    /// to both stop the thread AND wait for the thread to finish
    /// processing, the user should call waitForThread(true, ...).
    void stopThread();

    /// \brief Wait for the thread to exit (aka "joining" the thread).
    ///
    /// This method waits for a thread will "block" and wait for the
    /// thread (aka "join" the thread) before it returns.  This allows the
    /// user to be sure that the thread is properly cleaned up.  An example
    /// of when this might be particularly important is if the
    /// threadedFunction() is opening a set of network sockets, or
    /// downloading data from the web.  Destroying an ofThread subclass
    /// without releasing those sockets (or other resources), may result in
    /// segmentation faults, error signals or other undefined behaviors.
    ///
    /// \param callStopThread Set stop to true if you want to signal the thread
    ///     to exit before waiting.  This is the equivalent to calling
    ///     stopThread(). If you your threadedFunction uses a while-loop that
    ///     depends on isThreadRunning() and you do not call stopThread() or set
    ///     stop == true, waitForThread will hang indefinitely.  Set stop ==
    ///     false ONLY if you have already called stopThread() and you simply
    ///     need to be sure your thread has finished its tasks.
    ///
    /// \param milliseconds If millseconds is set to INFINITE_JOIN_TIMEOUT, the
    ///     waitForThread will wait indefinitely for the thread to complete.  If
    ///     milliseconds is set to a lower number (e.g. 10000 for 10 seconds),
    ///     waitForThread will wait for 10000 milliseconds and if the thread has
    ///     not yet stopped it will return and log an error message.  Users are
    ///     encouraged to use the default INFINITE_JOIN_TIMEOUT.  If the user is
    ///     unhappy with the amount of time it takes to join a thread, the user
    ///     is encouraged to seek more expedient ways of signalling their desire
    ///     for a thread to complete via other signalling methods such as
    ///     Poco::Event, Poco::Condition, or Poco::Semaphore.
    /// \sa http://pocoproject.org/slides/090-NotificationsEvents.pdf
    /// \sa http://pocoproject.org/docs/Poco.Condition.html
    /// \sa http://pocoproject.org/docs/Poco.Event.html
    /// \sa http://pocoproject.org/docs/Poco.Semaphore.html
    void waitForThread(bool callStopThread = true,
                       long milliseconds = INFINITE_JOIN_TIMEOUT);

    /// \brief Tell the thread to sleep for a certain amount of milliseconds.
    ///
    /// This is useful inside the threadedFunction() when a thread is waiting
    /// for input to process:
    ///
    ///     void MyThreadedClass::threadedFunction()
    ///     {
    ///		    // start
    ///		    while(isThreadRunning())
    ///         {
    ///             // bReadyToProcess can be set from outside the threadedFuntion.
    ///             // perhaps by another thread that downloads data, or loads
    ///             // some media, etc.
    ///
    ///		    	if(bReadyToProcess == true)
    ///             {
    ///		    		// do some time intensive processing
    ///		    		bReadyToProcess = false;
    ///		    	}
    ///             else
    ///             {
    ///		    		// sleep the thread to give up some cpu
    ///		    		sleep(20);
    ///		    	}
    ///		    }
    ///		    // done
    ///     }
    ///
    /// If the user does not give the thread a chance to sleep, the
    /// thread may take 100% of the CPU core while it's looping as it
    /// waits for something to do.  This may lead to poor application
    /// performance.
    ///
    /// \param milliseconds The number of milliseconds to sleep.
    void sleep(long milliseconds);

    /// \brief Tell the thread to give up its CPU time other threads.
    ///
    /// This method is similar to sleep() and can often be used in
    /// the same way.  The main difference is that 1 millisecond
    /// (the minimum sleep time available with sleep()) is a very
    /// long time on modern processors and yield() simply gives up
    /// processing time to the next thread, instead of waiting for
    /// number of milliseconds. In some cases, this behavior will
    /// be preferred.
    void yield();

    /// \brief Query whether the current thread is active.
    ///
    /// In multithreaded situations, it can be useful to know which
    /// thread is currently running some code in order to make sure
    /// only certain threads can do certain things.  For example,
    /// OpenGL can only run in the main execution thread.  Thus,
    /// situations where a thread is responsible for interacting
    /// with graphics resources may need to prevent graphics updates
    /// unless the main thread is accessing or updating resources
    /// shared with this ofThread (or its subclass).
    ///
    ///     if(myThread.isCurrentThread())
    ///     {
    ///         // do some myThread things,
    ///         // but keep your hands off my resources!
    ///     }
    ///     else if(ofThread::isMainThread())
    ///     {
    ///         // pheew! ok, update those graphics resources
    ///     }
    ///
    /// By way of another example, a subclass of ofThread may have
    /// an update() method that is called from ofBaseApp during the
    /// execution of the main application thread.  In these cases,
    /// the ofThread subclass might want to ask itself whether it
    /// can, for instance, call update() on an ofImage, in order to
    /// send copy some ofPixels to an ofTexture on the graphics
    /// card.
    ///
    /// \returns True iff this ofThread the currently active thread.
    bool isCurrentThread() const;

    /// \brief Get a reference to the underlying Poco thread.
    ///
    /// Poco::Thread provides a clean cross-platform wrapper for
    /// threads.  On occasion, it may be useful to interact with the
    /// underlying Poco::Thread directly.
    ///
    /// \returns A reference to the backing Poco thread.
    Poco::Thread& getPocoThread();

    /// \brief Get a const reference to the underlying Poco thread.
    ///
    /// Poco::Thread provides a clean cross-platform wrapper for
    /// threads.  On occasion, it may be useful to interact with the
    /// underlying Poco::Thread directly.
    ///
    /// \returns A reference to the backing Poco thread.
    const Poco::Thread& getPocoThread() const;

    /// \brief A query to see if the current thread is the main thread.
    ///
    /// Some functions (e.g. OpenGL calls) can only be executed
    /// the main thread.  This static function will tell the user
    /// what thread is currently active at the moment the method
    /// is called.
    ///
    ///     if (ofThread::isMainThread())
    ///     {
    ///         ofLogNotice() << "This is the main thread!";
    ///     }
    ///     else
    ///     {
    ///         ofLogNotice() << "This is NOT the main thread.";
    ///     }
    ///
    /// \returns true iff the current thread is the main thread.
    static bool isMainThread();

    /// \deprecated
    /// \warning This function is dangerous and should no longer be used.
    OF_DEPRECATED_MSG("use ofThread::getCurrentPocoThread() == &yourThread.getPocoThread() to compare threads.",
                      static ofThread* getCurrentThread());

    /// \brief Get the current Poco thread.
    ///
    /// In most cases, it is more appropriate to query the current
    /// thread by calling isCurrentThread() on an active thread or
    /// by calling ofThread::isMainThread().  See the method
    /// documentation for more information on those methods.
    ///
    /// \returns A pointer to the current active thread OR 0 iff the main
    ///     application thread is active.
    static Poco::Thread* getCurrentPocoThread();

    enum {
        INFINITE_JOIN_TIMEOUT = -1
            ///< \brief A sentinal value for an infinite join timeout.
            ///<
            ///< Primarily used with the waitForThread() method.
    };

protected:
    /// \brief The thread's run function.
    ///
    /// Users must overide this in your their derived class
    /// and then implement their threaded activity inside the loop.
    /// If the the users's threadedFunction does not have a loop,
    /// the contents of the threadedFunction will be executed once
    /// and the thread will then exit.
    ///
    /// For tasks that must be repeated, the user can use a while
    /// loop that will run repeatedly until the thread's
    /// threadRunning is set to false via the stopThread() method.
    ///
    ///     void MyThreadedClass::threadedFunction()
    ///     {
    ///         // Start the loop and continue until
    ///         // isThreadRunning() returns false.
    ///         while(isThreadRunning())
    ///         {
    ///             // Do activity repeatedly here:
    ///
    ///             // int j = 1 + 1;
    ///
    ///             // This while loop will run as fast as it possibly
    ///             // can, consuming as much processor speed as it can.
    ///             // To help the processor stay cool, users are
    ///             // encouraged to let the while loop sleep via the
    ///             // sleep() method, or call the yield() method to let
    ///             // other threads have a turn.  See the sleep() and
    ///             // yield() methods for more information.
    ///
    ///             // sleep(100);
    ///         }
    ///     }
    ///
    virtual void threadedFunction();

    /// \brief The Poco::Thread that runs the Poco::Runnable.
    Poco::Thread thread;

    /// \brief The internal mutex called through lock() & unlock().
    ///
    /// This mutext can also be used with ofScopedLock within the threaded
    /// function by calling:
    ///
    ///     ofScopedLock lock(mutex);
    ///
    mutable ofMutex mutex;

private:
    void run();
        ///< \brief Implements Poco::Runnable::run().

    Poco::AtomicCounter _threadRunning;
        ///< \brief Is the thread running?

    Poco::AtomicCounter _mutexBlocks;
        ///< \brief Should the mutex block?

    bool threadBeingWaitedFor;

};
