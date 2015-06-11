#pragma once

#if __cplusplus>=201103
#include <vector>
#include <functional>
#include <mutex>
#include <thread>
#include <memory>

class ofEventAttendedException: public std::exception{};

class ofBaseEvent{
public:
	ofBaseEvent()
	:enabled(true){

	}

    void enable() {
    	enabled = true;
    }

    void disable() {
    	enabled = false;
    }

protected:
    class Function{
    public:
    	int priority;

    	Function(int priority)
    	:priority(priority){}

    	virtual ~Function(){}

    	template<typename F>
    	bool operator==(const F & f1){
    		const auto * thisAsF = dynamic_cast<const F*>(this);
    		auto ret = thisAsF && f1.priority == priority;
    		if(ret){
    			if(f1.listener!=nullptr && f1.method!=nullptr){
    				ret &= f1.listener == thisAsF->listener && f1.method == thisAsF->method;
    			}
    		}
    		return ret;
    	}
    };

    template<typename TFunction>
    void add(TFunction * f){
        std::unique_lock<std::mutex> lck(mtx);
        auto it = functions.begin();
        for(; it!=functions.end(); ++it){
        	if((*it)->priority>f->priority) break;
        }
    	functions.emplace(it, f);
    }

    template<typename TFunction>
    void remove(const TFunction & function){
        std::unique_lock<std::mutex> lck(mtx);
    	functions.erase(std::remove_if(functions.begin(), functions.end(),
    		[&](const std::unique_ptr<Function> & f){
        		return *f == function;
        	}), functions.end());
    }

    std::mutex mtx;
    std::vector<std::unique_ptr<Function>> functions;
    bool enabled;
};


template<typename T>
class ofEvent: public ofBaseEvent{
	class BaseFunction: public ofBaseEvent::Function{
	public:
		BaseFunction(int priority)
		:ofBaseEvent::Function(priority){}
		virtual ~BaseFunction(){}
		virtual bool call(const void * sender, T & param) = 0;
	};

	template<class TObj, typename TMethod>
	class Function: public BaseFunction{
	public:
		std::function<bool(const void*,T&)> function;
		TObj * listener;
		TMethod method;

		Function(TObj * listener, bool (TObj::*method)(T&), int priority)
		:BaseFunction(priority)
		,function(std::bind(method,listener,std::placeholders::_2))
		,listener(listener)
		,method(method){

		}

		Function(TObj * listener, void (TObj::*method)(T&), int priority)
		:BaseFunction(priority)
		,function([listener, method](const void*, T&t){
			std::bind(method,listener,std::placeholders::_1)(t);
			return false;
		})
		,listener(listener)
		,method(method){

		}

		Function(TObj * listener, bool (TObj::*method)(const void*, T&), int priority)
		:BaseFunction(priority)
		,function(std::bind(method,listener,std::placeholders::_1,std::placeholders::_2))
		,listener(listener)
		,method(method){

		}

		Function(TObj * listener, void (TObj::*method)(const void*, T&), int priority)
		:BaseFunction(priority)
		,function([listener, method](const void*s, T&t){
			std::bind(method,listener,std::placeholders::_1,std::placeholders::_2)(s,t);
			return false;
		})
		,listener(listener)
		,method(method){

		}

		Function(std::function<bool(const void*,T&)> function, int priority)
		:BaseFunction(priority)
		,function(function)
		,listener(nullptr)
		,method(nullptr){

		}

		bool call(const void* sender, T & t){
			return function(sender, t);
		}
	};
public:
    template<class TObj, typename TMethod>
    void add(TObj * listener, TMethod method, int priority){
        ofBaseEvent::add(new Function<TObj,TMethod>(listener,method,priority));
    }

    template<class TObj, typename TMethod>
    void remove(TObj * listener, TMethod method, int priority){
        ofBaseEvent::remove(Function<TObj,TMethod>(listener,method,priority));
    }

    void notify(const void* sender, T & param){
        if(enabled){
            std::unique_lock<std::mutex> lck(mtx);
			for(auto & f: functions){
				if(static_cast<BaseFunction*>(f.get())->call(sender,param)){
					throw ofEventAttendedException();
				}
			}
        }
    }
};

template<>
class ofEvent<void>: public ofBaseEvent{
	class BaseFunction: public ofBaseEvent::Function{
	public:
		BaseFunction(int priority)
		:ofBaseEvent::Function(priority){}
		virtual ~BaseFunction(){}
		virtual bool call(const void * sender) = 0;
	};

	template<class TObj, typename TMethod>
	class Function: public BaseFunction{
	public:
		std::function<bool(const void*)> function;
		TObj * listener;
		TMethod method;

		Function(TObj * listener, bool (TObj::*method)(), int priority)
		:BaseFunction(priority)
		,function(std::bind(method,listener))
		,listener(listener)
		,method(method){

		}

		Function(TObj * listener, void (TObj::*method)(), int priority)
		:BaseFunction(priority)
		,function([listener, method](const void*){
			std::bind(method,listener)();
			return false;
		})
		,listener(listener)
		,method(method){

		}

		Function(TObj * listener, bool (TObj::*method)(const void*), int priority)
		:BaseFunction(priority)
		,function(std::bind(method,listener,std::placeholders::_1))
		,listener(listener)
		,method(method){

		}

		Function(TObj * listener, void (TObj::*method)(const void*), int priority)
		:BaseFunction(priority)
		,function([listener, method](const void*s){
			std::bind(method,listener,std::placeholders::_1)(s);
			return false;
		})
		,listener(listener)
		,method(method){

		}

		Function(std::function<bool(const void*)> function, int priority)
		:BaseFunction(priority)
		,function(function)
		,listener(nullptr)
		,method(nullptr){

		}

		bool call(const void* sender){
			return function(sender);
		}
	};
public:
    template<class TObj, typename TMethod>
    void add(TObj * listener, TMethod method, int priority){
        ofBaseEvent::add(new Function<TObj,TMethod>(listener,method,priority));
    }

    template<class TObj, typename TMethod>
    void remove(TObj * listener, TMethod method, int priority){
        ofBaseEvent::remove(Function<TObj,TMethod>(listener,method,priority));
    }

    void notify(const void* sender){
        if(enabled){
            std::unique_lock<std::mutex> lck(mtx);
			for(auto & f: functions){
				if(static_cast<BaseFunction*>(f.get())->call(sender)){
					throw ofEventAttendedException();
				}
			}
        }
    }
};
#else

//-----------------------------------------
// define ofEvent as a poco FIFOEvent
// to create your own events use:
// ofEvent<argType> myEvent


#include "Poco/PriorityEvent.h"
#include "Poco/PriorityDelegate.h"
#include "ofDelegate.h"

namespace Poco{
	template<class TObj, class T>
	ofDelegate<TObj,T,false> priorityDelegate(TObj * listener, bool(TObj::*method)(T&t), int priority){
		return ofDelegate<TObj,T,false>(listener,method,priority);
	}

	template<class TObj, class T>
	ofDelegate<TObj,T,true> priorityDelegate(TObj * listener, bool(TObj::*method)(const void *, T&t), int priority){
		return ofDelegate<TObj,T,false>(listener,method,priority);
	}

	template<class TObj>
	ofDelegate<TObj,void,false> priorityDelegate(TObj * listener, bool(TObj::*method)(), int priority){
		return ofDelegate<TObj,void,false>(listener,method,priority);
	}

	template<class TObj>
	ofDelegate<TObj,void,true> priorityDelegate(TObj * listener, bool(TObj::*method)(const void *), int priority){
		return ofDelegate<TObj,void,false>(listener,method,priority);
	}
}

template <typename ArgumentsType>
class ofEvent: public Poco::PriorityEvent<ArgumentsType> {
public:

	ofEvent():Poco::PriorityEvent<ArgumentsType>(){

	}

	// allow copy of events, by copying everything except the mutex
	ofEvent(const ofEvent<ArgumentsType> & mom)
	:Poco::PriorityEvent<ArgumentsType>()
	{
		this->_enabled = mom._enabled;
	}

	ofEvent<ArgumentsType> & operator=(const ofEvent<ArgumentsType> & mom){
		if(&mom == this) return *this;
		this->_enabled = mom._enabled;
		return *this;
	}

    template<class TObj, typename TMethod>
    void add(TObj * listener, TMethod method, int priority){
        *this += Poco::priorityDelegate(listener,method,priority);
    }

    template<class TObj, typename TMethod>
    void remove(TObj * listener, TMethod method, int priority){
        *this -= Poco::priorityDelegate(listener,method,priority);
    }
};
#endif
