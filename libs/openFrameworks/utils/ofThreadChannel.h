#pragma once


#include <mutex>
#include <queue>
#include <condition_variable>
#include "ofUtils.h"


/// \brief Safely send data between threads without additional synchronization.
///
/// ofThreadChannel is a powerful class that makes it easy to safely pass data
/// between threads. When data is sent form one thread to another via the
/// ofThreadChannel, the user does not need to use additonal mutexes, condition
/// variables, critical sections, semaphores, monitors or other synchronization
/// primitives or techniques. Data sent with ofThreadChannel is synchronized
/// by ofThreadChannel's internal `std::mutex` and notifies receiving threads when
/// data is available from the sending thread.
///
/// A single ofThreadChannel class implements uni-directional communication. In
/// most cases an additional ofThreadChannel can be used for bi-directional
/// communication.
///
/// Data is sent through the ofThreadChannel in a FIFO (first in, first out)
/// order, guaranteeing that that data will be received in the same order that
/// it was sent.
///
/// If multiple threads attempt to send data using the same ofThreadChannel, the
/// send method will block the calling thread untill it is free. ofThreadChannel
/// does not use recursive mutexes and is therefore not designed to send and
/// receive data from the same thread. Doing will result in deadlocks.
///
/// ofThreadChannel is a templated class, meaning that the value type must be
/// set as a template argument during declaration.
///
/// ofThreadChannel is ideal for sharing most complex data structures between
/// threads. For example, if you want to share `ofPixels` between threads, one
/// would declare `ofThreadChannel<ofPixels> myChannel`. When used
/// with traditional C++ syntax, the data type must be copyable and one copy of
/// the data will be made when it is sent. This copy (and any associated
/// performance penalties) can be avoided using modern C++ `std::move`
/// semantics, but remember that source data will be invalidated.
///
/// While ofThreadChannel will certainly work for "plain old datatypes" (e.g.
/// `int`, `char`, `float`, etc), users are encouraged to use the new
/// `std::atomic` data types available with C++11 and later. These "lock-free"
/// data types require no special data protection for shared use between threads
/// and are guaranteed to be "thread-safe".
///
/// \sa http://en.cppreference.com/w/cpp/thread/mutex
/// \sa http://en.cppreference.com/w/cpp/utility/move
/// \sa http://en.cppreference.com/w/cpp/atomic/atomic
/// \sa https://github.com/openframeworks/ofBook/blob/master/chapters/threads/chapter.md
/// \sa https://en.wikipedia.org/wiki/Thread_(computing)
/// \tparam T The data type sent by the ofThreadChannel.
template<typename T>
class ofThreadChannel{
public:
    /// \brief Create a default ofThreadChannel.
    ///
    /// ofThreadChannel must be instantiated with a template parameter such as:
    /// ~~~~{.cpp}
    /// ofThreadChannel<ofPixels> myThreadChannel;
    /// ~~~~
	ofThreadChannel()
	:closed(false){}

    /// \brief Block the receiving thread until a new sent value is available.
    ///
    /// The receiving thread will pause (or "block") until a new sent value
    /// is available. In order to receive data, the user must create an instance
    /// of the data type, and pass it to the receive method to be filled.
    ///
    /// ofThreadChannel::receive will not make a copy or reallocate data.
    ///
    /// ~~~~{.cpp}
    /// ofThreadChannel<ofPixels> myThreadChannel;
    ///
    /// // ofThreadChannel<ofPixels> initialized elsewhere.
    ///
    /// ofPixels myPixelsToSet;
    ///
    /// if (myThreadChannel.receive(myPixelsToSet)) {
    ///    // If true, `myPixelsToSet` can be used.
    /// } else {
    ///    // If false, thread channel was closed and the value of
    ///    // `myPixelsToSet` may be invalid depending on the scope of
    ///    // `myPixelsToSet`.
    /// }
    /// ~~~~
    ///
    /// \param sentValue A reference to a sent value.
	/// \returns True if a new value was received or false if the ofThreadChannel was closed.
	bool receive(T & sentValue){
		std::unique_lock<std::mutex> lock(mutex);
		if(closed){
			return false;
		}
		if(queue.empty()){
			condition.wait(lock);
		}
		if(!closed){
            std::swap(sentValue,queue.front());
			queue.pop();
			return true;
		}else{
			return false;
		}
	}

    /// \brief If available, receive a new sent value without blocking.
    ///
    /// ofThreadChannel::tryReceive is similar to ofThreadChannel::receive,
    /// except that it will not pause (or "block") the receiving thread as it
    /// waits for sent data.  If no data is available, it will return false and
    /// continue immediately.
    ///
    /// ofThreadChannel::tryReceive will not make a copy or reallocate data.
    ///
    /// Like ofThreadChannel::receive, in order to receive data, the user must
    /// create an instance of the data type, and pass it to the receive method
    /// to be filled.
    ///
    /// ~~~~{.cpp}
    /// ofThreadChannel<ofPixels> myThreadChannel;
    ///
    /// // ofThreadChannel<ofPixels> initialized elsewhere.
    ///
    /// ofPixels myPixelsToSet;
    ///
    /// if (myThreadChannel.tryReceive(myPixelsToSet)) {
    ///    // If true, `myPixelsToSet` can be used.
    /// } else {
    ///    // If false, there was no new data OR the thread channel was closed.
    ///    // Either way, the value of `myPixelsToSet` may be invalid depending
    ///    // on the scope of `myPixelsToSet`.
    /// }
    /// ~~~~
    ///
    /// \param sentValue A reference to a sent value.
    /// \returns True if a new value was received or false if the ofThreadChannel was closed.
	bool tryReceive(T & sentValue){
		std::unique_lock<std::mutex> lock(mutex);
		if(closed){
			return false;
		}
		if(!queue.empty()){
            std::swap(sentValue,queue.front());
			queue.pop();
			return true;
		}else{
			return false;
		}
	}

    /// \brief If available, receive a new sent value or wait for a user-specified duration.
    ///
    /// ofThreadChannel::tryReceive is similar to ofThreadChannel::receive,
    /// except that it will pause (or "block") the receiving thread for a
    /// maximum of `timeoutMs` while it waits for sent data.  If no data is
    /// available during that time, it will return false and continue.
    ///
    /// ofThreadChannel::tryReceive will not make a copy or reallocate data.
    ///
    /// Like ofThreadChannel::receive, in order to receive data, the user must
    /// create an instance of the data type, and pass it to the receive method
    /// to be filled.
    ///
    /// ~~~~{.cpp}
    /// ofThreadChannel<ofPixels> myThreadChannel;
    ///
    /// // ofThreadChannel<ofPixels> initialized elsewhere.
    ///
    /// ofPixels myPixelsToSet;
    ///
    /// if (myThreadChannel.tryReceive(myPixelsToSet)) {
    ///    // If true, `myPixelsToSet` can be used.
    /// } else {
    ///    // If false, there was no new data OR the thread channel was closed.
    ///    // Either way, the value of `myPixelsToSet` may be invalid depending
    ///    // on the scope of `myPixelsToSet`.
    /// }
    /// ~~~~
    ///
    /// \param sentValue A reference to a sent value.
    /// \param timeoutMs The number of milliseconds to wait for new data before continuing.
    /// \returns True if a new value was received or false if the ofThreadChannel was closed.
	bool tryReceive(T & sentValue, int64_t timeoutMs){
		std::unique_lock<std::mutex> lock(mutex);
		if(closed){
			return false;
		}
		if(queue.empty()){
			if(condition.wait_for(lock,std::chrono::milliseconds(timeoutMs))==std::cv_status::timeout){
				return false;
			}
		}

		if(!closed){
			swap(sentValue,queue.front());
			queue.pop();
			return true;
		}else{
			return false;
		}
	}

    /// \brief Send a value to the receiver by making a copy.
    ///
    /// This method copies the contents of the sent value, leaving the original
    /// data unchanged.
    ///
    /// ~~~~{.cpp}
    /// ofThreadChannel<ofPixels> myThreadChannel;
    ///
    /// // ofThreadChannel<ofPixels> initialized elsewhere.
    ///
    /// ofPixels myPixelsToSend;
    ///
    /// // Fill the pixels with valid data, an image for example.
    ///
    /// ofLoadImage(myPixelsToSend, "myImage.jpg");
    ///
    /// // Send `myPixelsToSend` by copying it. `myPixelsToSend` is still valid
    /// // after sending.
    /// if (myThreadChannel.send(myPixelsToSend)) {
    ///    // If true, `myPixelsToSend` was sent successfully.
    /// } else {
    ///    // If false, the thread channel was closed.
    /// }
    /// ~~~~
    ///
    /// \returns true if the value was sent successfully or false if the channel was closed.
	bool send(const T & value){
		std::unique_lock<std::mutex> lock(mutex);
		if(closed){
			return false;
		}
		queue.push(value);
		condition.notify_all();
		return true;
	}

    /// \brief Send a value to the receiver without making a copy.
    ///
    /// This method moves the contents of the sent value using modern `std::move`
    /// semantics. This avoids copying the data, but the original data data will
    /// be invalidated. Note that the original data will be invalideated even if
    /// the send fails because the channel is already closed.
    ///
    /// ~~~~{.cpp}
    /// ofThreadChannel<ofPixels> myThreadChannel;
    ///
    /// // ofThreadChannel<ofPixels> initialized elsewhere.
    ///
    /// ofPixels myPixelsToSend;
    ///
    /// // Fill the pixels with valid data, an image for example.
    ///
    /// ofLoadImage(myPixelsToSend, "myImage.jpg");
    ///
    /// // Send `myPixelsToSend` by moving it. `myPixelsToSend` will no longer
    /// // be valid, even if the send fails because the channel is closed.
    /// if (myThreadChannel.send(std::move(myPixelsToSend))) {
    ///    // If true, `myPixelsToSend` was sent successfully.
    ///    // `myPixelsToSend` is no longer valid because it was moved.
    /// } else {
    ///    // If false, the thread channel was closed.
    ///    // `myPixelsToSend` is no longer valid because it was moved.
    /// }
    ///
    /// ~~~~
    ///
    /// \returns true if the value was sent successfully or false if the channel was closed.
	bool send(T && value){
		std::unique_lock<std::mutex> lock(mutex);
		if(closed){
			return false;
		}
		queue.push(value);
		condition.notify_all();
		return true;
	}

    /// \brief Close the ofThreadChannel.
    ///
    /// Closing the ofThreadChannel means that no new messages can be sent or
    /// received. All threads waiting to receive new values will be notified and
    /// all ofThreadChannel::receive and ofThreadChannel::tryReceive methods
    /// will return false.
    void close(){
		std::unique_lock<std::mutex> lock(mutex);
		closed = true;
		condition.notify_all();
	}

private:
    /// \brief The FIFO data queue.
    std::queue<T> queue;

    /// \brief The mutext to protect the data.
	std::mutex mutex;

    /// \brief The condition even to notify receivers.
	std::condition_variable condition;

    /// \brief True if the channel is closed.
	bool closed;

};
