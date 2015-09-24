#pragma once

#include <vector>
#include <functional>
#include <mutex>
#include <thread>
#include <memory>
#include <iterator>


/*! \cond PRIVATE */
namespace of{
namespace priv{
	class NoopMutex{
	public:
		void lock(){}
		void unlock(){}
	};

	class AbstractFunctionId{
	public:
		AbstractFunctionId(){};
		virtual ~AbstractFunctionId(){};
		AbstractFunctionId (const AbstractFunctionId&) = delete;
		AbstractFunctionId & operator=(const AbstractFunctionId&) = delete;
	};

	class BaseFunctionId: public AbstractFunctionId{
	public:
		virtual ~BaseFunctionId(){};
		virtual bool operator==(const BaseFunctionId &) const = 0;
		virtual BaseFunctionId * clone() const = 0;
	};

	class StdFunctionId: public BaseFunctionId{
		static std::mutex mtx;
		static uint64_t nextId;
		uint64_t id;

		StdFunctionId(uint64_t id)
		:id(id){}

	public:
		StdFunctionId(){
			std::unique_lock<std::mutex> lck(mtx);
			id = nextId++;
		}

		bool operator==(const BaseFunctionId & otherid) const{
			const auto * other = dynamic_cast<const StdFunctionId*>(&otherid);
			return other && id == other->id;
		}

		BaseFunctionId * clone() const{
			return new StdFunctionId(id);
		}
	};

	inline std::unique_ptr<StdFunctionId> make_function_id(){
		return std::unique_ptr<StdFunctionId>(new StdFunctionId());
	}

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

		int priority;
		std::function<bool(const void*)> function;
		clone_ptr<BaseFunctionId> id;
	};

	class AbstractEventToken{
	public:
		virtual ~AbstractEventToken(){}
	};

	template<typename T, typename Mutex=std::mutex>
	class BaseEvent{
	public:
		BaseEvent()
		:data(new Data){}

		/// \brief Enable an event.
		///
		/// \see of::priv::BaseEvent::disable()
		/// \see of::priv::BaseEvent::isEnabled()
		void enable() {
			data->enabled = true;
		}

		/// \brief Disable an event.
		///
		/// \see of::priv::BaseEvent::enable()
		/// \see of::priv::BaseEvent::isEnabled()
		void disable() {
			data->enabled = false;
		}

		/// \brief Check whether an event is enabled or not.
		///
		/// \returns true if enables
		/// \see of::priv::BaseEvent::enable()
		/// \see of::priv::BaseEvent::disable()
		bool isEnabled() const {
			return data->enabled;
		}

		std::size_t size() const {
			return data->functions.size();
		}

		void clear(){
			data->functions.clear();
		}

	protected:

		class EventToken;

		std::vector<of::priv::Function<T>> & functions(){
			return data->functions;
		}

		bool isNotifying() const{
			return data->notifying;
		}

		void setNotifying(bool notifying){
			data->notifying = notifying;
		}

		template<typename TFunction>
		std::unique_ptr<EventToken> add(TFunction && f){
			auto token = make_token(f);
			data->add(f);
			return token;
		}

		template<typename TFunction>
		void addNoToken(TFunction && f){
			data->add(f);
		}

		void remove(const of::priv::AbstractFunctionId & token){
			data->remove(token);
		}

		struct Data{
			Data()
			:enabled(true)
			,notifying(false){}

			Mutex mtx;
			std::vector<of::priv::Function<T>> functions;
			bool enabled;
			bool notifying;

			template<typename TFunction>
			void add(TFunction && f){
				std::unique_lock<Mutex> lck(mtx);
				auto it = functions.begin();
				for(; it!=functions.end(); ++it){
					if(it->priority>f.priority) break;
				}
				functions.emplace(it, f);
			}

			void remove(const of::priv::AbstractFunctionId & token){
				auto & baseId = static_cast<const of::priv::BaseFunctionId&>(token);

				std::unique_lock<Mutex> lck(mtx);
				if(notifying){
					for(auto & f: functions){
						if(baseId == *f.id){
							f.function = nullptr;
						}
					}
				}else{
					functions.erase(std::remove_if(functions.begin(), functions.end(),
						[&](of::priv::Function<T> & f){
							return f.function == nullptr || *f.id == baseId;
						}), functions.end());
				}
			}
		};

		class EventToken: public AbstractEventToken{
		public:
			EventToken();
			EventToken(std::shared_ptr<BaseEvent<T,Mutex>::Data> & event, const of::priv::Function<T> & f)
			:event(event)
			,id(f.id->clone()){

			}
			~EventToken(){
				event->remove(*id);
			}

		private:
			std::shared_ptr<BaseEvent::Data> event;
			std::unique_ptr<of::priv::AbstractFunctionId> id;
		};

		std::unique_ptr<EventToken> make_token(const of::priv::Function<T> & f){
			return std::unique_ptr<EventToken>(new EventToken(data,f));
		}

		std::shared_ptr<Data> data;
	};
}
}
/*! \endcond */


enum ofEventOrder{
	OF_EVENT_ORDER_BEFORE_APP=0,
	OF_EVENT_ORDER_APP=100,
	OF_EVENT_ORDER_AFTER_APP=200
};

class ofEventListener{
public:
	ofEventListener(){}
	ofEventListener(std::unique_ptr<of::priv::AbstractEventToken> && token)
	:token(std::move(token)){}
	void unsubscribe(){
		token.reset();
	}
private:
	std::unique_ptr<of::priv::AbstractEventToken> token;
};

class ofEventAttendedException: public std::exception{};


template<typename T, typename Mutex=std::mutex>
class ofEvent: public of::priv::BaseEvent<T,Mutex>{
protected:

	template<class TObj, typename TMethod>
	class FunctionId: public of::priv::BaseFunctionId{
	public:
		TObj * listener;
		TMethod method;
		int priority;

		FunctionId(TObj * listener, TMethod method, int priority  = OF_EVENT_ORDER_APP)
		:listener(listener)
		,method(method)
		,priority(priority){

		}

		BaseFunctionId * clone() const{
			return new FunctionId<TObj,TMethod>(listener, method, priority);
		}

		template<typename F>
		bool operator==(const F & f1) const{
			return f1.listener == this->listener && f1.method == this->method && f1.priority == this->priority;
		}

		bool operator==(const BaseFunctionId & f) const{
			const auto * other = dynamic_cast<const FunctionId<TObj,TMethod>*>(&f);
			return other && other->listener == this->listener && other->method == this->method && other->priority == this->priority;
		}
	};

	of::priv::Function<T> make_function(std::function<bool(T&)> & f, int priority  = OF_EVENT_ORDER_APP){
		return of::priv::Function<T>(priority, [f](const void*,T&t){return f(t);}, of::priv::make_function_id());
	}

	of::priv::Function<T> make_function(std::function<bool(const void*,T&)> & f, int priority  = OF_EVENT_ORDER_APP){
		return of::priv::Function<T>(priority, f, of::priv::make_function_id());
	}

	of::priv::Function<T> make_function(std::function<void(T&)> & f, int priority  = OF_EVENT_ORDER_APP){
		return of::priv::Function<T>(priority, [f](const void*,T&t){f(t);return false;}, of::priv::make_function_id());
	}

	of::priv::Function<T> make_function(std::function<void(const void*,T&)> & f, int priority  = OF_EVENT_ORDER_APP){
		return of::priv::Function<T>(priority, [f](const void*s,T&t){f(s,t); return false;}, of::priv::make_function_id());
	}

	template<class TObj, typename TMethod>
	std::unique_ptr<FunctionId<TObj,TMethod>> make_function_id(TObj * listener, TMethod method, int priority  = OF_EVENT_ORDER_APP){
		return std::unique_ptr<FunctionId<TObj,TMethod>>(new FunctionId<TObj,TMethod>(listener,method,priority));
	}

	template<class TObj>
	of::priv::Function<T> make_function(TObj * listener, bool (TObj::*method)(T&), int priority  = OF_EVENT_ORDER_APP){
		return of::priv::Function<T>(priority, std::bind(method,listener,std::placeholders::_2), make_function_id(listener,method,priority));
	}

	template<class TObj>
	of::priv::Function<T> make_function(TObj * listener, void (TObj::*method)(T&), int priority  = OF_EVENT_ORDER_APP){
		return of::priv::Function<T>(priority, [listener, method](const void*, T&t){
			((listener)->*(method))(t);
			return false;
		}, make_function_id(listener,method,priority));
	}

	template<class TObj>
	of::priv::Function<T> make_function(TObj * listener, bool (TObj::*method)(const void*, T&), int priority  = OF_EVENT_ORDER_APP){
		return of::priv::Function<T>(priority, std::bind(method,listener,std::placeholders::_1,std::placeholders::_2), make_function_id(listener,method,priority));
	}

	template<class TObj>
	of::priv::Function<T> make_function(TObj * listener, void (TObj::*method)(const void*, T&), int priority  = OF_EVENT_ORDER_APP){
		return of::priv::Function<T>(priority, [listener, method](const void*s, T&t){
			std::bind(method,listener,std::placeholders::_1,std::placeholders::_2)(s,t);
			return false;
		}, make_function_id(listener,method,priority));
	}

	of::priv::Function<T> make_function(bool (*function)(T&), int priority  = OF_EVENT_ORDER_APP){
		return of::priv::Function<T>(priority, std::bind(function,std::placeholders::_2), make_function_id((ofEvent<T>*)nullptr,function,priority));
	}

	of::priv::Function<T> make_function(void (*function)(T&), int priority  = OF_EVENT_ORDER_APP){
		return of::priv::Function<T>(priority, [function](const void*, T&t){
			(function)(t);
			return false;
		}, make_function_id((ofEvent<T>*)nullptr,function,priority));
	}

	of::priv::Function<T> make_function(bool (*function)(const void*, T&), int priority  = OF_EVENT_ORDER_APP){
		return of::priv::Function<T>(priority, function, make_function_id((ofEvent<T>*)nullptr,function,priority));
	}

	of::priv::Function<T> make_function(void (*function)(const void*, T&), int priority  = OF_EVENT_ORDER_APP){
		return of::priv::Function<T>(priority, [function](const void*s, T&t){
			function(s,t);
			return false;
		}, make_function_id((ofEvent<T>*)nullptr,function,priority));
	}
public:
	template<class TObj>
	void add(TObj * listener, void (TObj::*method)(const void*, T&), int priority = OF_EVENT_ORDER_AFTER_APP){
		of::priv::BaseEvent<T,Mutex>::addNoToken(make_function(listener,method,priority));
	}

	template<class TObj>
	void add(TObj * listener, bool (TObj::*method)(const void*, T&), int priority = OF_EVENT_ORDER_AFTER_APP){
		of::priv::BaseEvent<T,Mutex>::addNoToken(make_function(listener,method,priority));
	}

	template<class TObj>
	void add(TObj * listener, void (TObj::*method)( T&), int priority = OF_EVENT_ORDER_AFTER_APP){
		of::priv::BaseEvent<T,Mutex>::addNoToken(make_function(listener,method,priority));
	}

	template<class TObj>
	void add(TObj * listener, bool (TObj::*method)(T&), int priority = OF_EVENT_ORDER_AFTER_APP){
		of::priv::BaseEvent<T,Mutex>::addNoToken(make_function(listener,method,priority));
	}

	template<class TObj>
	void remove(TObj * listener, void (TObj::*method)(const void*, T&), int priority = OF_EVENT_ORDER_AFTER_APP){
		of::priv::BaseEvent<T,Mutex>::remove(*make_function(listener,method,priority).id);
	}

	template<class TObj>
	void remove(TObj * listener, bool (TObj::*method)(const void*, T&), int priority = OF_EVENT_ORDER_AFTER_APP){
		of::priv::BaseEvent<T,Mutex>::remove(*make_function(listener,method,priority).id);
	}

	template<class TObj>
	void remove(TObj * listener, void (TObj::*method)(T&), int priority = OF_EVENT_ORDER_AFTER_APP){
		of::priv::BaseEvent<T,Mutex>::remove(*make_function(listener,method,priority).id);
	}

	template<class TObj>
	void remove(TObj * listener, bool (TObj::*method)(T&), int priority = OF_EVENT_ORDER_AFTER_APP){
		of::priv::BaseEvent<T,Mutex>::remove(*make_function(listener,method,priority).id);
	}

	ofEventListener newListener(std::function<bool(T&)> function, int priority  = OF_EVENT_ORDER_APP){
		return ofEventListener(of::priv::BaseEvent<T,Mutex>::add(make_function(function,priority)));
	}

	ofEventListener newListener(std::function<void(T&)> function, int priority  = OF_EVENT_ORDER_APP){
		return ofEventListener(of::priv::BaseEvent<T,Mutex>::add(make_function(function,priority)));
	}

	ofEventListener newListener(std::function<bool(const void*,T&)> function, int priority  = OF_EVENT_ORDER_APP){
		return ofEventListener(of::priv::BaseEvent<T,Mutex>::add(make_function(function,priority)));
	}

	ofEventListener newListener(std::function<void(const void*,T&)> function, int priority  = OF_EVENT_ORDER_APP){
		return ofEventListener(of::priv::BaseEvent<T,Mutex>::add(make_function(function,priority)));
	}

	template<class TObj>
	ofEventListener newListener(TObj * listener, void (TObj::*method)(const void*, T&), int priority  = OF_EVENT_ORDER_APP){
		return newListener(std::bind(method,listener,std::placeholders::_1,std::placeholders::_2),priority);
	}

	template<class TObj>
	ofEventListener newListener(TObj * listener, bool (TObj::*method)(const void*, T&), int priority  = OF_EVENT_ORDER_APP){
		return newListener(std::bind(method,listener,std::placeholders::_1,std::placeholders::_2),priority);
	}

	template<class TObj>
	ofEventListener newListener(TObj * listener, void (TObj::*method)(T&), int priority  = OF_EVENT_ORDER_APP){
		return newListener(std::bind(method,listener,std::placeholders::_1),priority);
	}

	template<class TObj>
	ofEventListener newListener(TObj * listener, bool (TObj::*method)(T&), int priority  = OF_EVENT_ORDER_APP){
		return newListener(std::bind(method,listener,std::placeholders::_1),priority);
	}

	void add(bool (*function)(T&), int priority = OF_EVENT_ORDER_AFTER_APP){
		of::priv::BaseEvent<T,Mutex>::addNoToken(make_function(function,priority));
	}

	void add(void (*function)(T&), int priority = OF_EVENT_ORDER_AFTER_APP){
		of::priv::BaseEvent<T,Mutex>::addNoToken(make_function(function,priority));
	}

	void add(bool (*function)(const void*,T&), int priority = OF_EVENT_ORDER_AFTER_APP){
		of::priv::BaseEvent<T,Mutex>::addNoToken(make_function(function,priority));
	}

	void add(void (*function)(const void*,T&), int priority = OF_EVENT_ORDER_AFTER_APP){
		of::priv::BaseEvent<T,Mutex>::addNoToken(make_function(function,priority));
	}

	template<typename TFunction>
	void remove(TFunction function, int priority = OF_EVENT_ORDER_AFTER_APP){
		of::priv::BaseEvent<T,Mutex>::remove(*make_function(function,priority).id);
	}

	inline void notify(const void* sender, T & param){
		if(ofEvent<T,Mutex>::data->enabled && !ofEvent<T,Mutex>::functions().empty()){
			std::vector<of::priv::Function<T>*> functions_copy;
			{
				std::unique_lock<Mutex> lck(ofEvent<T,Mutex>::data->mtx);
				functions_copy.resize(ofEvent<T,Mutex>::functions().size());
				std::transform(ofEvent<T,Mutex>::functions().begin(), ofEvent<T,Mutex>::functions().end(),
						functions_copy.begin(),
						[&](of::priv::Function<T>&f){return &f;});
	            ofEvent<T,Mutex>::setNotifying(true);
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
			ofEvent<T,Mutex>::setNotifying(false);
		}
	}
};

template<typename Mutex>
class ofEvent<void,Mutex>: public of::priv::BaseEvent<void,Mutex>{
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

	of::priv::Function<void> make_function(std::function<bool()> & f, int priority  = OF_EVENT_ORDER_APP){
		return of::priv::Function<void>(priority, [f](const void*){return f();}, of::priv::make_function_id());
	}

	of::priv::Function<void> make_function(std::function<bool(const void*)> & f, int priority  = OF_EVENT_ORDER_APP){
		return of::priv::Function<void>(priority, f, of::priv::make_function_id());
	}

	of::priv::Function<void> make_function(std::function<void()> & f, int priority  = OF_EVENT_ORDER_APP){
		return of::priv::Function<void>(priority, [f](const void*){f();return false;}, of::priv::make_function_id());
	}

	of::priv::Function<void> make_function(std::function<void(const void*)> & f, int priority  = OF_EVENT_ORDER_APP){
		return of::priv::Function<void>(priority, [f](const void*s){f(s); return false;}, of::priv::make_function_id());
	}

	template<class TObj, typename TMethod>
	std::unique_ptr<FunctionId<TObj,TMethod>> make_function_id(TObj * listener, TMethod method){
		return std::unique_ptr<FunctionId<TObj,TMethod>>(new FunctionId<TObj,TMethod>(listener,method));
	}

	template<class TObj>
	of::priv::Function<void> make_function(TObj * listener, bool (TObj::*method)(), int priority  = OF_EVENT_ORDER_APP){
		return of::priv::Function<void>(priority, std::bind(method,listener), make_function_id(listener,method));
	}

	template<class TObj>
	of::priv::Function<void> make_function(TObj * listener, void (TObj::*method)(), int priority  = OF_EVENT_ORDER_APP){
		return of::priv::Function<void>(priority,[listener, method](const void*){
			std::bind(method,listener)();
			return false;
		}, make_function_id(listener,method));
	}

	template<class TObj>
	of::priv::Function<void> make_function(TObj * listener, bool (TObj::*method)(const void*), int priority  = OF_EVENT_ORDER_APP){
		return of::priv::Function<void>(priority,std::bind(method,listener,std::placeholders::_1), make_function_id(listener,method));
	}

	template<class TObj>
	of::priv::Function<void> make_function(TObj * listener, void (TObj::*method)(const void*), int priority  = OF_EVENT_ORDER_APP){
		return of::priv::Function<void>(priority,[listener, method](const void* sender){
			std::bind(method,listener,std::placeholders::_1)(sender);
			return false;
		}, make_function_id(listener,method));
	}

	of::priv::Function<void> make_function(bool (*function)(), int priority  = OF_EVENT_ORDER_APP){
		return of::priv::Function<void>(priority, std::bind(function), make_function_id((ofEvent<void>*)nullptr,function));
	}

	of::priv::Function<void> make_function(void (*function)(), int priority  = OF_EVENT_ORDER_APP){
		return of::priv::Function<void>(priority,[function](const void*){
			function();
			return false;
		}, make_function_id((ofEvent<void>*)nullptr,function));
	}

	of::priv::Function<void> make_function(bool (*function)(const void*), int priority  = OF_EVENT_ORDER_APP){
		return of::priv::Function<void>(priority, function, make_function_id((ofEvent<void>*)nullptr,function));
	}

	of::priv::Function<void> make_function(void (*function)(const void*), int priority  = OF_EVENT_ORDER_APP){
		return of::priv::Function<void>(priority,[function](const void* sender){
			function(sender);
			return false;
		}, make_function_id((ofEvent<void>*)nullptr,function));
	}
public:
	template<class TObj>
	void add(TObj * listener, void (TObj::*method)(const void*), int priority = OF_EVENT_ORDER_AFTER_APP){
		of::priv::BaseEvent<void,Mutex>::addNoToken(make_function(listener,method,priority));
	}

	template<class TObj>
	void add(TObj * listener, bool (TObj::*method)(const void*), int priority = OF_EVENT_ORDER_AFTER_APP){
		of::priv::BaseEvent<void,Mutex>::addNoToken(make_function(listener,method,priority));
	}

	template<class TObj>
	void add(TObj * listener, void (TObj::*method)( ), int priority = OF_EVENT_ORDER_AFTER_APP){
		of::priv::BaseEvent<void,Mutex>::addNoToken(make_function(listener,method,priority));
	}

	template<class TObj>
	void add(TObj * listener, bool (TObj::*method)(), int priority = OF_EVENT_ORDER_AFTER_APP){
		of::priv::BaseEvent<void,Mutex>::addNoToken(make_function(listener,method,priority));
	}

	template<class TObj>
	void remove(TObj * listener, void (TObj::*method)(const void*), int priority = OF_EVENT_ORDER_AFTER_APP){
		of::priv::BaseEvent<void,Mutex>::remove(*make_function(listener,method,priority).id);
	}

	template<class TObj>
	void remove(TObj * listener, bool (TObj::*method)(const void*), int priority = OF_EVENT_ORDER_AFTER_APP){
		of::priv::BaseEvent<void,Mutex>::remove(*make_function(listener,method,priority).id);
	}

	template<class TObj>
	void remove(TObj * listener, void (TObj::*method)(), int priority = OF_EVENT_ORDER_AFTER_APP){
		of::priv::BaseEvent<void,Mutex>::remove(*make_function(listener,method,priority).id);
	}

	template<class TObj>
	void remove(TObj * listener, bool (TObj::*method)(), int priority = OF_EVENT_ORDER_AFTER_APP){
		of::priv::BaseEvent<void,Mutex>::remove(*make_function(listener,method,priority).id);
	}

	ofEventListener newListener(std::function<bool()> function, int priority  = OF_EVENT_ORDER_APP){
		return ofEventListener(of::priv::BaseEvent<void,Mutex>::add(make_function(function,priority)));
	}

	ofEventListener newListener(std::function<void()> function, int priority  = OF_EVENT_ORDER_APP){
		return ofEventListener(of::priv::BaseEvent<void,Mutex>::add(make_function(function,priority)));
	}

	ofEventListener newListener(std::function<bool(const void*)> function, int priority  = OF_EVENT_ORDER_APP){
		return ofEventListener(of::priv::BaseEvent<void,Mutex>::add(make_function(function,priority)));
	}

	ofEventListener newListener(std::function<void(const void*)> function, int priority  = OF_EVENT_ORDER_APP){
		return ofEventListener(of::priv::BaseEvent<void,Mutex>::add(make_function(function,priority)));
	}

	template<class TObj>
	ofEventListener newListener(TObj * listener, void (TObj::*method)(const void*), int priority  = OF_EVENT_ORDER_APP){
		return newListener(std::bind(method,listener,std::placeholders::_1,std::placeholders::_2),priority);
	}

	template<class TObj>
	ofEventListener newListener(TObj * listener, bool (TObj::*method)(const void*), int priority  = OF_EVENT_ORDER_APP){
		return newListener(std::bind(method,listener,std::placeholders::_1,std::placeholders::_2),priority);
	}

	template<class TObj>
	ofEventListener newListener(TObj * listener, void (TObj::*method)(), int priority  = OF_EVENT_ORDER_APP){
		return newListener(std::bind(method,listener,std::placeholders::_1),priority);
	}

	template<class TObj>
	ofEventListener newListener(TObj * listener, bool (TObj::*method)(), int priority  = OF_EVENT_ORDER_APP){
		return newListener(std::bind(method,listener,std::placeholders::_1),priority);
	}

	void add(bool (*function)(), int priority = OF_EVENT_ORDER_AFTER_APP){
		of::priv::BaseEvent<void,Mutex>::addNoToken(make_function(function,priority));
	}

	void add(void (*function)(), int priority = OF_EVENT_ORDER_AFTER_APP){
		of::priv::BaseEvent<void,Mutex>::addNoToken(make_function(function,priority));
	}

	void add(bool (*function)(const void*), int priority = OF_EVENT_ORDER_AFTER_APP){
		of::priv::BaseEvent<void,Mutex>::addNoToken(make_function(function,priority));
	}

	void add(void (*function)(const void*), int priority = OF_EVENT_ORDER_AFTER_APP){
		of::priv::BaseEvent<void,Mutex>::addNoToken(make_function(function,priority));
	}

	template<typename TFunction>
	void remove(TFunction function, int priority = OF_EVENT_ORDER_AFTER_APP){
		of::priv::BaseEvent<void,Mutex>::remove(*make_function(function,priority).id);
	}

	void notify(const void* sender){
		if(ofEvent<void,Mutex>::data->enabled && !ofEvent<void,Mutex>::functions().empty()){
			std::vector<of::priv::Function<void>*> functions_copy;
			{
				std::unique_lock<Mutex> lck(ofEvent<void,Mutex>::data->mtx);
				std::transform(ofEvent<void,Mutex>::functions().begin(), ofEvent<void,Mutex>::functions().end(),
						std::back_inserter(functions_copy),
						[&](of::priv::Function<void> & f){return &f;});
	            ofEvent<void,Mutex>::setNotifying(true);
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
			ofEvent<void,Mutex>::setNotifying(false);
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
		    ofFastEvent<T>::setNotifying(true);
			for(auto & f: ofFastEvent::functions){
                bool ret = false;
                try{
                    ret = f->function(sender,param);
                }catch(std::bad_function_call &){}
                if(ret){
                    throw ofEventAttendedException();
                }
			}
			ofFastEvent<T>::setNotifying(false);
		}
	}
};
