#pragma once
#include <mutex>
#include <queue>
#include <condition_variable>
#include "ofUtils.h"

/// Communication channel between different threads
/// allows for multithreaded programming without using
/// mutexes or other synchronization structures.
/// Instead of using some shared memory send a message
/// through a channel whenever one thread needs to send
/// some data to another.
/// Usually a second channel can be used to send the
/// processed data back to the first thread.
template<typename T>
class ofThreadChannel{
public:
	ofThreadChannel()
	:closed(false){}

	/// block until a new value is available
	/// and receive it in the passed parameter.
	/// returns true if there was a new value
	/// or false if the channel was closed
	bool receive(T & ret){
		std::unique_lock<std::mutex> lock(mutex);
		if(closed){
			return false;
		}
		if(queue.empty()){
			condition.wait(lock);
		}
		if(!closed){
			swap(ret,queue.front());
			queue.pop();
			return true;
		}else{
			return false;
		}
	}

	/// receives a new value in the passed parameter
	/// and returns true or returns false if there
	/// is no data available or the channel was closed
	bool tryReceive(T & ret){
		std::unique_lock<std::mutex> lock(mutex);
		if(closed){
			return false;
		}
		if(!queue.empty()){
			swap(ret,queue.front());
			queue.pop();
			return true;
		}else{
			return false;
		}
	}

	/// receives a new value in the passed parameter
	/// and returns true or returns false if there
	/// after the specified timeout in ms there is
	/// no data available or the channel was closed
	bool tryReceive(T & ret, int64_t timeoutMs){
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
			swap(ret,queue.front());
			queue.pop();
			return true;
		}else{
			return false;
		}
	}

	/// sends a copy of the passed value
	/// returns true if it was sent successfully
	/// or false if the channel was closed
	bool send(const T & val){
		std::unique_lock<std::mutex> lock(mutex);
		if(closed){
			return false;
		}
		queue.push(val);
		condition.notify_all();
		return true;
	}

	/// sends a value by moving it to avoid a copy.
	/// the original is invalidated. use like:
	///
	/// channel.send(std::move(value))
	///
	/// only c++11
	bool send(T && val){
		std::unique_lock<std::mutex> lock(mutex);
		if(closed){
			return false;
		}
		queue.push(val);
		condition.notify_all();
		return true;
	}

	/// closes the channel, from here on
	/// no new messages can be sent or received
	/// and any threads waiting to receive a value
	/// are awaken and return false
	void close(){
		std::unique_lock<std::mutex> lock(mutex);
		closed = true;
		condition.notify_all();
	}

private:
	std::queue<T> queue;
	std::mutex mutex;
	std::condition_variable condition;
	bool closed;
};
