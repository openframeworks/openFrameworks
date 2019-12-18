#pragma once

#include <vector>
#include <functional>
#include <mutex>
#include <thread>
#include <memory>
#include <iterator>
#include <atomic>
#include <stddef.h>
#include <functional>
#include <deque>


/*! \cond PRIVATE */
namespace of{
namespace priv{
	// Helper classes and methods, only for internal use of ofEvent

	// -------------------------------------
	class NoopMutex{
	public:
		void lock(){}
		void unlock(){}
	};

	// -------------------------------------
	class AbstractEventToken{
		public:
			virtual ~AbstractEventToken();
	};


	// -------------------------------------
	class BaseFunctionId{
	public:
		BaseFunctionId(){}
		BaseFunctionId(const BaseFunctionId &) = delete;
		BaseFunctionId & operator=(const BaseFunctionId &) = delete;
		virtual ~BaseFunctionId();
		virtual bool operator==(const BaseFunctionId &) const = 0;
		virtual BaseFunctionId * clone() const = 0;
	};

	// -------------------------------------
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


	// -------------------------------------
	inline std::unique_ptr<StdFunctionId> make_function_id(){
		return std::make_unique<StdFunctionId>();
	}

	// -------------------------------------
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

	// -------------------------------------
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


	// -------------------------------------
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
			return std::make_unique<EventToken>(self,*f.id);
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
	};




	// -------------------------------------
	// Helper functions to disambiguate parameters
	// https://github.com/sth/callable.hpp
	namespace detail {

	/** Count the number of types given to the template */
	template<typename... Types>
	struct tva_count;

	template<>
	struct tva_count<> {
		static const size_t value = 0;
	};

	template<typename Type, typename... Types>
	struct tva_count<Type, Types...> {
		static const size_t value = tva_count<Types...>::value + 1;
	};


	/** Get the nth type given to the template */
	template<size_t n, typename... Types>
	struct tva_n;

	template<size_t N, typename Type, typename... Types>
	struct tva_n<N, Type, Types...> : tva_n<N-1, Types...> {
	};

	template<typename Type, typename... Types>
	struct tva_n<0, Type, Types...> {
		typedef Type type;
	};


	/** Define traits for a function type */
	template<typename Fun>
	struct callable_traits_fn;

	template<typename Ret, typename... Args>
	struct callable_traits_fn<Ret (Args...)> {
		typedef Ret (*function_ptr)(Args...);
		typedef Ret function_type(Args...);
		typedef Ret return_type;
		static const size_t argc;

		template<size_t N>
		using argument_type = typename tva_n<N, Args...>::type;
	};

	template<typename Ret, typename... Args>
	const size_t callable_traits_fn<Ret (Args...)>::argc = tva_count<Args...>::value;


	/** Define traits for a operator() member function pointer type */
	template<typename MemFun>
	struct callable_traits_memfn;

	template<typename Class, typename Ret, typename... Args>
	struct callable_traits_memfn<Ret (Class::*)(Args...)> : callable_traits_fn<Ret (Args...)> {
	};

	template<typename Class, typename Ret, typename... Args>
	struct callable_traits_memfn<Ret (Class::*)(Args...) const> : callable_traits_fn<Ret (Args...)> {
	};


	// classes with operator()
	template<typename Callable>
	struct callable_traits_d : detail::callable_traits_memfn<decltype(&Callable::operator())> {
	};

	// functions
	template<typename Ret, typename... Args>
	struct callable_traits_d<Ret (Args...)> : detail::callable_traits_fn<Ret (Args...)> {
	};

	// function pointers
	template<typename Ret, typename... Args>
	struct callable_traits_d<Ret (*)(Args...)> : detail::callable_traits_fn<Ret (Args...)> {
	};

	// std::function specializations
	template<typename Ret, typename... Args>
	struct callable_traits_d<std::function<Ret (Args...)>> : detail::callable_traits_fn<Ret (Args...)> {
	};

	} // namespace detail


	// Main template

	template<typename Callable>
	struct callable_traits : detail::callable_traits_d<typename std::remove_reference<Callable>::type> {
	};
}
}
/*! \endcond */

// -------------------------------------
enum ofEventOrder{
	OF_EVENT_ORDER_BEFORE_APP=0,
	OF_EVENT_ORDER_APP=100,
	OF_EVENT_ORDER_AFTER_APP=200
};

// -------------------------------------
class ofEventListener{
public:
	ofEventListener(){}
	ofEventListener(const ofEventListener &) = delete;
	ofEventListener(ofEventListener &&) = delete;
	ofEventListener & operator=(const ofEventListener&) = delete;
	ofEventListener & operator=(ofEventListener&&) = delete;

	ofEventListener(std::unique_ptr<of::priv::AbstractEventToken> && token)
		:token(std::move(token)){}

	ofEventListener & operator=(std::unique_ptr<of::priv::AbstractEventToken> && token){
		std::swap(this->token, token);
		return *this;
	}

	void unsubscribe(){
		token.reset();
	}
private:
	std::unique_ptr<of::priv::AbstractEventToken> token;
};



// -------------------------------------
class ofEventListeners{
public:
	ofEventListeners(){};
	ofEventListeners(const ofEventListeners &) = delete;
	ofEventListeners(ofEventListeners &&) = delete;
	ofEventListeners & operator=(const ofEventListeners&) = delete;
	ofEventListeners & operator=(ofEventListeners&&) = delete;


	void push(std::unique_ptr<of::priv::AbstractEventToken> && listener){
		listeners.emplace_back(std::move(listener));
	}

	OF_DEPRECATED_MSG("Don't use this method. If you need granular control over each listener, then use individual ofEventListener instances for each.", void unsubscribe(std::size_t pos));

	void unsubscribeAll(){
		listeners.clear();
	}

    bool empty() const {
        return listeners.size() == 0 ;
    }
private:
	std::deque<ofEventListener> listeners;
};

inline void ofEventListeners::unsubscribe(std::size_t pos){
	listeners[pos].unsubscribe();
}

// -------------------------------------
// ofEvent main implementation
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
		return std::make_unique<FunctionId<TObj,TMethod>>(listener,method);
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

	template<typename F>
	std::unique_ptr<of::priv::BaseFunctionId> make_std_function_id(const F & f){
		auto function = f.template target<typename of::priv::callable_traits<F>::function_ptr>();
		if(function){
			return make_function_id((ofEvent<T>*)nullptr,*function);
		}else{
			return of::priv::make_function_id();
		}
	}

	FunctionPtr make_function(std::function<bool(T&)> f, int priority) {
		return std::make_shared<Function>(priority, [f](const void*, T&t) {return f(t); }, make_std_function_id(f));
	}

	FunctionPtr make_function(std::function<bool(const void*, T&)> f, int priority) {
		return std::make_shared<Function>(priority, f, make_std_function_id(f));
	}

	FunctionPtr make_function(std::function<void(T&)> f, int priority) {
		return std::make_shared<Function>(priority, [f](const void*, T&t) {f(t); return false; }, make_std_function_id(f));
	}

	FunctionPtr make_function(std::function<void(const void*, T&)> f, int priority) {
		return std::make_shared<Function>(priority, [f](const void*s, T&t) {f(s, t); return false; }, make_std_function_id(f));
	}


	using of::priv::BaseEvent<of::priv::Function<T,Mutex>,Mutex>::addFunction;
	using of::priv::BaseEvent<of::priv::Function<T,Mutex>,Mutex>::addNoToken;

public:
	template<class TObj, typename TMethod>
	std::unique_ptr<of::priv::AbstractEventToken> newListener(TObj * listener, TMethod method, int priority = OF_EVENT_ORDER_AFTER_APP){
		return addFunction(make_function(listener,method,priority));
	}

	template<class TObj, typename TMethod>
	void add(TObj * listener, TMethod method, int priority){
		addNoToken(make_function(listener,method,priority));
	}

	template<class TObj, typename TMethod>
	void remove(TObj * listener, TMethod method, int priority){
		 ofEvent<T,Mutex>::self->remove(*make_function_id(listener,method));
	}

	template<typename TFunction>
	std::unique_ptr<of::priv::AbstractEventToken> newListener(TFunction function, int priority = OF_EVENT_ORDER_AFTER_APP) {
		return addFunction(make_function(std::function<typename of::priv::callable_traits<TFunction>::function_type>(function), priority));
	}

	template<typename TFunction>
	void add(TFunction function, int priority){
		addNoToken(make_function(std::function<typename of::priv::callable_traits<TFunction>::function_type>(function),priority));
	}

	template<typename TFunction>
	void remove(TFunction function, int priority){
		 ofEvent<T,Mutex>::self->remove(*make_function(std::function<typename of::priv::callable_traits<TFunction>::function_type>(function), priority)->id);
	}

	inline bool notify(const void* sender, T & param){
		if(ofEvent<T,Mutex>::self->enabled && !ofEvent<T,Mutex>::self->functions.empty()){
			std::unique_lock<Mutex> lck(ofEvent<T,Mutex>::self->mtx);
			auto functions_copy = ofEvent<T,Mutex>::self->functions;
			lck.unlock();
			for(auto & f: functions_copy){
                if(f->notify(sender,param)){
					return true;
                }
			}
		}
		return false;
	}

	inline bool notify(T & param){
		if(ofEvent<T,Mutex>::self->enabled && !ofEvent<T,Mutex>::self->functions.empty()){
			std::unique_lock<Mutex> lck(ofEvent<T,Mutex>::self->mtx);
			auto functions_copy = ofEvent<T,Mutex>::self->functions;
			lck.unlock();
			for(auto & f: functions_copy){
				if(f->notify(nullptr,param)){
					return true;
				}
			}
		}
		return false;
	}
};



// -------------------------------------
// void event template specialization,
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

	template<typename F>
	std::unique_ptr<of::priv::BaseFunctionId> make_std_function_id(const F & f){
		auto function = f.template target<typename of::priv::callable_traits<F>::function_ptr>();
		if(function){
			return make_function_id((ofEvent<void>*)nullptr,*function);
		}else{
			return of::priv::make_function_id();
		}
	}

	FunctionPtr make_function(std::function<bool()> f, int priority) {
		return std::make_shared<Function>(priority, [f](const void*) {return f(); }, make_std_function_id(f));
	}

	FunctionPtr make_function(std::function<bool(const void*)> f, int priority) {
		return std::make_shared<Function>(priority, f, make_std_function_id(f));
	}

	FunctionPtr make_function(std::function<void()> f, int priority) {
		return std::make_shared<Function>(priority, [f](const void*) {f(); return false; }, make_std_function_id(f));
	}

	FunctionPtr make_function(std::function<void(const void*)> f, int priority) {
		return std::make_shared<Function>(priority, [f](const void*s) {f(s); return false; }, make_std_function_id(f));
	}

	using of::priv::BaseEvent<of::priv::Function<void,Mutex>,Mutex>::addFunction;
	using of::priv::BaseEvent<of::priv::Function<void,Mutex>,Mutex>::addNoToken;

public:
	template<class TObj, typename TMethod>
	void add(TObj * listener, TMethod method, int priority){
		addNoToken(make_function(listener,method,priority));
	}

	template<class TObj, typename TMethod>
	std::unique_ptr<of::priv::AbstractEventToken> newListener(TObj * listener, TMethod method, int priority = OF_EVENT_ORDER_AFTER_APP){
		return addFunction(make_function(listener,method,priority));
	}

	template<class TObj, typename TMethod>
	void remove(TObj * listener, TMethod method, int priority){
		ofEvent<void,Mutex>::self->remove(*make_function_id(listener,method));
	}

	template<typename TFunction>
	void add(TFunction function, int priority){
		addNoToken(make_function(std::function<typename of::priv::callable_traits<TFunction>::function_type>(function),priority));
	}

	template<typename TFunction>
	std::unique_ptr<of::priv::AbstractEventToken> newListener(TFunction function, int priority = OF_EVENT_ORDER_AFTER_APP) {
		return addFunction(make_function(std::function<typename of::priv::callable_traits<TFunction>::function_type>(function), priority));
	}

	template<typename TFunction>
	void remove(TFunction function, int priority){
		 ofEvent<void,Mutex>::self->remove(*make_function(std::function<typename of::priv::callable_traits<TFunction>::function_type>(function),priority)->id);
	}

	bool notify(const void* sender){
		if(ofEvent<void,Mutex>::self->enabled && !ofEvent<void,Mutex>::self->functions.empty()){
			std::unique_lock<Mutex> lck(ofEvent<void,Mutex>::self->mtx);
			auto functions_copy = ofEvent<void,Mutex>::self->functions;
			lck.unlock();
			for(auto & f: functions_copy){
				if(f->notify(sender)){
					return true;
				}
			}
		}
		return false;
	}

	bool notify(){
		if(ofEvent<void,Mutex>::self->enabled && !ofEvent<void,Mutex>::self->functions.empty()){
			std::unique_lock<Mutex> lck(ofEvent<void,Mutex>::self->mtx);
			auto functions_copy = ofEvent<void,Mutex>::self->functions;
			lck.unlock();
			for(auto & f: functions_copy){
				if(f->notify(nullptr)){
					return true;
				}
			}
		}
		return false;
	}
};

// -------------------------------------
/// Non thread safe event that avoids locks and copies of the listeners
/// making it faster than a plain ofEvent
template<typename T>
class ofFastEvent: public ofEvent<T,of::priv::NoopMutex>{
public:
	inline bool notify(const void* sender, T & param){
		if(this->isEnabled()){
			for(auto & f: ofFastEvent<T>::self->functions){
				if(f->notify(sender, param)){
					return true;
				}
			}
		}
		return false;
	}
};

