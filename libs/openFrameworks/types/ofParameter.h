#pragma once

#include "ofEvents.h"
#include "ofTypes.h"
#include "ofUtils.h"
#include "ofConstants.h"
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
	virtual ~ofAbstractParameter(){};
	virtual string getName() const = 0;
	virtual void setName(const string & name) = 0;
	virtual string toString() const = 0;
	virtual void fromString(const string & str) = 0;

	virtual string type() const;
	virtual string getEscapedName() const;

	virtual void setParent(ofParameterGroup & _parent) = 0;
	vector<string> getGroupHierarchyNames() const;

	template<typename ParameterType>
	ofParameter<ParameterType> & cast(){
		return static_cast<ofParameter<ParameterType> &>(*this);
	}

	template<typename ParameterType>
	const ofParameter<ParameterType> & cast() const{
		return static_cast<const ofParameter<ParameterType> &>(*this);
	}

	friend ostream& operator<<(ostream& os, const ofAbstractParameter& p);
	friend istream& operator>>(istream& is, ofAbstractParameter& p);

	virtual bool isSerializable() const = 0;
	virtual shared_ptr<ofAbstractParameter> newReference() const = 0;

protected:
	virtual const ofParameterGroup getFirstParent() const = 0;
	virtual void setSerializable(bool serializable)=0;
	virtual string escape(const string& str) const;
};





//----------------------------------------------------------------------
/// A collection of parameters with events to notify if a parameter changed
/// and serialization facilities
class ofParameterGroup: public ofAbstractParameter {
public:
	ofParameterGroup();

	template<typename ...Args>
	ofParameterGroup(const string & name, Args&... p)
	:obj(new Value){
		add(p...);
		setName(name);
	}

	template<typename ...Args>
	void add(ofAbstractParameter & p, Args&... parameters){
		add(p);
		add(parameters...);
	}

	void add(ofAbstractParameter & param);


	void clear();

	ofParameter<bool> getBool(const string& name) const;
	ofParameter<int> getInt(const string& name) const;
	ofParameter<float> getFloat(const string& name) const;
	ofParameter<char> getChar(const string& name) const;
	ofParameter<string> getString(const string& name)	 const;
	ofParameter<ofPoint> getPoint(const string& name)	 const;
	ofParameter<ofVec2f> getVec2f(const string& name) const;
	ofParameter<ofVec3f> getVec3f(const string& name) const;
	ofParameter<ofVec4f> getVec4f(const string& name) const;
	ofParameter<ofColor> getColor(const string& name) const;
	ofParameter<ofShortColor> getShortColor(const string& name) const;
	ofParameter<ofFloatColor> getFloatColor(const string& name) const;

	ofParameterGroup getGroup(string name) const;


	ofParameter<bool> getBool(int pos) const;
	ofParameter<int> getInt(int pos) const;
	ofParameter<float> getFloat(int pos) const;
	ofParameter<char> getChar(int pos) const;
	ofParameter<string> getString(int pos)	 const;
	ofParameter<ofPoint> getPoint(int pos)	 const;
	ofParameter<ofVec2f> getVec2f(int pos) const;
	ofParameter<ofVec3f> getVec3f(int pos) const;
	ofParameter<ofVec4f> getVec4f(int pos) const;
	ofParameter<ofColor> getColor(int pose) const;
	ofParameter<ofShortColor> getShortColor(int pos) const;
	ofParameter<ofFloatColor> getFloatColor(int pos) const;
	ofParameterGroup getGroup(int pos) const;

	ofAbstractParameter & get(const string& name) const;
	ofAbstractParameter & get(int pos) const;

	ofAbstractParameter & operator[](const string& name) const;
	ofAbstractParameter & operator[](int pos) const;

	template<typename ParameterType>
	ofParameter<ParameterType> get(const string& name) const;

	template<typename ParameterType>
	ofParameter<ParameterType> get(int pos) const;

	int size() const;
	string getName(int position) const;
	string getType(int position) const;
	int getPosition(const string& name) const;

	friend ostream& operator<<(ostream& os, const ofParameterGroup& group);

	string getName() const;
	void setName(const string& name);
	string getEscapedName() const;
	string toString() const;
	void fromString(const string& name);

	bool contains(const string& name);

	ofAbstractParameter & back();
	ofAbstractParameter & front();
	const ofAbstractParameter & back() const;
	const ofAbstractParameter & front() const;

	void setSerializable(bool serializable);
	bool isSerializable() const;
	shared_ptr<ofAbstractParameter> newReference() const;

	void setParent(ofParameterGroup & parent);

	operator bool() const;

	ofEvent<ofAbstractParameter> & parameterChangedE();

	vector<shared_ptr<ofAbstractParameter> >::iterator begin();
	vector<shared_ptr<ofAbstractParameter> >::iterator end();
	vector<shared_ptr<ofAbstractParameter> >::const_iterator begin() const;
	vector<shared_ptr<ofAbstractParameter> >::const_iterator end() const;
	vector<shared_ptr<ofAbstractParameter> >::reverse_iterator rbegin();
	vector<shared_ptr<ofAbstractParameter> >::reverse_iterator rend();
	vector<shared_ptr<ofAbstractParameter> >::const_reverse_iterator rbegin() const;
	vector<shared_ptr<ofAbstractParameter> >::const_reverse_iterator rend() const;

private:
	class Value{
	public:
		Value()
		:serializable(true){}

		void notifyParameterChanged(ofAbstractParameter & param);

		map<string,int> parametersIndex;
		vector<shared_ptr<ofAbstractParameter> > parameters;
		string name;
		bool serializable;
		vector<weak_ptr<Value>> parents;
		ofEvent<ofAbstractParameter> parameterChangedE;
	};
	shared_ptr<Value> obj;
	ofParameterGroup(shared_ptr<Value> obj)
	:obj(obj){}

	template<typename T>
	friend class ofParameter;

	template<typename T, typename F>
	friend class ofReadOnlyParameter;

	const ofParameterGroup getFirstParent() const;
};

template<typename ParameterType>
ofParameter<ParameterType> ofParameterGroup::get(const string& name) const{
	return static_cast<ofParameter<ParameterType>& >(get(name));
}

template<typename ParameterType>
ofParameter<ParameterType> ofParameterGroup::get(int pos) const{
	return static_cast<ofParameter<ParameterType>& >(get(pos));
}




//----------------------------------------------------------------------
// Mechanism to provide min and max default values for types where it makes sense
template<typename T, bool B>
struct ofTypeInfo_ {
};

// Types with numeric_limits resolve to this template specialization:
template<typename T>
struct ofTypeInfo_<T, true> {
    static T min() { return std::numeric_limits<T>::is_signed ? -std::numeric_limits<T>::max() : 0; }
    static T max() { return std::numeric_limits<T>::max(); }
};

// Types without numeric_limits resolve to this template specialization:
template<typename T>
struct ofTypeInfo_<T, false> {
    static T min() { return T(); }
    static T max() { return T(); }
};

template<typename T>
struct ofTypeInfo : public ofTypeInfo_<T, std::numeric_limits<T>::is_specialized> {
};

// Here we provide some of our own specializations:
template<>
struct ofTypeInfo <ofVec2f> {
    static ofVec2f min() { return ofVec2f(-std::numeric_limits<float>::max()); };
    static ofVec2f max() { return ofVec3f(std::numeric_limits<float>::max()); };
};

template<>
struct ofTypeInfo <ofVec3f> {
    static ofVec3f min() { return ofVec3f(-std::numeric_limits<float>::max()); };
    static ofVec3f max() { return ofVec3f(std::numeric_limits<float>::max()); };
};

template<>
struct ofTypeInfo <ofVec4f> {
    static ofVec4f min() { return ofVec4f(-std::numeric_limits<float>::max()); };
    static ofVec4f max() { return ofVec4f(std::numeric_limits<float>::max()); };
};

template<typename T>
struct ofTypeInfo <ofColor_<T>> {
    static ofColor_<T> min() { return ofColor_<T>(0,0); };
    static ofColor_<T> max() { return ofColor_<T>(ofColor_<T>::limit(),ofColor_<T>::limit()); };
};





//----------------------------------------------------------------------
/// Holds a value and notify it's listeners when it changes. Can be used as
/// the value itself. For example an ofParameter<int> can be added, multiplied
/// substracted... with another number.
/// for ofParameter of other objects it's methods can be access using pointer
/// syntax ->
template<typename ParameterType>
class ofParameter: public ofAbstractParameter{
public:
	ofParameter();
	ofParameter(ParameterType v);
	ofParameter(const string& name, ParameterType v);
	ofParameter(const string& name, ParameterType v, ParameterType min, ParameterType max);

	const ParameterType & get() const;
	const ParameterType * operator->() const;
	operator const ParameterType & () const;

	void setName(const string & name);
	string getName() const;

	ParameterType getMin();

	ParameterType getMax();

	string toString() const;

	template<class ListenerClass, typename ListenerMethod>
	void addListener(ListenerClass * listener, ListenerMethod method){
		ofAddListener(obj->changedE,listener,method);
	}

	template<class ListenerClass, typename ListenerMethod>
	void removeListener(ListenerClass * listener, ListenerMethod method){
		ofRemoveListener(obj->changedE,listener,method);
	}

	void enableEvents();
	void disableEvents();
	bool isSerializable() const;

	void makeReferenceTo(ofParameter<ParameterType> mom);

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


	ofParameter<ParameterType> & set(ParameterType v);
	ofParameter<ParameterType> & set(const string& name, ParameterType value);
	ofParameter<ParameterType> & set(const string& name, ParameterType value, ParameterType min, ParameterType max);

	void setMin(ParameterType min);
	void setMax(ParameterType max);

	void fromString(const string & name);

	void setSerializable(bool serializable);
	shared_ptr<ofAbstractParameter> newReference() const;

	void setParent(ofParameterGroup & _parent);

	const ofParameterGroup getFirstParent() const{
		auto first = std::find_if(obj->parents.begin(),obj->parents.end(),[](weak_ptr<ofParameterGroup::Value> p){return p.lock()!=nullptr;});
		if(first!=obj->parents.end()){
			return first->lock();
		}else{
			return shared_ptr<ofParameterGroup::Value>(nullptr);
		}
	}
private:
	class Value{
	public:
		Value()
		:min(ofTypeInfo<ParameterType>::min())
		,max(ofTypeInfo<ParameterType>::min())
		,bInNotify(false)
		,serializable(true){};

		Value(ParameterType v)
		:value(v)
		,min(ofTypeInfo<ParameterType>::min())
		,max(ofTypeInfo<ParameterType>::min())
		,bInNotify(false)
		,serializable(true){};

		Value(string name, ParameterType v)
		:name(name)
		,min(ofTypeInfo<ParameterType>::min())
		,max(ofTypeInfo<ParameterType>::min())
		,value(v)
		,bInNotify(false)
		,serializable(true){};

		Value(string name, ParameterType v, ParameterType min, ParameterType max)
		:name(name)
		,value(v)
		,min(min)
		,max(max)
		,bInNotify(false)
		,serializable(true){};

		string name;
		ParameterType value, prevValue;
		ParameterType min, max;
		ofEvent<ParameterType> changedE;
		bool bInNotify;
		bool serializable;
		vector<weak_ptr<ofParameterGroup::Value>> parents;
	};
	shared_ptr<Value> obj;
	void (ofParameter<ParameterType>::*setMethod)(ParameterType v);

	void eventsSetValue(ParameterType v);
	void noEventsSetValue(ParameterType v);
};


template<typename ParameterType>
ofParameter<ParameterType>::ofParameter()
:obj(new Value)
,setMethod(&ofParameter<ParameterType>::eventsSetValue){}

template<typename ParameterType>
ofParameter<ParameterType>::ofParameter(ParameterType v)
:obj(shared_ptr<Value>(new Value(v)))
,setMethod(&ofParameter<ParameterType>::eventsSetValue) {}

template<typename ParameterType>
ofParameter<ParameterType>::ofParameter(const string& name, ParameterType v)
:obj(shared_ptr<Value>(new Value(name, v)))
,setMethod(&ofParameter<ParameterType>::eventsSetValue){}

template<typename ParameterType>
ofParameter<ParameterType>::ofParameter(const string& name, ParameterType v, ParameterType min, ParameterType max)
:obj(shared_ptr<Value>(new Value(name, v, min, max)))
,setMethod(&ofParameter<ParameterType>::eventsSetValue){}


template<typename ParameterType>
ofParameter<ParameterType> & ofParameter<ParameterType>::operator=(const ofParameter<ParameterType> & v){
	set(v);
	return *this;
}

template<typename ParameterType>
inline const ParameterType & ofParameter<ParameterType>::operator=(const ParameterType & v){
	set(v);
	return obj->value;
}

template<typename ParameterType>
inline ofParameter<ParameterType> & ofParameter<ParameterType>::set(ParameterType v){
	(this->*setMethod)(v);
	return *this;
}

template<typename ParameterType>
ofParameter<ParameterType> & ofParameter<ParameterType>::set(const string& name, ParameterType value, ParameterType min, ParameterType max){
	setName(name);
	set(value);
	setMin(min);
	setMax(max);
	return *this;
}

template<typename ParameterType>
ofParameter<ParameterType> & ofParameter<ParameterType>::set(const string& name, ParameterType value){
	setName(name);
	set(value);
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
inline void ofParameter<ParameterType>::eventsSetValue(ParameterType v){
	if(obj->bInNotify) {
		noEventsSetValue(v);
		return;
	}
	obj->bInNotify = true;
	obj->value = v;
	ofNotifyEvent(obj->changedE,obj->value,this);
	obj->parents.erase(std::remove_if(obj->parents.begin(),obj->parents.end(),[this](weak_ptr<ofParameterGroup::Value> p){
		auto parent = p.lock();
		if(parent) parent->notifyParameterChanged(*this);
		return !parent;
	}),obj->parents.end());
	obj->bInNotify = false;
}

template<typename ParameterType>
inline void ofParameter<ParameterType>::noEventsSetValue(ParameterType v){
	obj->value = v;
}


template<typename ParameterType>
void ofParameter<ParameterType>::setSerializable(bool serializable){
	obj->serializable = serializable;
}

template<typename ParameterType>
bool ofParameter<ParameterType>::isSerializable() const{
	return obj->serializable;
}

template<typename ParameterType>
void ofParameter<ParameterType>::setMin(ParameterType min){
	obj->min = min;
}

template<typename ParameterType>
ParameterType ofParameter<ParameterType>::getMin(){
	return obj->min;
}

template<typename ParameterType>
void ofParameter<ParameterType>::setMax(ParameterType max){
	obj->max = max;
}

template<typename ParameterType>
ParameterType ofParameter<ParameterType>::getMax(){
	return obj->max;
}

template<typename ParameterType>
inline ofParameter<ParameterType>::operator const ParameterType & () const{
	return obj->value;
}

template<typename ParameterType>
void ofParameter<ParameterType>::setName(const string & name){
	obj->name = name;
}

template<typename ParameterType>
string ofParameter<ParameterType>::getName() const{
	return obj->name;
}

template<typename ParameterType>
string ofParameter<ParameterType>::toString() const{
	return ofToString(obj->value);
}


template<typename ParameterType>
void ofParameter<ParameterType>::fromString(const string & str){
	set(ofFromString<ParameterType>(str));
}


template<typename ParameterType>
void ofParameter<ParameterType>::enableEvents(){
	setMethod = &ofParameter<ParameterType>::eventsSetValue;
}

template<typename ParameterType>
void ofParameter<ParameterType>::disableEvents(){
	setMethod = &ofParameter<ParameterType>::noEventsSetValue;
}

template<typename ParameterType>
ParameterType ofParameter<ParameterType>::operator++(int v){
	ParameterType r = obj->value;
	obj->value++;
	set(obj->value);
	return r;
}

template<typename ParameterType>
ofParameter<ParameterType> & ofParameter<ParameterType>::operator++(){
	++obj->value;
	set(obj->value);
	return *this;
}

template<typename ParameterType>
ParameterType ofParameter<ParameterType>::operator--(int v){
	ParameterType r = obj->value;
	obj->value--;
	set(obj->value);
	return r;
}

template<typename ParameterType>
ofParameter<ParameterType> & ofParameter<ParameterType>::operator--(){
	--obj->value;
	set(obj->value);
	return *this;
}

template<typename ParameterType>
template<typename OtherType>
ofParameter<ParameterType> & ofParameter<ParameterType>::operator+=(const OtherType & v){
	obj->value+=v;
	set(obj->value);
	return *this;
}

template<typename ParameterType>
template<typename OtherType>
ofParameter<ParameterType> & ofParameter<ParameterType>::operator-=(const OtherType & v){
	obj->value-=v;
	set(obj->value);
	return *this;
}

template<typename ParameterType>
template<typename OtherType>
ofParameter<ParameterType> & ofParameter<ParameterType>::operator*=(const OtherType & v){
	obj->value*=v;
	set(obj->value);
	return *this;
}

template<typename ParameterType>
template<typename OtherType>
ofParameter<ParameterType> & ofParameter<ParameterType>::operator/=(const OtherType & v){
	obj->value/=v;
	set(obj->value);
	return *this;
}

template<typename ParameterType>
template<typename OtherType>
ofParameter<ParameterType> & ofParameter<ParameterType>::operator%=(const OtherType & v){
	obj->value%=v;
	set(obj->value);
	return *this;
}

template<typename ParameterType>
template<typename OtherType>
ofParameter<ParameterType> & ofParameter<ParameterType>::operator&=(const OtherType & v){
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
ofParameter<ParameterType> & ofParameter<ParameterType>::operator^=(const OtherType & v){
	obj->value^=v;
	set(obj->value);
	return *this;
}

template<typename ParameterType>
template<typename OtherType>
ofParameter<ParameterType> & ofParameter<ParameterType>::operator<<=(const OtherType & v){
	obj->value<<=v;
	set(obj->value);
	return *this;
}

template<typename ParameterType>
template<typename OtherType>
ofParameter<ParameterType> & ofParameter<ParameterType>::operator>>=(const OtherType & v){
	obj->value>>=v;
	set(obj->value);
	return *this;
}

template<typename ParameterType>
void ofParameter<ParameterType>::makeReferenceTo(ofParameter<ParameterType> mom){
	obj = mom.obj;
}

template<typename ParameterType>
shared_ptr<ofAbstractParameter> ofParameter<ParameterType>::newReference() const{
	return shared_ptr<ofAbstractParameter>(new ofParameter<ParameterType>(*this));
}

template<typename ParameterType>
void ofParameter<ParameterType>::setParent(ofParameterGroup & parent){
	obj->parents.emplace_back(parent.obj);
}






//----------------------------------------------------------------------
/// Same as ofParameter but can only be modified by a friend class specified
/// as the second template argument
template<typename ParameterType,typename Friend>
class ofReadOnlyParameter: public ofAbstractParameter{
public:
	ofReadOnlyParameter();
	ofReadOnlyParameter(ofParameter<ParameterType> & p);
	ofReadOnlyParameter(ParameterType v);
	ofReadOnlyParameter(const string& name, ParameterType v);
	ofReadOnlyParameter(const string& name, ParameterType v, ParameterType min, ParameterType max);

	const ParameterType & get() const;
	const ParameterType * operator->() const;
	operator const ParameterType & () const;

	string getName() const;

	ParameterType getMin();

	ParameterType getMax();

	string toString() const;

	template<class ListenerClass, typename ListenerMethod>
	void addListener(ListenerClass * listener, ListenerMethod method);

	template<class ListenerClass, typename ListenerMethod>
	void removeListener(ListenerClass * listener, ListenerMethod method);
	shared_ptr<ofAbstractParameter> newReference() const;

protected:
	void setName(const string & name);
	void enableEvents();
	void disableEvents();
	bool isSerializable() const;
	void setSerializable(bool s);

	void makeReferenceTo(ofReadOnlyParameter<ParameterType,Friend> mom);
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


	ofReadOnlyParameter<ParameterType,Friend>& set(ParameterType v);
	
	ofReadOnlyParameter<ParameterType,Friend>& set(const string& name, ParameterType value);
	ofReadOnlyParameter<ParameterType,Friend>& set(const string& name, ParameterType value, ParameterType min, ParameterType max);

	void setMin(ParameterType min);
	void setMax(ParameterType max);

	void fromString(const string & str);

	void setParent(ofParameterGroup & _parent);

	const ofParameterGroup getFirstParent() const{
		return parameter.getFirstParent();
	}


	ofParameter<ParameterType> parameter;
	
	friend class ofParameterGroup;
};


template<typename ParameterType,typename Friend>
inline ofReadOnlyParameter<ParameterType,Friend>::ofReadOnlyParameter(){}

template<typename ParameterType,typename Friend>
inline ofReadOnlyParameter<ParameterType,Friend>::ofReadOnlyParameter(ofParameter<ParameterType> & p)
:parameter(p){}

template<typename ParameterType,typename Friend>
inline ofReadOnlyParameter<ParameterType,Friend>::ofReadOnlyParameter(ParameterType v)
:parameter(v){}

template<typename ParameterType,typename Friend>
inline ofReadOnlyParameter<ParameterType,Friend>::ofReadOnlyParameter(const string& name, ParameterType v)
:parameter(name,v){}

template<typename ParameterType,typename Friend>
inline ofReadOnlyParameter<ParameterType,Friend>::ofReadOnlyParameter(const string& name, ParameterType v, ParameterType min, ParameterType max)
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
inline string ofReadOnlyParameter<ParameterType,Friend>::getName() const{
	return parameter.getName();
}


template<typename ParameterType,typename Friend>
inline ParameterType ofReadOnlyParameter<ParameterType,Friend>::getMin(){
	return parameter.getMin();
}


template<typename ParameterType,typename Friend>
inline ParameterType ofReadOnlyParameter<ParameterType,Friend>::getMax(){
	return parameter.getMax();
}


template<typename ParameterType,typename Friend>
inline string ofReadOnlyParameter<ParameterType,Friend>::toString() const{
	return parameter.toString();
}


template<typename ParameterType,typename Friend>
template<class ListenerClass, typename ListenerMethod>
inline void ofReadOnlyParameter<ParameterType,Friend>::addListener(ListenerClass * listener, ListenerMethod method){
	parameter.addListener(listener,method);
}


template<typename ParameterType,typename Friend>
template<class ListenerClass, typename ListenerMethod>
inline void ofReadOnlyParameter<ParameterType,Friend>::removeListener(ListenerClass * listener, ListenerMethod method){
	parameter.removeListener(listener,method);
}


template<typename ParameterType,typename Friend>
inline void ofReadOnlyParameter<ParameterType,Friend>::setName(const string & name){
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
inline void ofReadOnlyParameter<ParameterType,Friend>::setSerializable(bool s){
	parameter.setSerializable(s);
}

template<typename ParameterType,typename Friend>
inline void ofReadOnlyParameter<ParameterType,Friend>::makeReferenceTo(ofReadOnlyParameter<ParameterType,Friend> mom){
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
inline ParameterType ofReadOnlyParameter<ParameterType,Friend>::operator++(int v){
	return parameter++;
}

template<typename ParameterType,typename Friend>
inline ofReadOnlyParameter<ParameterType,Friend> & ofReadOnlyParameter<ParameterType,Friend>::operator++(){
	return ++parameter;
}


template<typename ParameterType,typename Friend>
inline ParameterType ofReadOnlyParameter<ParameterType,Friend>::operator--(int v){
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
inline ofReadOnlyParameter<ParameterType,Friend> & ofReadOnlyParameter<ParameterType,Friend>::set(ParameterType v){
	parameter.set(v);
	return *this;
}

template<typename ParameterType,typename Friend>
inline ofReadOnlyParameter<ParameterType,Friend> & ofReadOnlyParameter<ParameterType,Friend>::set(const string& name, ParameterType value){
	parameter.set(name,value);
	return *this;
}

template<typename ParameterType,typename Friend>
inline ofReadOnlyParameter<ParameterType,Friend> & ofReadOnlyParameter<ParameterType,Friend>::set(const string& name, ParameterType value, ParameterType min, ParameterType max){
	parameter.set(name,value,min,max);
	return *this;
}


template<typename ParameterType,typename Friend>
inline void ofReadOnlyParameter<ParameterType,Friend>::setMin(ParameterType min){
	parameter.setMin(min);
}

template<typename ParameterType,typename Friend>
inline void ofReadOnlyParameter<ParameterType,Friend>::setMax(ParameterType max){
	parameter.setMax(max);
}


template<typename ParameterType,typename Friend>
inline void ofReadOnlyParameter<ParameterType,Friend>::fromString(const string & str){
	parameter.fromString(str);
}

template<typename ParameterType,typename Friend>
shared_ptr<ofAbstractParameter> ofReadOnlyParameter<ParameterType,Friend>::newReference() const{
	return shared_ptr<ofAbstractParameter>(new ofParameter<ParameterType>(*this));
}

template<typename ParameterType,typename Friend>
void ofReadOnlyParameter<ParameterType,Friend>::setParent(ofParameterGroup & _parent){
	parameter.setParent(_parent);
}
