#pragma once

#include "ofConstants.h"
#include "ofColor.h"

class ofSerial;

/// \brief Describes a Serial device, including ID, name and path.
class ofSerialDeviceInfo{
	friend class ofSerial;

	public:
		/// \brief Construct an ofSerialDeviceInfo with parameters.
		/// \param devicePathIn The path to the device.
		/// \param deviceNameIn The name of the device.
		/// \param deviceIDIn The ID of the device.
		ofSerialDeviceInfo(std::string devicePathIn, std::string deviceNameIn, int deviceIDIn){
			devicePath = devicePathIn;
			deviceName = deviceNameIn;
			deviceID = deviceIDIn;
		}

		/// \brief Construct an undefined serial device.
		ofSerialDeviceInfo(){
			deviceName = "device undefined";
			deviceID   = -1;
		}

		/// \brief Gets the path to the device
		///
		/// Example: `/dev/tty.cu/usbdevice-a440`.
		///
		/// \returns the device path.
		std::string getDevicePath(){
			return devicePath;
		}

		/// \brief Gets the name of the device
		///
		/// Example: `usbdevice-a440` or `COM4`.
		///
		/// \returns the device name.
		std::string getDeviceName(){
			return deviceName;
		}

		/// \brief Gets the ID of the device
		///
		/// Example: `0`,`1`,`2`,`3` etc.
		///
		/// \returns the device ID.
		int getDeviceID(){
			return deviceID;
		}

	protected:
		/// \cond INTERNAL

		/// \brief The device path (e.g /dev/tty.cu/usbdevice-a440).
		std::string devicePath;

		/// \brief The device name (e.g. usbdevice-a440 / COM4).
		std::string deviceName;

		/// \brief The device ID (e.g. 0, 1, 2, 3, etc).
		int deviceID;

		/// \endcond
};

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


// This is a helper method for make unique on platforms that support C++11, but not C++14.
#if !defined(NO_OF_MAKE_UNIQUE) && (defined(_MSC_VER) && _MSC_VER < 1800) || (!defined(_MSC_VER) && __cplusplus <= 201103L)

// Implementation for C++11 platforms that do not yet have std::make_unique.
// Implementation from http://stackoverflow.com/a/13512344/1518329
namespace std {


template <typename T, typename... Args>
std::unique_ptr<T> make_unique_helper(std::false_type, Args&&... args) {
	return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
}

template <typename T, typename... Args>
std::unique_ptr<T> make_unique_helper(std::true_type, Args&&... args) {
	static_assert(std::extent<T>::value == 0,
				  "make_unique<T[N]>() is forbidden, please use make_unique<T[]>().");

	typedef typename std::remove_extent<T>::type U;
	return std::unique_ptr<T>(new U[sizeof...(Args)]{std::forward<Args>(args)...});
}

template <typename T, typename... Args>
std::unique_ptr<T> make_unique(Args&&... args) {
	return make_unique_helper<T>(std::is_array<T>(), std::forward<Args>(args)...);
}


} // namespace std

#endif
