#pragma once

#include "ofConstants.h"

//----------------------------------------------------------
// ofMutex
//----------------------------------------------------------

#include <mutex>
/// \brief A typedef for a cross-platform mutex.
/// \deprecated Please use std::mutex instead of ofMutex. See also the note below.
///
/// A mutex is used to lock data when it is accessible from multiple threads.
/// Locking data with a mutex prevents data-races, deadlocks and other problems
/// associated with concurrent access to data.
///
/// The mutex can be locked with a call to ofMutex::lock(). All calls to
/// ofMutex::lock() must be paired with a call to ofMutex::unlock().
///
/// ~~~~{.cpp}
///
///     ofMutex myMutex; // Your member mutex.
///     int mySharedData; // Your member shared data.
///
///     // ...
///
///     // A method to modify some shared data.
///     void modifyMySharedData() {
///         myMutex.lock(); // Lock the mutex.
///         mySharedData++; // Modify the shared data.
///         myMutex.unlock(); // Unlock the mutex;
///     }
///
/// ~~~~
///
/// \note Currently ofMutex is a typedef for std::mutex. This is done
/// to preserve backwards compatibility. Please use std::mutex for new
/// code.
///
/// \sa http://www.cplusplus.com/reference/mutex/mutex/
/// \sa ofScopedLock
typedef std::mutex ofMutex;

/// \brief A typedef for a cross-platform scoped mutex.
/// \deprecated Please use std::unique_lock<std::mutex> instead of ofScopedLock. See also the note below.
///
/// Normally ofMutex requres explicit calls to ofMutex::lock() and
/// ofMutex::unlock() to lock and release the mutex. Sometimes, despite best
/// efforts, developers forget to unlock a mutex, leaving the data inaccessible.
/// ofScopedLock makes ofMutex easier to use by calling ofMutex::unlock when
/// the scoped lock's destructor is called. Since the destructor is called when
/// a variable goes out of scope, we call this a "scoped lock". A "scoped lock"
/// is sometimes known as a "lock guard" as well.
///
/// ofScopedLock is used to lock and unlock an existing ofMutex.
///
/// ~~~~{.cpp}
///
///     ofMutex myMutex; // Your member mutex.
///     int mySharedData; // Your member shared data.
///
///     // ...
///
///     // A method to modify some shared data.
///     void modifyMySharedData() {
///         ofScopedLock lock(myMutex); // Lock the mutex.
///         mySharedData++; // Modify the shared data.
///
///         // `lock` will unlock the mutex when it goes out of scope.
///     }
///
/// ~~~~
///
/// \warning Currently ofScopedLock is a typedef for std::unique_lock<std::mutex>.
/// This is done to preserve backwards compatibility. Please use
/// std::unique_lock<std::mutex> for new code.
///
/// \sa http://en.cppreference.com/w/cpp/thread/unique_lock
/// \sa ofMutex
typedef std::unique_lock<std::mutex> ofScopedLock;



//----------------------------------------------------------
// ofPtr
//----------------------------------------------------------
template <typename T>
using ofPtr = std::shared_ptr<T>;
