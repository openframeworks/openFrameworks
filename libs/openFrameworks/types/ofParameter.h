#pragma once

#include "ofEvents.h"
#include "ofConstants.h"
#include "ofPoint.h"
#include "ofRectangle.h"
#include "ofColor.h"
#include "ofLog.h"
#include <map>

template<typename ParameterType>
class ofParameter;

template<typename ParameterType, typename Friend>
class ofReadOnlyParameter;

class ofParameterGroup;



//----------------------------------------------------------------------
/// Base class for ofParameter, ofReadOnlyParameter and ofParameterGroup
class ofAbstractParameter{
public:
	virtual ~ofAbstractParameter(){}
	virtual std::string getName() const = 0;
	virtual void setName(const std::string & name) = 0;
	virtual std::string toString() const = 0;
	virtual void fromString(const std::string & str) = 0;

	virtual std::string type() const;
	virtual std::string getEscapedName() const;
	virtual std::string valueType() const = 0;

	virtual void setParent(ofParameterGroup & _parent) = 0;
	std::vector<std::string> getGroupHierarchyNames() const;

	template<typename ParameterType>
	ofParameter<ParameterType> & cast(){
		return static_cast<ofParameter<ParameterType> &>(*this);
	}

	template<typename ParameterType>
	const ofParameter<ParameterType> & cast() const{
		return static_cast<const ofParameter<ParameterType> &>(*this);
	}

	template<typename ParameterType, typename Friend>
	ofReadOnlyParameter<ParameterType, Friend> & castReadOnly(){
		return static_cast<ofReadOnlyParameter<ParameterType, Friend> &>(*this);
	}

	template<typename ParameterType, typename Friend>
	const ofReadOnlyParameter<ParameterType, Friend> & castReadOnly() const{
		return static_cast<const ofReadOnlyParameter<ParameterType, Friend> &>(*this);
	}

	ofParameterGroup & castGroup();
	const ofParameterGroup & castGroup() const;

	friend std::ostream& operator<<(std::ostream& os, const ofAbstractParameter& p);
	friend std::istream& operator>>(std::istream& is, ofAbstractParameter& p);

	virtual bool isSerializable() const = 0;
	virtual bool isReadOnly() const = 0;
	virtual std::shared_ptr<ofAbstractParameter> newReference() const = 0;

	virtual bool isReferenceTo(const ofAbstractParameter& other) const;

protected:
	virtual const ofParameterGroup getFirstParent() const = 0;
	virtual void setSerializable(bool serializable)=0;
	virtual std::string escape(const std::string& str) const;
	virtual const void* getInternalObject() const = 0;
};




//----------------------------------------------------------------------
/// A collection of parameters with events to notify if a parameter changed
/// and serialization facilities
class ofParameterGroup: public ofAbstractParameter {
public:
	ofParameterGroup();

	template<typename ...Args>
	ofParameterGroup(const std::string & name)
	:obj(std::make_shared<Value>()){
		setName(name);
	}

	template<typename ...Args>
	ofParameterGroup(const std::string & name, Args&... p)
	:obj(std::make_shared<Value>()){
		add(p...);
		setName(name);
	}

	template<typename ...Args>
	void add(ofAbstractParameter & p, Args&... parameters){
		add(p);
		add(parameters...);
	}

	void add(ofAbstractParameter & param);
	std::string valueType() const;

	void remove(ofAbstractParameter & param);
	void remove(std::size_t index);
	void remove(const std::string& name);

	void clear();

	const ofParameter<void> & getVoid(const std::string& name) const;
	const ofParameter<bool> & getBool(const std::string& name) const;
	const ofParameter<int> & getInt(const std::string& name) const;
	const ofParameter<float> & getFloat(const std::string& name) const;
	const ofParameter<char> & getChar(const std::string& name) const;
	const ofParameter<std::string> & getString(const std::string& name) const;
	const ofParameter<ofPoint> & getPoint(const std::string& name) const;
	const ofParameter<ofDefaultVec2> & getVec2f(const std::string& name) const;
	const ofParameter<ofDefaultVec3> & getVec3f(const std::string& name) const;
	const ofParameter<ofDefaultVec4> & getVec4f(const std::string& name) const;
	const ofParameter<ofColor> & getColor(const std::string& name) const;
	const ofParameter<ofShortColor> & getShortColor(const std::string& name) const;
	const ofParameter<ofFloatColor> & getFloatColor(const std::string& name) const;
	const ofParameter<ofRectangle> & getRectangle(const std::string& name) const;
	const ofParameterGroup & getGroup(const std::string& name) const;


	const ofParameter<void> & getVoid(std::size_t pos) const;
	const ofParameter<bool> & getBool(std::size_t pos) const;
	const ofParameter<int> & getInt(std::size_t pos) const;
	const ofParameter<float> & getFloat(std::size_t pos) const;
	const ofParameter<char> & getChar(std::size_t pos) const;
	const ofParameter<std::string> & getString(std::size_t pos) const;
	const ofParameter<ofPoint> & getPoint(std::size_t pos) const;
	const ofParameter<ofDefaultVec2> & getVec2f(std::size_t pos) const;
	const ofParameter<ofDefaultVec3> & getVec3f(std::size_t pos) const;
	const ofParameter<ofDefaultVec4> & getVec4f(std::size_t pos) const;
	const ofParameter<ofColor> & getColor(std::size_t pose) const;
	const ofParameter<ofShortColor> & getShortColor(std::size_t pos) const;
	const ofParameter<ofFloatColor> & getFloatColor(std::size_t pos) const;
	const ofParameter<ofRectangle> & getRectangle(std::size_t pos) const;
	const ofParameterGroup & getGroup(std::size_t pos) const;

	ofParameter<void> & getVoid(const std::string& name);
	ofParameter<bool> & getBool(const std::string& name);
	ofParameter<int> & getInt(const std::string& name);
	ofParameter<float> & getFloat(const std::string& name);
	ofParameter<char> & getChar(const std::string& name);
	ofParameter<std::string> & getString(const std::string& name);
	ofParameter<ofPoint> & getPoint(const std::string& name);
	ofParameter<ofDefaultVec2> & getVec2f(const std::string& name);
	ofParameter<ofDefaultVec3> & getVec3f(const std::string& name);
	ofParameter<ofDefaultVec4> & getVec4f(const std::string& name);
	ofParameter<ofColor> & getColor(const std::string& name);
	ofParameter<ofShortColor> & getShortColor(const std::string& name);
	ofParameter<ofFloatColor> & getFloatColor(const std::string& name);
	ofParameter<ofRectangle> & getRectangle(const std::string& name);
	ofParameterGroup & getGroup(const std::string& name);


	ofParameter<void> & getVoid(std::size_t pos);
	ofParameter<bool> & getBool(std::size_t pos);
	ofParameter<int> & getInt(std::size_t pos);
	ofParameter<float> & getFloat(std::size_t pos);
	ofParameter<char> & getChar(std::size_t pos);
	ofParameter<std::string> & getString(std::size_t pos);
	ofParameter<ofPoint> & getPoint(std::size_t pos);
	ofParameter<ofDefaultVec2> & getVec2f(std::size_t pos);
	ofParameter<ofDefaultVec3> & getVec3f(std::size_t pos);
	ofParameter<ofDefaultVec4> & getVec4f(std::size_t pos);
	ofParameter<ofColor> & getColor(std::size_t pose);
	ofParameter<ofShortColor> & getShortColor(std::size_t pos);
	ofParameter<ofFloatColor> & getFloatColor(std::size_t pos);
	ofParameter<ofRectangle> & getRectangle(std::size_t pos);
	ofParameterGroup & getGroup(std::size_t pos);

	const ofAbstractParameter & get(const std::string& name) const;
	const ofAbstractParameter & get(std::size_t pos) const;

	const ofAbstractParameter & operator[](const std::string& name) const;
	const ofAbstractParameter & operator[](std::size_t pos) const;

	ofAbstractParameter & get(const std::string& name);
	ofAbstractParameter & get(std::size_t pos);

	ofAbstractParameter & operator[](const std::string& name);
	ofAbstractParameter & operator[](std::size_t pos);

	template<typename ParameterType>
	const ofParameter<ParameterType> & get(const std::string& name) const;

	template<typename ParameterType>
	const ofParameter<ParameterType> & get(std::size_t pos) const;

	template<typename ParameterType>
	ofParameter<ParameterType> & get(const std::string& name);

	template<typename ParameterType>
	ofParameter<ParameterType> & get(std::size_t pos);

	template<typename ParameterType, typename Friend>
	const ofReadOnlyParameter<ParameterType, Friend> & getReadOnly(const std::string& name) const;

	template<typename ParameterType, typename Friend>
	const ofReadOnlyParameter<ParameterType, Friend> & getReadOnly(std::size_t pos) const;

	template<typename ParameterType, typename Friend>
	ofReadOnlyParameter<ParameterType, Friend> & getReadOnly(const std::string& name);

	template<typename ParameterType, typename Friend>
	ofReadOnlyParameter<ParameterType, Friend> & getReadOnly(std::size_t pos);

	std::size_t size() const;
	std::string getName(std::size_t position) const;
	std::string getType(std::size_t position) const;
	bool getIsReadOnly(int position) const;
	int getPosition(const std::string& name) const;

	friend std::ostream& operator<<(std::ostream& os, const ofParameterGroup& group);

	std::string getName() const;
	void setName(const std::string& name);
	std::string getEscapedName() const;
	std::string toString() const;
	void fromString(const std::string& name);

	bool contains(const std::string& name) const;

	ofAbstractParameter & back();
	ofAbstractParameter & front();
	const ofAbstractParameter & back() const;
	const ofAbstractParameter & front() const;

	void setSerializable(bool serializable);
	bool isSerializable() const;
	bool isReadOnly() const;
	std::shared_ptr<ofAbstractParameter> newReference() const;

	void setParent(ofParameterGroup & parent);

	operator bool() const;

	ofEvent<ofAbstractParameter> & parameterChangedE();

	std::vector<std::shared_ptr<ofAbstractParameter> >::iterator begin();
	std::vector<std::shared_ptr<ofAbstractParameter> >::iterator end();
	std::vector<std::shared_ptr<ofAbstractParameter> >::const_iterator begin() const;
	std::vector<std::shared_ptr<ofAbstractParameter> >::const_iterator end() const;
	std::vector<std::shared_ptr<ofAbstractParameter> >::reverse_iterator rbegin();
	std::vector<std::shared_ptr<ofAbstractParameter> >::reverse_iterator rend();
	std::vector<std::shared_ptr<ofAbstractParameter> >::const_reverse_iterator rbegin() const;
	std::vector<std::shared_ptr<ofAbstractParameter> >::const_reverse_iterator rend() const;

protected:
	const void* getInternalObject() const;

private:
	class Value{
	public:
		Value()
		:serializable(true){}

		void notifyParameterChanged(ofAbstractParameter & param);

		std::map<std::string,std::size_t> parametersIndex;
		std::vector<std::shared_ptr<ofAbstractParameter> > parameters;
		std::string name;
		bool serializable;
		std::vector<std::weak_ptr<Value>> parents;
		ofEvent<ofAbstractParameter> parameterChangedE;
	};
	std::shared_ptr<Value> obj;
	ofParameterGroup(std::shared_ptr<Value> obj)
	:obj(obj){}

	template<typename T>
	friend class ofParameter;

	template<typename T, typename F>
	friend class ofReadOnlyParameter;

	const ofParameterGroup getFirstParent() const;
};

template<typename ParameterType>
const ofParameter<ParameterType> & ofParameterGroup::get(const std::string& name) const{
	return static_cast<const ofParameter<ParameterType>& >(get(name));
}

template<typename ParameterType>
const ofParameter<ParameterType> & ofParameterGroup::get(std::size_t pos) const{
	return static_cast<const ofParameter<ParameterType>& >(get(pos));
}

template<typename ParameterType>
ofParameter<ParameterType> & ofParameterGroup::get(const std::string& name){
	return static_cast<ofParameter<ParameterType>& >(get(name));
}

template<typename ParameterType>
ofParameter<ParameterType> & ofParameterGroup::get(std::size_t pos){
	return static_cast<ofParameter<ParameterType>& >(get(pos));
}


template<typename ParameterType, typename Friend>
const ofReadOnlyParameter<ParameterType, Friend> & ofParameterGroup::getReadOnly(const std::string& name) const{
	return static_cast<const ofReadOnlyParameter<ParameterType,Friend>& >(get(name));
}

template<typename ParameterType, typename Friend>
const ofReadOnlyParameter<ParameterType, Friend> & ofParameterGroup::getReadOnly(std::size_t pos) const{
	return static_cast<const ofReadOnlyParameter<ParameterType,Friend>& >(get(pos));
}

template<typename ParameterType, typename Friend>
ofReadOnlyParameter<ParameterType, Friend> & ofParameterGroup::getReadOnly(const std::string& name){
	return static_cast<const ofReadOnlyParameter<ParameterType,Friend>& >(get(name));
}

template<typename ParameterType, typename Friend>
ofReadOnlyParameter<ParameterType, Friend> & ofParameterGroup::getReadOnly(std::size_t pos){
	return static_cast<const ofReadOnlyParameter<ParameterType,Friend>& >(get(pos));
}


/*! \cond PRIVATE */
namespace of{
namespace priv{
	//----------------------------------------------------------------------
	// Mechanism to provide min and max default values for types where it makes sense
	template<typename T, bool B>
	struct TypeInfo_ {
	};

	// Types with numeric_limits resolve to this template specialization:
	template<typename T>
	struct TypeInfo_<T, true> {
		static T min() { return std::numeric_limits<T>::lowest(); }
		static T max() { return std::numeric_limits<T>::max(); }
	};

	template<>
	struct TypeInfo_<float, true> {
		static float min() { return 0; }
		static float max() { return 1; }
	};

	template<>
	struct TypeInfo_<double, true> {
		static float min() { return 0; }
		static float max() { return 1; }
	};

	// Types without numeric_limits resolve to this template specialization:
	template<typename T>
	struct TypeInfo_<T, false> {
		static T min() { return T(); }
		static T max() { return T(); }
	};

	template<typename T>
	struct TypeInfo : public of::priv::TypeInfo_<T, std::numeric_limits<T>::is_specialized> {
	};

	// Here we provide some of our own specializations:
	template<>
	struct TypeInfo <ofVec2f> {
		static ofVec2f min() { return ofVec2f(0); }
		static ofVec2f max() { return ofVec2f(1); }
	};

	template<>
	struct TypeInfo <glm::vec2> {
		static glm::vec2 min() { return glm::vec2(0); }
		static glm::vec2 max() { return glm::vec2(1); }
	};

	template<>
	struct TypeInfo <ofVec3f> {
		static ofVec3f min() { return ofVec3f(0); }
		static ofVec3f max() { return ofVec3f(1); }
	};

	template<>
	struct TypeInfo <glm::vec3> {
		static glm::vec3 min() { return glm::vec3(0); }
		static glm::vec3 max() { return glm::vec3(1); }
	};

	template<>
	struct TypeInfo <ofVec4f> {
		static ofVec4f min() { return ofVec4f(0); }
		static ofVec4f max() { return ofVec4f(1); }
	};

	template<>
	struct TypeInfo <glm::vec4> {
		static glm::vec4 min() { return glm::vec4(0); }
		static glm::vec4 max() { return glm::vec4(1); }
	};

	template<typename T>
	struct TypeInfo <ofColor_<T>> {
		static ofColor_<T> min() { return ofColor_<T>(0,0); }
		static ofColor_<T> max() { return ofColor_<T>(ofColor_<T>::limit(),ofColor_<T>::limit()); }
	};
	template<>
	struct TypeInfo <ofRectangle> {
	//Not really sure what would make sense here!!!
		static ofRectangle min() { return ofRectangle(0,0,0,0); }
		static ofRectangle max() { return ofRectangle(0,0,1,1); }
	};


	// detection of stream operators
	typedef char yes;
	typedef char (&no)[2];

	struct anyx { template <class T> anyx(const T &); };

	no operator << (const anyx &, const anyx &);
	no operator >> (const anyx &, const anyx &);

	
	template <class T> yes check_op(T const&);
	no check_op(no);

	template <typename T>
	struct has_loading_support {
		static std::istream & stream;
		static T & x;
		static constexpr bool value = sizeof(check_op(stream >> x)) == sizeof(yes);
	};

	template <typename T>
	struct has_saving_support {
		static std::ostream & stream;
		static T & x;
		static constexpr bool value = sizeof(check_op(stream << x)) == sizeof(yes);
	};

	template <typename T>
	struct has_stream_operators {
		static constexpr bool can_load = has_loading_support<T>::value;
		static constexpr bool can_save = has_saving_support<T>::value;
		static constexpr bool value = can_load && can_save;
	};

	template<typename ParameterType>
	typename std::enable_if<of::priv::has_saving_support<ParameterType>::value, std::string>::type toStringImpl(const ParameterType & value){
		return ofToString(value);
	}

	template<typename ParameterType>
	typename std::enable_if<!of::priv::has_saving_support<ParameterType>::value, std::string>::type toStringImpl(const ParameterType &){
		throw std::exception();
	}

	template<typename ParameterType>
	typename std::enable_if<of::priv::has_loading_support<ParameterType>::value, ParameterType>::type fromStringImpl(const std::string & str){
		return ofFromString<ParameterType>(str);
	}

	template<typename ParameterType>
	typename std::enable_if<!of::priv::has_loading_support<ParameterType>::value, ParameterType>::type fromStringImpl(const std::string &){
		throw std::exception();

	}
}
}
/*! \endcond */



/// \brief ofParameter holds a value and notifies its listeners when it changes.
///
/// ofParameter can be used as the value itself. For example an `ofParameter<int>`
/// can be added, multiplied, substracted, etc with another number.
///
/// For an ofParameter with a custom object such as `ofParameter<MyObject> myObject`,
/// `MyObject`'s methods can be accessed using pointer syntax,
/// e.g. `myObject->myMethod();`.
///
/// \tparam ParameterType The data wrapped by the ofParameter.
template<typename ParameterType>
class ofParameter: public ofAbstractParameter{
public:
	ofParameter();
	ofParameter(const ofParameter<ParameterType> & v);
	ofParameter(const ParameterType & v);
	ofParameter(const std::string& name, const ParameterType & v);
	ofParameter(const std::string& name, const ParameterType & v, const ParameterType & min, const ParameterType & max);

	const ParameterType & get() const;
	const ParameterType * operator->() const;
	operator const ParameterType & () const;

	void setName(const std::string & name);
	std::string getName() const;

	ParameterType getMin() const;

	ParameterType getMax() const;


	std::string toString() const;
	void fromString(const std::string & name);

	template<class ListenerClass, typename ListenerMethod>
	void addListener(ListenerClass * listener, ListenerMethod method, int prio=OF_EVENT_ORDER_AFTER_APP){
		ofAddListener(obj->changedE,listener,method,prio);
	}

	template<class ListenerClass, typename ListenerMethod>
	void removeListener(ListenerClass * listener, ListenerMethod method, int prio=OF_EVENT_ORDER_AFTER_APP){
		ofRemoveListener(obj->changedE,listener,method,prio);
	}

	template<typename... Args>
	std::unique_ptr<of::priv::AbstractEventToken> newListener(Args...args) {
		return obj->changedE.newListener(args...);
	}

	void enableEvents();
	void disableEvents();
	bool isSerializable() const;
	bool isReadOnly() const;
	std::string valueType() const;

	void makeReferenceTo(ofParameter<ParameterType> & mom);

	ofParameter<ParameterType> & operator=(const ofParameter<ParameterType> & v);
	const ParameterType & operator=(const ParameterType & v);

	ParameterType operator++(int v);
	ofParameter<ParameterType> & operator++();

	ParameterType operator--(int v);
	ofParameter<ParameterType> & operator--();

	template<typename OtherType>
	ofParameter<ParameterType> & operator+=(const OtherType & v);
	template<typename OtherType>
	ofParameter<ParameterType> & operator-=(const OtherType & v);
	template<typename OtherType>
	ofParameter<ParameterType> & operator*=(const OtherType & v);
	template<typename OtherType>
	ofParameter<ParameterType> & operator/=(const OtherType & v);
	template<typename OtherType>
	ofParameter<ParameterType> & operator%=(const OtherType & v);
	template<typename OtherType>
	ofParameter<ParameterType> & operator&=(const OtherType & v);
	template<typename OtherType>
	ofParameter<ParameterType> & operator|=(const OtherType & v);
	template<typename OtherType>
	ofParameter<ParameterType> & operator^=(const OtherType & v);
	template<typename OtherType>
	ofParameter<ParameterType> & operator<<=(const OtherType & v);
	template<typename OtherType>
	ofParameter<ParameterType> & operator>>=(const OtherType & v);


	ofParameter<ParameterType> & set(const ParameterType & v);
	ofParameter<ParameterType> & set(const std::string& name, const ParameterType & v);
	ofParameter<ParameterType> & set(const std::string& name, const ParameterType & v, const ParameterType & min, const ParameterType & max);

	ofParameter<ParameterType> & setWithoutEventNotifications(const ParameterType & v);

	void setMin(const ParameterType & min);
	void setMax(const ParameterType & max);

	void setSerializable(bool serializable);
	std::shared_ptr<ofAbstractParameter> newReference() const;

	void setParent(ofParameterGroup & _parent);

	const ofParameterGroup getFirstParent() const{
		obj->parents.erase(std::remove_if(obj->parents.begin(),obj->parents.end(),
						   [](std::weak_ptr<ofParameterGroup::Value> p){return p.lock()==nullptr;}),
						obj->parents.end());
		if(!obj->parents.empty()){
			return obj->parents.front().lock();
		}else{
			return std::shared_ptr<ofParameterGroup::Value>(nullptr);
		}
	}

	size_t getNumListeners() const;
	const void* getInternalObject() const;

protected:

private:
	class Value{
	public:
		Value()
		:min(of::priv::TypeInfo<ParameterType>::min())
		,max(of::priv::TypeInfo<ParameterType>::max())
		,bInNotify(false)
		,serializable(true){}

		Value(ParameterType v)
		:value(v)
		,min(of::priv::TypeInfo<ParameterType>::min())
		,max(of::priv::TypeInfo<ParameterType>::max())
		,bInNotify(false)
		,serializable(true){}

		Value(std::string name, ParameterType v)
		:name(name)
		,value(v)
		,min(of::priv::TypeInfo<ParameterType>::min())
		,max(of::priv::TypeInfo<ParameterType>::max())
		,bInNotify(false)
		,serializable(true){}

		Value(std::string name, ParameterType v, ParameterType min, ParameterType max)
		:name(name)
		,value(v)
		,min(min)
		,max(max)
		,bInNotify(false)
		,serializable(true){}

		std::string name;
		ParameterType value;
		ParameterType min, max;
		ofEvent<ParameterType> changedE;
		bool bInNotify;
		bool serializable;
		std::vector<std::weak_ptr<ofParameterGroup::Value>> parents;
	};

	std::shared_ptr<Value> obj;
	std::function<void(const ParameterType & v)> setMethod;

	void eventsSetValue(const ParameterType & v);
	void noEventsSetValue(const ParameterType & v);

	template<typename T, typename F>
	friend class ofReadOnlyParameter;
};


template<typename ParameterType>
ofParameter<ParameterType>::ofParameter()
:obj(std::make_shared<Value>())
,setMethod(std::bind(&ofParameter<ParameterType>::eventsSetValue, this, std::placeholders::_1)){}

template<typename ParameterType>
ofParameter<ParameterType>::ofParameter(const ofParameter<ParameterType> & v)
:obj(v.obj)
,setMethod(std::bind(&ofParameter<ParameterType>::eventsSetValue, this, std::placeholders::_1)) {}

template<typename ParameterType>
ofParameter<ParameterType>::ofParameter(const ParameterType & v)
:obj(std::make_shared<Value>(v))
,setMethod(std::bind(&ofParameter<ParameterType>::eventsSetValue, this, std::placeholders::_1)) {}

template<typename ParameterType>
ofParameter<ParameterType>::ofParameter(const std::string& name, const ParameterType & v)
:obj(std::make_shared<Value>(name, v))
,setMethod(std::bind(&ofParameter<ParameterType>::eventsSetValue, this, std::placeholders::_1)){}

template<typename ParameterType>
ofParameter<ParameterType>::ofParameter(const std::string& name, const ParameterType & v, const ParameterType & min, const ParameterType & max)
:obj(std::make_shared<Value>(name, v, min, max))
,setMethod(std::bind(&ofParameter<ParameterType>::eventsSetValue, this, std::placeholders::_1)){}


template<typename ParameterType>
inline ofParameter<ParameterType> & ofParameter<ParameterType>::operator=(const ofParameter<ParameterType> & v){
	set(v);
	return *this;
}

template<typename ParameterType>
inline const ParameterType & ofParameter<ParameterType>::operator=(const ParameterType & v){
	set(v);
	return obj->value;
}

template<typename ParameterType>
inline ofParameter<ParameterType> & ofParameter<ParameterType>::set(const ParameterType & v){
	setMethod(v);
	return *this;
}

template<typename ParameterType>
ofParameter<ParameterType> & ofParameter<ParameterType>::set(const std::string& name, const ParameterType & value, const ParameterType & min, const ParameterType & max){
	setName(name);
	set(value);
	setMin(min);
	setMax(max);
	return *this;
}

template<typename ParameterType>
ofParameter<ParameterType> & ofParameter<ParameterType>::set(const std::string& name, const ParameterType & value){
	setName(name);
	set(value);
	return *this;
}

template<typename ParameterType>
inline ofParameter<ParameterType> & ofParameter<ParameterType>::setWithoutEventNotifications(const ParameterType & v){
	noEventsSetValue(v);
	return *this;
}

template<typename ParameterType>
inline const ParameterType & ofParameter<ParameterType>::get() const{
	return obj->value;
}
template<typename ParameterType>
inline const ParameterType * ofParameter<ParameterType>::operator->() const{
	return &obj->value;
}

template<typename ParameterType>
inline void ofParameter<ParameterType>::eventsSetValue(const ParameterType & v){

	// If the object is notifying its parents, just set the value without triggering an event.
	if(obj->bInNotify)
	{
		noEventsSetValue(v);
	}
	else
	{
		// Mark the object as in its notification loop.
		obj->bInNotify = true;

		// Set the value.
		obj->value = v;

		// Notify any local subscribers.
		ofNotifyEvent(obj->changedE,obj->value,this);

		// Notify all parents, if there are any.
		if(!obj->parents.empty())
		{
			// Erase each invalid parent
			obj->parents.erase(std::remove_if(obj->parents.begin(),
											  obj->parents.end(),
											  [](const std::weak_ptr<ofParameterGroup::Value> & p){ return p.expired(); }),
							   obj->parents.end());

			// notify all leftover (valid) parents of this object's changed value.
			// this can't happen in the same iterator as above, because a notified listener
			// might perform similar cleanups that would corrupt our iterator
			// (which appens for example if the listener calls getFirstParent on us)
			for(auto & parent: obj->parents){
				auto p = parent.lock();
				if(p){
					p->notifyParameterChanged(*this);
				}
			}
		}
		obj->bInNotify = false;
	}
}

template<typename ParameterType>
inline void ofParameter<ParameterType>::noEventsSetValue(const ParameterType & v){
	obj->value = v;
}


template<typename ParameterType>
void ofParameter<ParameterType>::setSerializable(bool serializable){
	obj->serializable = serializable;
}

template<typename ParameterType>
bool ofParameter<ParameterType>::isSerializable() const{
	return of::priv::has_stream_operators<ParameterType>::value && obj->serializable;
}

template<typename ParameterType>
bool ofParameter<ParameterType>::isReadOnly() const{
	return false;
}

template<typename ParameterType>
std::string ofParameter<ParameterType>::valueType() const{
	return typeid(ParameterType).name();
}

template<typename ParameterType>
void ofParameter<ParameterType>::setMin(const ParameterType & min){
	obj->min = min;
}

template<typename ParameterType>
ParameterType ofParameter<ParameterType>::getMin() const {
	return obj->min;
}

template<typename ParameterType>
void ofParameter<ParameterType>::setMax(const ParameterType & max){
	obj->max = max;
}

template<typename ParameterType>
ParameterType ofParameter<ParameterType>::getMax() const {
	return obj->max;
}

template<typename ParameterType>
inline ofParameter<ParameterType>::operator const ParameterType & () const{
	return obj->value;
}

template<typename ParameterType>
void ofParameter<ParameterType>::setName(const std::string & name){
	obj->name = name;
}

template<typename ParameterType>
std::string ofParameter<ParameterType>::getName() const{
	return obj->name;
}

template<typename ParameterType>
inline std::string ofParameter<ParameterType>::toString() const{
	try{
		return of::priv::toStringImpl(obj->value);
	}catch(...){
		ofLogError("ofParameter") << "Trying to serialize non-serializable parameter";
		return "";
	}
}

template<typename ParameterType>
inline void ofParameter<ParameterType>::fromString(const std::string & str){
	try{
		set(of::priv::fromStringImpl<ParameterType>(str));
	}catch(...){
		ofLogError("ofParameter") << "Trying to de-serialize non-serializable parameter";
	}
}

template<typename ParameterType>
void ofParameter<ParameterType>::enableEvents(){
	setMethod = std::bind(&ofParameter<ParameterType>::eventsSetValue, this, std::placeholders::_1);
}

template<typename ParameterType>
void ofParameter<ParameterType>::disableEvents(){
	setMethod = std::bind(&ofParameter<ParameterType>::noEventsSetValue, this, std::placeholders::_1);
}

template<typename ParameterType>
inline ParameterType ofParameter<ParameterType>::operator++(int){
	ParameterType r = obj->value;
	obj->value++;
	set(obj->value);
	return r;
}

template<typename ParameterType>
inline ofParameter<ParameterType> & ofParameter<ParameterType>::operator++(){
	++obj->value;
	set(obj->value);
	return *this;
}

template<typename ParameterType>
inline ParameterType ofParameter<ParameterType>::operator--(int){
	ParameterType r = obj->value;
	obj->value--;
	set(obj->value);
	return r;
}

template<typename ParameterType>
inline ofParameter<ParameterType> & ofParameter<ParameterType>::operator--(){
	--obj->value;
	set(obj->value);
	return *this;
}

template<typename ParameterType>
template<typename OtherType>
inline ofParameter<ParameterType> & ofParameter<ParameterType>::operator+=(const OtherType & v){
	obj->value+=v;
	set(obj->value);
	return *this;
}

template<typename ParameterType>
template<typename OtherType>
inline ofParameter<ParameterType> & ofParameter<ParameterType>::operator-=(const OtherType & v){
	obj->value-=v;
	set(obj->value);
	return *this;
}

template<typename ParameterType>
template<typename OtherType>
inline ofParameter<ParameterType> & ofParameter<ParameterType>::operator*=(const OtherType & v){
	obj->value*=v;
	set(obj->value);
	return *this;
}

template<typename ParameterType>
template<typename OtherType>
inline ofParameter<ParameterType> & ofParameter<ParameterType>::operator/=(const OtherType & v){
	obj->value/=v;
	set(obj->value);
	return *this;
}

template<typename ParameterType>
template<typename OtherType>
inline ofParameter<ParameterType> & ofParameter<ParameterType>::operator%=(const OtherType & v){
	obj->value%=v;
	set(obj->value);
	return *this;
}

template<typename ParameterType>
template<typename OtherType>
inline ofParameter<ParameterType> & ofParameter<ParameterType>::operator&=(const OtherType & v){
	obj->value&=v;
	set(obj->value);
	return *this;
}

template<typename ParameterType>
template<typename OtherType>
ofParameter<ParameterType> & ofParameter<ParameterType>::operator|=(const OtherType & v){
	obj->value|=v;
	set(obj->value);
	return *this;
}

template<typename ParameterType>
template<typename OtherType>
inline ofParameter<ParameterType> & ofParameter<ParameterType>::operator^=(const OtherType & v){
	obj->value^=v;
	set(obj->value);
	return *this;
}

template<typename ParameterType>
template<typename OtherType>
inline ofParameter<ParameterType> & ofParameter<ParameterType>::operator<<=(const OtherType & v){
	obj->value<<=v;
	set(obj->value);
	return *this;
}

template<typename ParameterType>
template<typename OtherType>
inline ofParameter<ParameterType> & ofParameter<ParameterType>::operator>>=(const OtherType & v){
	obj->value>>=v;
	set(obj->value);
	return *this;
}

template<typename ParameterType>
void ofParameter<ParameterType>::makeReferenceTo(ofParameter<ParameterType> & mom){
	obj = mom.obj;
}

template<typename ParameterType>
std::shared_ptr<ofAbstractParameter> ofParameter<ParameterType>::newReference() const{
	return std::make_shared<ofParameter<ParameterType>>(*this);
}

template<typename ParameterType>
void ofParameter<ParameterType>::setParent(ofParameterGroup & parent){
	obj->parents.emplace_back(parent.obj);
}

template<typename ParameterType>
size_t ofParameter<ParameterType>::getNumListeners() const{
	return obj->changedE.size();
}

template<typename ParameterType>
const void* ofParameter<ParameterType>::getInternalObject() const{
	return obj.get();
}

template<>
class ofParameter<void>: public ofAbstractParameter{
public:
	ofParameter();
	ofParameter(const std::string& name);

	ofParameter<void>& set(const std::string & name);

	void setName(const std::string & name);
	std::string getName() const;

	std::string toString() const;
	void fromString(const std::string & name);

	template<class ListenerClass, typename ListenerMethod>
	void addListener(ListenerClass * listener, ListenerMethod method, int prio=OF_EVENT_ORDER_AFTER_APP){
		ofAddListener(obj->changedE,listener,method,prio);
	}

	template<class ListenerClass, typename ListenerMethod>
	void removeListener(ListenerClass * listener, ListenerMethod method, int prio=OF_EVENT_ORDER_AFTER_APP){
		ofRemoveListener(obj->changedE,listener,method,prio);
	}

	template<typename... Args>
	std::unique_ptr<of::priv::AbstractEventToken> newListener(Args...args) {
		return obj->changedE.newListener(args...);
	}

	void trigger();
	void trigger(const void * sender);

	void enableEvents();
	void disableEvents();
	bool isSerializable() const;
	bool isReadOnly() const;
	std::string valueType() const;

	void makeReferenceTo(ofParameter<void> & mom);

	void setSerializable(bool serializable);
	std::shared_ptr<ofAbstractParameter> newReference() const;

	void setParent(ofParameterGroup & _parent);

	const ofParameterGroup getFirstParent() const{
		auto first = std::find_if(obj->parents.begin(),obj->parents.end(),[](std::weak_ptr<ofParameterGroup::Value> p){return p.lock()!=nullptr;});
		if(first!=obj->parents.end()){
			return first->lock();
		}else{
			return std::shared_ptr<ofParameterGroup::Value>(nullptr);
		}
	}
	size_t getNumListeners() const;

	const void* getInternalObject() const{
		return obj.get();
	}
protected:

private:
	class Value{
	public:
		Value()
		:serializable(false){}

		Value(std::string name)
		:name(name)
		,serializable(false){}

		std::string name;
		ofEvent<void> changedE;
		bool serializable;
		std::vector<std::weak_ptr<ofParameterGroup::Value>> parents;
	};
	std::shared_ptr<Value> obj;
};



/// \brief ofReadOnlyParameter holds a value and notifies its listeners when it changes.
///
/// ofReadOnlyParameter is a "read only" version of `ofPareameter`.  "Friend"
/// classes specified in the template arguments allow other classes
/// write-access to the internal data.  Otherwise, all other access is
/// "read only".
///
/// \sa ofParameter
/// \tparam ParameterType The data wrapped by the ofParameter.
/// \tparam Friend The type of the "friend" class with write access.
template<typename ParameterType,typename Friend>
class ofReadOnlyParameter: public ofAbstractParameter{
public:
	ofReadOnlyParameter();
//	ofReadOnlyParameter(ofParameter<ParameterType> & p);
//	ofReadOnlyParameter(ofReadOnlyParameter<ParameterType,Friend> & p);
	ofReadOnlyParameter(const ParameterType & v);
	ofReadOnlyParameter(const std::string& name, const ParameterType & v);
	ofReadOnlyParameter(const std::string& name, const ParameterType & v, const ParameterType & min, const ParameterType & max);

	const ParameterType & get() const;
	const ParameterType * operator->() const;
	operator const ParameterType & () const;

	std::string getName() const;

	ParameterType getMin() const;

	ParameterType getMax() const;

	std::string toString() const;

	template<class ListenerClass, typename ListenerMethod>
	void addListener(ListenerClass * listener, ListenerMethod method, int prio=OF_EVENT_ORDER_AFTER_APP);

	template<class ListenerClass, typename ListenerMethod>
	void removeListener(ListenerClass * listener, ListenerMethod method, int prio=OF_EVENT_ORDER_AFTER_APP);

	std::shared_ptr<ofAbstractParameter> newReference() const;

	template<typename... Args>
	std::unique_ptr<of::priv::AbstractEventToken> newListener(Args...args);

	bool isSerializable() const;
	bool isReadOnly() const;
	std::string valueType() const;

protected:
	void setName(const std::string & name);
	void enableEvents();
	void disableEvents();
	void setSerializable(bool s);

	template<typename OtherFriend>
	void makeReferenceTo(ofReadOnlyParameter<ParameterType,OtherFriend> mom);
	void makeReferenceTo(ofParameter<ParameterType> mom);

	ofReadOnlyParameter<ParameterType,Friend> & operator=(const ofReadOnlyParameter<ParameterType,Friend>& v);
	ofReadOnlyParameter<ParameterType,Friend> & operator=(const ofParameter<ParameterType>& v);
	const ParameterType & operator=(const ParameterType & v);

	ParameterType operator++(int v);
	ofReadOnlyParameter<ParameterType,Friend>& operator++();

	ParameterType operator--(int v);
	ofReadOnlyParameter<ParameterType,Friend>& operator--();

	template<typename OtherType>
	ofReadOnlyParameter<ParameterType,Friend>& operator+=(const OtherType & v);
	template<typename OtherType>
	ofReadOnlyParameter<ParameterType,Friend>& operator-=(const OtherType & v);
	template<typename OtherType>
	ofReadOnlyParameter<ParameterType,Friend>& operator*=(const OtherType & v);
	template<typename OtherType>
	ofReadOnlyParameter<ParameterType,Friend>& operator/=(const OtherType & v);
	template<typename OtherType>
	ofReadOnlyParameter<ParameterType,Friend>& operator%=(const OtherType & v);
	template<typename OtherType>
	ofReadOnlyParameter<ParameterType,Friend>& operator&=(const OtherType & v);
	template<typename OtherType>
	ofReadOnlyParameter<ParameterType,Friend>& operator|=(const OtherType & v);
	template<typename OtherType>
	ofReadOnlyParameter<ParameterType,Friend>& operator^=(const OtherType & v);
	template<typename OtherType>
	ofReadOnlyParameter<ParameterType,Friend>& operator<<=(const OtherType & v);
	template<typename OtherType>
	ofReadOnlyParameter<ParameterType,Friend>& operator>>=(const OtherType & v);


	ofReadOnlyParameter<ParameterType,Friend>& set(const ParameterType & v);
	
	ofReadOnlyParameter<ParameterType,Friend>& set(const std::string& name, const ParameterType & value);
	ofReadOnlyParameter<ParameterType,Friend>& set(const std::string& name, const ParameterType & value, const ParameterType & min, const ParameterType & max);

	void setMin(const ParameterType & min);
	void setMax(const ParameterType & max);

	void fromString(const std::string & str);

	void setParent(ofParameterGroup & _parent);

	const ofParameterGroup getFirstParent() const{
		return parameter.getFirstParent();
	}

	const void* getInternalObject() const{
		return parameter.getInternalObject();
	}

	ofParameter<ParameterType> parameter;

	template<typename T>
	friend class ofParameter;
	friend class ofParameterGroup;
	friend Friend;
	template<typename T, typename OtherFriend>
	friend class ofReadOnlyParameter;
};


template<typename ParameterType,typename Friend>
inline ofReadOnlyParameter<ParameterType,Friend>::ofReadOnlyParameter(){}

//template<typename ParameterType,typename Friend>
//inline ofReadOnlyParameter<ParameterType,Friend>::ofReadOnlyParameter(ofParameter<ParameterType> & p)
//:parameter(p){}

//template<typename ParameterType,typename Friend>
//inline ofReadOnlyParameter<ParameterType,Friend>::ofReadOnlyParameter(ofReadOnlyParameter<ParameterType,Friend> & p)
//:parameter(p){}

template<typename ParameterType,typename Friend>
inline ofReadOnlyParameter<ParameterType,Friend>::ofReadOnlyParameter(const ParameterType & v)
:parameter(v){}

template<typename ParameterType,typename Friend>
inline ofReadOnlyParameter<ParameterType,Friend>::ofReadOnlyParameter(const std::string& name, const ParameterType & v)
:parameter(name,v){}

template<typename ParameterType,typename Friend>
inline ofReadOnlyParameter<ParameterType,Friend>::ofReadOnlyParameter(const std::string& name, const ParameterType & v, const ParameterType & min, const ParameterType & max)
:parameter(name,v,min,max){}


template<typename ParameterType,typename Friend>
inline const ParameterType & ofReadOnlyParameter<ParameterType,Friend>::get() const{
	return parameter.get();
}

template<typename ParameterType,typename Friend>
inline const ParameterType * ofReadOnlyParameter<ParameterType,Friend>::operator->() const{
	return &parameter.get();
}

template<typename ParameterType,typename Friend>
inline ofReadOnlyParameter<ParameterType,Friend>::operator const ParameterType & () const{
	return parameter.get();
}


template<typename ParameterType,typename Friend>
inline std::string ofReadOnlyParameter<ParameterType,Friend>::getName() const{
	return parameter.getName();
}


template<typename ParameterType,typename Friend>
inline ParameterType ofReadOnlyParameter<ParameterType,Friend>::getMin() const{
	return parameter.getMin();
}


template<typename ParameterType,typename Friend>
inline ParameterType ofReadOnlyParameter<ParameterType,Friend>::getMax() const{
	return parameter.getMax();
}


template<typename ParameterType,typename Friend>
inline std::string ofReadOnlyParameter<ParameterType,Friend>::toString() const{
	return parameter.toString();
}

template<typename ParameterType,typename Friend>
std::string ofReadOnlyParameter<ParameterType,Friend>::valueType() const{
	return typeid(ParameterType).name();
}


template<typename ParameterType,typename Friend>
template<class ListenerClass, typename ListenerMethod>
inline void ofReadOnlyParameter<ParameterType,Friend>::addListener(ListenerClass * listener, ListenerMethod method, int prio){
	parameter.addListener(listener,method,prio);
}


template<typename ParameterType,typename Friend>
template<class ListenerClass, typename ListenerMethod>
inline void ofReadOnlyParameter<ParameterType,Friend>::removeListener(ListenerClass * listener, ListenerMethod method, int prio){
	parameter.removeListener(listener,method,prio);
}


template<typename ParameterType,typename Friend>
template<typename... Args>
inline std::unique_ptr<of::priv::AbstractEventToken> ofReadOnlyParameter<ParameterType,Friend>::newListener(Args...args) {
	return parameter.newListener(args...);
}

template<typename ParameterType,typename Friend>
inline void ofReadOnlyParameter<ParameterType,Friend>::setName(const std::string & name){
	parameter.setName(name);
}

template<typename ParameterType,typename Friend>
inline void ofReadOnlyParameter<ParameterType,Friend>::enableEvents(){
	parameter.enableEvents();
}

template<typename ParameterType,typename Friend>
inline void ofReadOnlyParameter<ParameterType,Friend>::disableEvents(){
	parameter.disableEvents();
}

template<typename ParameterType,typename Friend>
inline bool ofReadOnlyParameter<ParameterType,Friend>::isSerializable() const{
	return parameter.isSerializable();
}

template<typename ParameterType,typename Friend>
inline bool ofReadOnlyParameter<ParameterType,Friend>::isReadOnly() const{
	return true;
}

template<typename ParameterType,typename Friend>
inline void ofReadOnlyParameter<ParameterType,Friend>::setSerializable(bool s){
	parameter.setSerializable(s);
}

template<typename ParameterType,typename Friend>
template<typename OtherFriend>
inline void ofReadOnlyParameter<ParameterType,Friend>::makeReferenceTo(ofReadOnlyParameter<ParameterType,OtherFriend> mom){
	parameter.makeReferenceTo(mom.parameter);
}

template<typename ParameterType,typename Friend>
void ofReadOnlyParameter<ParameterType,Friend>::makeReferenceTo(ofParameter<ParameterType> mom){
	parameter.makeReferenceTo(mom);
}

template<typename ParameterType,typename Friend>
inline ofReadOnlyParameter<ParameterType,Friend> & ofReadOnlyParameter<ParameterType,Friend>::operator=(const ofReadOnlyParameter<ParameterType,Friend> & v){
	parameter = v.parameter;
	return *this;
}

template<typename ParameterType,typename Friend>
inline ofReadOnlyParameter<ParameterType,Friend> & ofReadOnlyParameter<ParameterType,Friend>::operator=(const ofParameter<ParameterType>& v){
	parameter = v;
	return *this;
}

template<typename ParameterType,typename Friend>
inline const ParameterType & ofReadOnlyParameter<ParameterType,Friend>::operator=(const ParameterType & v){
	parameter = v;
	return v;
}


template<typename ParameterType,typename Friend>
inline ParameterType ofReadOnlyParameter<ParameterType,Friend>::operator++(int){
	return parameter++;
}

template<typename ParameterType,typename Friend>
inline ofReadOnlyParameter<ParameterType,Friend> & ofReadOnlyParameter<ParameterType,Friend>::operator++(){
	return ++parameter;
}


template<typename ParameterType,typename Friend>
inline ParameterType ofReadOnlyParameter<ParameterType,Friend>::operator--(int){
	return parameter--;
}

template<typename ParameterType,typename Friend>
inline ofReadOnlyParameter<ParameterType,Friend> & ofReadOnlyParameter<ParameterType,Friend>::operator--(){
	return --parameter;
}


template<typename ParameterType,typename Friend>
template<typename OtherType>
inline ofReadOnlyParameter<ParameterType,Friend> & ofReadOnlyParameter<ParameterType,Friend>::operator+=(const OtherType & v){
	parameter+=v;
	return *this;
}

template<typename ParameterType,typename Friend>
template<typename OtherType>
inline ofReadOnlyParameter<ParameterType,Friend> & ofReadOnlyParameter<ParameterType,Friend>::operator-=(const OtherType & v){
	parameter-=v;
	return *this;
}

template<typename ParameterType,typename Friend>
template<typename OtherType>
inline ofReadOnlyParameter<ParameterType,Friend> & ofReadOnlyParameter<ParameterType,Friend>::operator*=(const OtherType & v){
	parameter*=v;
	return *this;
}

template<typename ParameterType,typename Friend>
template<typename OtherType>
inline ofReadOnlyParameter<ParameterType,Friend> & ofReadOnlyParameter<ParameterType,Friend>::operator/=(const OtherType & v){
	parameter/=v;
	return *this;
}

template<typename ParameterType,typename Friend>
template<typename OtherType>
inline ofReadOnlyParameter<ParameterType,Friend> & ofReadOnlyParameter<ParameterType,Friend>::operator%=(const OtherType & v){
	parameter%=v;
	return *this;
}

template<typename ParameterType,typename Friend>
template<typename OtherType>
inline ofReadOnlyParameter<ParameterType,Friend> & ofReadOnlyParameter<ParameterType,Friend>::operator&=(const OtherType & v){
	parameter&=v;
	return *this;
}

template<typename ParameterType,typename Friend>
template<typename OtherType>
inline ofReadOnlyParameter<ParameterType,Friend> & ofReadOnlyParameter<ParameterType,Friend>::operator|=(const OtherType & v){
	parameter|=v;
	return *this;
}

template<typename ParameterType,typename Friend>
template<typename OtherType>
inline ofReadOnlyParameter<ParameterType,Friend> & ofReadOnlyParameter<ParameterType,Friend>::operator^=(const OtherType & v){
	parameter^=v;
	return *this;
}

template<typename ParameterType,typename Friend>
template<typename OtherType>
inline ofReadOnlyParameter<ParameterType,Friend> & ofReadOnlyParameter<ParameterType,Friend>::operator<<=(const OtherType & v){
	parameter<<=v;
	return *this;
}

template<typename ParameterType,typename Friend>
template<typename OtherType>
inline ofReadOnlyParameter<ParameterType,Friend> & ofReadOnlyParameter<ParameterType,Friend>::operator>>=(const OtherType & v){
	parameter>>=v;
	return *this;
}



template<typename ParameterType,typename Friend>
inline ofReadOnlyParameter<ParameterType,Friend> & ofReadOnlyParameter<ParameterType,Friend>::set(const ParameterType & v){
	parameter.set(v);
	return *this;
}

template<typename ParameterType,typename Friend>
inline ofReadOnlyParameter<ParameterType,Friend> & ofReadOnlyParameter<ParameterType,Friend>::set(const std::string& name, const ParameterType & value){
	parameter.set(name,value);
	return *this;
}

template<typename ParameterType,typename Friend>
inline ofReadOnlyParameter<ParameterType,Friend> & ofReadOnlyParameter<ParameterType,Friend>::set(const std::string& name, const ParameterType & value, const ParameterType & min, const ParameterType & max){
	parameter.set(name,value,min,max);
	return *this;
}


template<typename ParameterType,typename Friend>
inline void ofReadOnlyParameter<ParameterType,Friend>::setMin(const ParameterType & min){
	parameter.setMin(min);
}

template<typename ParameterType,typename Friend>
inline void ofReadOnlyParameter<ParameterType,Friend>::setMax(const ParameterType & max){
	parameter.setMax(max);
}


template<typename ParameterType,typename Friend>
inline void ofReadOnlyParameter<ParameterType,Friend>::fromString(const std::string & str){
	parameter.fromString(str);
}

template<typename ParameterType,typename Friend>
std::shared_ptr<ofAbstractParameter> ofReadOnlyParameter<ParameterType,Friend>::newReference() const{
	return std::make_shared<ofReadOnlyParameter<ParameterType,Friend>>(*this);
}

template<typename ParameterType,typename Friend>
void ofReadOnlyParameter<ParameterType,Friend>::setParent(ofParameterGroup & _parent){
	parameter.setParent(_parent);
}
