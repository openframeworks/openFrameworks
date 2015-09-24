#pragma once

#include <vector>
#include <functional>
#include <mutex>
#include <thread>
#include <memory>
#include <iterator>

class ofEventAttendedException: public std::exception{};


template<typename Function, typename Mutex=std::mutex>
class ofBaseEvent{
public:
	/// \brief Basic constructor enabling an ofBaseEvent.
	///
	/// \see ofBaseEvent::ofBaseEvent(const ofBaseEvent & mom)
	/// \see ofBaseEvent::enable()
	/// \see ofBaseEvent::disable()
	/// \see ofBaseEvent::isEnabled()
	ofBaseEvent()
	:enabled(true)
    ,notifying(false){
	}

	/// \brief Copy-constructor for ofBaseEvent.
	///
	/// \see ofBaseEvent::ofBaseEvent()
	ofBaseEvent(const ofBaseEvent & mom)
	:enabled(mom.enabled)
	,notifying(false){
		std::unique_lock<Mutex> lck(const_cast<ofBaseEvent&>(mom).mtx);
		functions = mom.functions;
	}

	/// \brief Overloading the assignment operator.
	ofBaseEvent & operator=(const ofBaseEvent & mom){
		if(&mom==this){
			return *this;
		}
		std::unique_lock<Mutex> lck(const_cast<ofBaseEvent&>(mom).mtx);
		functions = mom.functions;
		enabled = mom.enabled;
        notifying = mom.notifying;
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
			if(it->priority>f.priority) break;
		}
		functions.emplace(it, f);
	}

	template<typename TFunction>
	void remove(const TFunction & function){
		std::unique_lock<Mutex> lck(mtx);
		if(notifying){
            for(auto & f: functions){
                if(f == function){
                    f.function = nullptr;
                }
            }
		}else{
            functions.erase(std::remove_if(functions.begin(), functions.end(),
                [&](Function & f){
                    return f.function == nullptr || f == function;
                }), functions.end());
		}
	}

	Mutex mtx;
	std::vector<Function> functions;
	bool enabled;
	bool notifying;
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
		virtual BaseFunctionId * clone() const = 0;
	};

	template <class T>
	class clone_ptr : public std::unique_ptr<T> {
	public:
		clone_ptr(T * t)
		  :std::unique_ptr<T>(t) { };

		clone_ptr(std::unique_ptr<T> && t)
		  :std::unique_ptr<T>(std::move(t)) { };

		clone_ptr(clone_ptr<T> && other) = default;
		clone_ptr<T> & operator=(clone_ptr<T> && other) = default;

		clone_ptr(const clone_ptr<T> & other)
		  :std::unique_ptr<T>(other->clone()) { }

		clone_ptr & operator=(const clone_ptr<T> & other) {
			this->reset(other->clone());
			return *this;
		}
	};

	template<typename T>
	class Function{
	public:
		Function(int priority, std::function<bool(const void*,T&)> function,  std::unique_ptr<BaseFunctionId>&& id )
		:priority(priority)
		,function(function)
		,id(std::move(id)){}

		bool operator==(const Function<T> & f) const{
			return f.priority == priority && *id == *f.id;
		}

		int priority;
		std::function<bool(const void*,T&)> function;
		clone_ptr<BaseFunctionId> id;
	};

	template<>
	class Function<void>{
	public:
		Function(int priority, std::function<bool(const void*)> function,  std::unique_ptr<BaseFunctionId> && id )
		:priority(priority)
		,function(function)
		,id(std::move(id)){}

		bool operator==(const Function<void> & f) const{
			return f.priority == priority && *id == *f.id;
		}

		int priority;
		std::function<bool(const void*)> function;
		clone_ptr<BaseFunctionId> id;
	};
}
}
/*! \endcond */

template<typename T, typename Mutex=std::mutex>
class ofEvent: public ofBaseEvent<of::priv::Function<T>,Mutex>{
protected:

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
	of::priv::Function<T> make_function(TObj * listener, bool (TObj::*method)(T&), int priority){
		return of::priv::Function<T>(priority, std::bind(method,listener,std::placeholders::_2), make_function_id(listener,method));
	}

	template<class TObj>
	of::priv::Function<T> make_function(TObj * listener, void (TObj::*method)(T&), int priority){
		return of::priv::Function<T>(priority, [listener, method](const void*, T&t){
			((listener)->*(method))(t);
			return false;
		}, make_function_id(listener,method));
	}

	template<class TObj>
	of::priv::Function<T> make_function(TObj * listener, bool (TObj::*method)(const void*, T&), int priority){
		return of::priv::Function<T>(priority, std::bind(method,listener,std::placeholders::_1,std::placeholders::_2), make_function_id(listener,method));
	}

	template<class TObj>
	of::priv::Function<T> make_function(TObj * listener, void (TObj::*method)(const void*, T&), int priority){
		return of::priv::Function<T>(priority, [listener, method](const void*s, T&t){
			std::bind(method,listener,std::placeholders::_1,std::placeholders::_2)(s,t);
			return false;
		}, make_function_id(listener,method));
	}

	of::priv::Function<T> make_function(bool (*function)(T&), int priority){
		return of::priv::Function<T>(priority, std::bind(function,std::placeholders::_2), make_function_id((ofEvent<T>*)nullptr,function));
	}

	of::priv::Function<T> make_function(void (*function)(T&), int priority){
		return of::priv::Function<T>(priority, [function](const void*, T&t){
			(function)(t);
			return false;
		}, make_function_id((ofEvent<T>*)nullptr,function));
	}

	of::priv::Function<T> make_function(bool (*function)(const void*, T&), int priority){
		return of::priv::Function<T>(priority, function, make_function_id((ofEvent<T>*)nullptr,function));
	}

	of::priv::Function<T> make_function(void (*function)(const void*, T&), int priority){
		return of::priv::Function<T>(priority, [function](const void*s, T&t){
			function(s,t);
			return false;
		}, make_function_id((ofEvent<T>*)nullptr,function));
	}
public:
	template<class TObj, typename TMethod>
	void add(TObj * listener, TMethod method, int priority){
		ofBaseEvent<of::priv::Function<T>,Mutex>::add(make_function(listener,method,priority));
	}

	template<class TObj, typename TMethod>
	void remove(TObj * listener, TMethod method, int priority){
		ofBaseEvent<of::priv::Function<T>,Mutex>::remove(make_function(listener,method,priority));
	}

	template<typename TFunction>
	void add(TFunction function, int priority){
		ofBaseEvent<of::priv::Function<T>,Mutex>::add(make_function(function,priority));
	}

	template<typename TFunction>
	void remove(TFunction function, int priority){
		ofBaseEvent<of::priv::Function<T>,Mutex>::remove(make_function(function,priority));
	}

	inline void notify(const void* sender, T & param){
		if(ofEvent<T,Mutex>::enabled && !ofEvent<T,Mutex>::functions.empty()){
			std::vector<of::priv::Function<T>*> functions_copy;
			{
				std::unique_lock<Mutex> lck(ofEvent<T,Mutex>::mtx);
				functions_copy.resize(ofEvent<T,Mutex>::functions.size());
				std::transform(ofEvent<T,Mutex>::functions.begin(), ofEvent<T,Mutex>::functions.end(),
						functions_copy.begin(),
						[&](of::priv::Function<T>&f){return &f;});
	            ofEvent<T,Mutex>::notifying = true;
			}
			for(auto & f: functions_copy){
                bool ret = false;
                try{
                    ret = f->function(sender,param);
                }catch(std::bad_function_call &){}
                if(ret){
                    throw ofEventAttendedException();
                }
			}
			ofEvent<T,Mutex>::notifying = false;
		}
	}
};

template<typename Mutex>
class ofEvent<void,Mutex>: public ofBaseEvent<of::priv::Function<void>,Mutex>{
protected:

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
	of::priv::Function<void> make_function(TObj * listener, bool (TObj::*method)(), int priority){
		return of::priv::Function<void>(priority, std::bind(method,listener), make_function_id(listener,method));
	}

	template<class TObj>
	of::priv::Function<void> make_function(TObj * listener, void (TObj::*method)(), int priority){
		return of::priv::Function<void>(priority,[listener, method](const void*){
			std::bind(method,listener)();
			return false;
		}, make_function_id(listener,method));
	}

	template<class TObj>
	of::priv::Function<void> make_function(TObj * listener, bool (TObj::*method)(const void*), int priority){
		return of::priv::Function<void>(priority,std::bind(method,listener,std::placeholders::_1), make_function_id(listener,method));
	}

	template<class TObj>
	of::priv::Function<void> make_function(TObj * listener, void (TObj::*method)(const void*), int priority){
		return of::priv::Function<void>(priority,[listener, method](const void* sender){
			std::bind(method,listener,std::placeholders::_1)(sender);
			return false;
		}, make_function_id(listener,method));
	}

	of::priv::Function<void> make_function(bool (*function)(), int priority){
		return of::priv::Function<void>(priority, std::bind(function), make_function_id((ofEvent<void>*)nullptr,function));
	}

	of::priv::Function<void> make_function(void (*function)(), int priority){
		return of::priv::Function<void>(priority,[function](const void*){
			function();
			return false;
		}, make_function_id((ofEvent<void>*)nullptr,function));
	}

	of::priv::Function<void> make_function(bool (*function)(const void*), int priority){
		return of::priv::Function<void>(priority, function, make_function_id((ofEvent<void>*)nullptr,function));
	}

	of::priv::Function<void> make_function(void (*function)(const void*), int priority){
		return of::priv::Function<void>(priority,[function](const void* sender){
			function(sender);
			return false;
		}, make_function_id((ofEvent<void>*)nullptr,function));
	}
public:
	template<class TObj, typename TMethod>
	void add(TObj * listener, TMethod method, int priority){
		ofBaseEvent<of::priv::Function<void>,Mutex>::add(make_function(listener,method,priority));
	}

	template<class TObj, typename TMethod>
	void remove(TObj * listener, TMethod method, int priority){
		ofBaseEvent<of::priv::Function<void>,Mutex>::remove(make_function(listener,method,priority));
	}

	template<typename TFunction>
	void add(TFunction function, int priority){
		ofBaseEvent<of::priv::Function<void>,Mutex>::add(make_function(function,priority));
	}

	template<typename TFunction>
	void remove(TFunction function, int priority){
		ofBaseEvent<of::priv::Function<void>,Mutex>::remove(make_function(function,priority));
	}

	void notify(const void* sender){
		if(ofEvent<void,Mutex>::enabled && !ofEvent<void,Mutex>::functions.empty()){
			std::vector<of::priv::Function<void>*> functions_copy;
			{
				std::unique_lock<Mutex> lck(ofEvent<void,Mutex>::mtx);
				std::transform(ofEvent<void,Mutex>::functions.begin(), ofEvent<void,Mutex>::functions.end(),
						std::back_inserter(functions_copy),
						[&](of::priv::Function<void> & f){return &f;});
	            ofEvent<void,Mutex>::notifying = true;
			}
			for(auto & f: functions_copy){
			    bool ret = false;
			    try{
			        ret = f->function(sender);
			    }catch(std::bad_function_call &){}
				if(ret){
					throw ofEventAttendedException();
				}
			}
			ofEvent<void,Mutex>::notifying = false;
		}
	}
};

/// Non thread safe event that avoids locks and copies of the listeners
/// making it faster than a plain ofEvent
template<typename T>
class ofFastEvent: public ofEvent<T,of::priv::NoopMutex>{
public:
	inline void notify(const void* sender, T & param){
		if(ofFastEvent::enabled && !ofFastEvent::functions.empty()){
		    ofFastEvent<T>::notifying = true;
			for(auto & f: ofFastEvent::functions){
                bool ret = false;
                try{
                    ret = f->function(sender,param);
                }catch(std::bad_function_call &){}
                if(ret){
                    throw ofEventAttendedException();
                }
			}
			ofFastEvent<T>::notifying = false;
		}
	}
};
