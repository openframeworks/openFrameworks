#pragma once

#include <vector>
#include <functional>
#include <mutex>
#include <thread>
#include <memory>
#include <iterator>
#include <atomic>

class ofEventAttendedException: public std::exception{};


/*! \cond PRIVATE */
namespace of{
namespace priv{
	class NoopMutex{
	public:
		void lock(){}
		void unlock(){}
	};

	class AbstractEventToken{
		public:
			virtual ~AbstractEventToken();
	};

	class BaseFunctionId{
	public:
		BaseFunctionId(){}
		BaseFunctionId(const BaseFunctionId &) = delete;
		BaseFunctionId & operator=(const BaseFunctionId &) = delete;
		virtual ~BaseFunctionId();
		virtual bool operator==(const BaseFunctionId &) const = 0;
		virtual BaseFunctionId * clone() const = 0;
	};

	class StdFunctionId: public BaseFunctionId{
		static std::atomic<uint_fast64_t> nextId;
		uint64_t id;

		StdFunctionId(uint64_t id)
		:id(id){}
	public:
		StdFunctionId(){
			id = nextId++;
		}

		virtual ~StdFunctionId();

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


	template<typename Function, typename Mutex=std::recursive_mutex>
	class BaseEvent{
	public:
		BaseEvent(){}

		BaseEvent(const BaseEvent & mom){
			std::unique_lock<Mutex> lck(const_cast<BaseEvent&>(mom).self->mtx);
			self->functions = mom.self->functions;
		}

		BaseEvent & operator=(const BaseEvent & mom){
			if(&mom==this){
				return *this;
			}
			std::unique_lock<Mutex> lck(const_cast<BaseEvent&>(mom).self->mtx);
			std::unique_lock<Mutex> lck2(self->mtx);
			self->functions = mom.self->functions;
			self->enabled = mom.self->enabled;
			return *this;
		}

		BaseEvent(BaseEvent && mom){
			std::unique_lock<Mutex> lck(const_cast<BaseEvent&>(mom).self->mtx);
			self->functions = std::move(mom.self->functions);
			self->enabled = std::move(mom.self->enabled);
		}

		BaseEvent & operator=(BaseEvent && mom){
			if(&mom==this){
				return *this;
			}
			std::unique_lock<Mutex> lck(const_cast<BaseEvent&>(mom).self->mtx);
			std::unique_lock<Mutex> lck2(self->mtx);
			self->functions = mom.self->functions;
			self->enabled = mom.self->enabled;
			return *this;
		}

		void enable() {
			self->enabled = true;
		}

		void disable() {
			self->enabled = false;
		}

		bool isEnabled() const {
			return self->enabled;
		}

		std::size_t size() const {
			return self->functions.size();
		}

	protected:

#if _MSC_VER
		template<typename> struct check_function;
		template<typename R, typename... Args>
		struct check_function<R(Args...)> : public std::function<R(Args...)> {
			template<typename T,
			class = typename std::enable_if<
				std::is_same<R, void>::value
				|| std::is_convertible<
				decltype(std::declval<T>()(std::declval<Args>()...)),
				R>::value>::type>
				check_function(T &&t) : std::function<R(Args...)>(std::forward<T>(t)) { }
		};
#endif

		struct Data{
			Mutex mtx;
			std::vector<std::shared_ptr<Function>> functions;
			bool enabled = true;

			void remove(const BaseFunctionId & id){
				std::unique_lock<Mutex> lck(mtx);
				auto it = functions.begin();
				for(; it!=functions.end(); ++it){
					auto f = *it;
					if(*f->id == id){
						f->disable();
						functions.erase(it);
						break;
					}
				}
			}
		};
		std::shared_ptr<Data> self{new Data};

		class EventToken: public AbstractEventToken{
			public:
				EventToken() {};
				template<typename Id>
				EventToken(std::shared_ptr<Data> & event, const Id & id)
				:event(event)
				,id(id.clone()){

				}

				~EventToken(){
					auto event = this->event.lock();
					if(event){
						event->remove(*id);
					}
				}

			private:
				std::weak_ptr<Data> event;
				std::unique_ptr<BaseFunctionId> id;
		};

		std::unique_ptr<EventToken> make_token(const Function & f){
			return std::unique_ptr<EventToken>(new EventToken(self,*f.id));
		}

		template<typename TFunction>
		void addNoToken(TFunction && f){
			std::unique_lock<Mutex> lck(self->mtx);
			auto it = self->functions.begin();
			for(; it!=self->functions.end(); ++it){
				if((*it)->priority>f->priority) break;
			}
			self->functions.emplace(it, f);
		}

		template<typename TFunction>
		std::unique_ptr<EventToken> addFunction(TFunction && f){
			std::unique_lock<Mutex> lck(self->mtx);
			auto it = self->functions.begin();
			for(; it!=self->functions.end(); ++it){
				if((*it)->priority>f->priority) break;
			}
			self->functions.emplace(it, f);
			return make_token(*f);
		}

		template<typename TFunction>
		void removeFunction(const TFunction & function){
			self->remove(*function->id);
		}
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


template<typename T, typename Mutex=std::recursive_mutex>
class ofEvent: public of::priv::BaseEvent<of::priv::Function<T,Mutex>,Mutex>{
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

#ifdef _MSC_VER
	FunctionPtr make_function(check_function<bool(T&)> f, int priority){
		return std::make_shared<Function>(priority, [f](const void*,T&t){return f(t);}, of::priv::make_function_id());
	}

	FunctionPtr make_function(check_function<bool(const void*,T&)> f, int priority){
		return std::make_shared<Function>(priority, f, of::priv::make_function_id());
	}

	FunctionPtr make_function(check_function<void(T&)> f, int priority){
		return std::make_shared<Function>(priority, [f](const void*,T&t){f(t);return false;}, of::priv::make_function_id());
	}

	FunctionPtr make_function(check_function<void(const void*,T&)> f, int priority){
		return std::make_shared<Function>(priority, [f](const void*s,T&t){f(s,t); return false;}, of::priv::make_function_id());
	}
#else
	FunctionPtr make_function(std::function<bool(T&)> f, int priority) {
		return std::make_shared<Function>(priority, [f](const void*, T&t) {return f(t); }, of::priv::make_function_id());
	}

	FunctionPtr make_function(std::function<bool(const void*, T&)> f, int priority) {
		return std::make_shared<Function>(priority, f, of::priv::make_function_id());
	}

	FunctionPtr make_function(std::function<void(T&)> f, int priority) {
		return std::make_shared<Function>(priority, [f](const void*, T&t) {f(t); return false; }, of::priv::make_function_id());
	}

	FunctionPtr make_function(std::function<void(const void*, T&)> f, int priority) {
		return std::make_shared<Function>(priority, [f](const void*s, T&t) {f(s, t); return false; }, of::priv::make_function_id());
	}

	using of::priv::BaseEvent<of::priv::Function<T,Mutex>,Mutex>::removeFunction;
	using of::priv::BaseEvent<of::priv::Function<T,Mutex>,Mutex>::addFunction;
	using of::priv::BaseEvent<of::priv::Function<T,Mutex>,Mutex>::addNoToken;
#endif

public:
	template<class TObj, typename TMethod>
	ofEventListener newListener(TObj * listener, TMethod method, int priority = OF_EVENT_ORDER_AFTER_APP){
		return ofEventListener(addFunction(make_function(listener,method,priority)));
	}

	template<class TObj, typename TMethod>
	void add(TObj * listener, TMethod method, int priority){
		addNoToken(make_function(listener,method,priority));
	}

	template<class TObj, typename TMethod>
	void remove(TObj * listener, TMethod method, int priority){
		removeFunction(make_function(listener,method,priority));
	}

	template<typename TFunction>
	ofEventListener newListener(TFunction function, int priority = OF_EVENT_ORDER_AFTER_APP) {
		return ofEventListener(addFunction(make_function(function, priority)));
	}

	template<typename TFunction>
	void add(TFunction function, int priority){
		addNoToken(make_function(function,priority));
	}

	template<typename TFunction>
	void remove(TFunction function, int priority){
		removeFunction(make_function(function,priority));
	}

	inline void notify(const void* sender, T & param){
		if(ofEvent<T,Mutex>::self->enabled && !ofEvent<T,Mutex>::self->functions.empty()){
			std::unique_lock<Mutex> lck(ofEvent<T,Mutex>::self->mtx);
			std::vector<std::shared_ptr<of::priv::Function<T,Mutex>>> functions_copy(ofEvent<T,Mutex>::self->functions);
			lck.unlock();
			for(auto & f: functions_copy){
                if(f->notify(sender,param)){
                    throw ofEventAttendedException();
                }
			}
		}
	}

	inline void notify(T & param){
		if(ofEvent<T,Mutex>::self->enabled && !ofEvent<T,Mutex>::self->functions.empty()){
			std::unique_lock<Mutex> lck(ofEvent<T,Mutex>::self->mtx);
			std::vector<std::shared_ptr<of::priv::Function<T,Mutex>>> functions_copy(ofEvent<T,Mutex>::self->functions);
			lck.unlock();
			for(auto & f: functions_copy){
				if(f->notify(nullptr,param)){
					throw ofEventAttendedException();
				}
			}
		}
	}
};

template<typename Mutex>
class ofEvent<void,Mutex>: public of::priv::BaseEvent<of::priv::Function<void,Mutex>,Mutex>{
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

#ifdef _MSC_VER
	FunctionPtr make_function(check_function<bool()> f, int priority){
		return std::make_shared<Function>(priority, [f](const void*){return f();}, of::priv::make_function_id());
	}

	FunctionPtr make_function(check_function<bool(const void*)> f, int priority){
		return std::make_shared<Function>(priority, f, of::priv::make_function_id());
	}

	FunctionPtr make_function(check_function<void()> f, int priority){
		return std::make_shared<Function>(priority, [f](const void*){f(); return false;}, of::priv::make_function_id());
	}

	FunctionPtr make_function(check_function<void(const void*)> f, int priority){
		return std::make_shared<Function>(priority, [f](const void*s){f(s); return false;}, of::priv::make_function_id());
	}
#else
	FunctionPtr make_function(std::function<bool()> f, int priority) {
		return std::make_shared<Function>(priority, [f](const void*) {return f(); }, of::priv::make_function_id());
	}

	FunctionPtr make_function(std::function<bool(const void*)> f, int priority) {
		return std::make_shared<Function>(priority, f, of::priv::make_function_id());
	}

	FunctionPtr make_function(std::function<void()> f, int priority) {
		return std::make_shared<Function>(priority, [f](const void*) {f(); return false; }, of::priv::make_function_id());
	}

	FunctionPtr make_function(std::function<void(const void*)> f, int priority) {
		return std::make_shared<Function>(priority, [f](const void*s) {f(s); return false; }, of::priv::make_function_id());
	}

	using of::priv::BaseEvent<of::priv::Function<void,Mutex>,Mutex>::removeFunction;
	using of::priv::BaseEvent<of::priv::Function<void,Mutex>,Mutex>::addFunction;
	using of::priv::BaseEvent<of::priv::Function<void,Mutex>,Mutex>::addNoToken;
#endif

public:
	template<class TObj, typename TMethod>
	void add(TObj * listener, TMethod method, int priority){
		addNoToken(make_function(listener,method,priority));
	}

	template<class TObj, typename TMethod>
	ofEventListener newListener(TObj * listener, TMethod method, int priority = OF_EVENT_ORDER_AFTER_APP){
		return ofEventListener(addFunction(make_function(listener,method,priority)));
	}

	template<class TObj, typename TMethod>
	void remove(TObj * listener, TMethod method, int priority){
		removeFunction(make_function(listener,method,priority));
	}

	template<typename TFunction>
	void add(TFunction function, int priority){
		addNoToken(make_function(function,priority));
	}

	template<typename TFunction>
	ofEventListener newListener(TFunction function, int priority = OF_EVENT_ORDER_AFTER_APP) {
		return ofEventListener(addFunction(make_function(function, priority)));
	}

	template<typename TFunction>
	void remove(TFunction function, int priority){
		removeFunction(make_function(function,priority));
	}

	void notify(const void* sender){
		if(ofEvent<void,Mutex>::self->enabled && !ofEvent<void,Mutex>::self->functions.empty()){
			std::unique_lock<Mutex> lck(ofEvent<void,Mutex>::self->mtx);
			std::vector<std::shared_ptr<of::priv::Function<void,Mutex>>> functions_copy(ofEvent<void,Mutex>::self->functions);
			lck.unlock();
			for(auto & f: functions_copy){
				if(f->notify(sender)){
					throw ofEventAttendedException();
				}
			}
		}
	}

	void notify(){
		if(ofEvent<void,Mutex>::self->enabled && !ofEvent<void,Mutex>::self->functions.empty()){
			std::unique_lock<Mutex> lck(ofEvent<void,Mutex>::self->mtx);
			std::vector<std::shared_ptr<of::priv::Function<void,Mutex>>> functions_copy(ofEvent<void,Mutex>::self->functions);
			lck.unlock();
			for(auto & f: functions_copy){
				if(f->notify(nullptr)){
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
