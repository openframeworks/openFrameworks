#pragma once

#include <vector>
#include <functional>
#include <mutex>
#include <thread>
#include <memory>
#include <iterator>

class ofEventAttendedException: public std::exception{};


template<typename Function, typename Mutex=std::recursive_mutex>
class ofBaseEvent{
public:
	/// \brief Basic constructor enabling an ofBaseEvent.
	///
	/// \see ofBaseEvent::ofBaseEvent(const ofBaseEvent & mom)
	/// \see ofBaseEvent::enable()
	/// \see ofBaseEvent::disable()
	/// \see ofBaseEvent::isEnabled()
	ofBaseEvent()
	:enabled(true){
	}

	/// \brief Copy-constructor for ofBaseEvent.
	///
	/// \see ofBaseEvent::ofBaseEvent()
	ofBaseEvent(const ofBaseEvent & mom)
	:enabled(mom.enabled){
		std::unique_lock<Mutex> lck(const_cast<ofBaseEvent&>(mom).mtx);
		functions = mom.functions;
	}

	/// \brief Overloading the assignment operator.
	ofBaseEvent & operator=(const ofBaseEvent & mom){
		if(&mom==this){
			return *this;
		}
		std::unique_lock<Mutex> lck(const_cast<ofBaseEvent&>(mom).mtx);
		std::unique_lock<Mutex> lck2(mtx);
		functions = mom.functions;
		enabled = mom.enabled;
		return *this;
	}

	/// \brief Enable an event.
	///
	/// \see ofBaseEvent::disable()
	/// \see ofBaseEvent::isEnabled()
	void enable() {
		enabled = true;
	}

	/// \brief Disable an event.
	///
	/// \see ofBaseEvent::enable()
	/// \see ofBaseEvent::isEnabled()
	void disable() {
		enabled = false;
	}

	/// \brief Check whether an event is enabled or not.
	///
	/// \returns true if enables
	/// \see ofBaseEvent::enable()
	/// \see ofBaseEvent::disable()
	bool isEnabled() const {
		return enabled;
	}

	std::size_t size() const {
		return functions.size();
	}

protected:
	template<typename TFunction>
	void add(TFunction && f){
		std::unique_lock<Mutex> lck(mtx);
		auto it = functions.begin();
		for(; it!=functions.end(); ++it){
			if((*it)->priority>f->priority) break;
		}
		functions.emplace(it, f);
	}

	template<typename TFunction>
	void remove(const TFunction & function){
		std::unique_lock<Mutex> lck(mtx);
		auto it = functions.begin();
		for(; it!=functions.end(); ++it){
			auto f = *it;
			if(*f == *function){
				f->disable();
				functions.erase(it);
				break;
			}
		}
	}

	Mutex mtx;
	std::vector<std::shared_ptr<Function>> functions;
	bool enabled;
};

/*! \cond PRIVATE */
namespace of{
namespace priv{
	class NoopMutex{
	public:
		void lock(){}
		void unlock(){}
	};

	class BaseFunctionId{
	public:
		virtual ~BaseFunctionId(){};
		virtual bool operator==(const BaseFunctionId &) const = 0;
	};

	template<typename T, class Mutex>
	class Function{
	public:
		Function(int priority, std::function<bool(const void*,T&)> function,  std::unique_ptr<BaseFunctionId>&& id )
		:priority(priority)
		,id(std::move(id))
		,function(function){}

		bool operator==(const Function<T,Mutex> & f) const{
			return f.priority == priority && *id == *f.id;
		}

		inline bool notify(const void*s,T&t){
			std::unique_lock<Mutex> lck(mtx);
			try{
				return function(s,t);
			}catch(std::bad_function_call &){
				return false;
			}
		}

		inline void disable(){
			std::unique_lock<Mutex> lck(mtx);
			function = nullptr;
		}

		int priority;
		std::unique_ptr<BaseFunctionId> id;

	private:
		std::function<bool(const void*,T&)> function;
		Mutex mtx;
	};

	template<class Mutex>
	class Function<void,Mutex>{
	public:
		Function(int priority, std::function<bool(const void*)> function,  std::unique_ptr<BaseFunctionId> && id )
		:priority(priority)
		,id(std::move(id))
		,function(function){}

		bool operator==(const Function<void,Mutex> & f) const{
			return f.priority == priority && *id == *f.id;
		}

		inline bool notify(const void*s){
			std::unique_lock<Mutex> lck(mtx);
			try{
				return function(s);
			}catch(std::bad_function_call &){
				return false;
			}
		}

		inline void disable(){
			std::unique_lock<Mutex> lck(mtx);
			function = nullptr;
		}

		int priority;
		std::unique_ptr<BaseFunctionId> id;
	private:
		std::function<bool(const void*)> function;
		Mutex mtx;
	};
}
}
/*! \endcond */

template<typename T, typename Mutex=std::recursive_mutex>
class ofEvent: public ofBaseEvent<of::priv::Function<T,Mutex>,Mutex>{
protected:
	typedef of::priv::Function<T,Mutex> Function;
	typedef std::shared_ptr<Function> FunctionPtr;

	template<class TObj, typename TMethod>
	class FunctionId: public of::priv::BaseFunctionId{
	public:
		TObj * listener;
		TMethod method;

		FunctionId(TObj * listener, TMethod method)
		:listener(listener)
		,method(method){

		}

		BaseFunctionId * clone() const{
			return new FunctionId<TObj,TMethod>(listener, method);
		}

		template<typename F>
		bool operator==(const F & f1) const{
			return f1.listener == this->listener && f1.method == this->method;
		}

		bool operator==(const BaseFunctionId & f) const{
			const auto * other = dynamic_cast<const FunctionId<TObj,TMethod>*>(&f);
			return other && other->listener == this->listener && other->method == this->method;
		}
	};

	template<class TObj, typename TMethod>
	std::unique_ptr<FunctionId<TObj,TMethod>> make_function_id(TObj * listener, TMethod method){
		return std::unique_ptr<FunctionId<TObj,TMethod>>(new FunctionId<TObj,TMethod>(listener,method));
	}

	template<class TObj>
	FunctionPtr make_function(TObj * listener, bool (TObj::*method)(T&), int priority){
		return std::make_shared<Function>(priority, std::bind(method,listener,std::placeholders::_2), make_function_id(listener,method));
	}

	template<class TObj>
	FunctionPtr make_function(TObj * listener, void (TObj::*method)(T&), int priority){
		return std::make_shared<Function>(priority, [listener, method](const void*, T&t){
			((listener)->*(method))(t);
			return false;
		}, make_function_id(listener,method));
	}

	template<class TObj>
	FunctionPtr make_function(TObj * listener, bool (TObj::*method)(const void*, T&), int priority){
		return std::make_shared<Function>(priority, std::bind(method,listener,std::placeholders::_1,std::placeholders::_2), make_function_id(listener,method));
	}

	template<class TObj>
	FunctionPtr make_function(TObj * listener, void (TObj::*method)(const void*, T&), int priority){
		return std::make_shared<Function>(priority, [listener, method](const void*s, T&t){
			std::bind(method,listener,std::placeholders::_1,std::placeholders::_2)(s,t);
			return false;
		}, make_function_id(listener,method));
	}

	FunctionPtr make_function(bool (*function)(T&), int priority){
		return std::make_shared<Function>(priority, std::bind(function,std::placeholders::_2), make_function_id((ofEvent<T>*)nullptr,function));
	}

	FunctionPtr make_function(void (*function)(T&), int priority){
		return std::make_shared<Function>(priority, [function](const void*, T&t){
			(function)(t);
			return false;
		}, make_function_id((ofEvent<T>*)nullptr,function));
	}

	FunctionPtr make_function(bool (*function)(const void*, T&), int priority){
		return std::make_shared<Function>(priority, function, make_function_id((ofEvent<T>*)nullptr,function));
	}

	FunctionPtr make_function(void (*function)(const void*, T&), int priority){
		return std::make_shared<Function>(priority, [function](const void*s, T&t){
			function(s,t);
			return false;
		}, make_function_id((ofEvent<T>*)nullptr,function));
	}
public:
	template<class TObj, typename TMethod>
	void add(TObj * listener, TMethod method, int priority){
		ofBaseEvent<of::priv::Function<T,Mutex>,Mutex>::add(make_function(listener,method,priority));
	}

	template<class TObj, typename TMethod>
	void remove(TObj * listener, TMethod method, int priority){
		ofBaseEvent<of::priv::Function<T,Mutex>,Mutex>::remove(make_function(listener,method,priority));
	}

	template<typename TFunction>
	void add(TFunction function, int priority){
		ofBaseEvent<of::priv::Function<T,Mutex>,Mutex>::add(make_function(function,priority));
	}

	template<typename TFunction>
	void remove(TFunction function, int priority){
		ofBaseEvent<of::priv::Function<T,Mutex>,Mutex>::remove(make_function(function,priority));
	}

	inline void notify(const void* sender, T & param){
		if(ofEvent<T,Mutex>::enabled && !ofEvent<T,Mutex>::functions.empty()){
			std::unique_lock<Mutex> lck(ofEvent<T,Mutex>::mtx);
			std::vector<std::shared_ptr<of::priv::Function<T,Mutex>>> functions_copy(ofEvent<T,Mutex>::functions);
			lck.unlock();
			for(auto & f: functions_copy){
                if(f->notify(sender,param)){
                    throw ofEventAttendedException();
                }
			}
		}
	}
};

template<typename Mutex>
class ofEvent<void,Mutex>: public ofBaseEvent<of::priv::Function<void,Mutex>,Mutex>{
protected:
	typedef of::priv::Function<void,Mutex> Function;
	typedef std::shared_ptr<Function> FunctionPtr;

	template<class TObj, typename TMethod>
	class FunctionId: public of::priv::BaseFunctionId{
	public:
		TObj * listener;
		TMethod method;

		FunctionId(TObj * listener, TMethod method)
		:listener(listener)
		,method(method){

		}

		BaseFunctionId * clone() const{
			return new FunctionId<TObj,TMethod>(listener, method);
		}

		template<typename F>
		bool operator==(const F & f1) const{
			return f1.listener == this->listener && f1.method == this->method;
		}

		bool operator==(const BaseFunctionId & f) const{
			const auto * other = dynamic_cast<const FunctionId<TObj,TMethod>*>(&f);
			return other && other->listener == this->listener && other->method == this->method;
		}
	};


	template<class TObj, typename TMethod>
	std::unique_ptr<FunctionId<TObj,TMethod>> make_function_id(TObj * listener, TMethod method){
		return std::unique_ptr<FunctionId<TObj,TMethod>>(new FunctionId<TObj,TMethod>(listener,method));
	}

	template<class TObj>
	FunctionPtr make_function(TObj * listener, bool (TObj::*method)(), int priority){
		return std::make_shared<Function>(priority, std::bind(method,listener), make_function_id(listener,method));
	}

	template<class TObj>
	FunctionPtr make_function(TObj * listener, void (TObj::*method)(), int priority){
		return std::make_shared<Function>(priority,[listener, method](const void*){
			std::bind(method,listener)();
			return false;
		}, make_function_id(listener,method));
	}

	template<class TObj>
	FunctionPtr make_function(TObj * listener, bool (TObj::*method)(const void*), int priority){
		return std::make_shared<Function>(priority,std::bind(method,listener,std::placeholders::_1), make_function_id(listener,method));
	}

	template<class TObj>
	FunctionPtr make_function(TObj * listener, void (TObj::*method)(const void*), int priority){
		return std::make_shared<Function>(priority,[listener, method](const void* sender){
			std::bind(method,listener,std::placeholders::_1)(sender);
			return false;
		}, make_function_id(listener,method));
	}

	FunctionPtr make_function(bool (*function)(), int priority){
		return std::make_shared<Function>(priority, std::bind(function), make_function_id((ofEvent<void>*)nullptr,function));
	}

	FunctionPtr make_function(void (*function)(), int priority){
		return std::make_shared<Function>(priority,[function](const void*){
			function();
			return false;
		}, make_function_id((ofEvent<void>*)nullptr,function));
	}

	FunctionPtr make_function(bool (*function)(const void*), int priority){
		return std::make_shared<Function>(priority, function, make_function_id((ofEvent<void>*)nullptr,function));
	}

	FunctionPtr make_function(void (*function)(const void*), int priority){
		return std::make_shared<Function>(priority,[function](const void* sender){
			function(sender);
			return false;
		}, make_function_id((ofEvent<void>*)nullptr,function));
	}
public:
	template<class TObj, typename TMethod>
	void add(TObj * listener, TMethod method, int priority){
		ofBaseEvent<of::priv::Function<void,Mutex>,Mutex>::add(make_function(listener,method,priority));
	}

	template<class TObj, typename TMethod>
	void remove(TObj * listener, TMethod method, int priority){
		ofBaseEvent<of::priv::Function<void,Mutex>,Mutex>::remove(make_function(listener,method,priority));
	}

	template<typename TFunction>
	void add(TFunction function, int priority){
		ofBaseEvent<of::priv::Function<void,Mutex>,Mutex>::add(make_function(function,priority));
	}

	template<typename TFunction>
	void remove(TFunction function, int priority){
		ofBaseEvent<of::priv::Function<void,Mutex>,Mutex>::remove(make_function(function,priority));
	}

	void notify(const void* sender){
		if(ofEvent<void,Mutex>::enabled && !ofEvent<void,Mutex>::functions.empty()){
			std::unique_lock<Mutex> lck(ofEvent<void,Mutex>::mtx);
			std::vector<std::shared_ptr<of::priv::Function<void,Mutex>>> functions_copy(ofEvent<void,Mutex>::functions);
			lck.unlock();
			for(auto & f: functions_copy){
				if(f->notify(sender)){
					throw ofEventAttendedException();
				}
			}
		}
	}
};

/// Non thread safe event that avoids locks and copies of the listeners
/// making it faster than a plain ofEvent
template<typename T>
class ofFastEvent: public ofEvent<T,of::priv::NoopMutex>{
public:
	inline void notify(const void* sender, T & param){
		if(ofFastEvent::enabled){
			for(auto & f: ofFastEvent::functions){
                if(f->notify(sender,param)){
                    throw ofEventAttendedException();
                }
			}
		}
	}
};
