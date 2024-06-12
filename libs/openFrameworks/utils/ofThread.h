#pragma once

#ifndef TARGET_NO_THREADS

#include <atomic>
#include <condition_variable>
#include <mutex>
#include <thread>
#include <string>

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
/// ofThread is a convenient wrapper for std::thread. It is a
/// pathway for quickly writing threaded classes. Implementing
/// threads that access shared data is tricky and can lead to deadlocks.
/// A deadlock is as bad as it sounds.  It means your program
/// just stops.
///
/// Uncaught Exceptions throw from within ofThread will cause the thread to stop
/// and the Exception will be delivered to the default ofBaseThreadErrorHandler.
/// The ofBaseThreadErrorHandler will print the exception details, if available.
/// The ofBaseThreadErrorHandler offers no opportunity to take corrective action
/// and only allows the user to receive more valuable debugging information
/// about the uncaught exception.  Users should design ofThread subclasses to
/// catch and respond to all anticipated exceptions.
class ofThread {
public:
	/// \brief Create an ofThread.
	ofThread();

	/// \brief Check the running status of the thread.
	/// \returns true iff the thread is currently running.
	bool isThreadRunning() const;

	/// \brief Get the unique thread id.
	/// \note This is NOT the the same as the operating thread id!
	std::thread::id getThreadId() const;

	/// \brief Get the unique thread name, in the form of "Thread id#"
	/// \returns the Thread ID string.
	std::string getThreadName() const;

	void setThreadName(const std::string & name);

	/// \brief Start the thread.
	/// \note Subclasses can directly access the mutex and employ thier
	///       own locking strategy.
	void startThread();

	/// \brief Start the thread with options.
	/// \param mutexBlocks Set blocking to true if you want the mutex to
	///        block when lock() is called.
	/// \note Subclasses can directly access the mutex and employ thier
	///       own locking strategy.
	[[deprecated("Use tryLock instead of setting the type of lock on startThread")]]
	void startThread(bool mutexBlocks);

	/// \brief Lock the mutex.
	///
	/// If the thread was started startThread(true), then this call will wait
	/// until the mutex is available and return true.  If the thread was started
	/// startThread(false), this call will return true iff the mutex is
	/// was successfully acquired.
	///
	/// \returns true if the lock was successfully acquired.
	bool lock();

	/// \brief Tries to lock the mutex.
	///
	/// If the thread was started startThread(true), then this call will wait
	/// until the mutex is available and return true.  If the thread was started
	/// startThread(false), this call will return true iff the mutex is
	/// was successfully acquired.
	///
	/// \returns true if the lock was successfully acquired.
	bool tryLock();

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
	///     for a thread to complete via other signalling methods.

	void waitForThread(bool callStopThread = true,
		long milliseconds = INFINITE_JOIN_TIMEOUT);

	/// \brief Tell the thread to sleep for a certain amount of milliseconds.
	///
	/// This is useful inside the threadedFunction() when a thread is waiting
	/// for input to process:
	///
	///     void MyThreadedClass::threadedFunction()
	///     {
	///            // start
	///            while(isThreadRunning())
	///         {
	///             // bReadyToProcess can be set from outside the threadedFuntion.
	///             // perhaps by another thread that downloads data, or loads
	///             // some media, etc.
	///
	///                if(bReadyToProcess == true)
	///             {
	///                    // do some time intensive processing
	///                    bReadyToProcess = false;
	///                }
	///             else
	///             {
	///                    // sleep the thread to give up some cpu
	///                    sleep(20);
	///                }
	///            }
	///            // done
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

	/// \brief Get a reference to the underlying thread.
	/// \returns A reference to the underlying thread.
	std::thread & getNativeThread();

	/// \brief Get a const reference to the underlying  thread.
	/// \returns A reference to the backing thread.
	const std::thread & getNativeThread() const;

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

	/// \brief The thread object
	std::thread thread;

	/// \brief The internal mutex called through lock() & unlock().
	///
	/// This mutext can also be used with std::unique_lock or lock_guard
	/// within the threaded function by calling:
	///
	///     std::unique_lock<std::mutex> lock(mutex);
	///
	mutable std::mutex mutex;

private:
	///< \brief runs the threadedFunction.
	void run();

	///< \brief Is the thread running?
	std::atomic<bool> threadRunning;
	std::atomic<bool> threadDone;

	///< \brief Should the mutex block?
	std::atomic<bool> mutexBlocks;

	std::string name;
	std::condition_variable condition;
};

#else

class ofThread {
public:
	void lock() { }
	void unlock() { }
	void startThread() { }
	void stopThread() {};
	bool isThreadRunning() { return false; }

	enum {
		INFINITE_JOIN_TIMEOUT = LONG_MAX
	};
};
#endif
