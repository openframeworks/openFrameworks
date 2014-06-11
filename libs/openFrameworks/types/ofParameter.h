#pragma once

#include "ofEvents.h"
#include "ofTypes.h"
#include "ofUtils.h"

template<typename ParameterType>
class ofParameter;

template<typename ParameterType, typename Friend>
class ofReadOnlyParameter;

class ofParameterGroup;

class ofAbstractParameter{
public:
	ofAbstractParameter();
	virtual ~ofAbstractParameter();
	virtual string getName() const;
	virtual void setName(string name);
	virtual string toString() const;
	virtual void fromString(string str);
	virtual string type() const;

	virtual string getEscapedName() const;

	void setParent(ofParameterGroup * _parent);
	const ofParameterGroup * getParent() const;
	ofParameterGroup * getParent();
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

	virtual bool isSerializable() const;
	virtual shared_ptr<ofAbstractParameter> newReference() const;

protected:
	virtual void setSerializable(bool serializable);
	void notifyParent();
	virtual string escape(string str) const;
private:
	ofParameterGroup * parent;
};


template<typename ParameterType>
class ofParameter: public ofAbstractParameter{
public:
	ofParameter();
	ofParameter(ParameterType v);
	ofParameter(string name, ParameterType v);
	ofParameter(string name, ParameterType v, ParameterType min, ParameterType max);

	const ParameterType & get() const;
	const ParameterType * operator->() const;
	operator const ParameterType & () const;

	void setName(string name);
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
	ofParameter<ParameterType> & set(string name, ParameterType value);
	ofParameter<ParameterType> & set(string name, ParameterType value, ParameterType min, ParameterType max);

	void setMin(ParameterType min);
	void setMax(ParameterType max);

	void fromString(string str);

	void setSerializable(bool serializable);
	shared_ptr<ofAbstractParameter> newReference() const;
private:
	class Value{
	public:
		Value()
		:bInNotify(false)
		,serializable(true){};

		Value(ParameterType v)
		:value(v)
		,bInNotify(false)
		,serializable(true){};

		Value(string name, ParameterType v)
		:name(name)
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
ofParameter<ParameterType>::ofParameter(string name, ParameterType v)
:obj(shared_ptr<Value>(new Value(name, v)))
,setMethod(&ofParameter<ParameterType>::eventsSetValue){}

template<typename ParameterType>
ofParameter<ParameterType>::ofParameter(string name, ParameterType v, ParameterType min, ParameterType max)
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
ofParameter<ParameterType> & ofParameter<ParameterType>::set(string name, ParameterType value, ParameterType min, ParameterType max){
	setName(name);
	set(value);
	setMin(min);
	setMax(max);
	return *this;
}

template<typename ParameterType>
ofParameter<ParameterType> & ofParameter<ParameterType>::set(string name, ParameterType value){
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
	notifyParent();
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
void ofParameter<ParameterType>::setName(string _name){
	obj->name = _name;
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
void ofParameter<ParameterType>::fromString(string str){
	ParameterType v;
	stringstream sstr;
	sstr << str;
	sstr >> v;
	set(v);
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



template <typename T>
struct FriendMaker {typedef T Type;};

template<typename ParameterType,typename Friend>
class ofReadOnlyParameter: public ofAbstractParameter{
public:
	ofReadOnlyParameter();
	ofReadOnlyParameter(ofParameter<ParameterType> & p);
	ofReadOnlyParameter(ParameterType v);
	ofReadOnlyParameter(string name, ParameterType v);
	ofReadOnlyParameter(string name, ParameterType v, ParameterType min, ParameterType max);

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
	void setName(string name);
	void enableEvents();
	void disableEvents();
	bool isSerializable() const;

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
	
	ofReadOnlyParameter<ParameterType,Friend>& set(string name, ParameterType value);
	ofReadOnlyParameter<ParameterType,Friend>& set(string name, ParameterType value, ParameterType min, ParameterType max);

	void setMin(ParameterType min);
	void setMax(ParameterType max);

	void fromString(string str);


	ofParameter<ParameterType> parameter;
	
	friend class ofParameterGroup;
};


template<typename ParameterType,typename Friend>
inline ofReadOnlyParameter<ParameterType,Friend>::ofReadOnlyParameter(){};

template<typename ParameterType,typename Friend>
inline ofReadOnlyParameter<ParameterType,Friend>::ofReadOnlyParameter(ofParameter<ParameterType> & p)
:parameter(p){}

template<typename ParameterType,typename Friend>
inline ofReadOnlyParameter<ParameterType,Friend>::ofReadOnlyParameter(ParameterType v)
:parameter(v){};

template<typename ParameterType,typename Friend>
inline ofReadOnlyParameter<ParameterType,Friend>::ofReadOnlyParameter(string name, ParameterType v)
:parameter(name,v){};

template<typename ParameterType,typename Friend>
inline ofReadOnlyParameter<ParameterType,Friend>::ofReadOnlyParameter(string name, ParameterType v, ParameterType min, ParameterType max)
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
inline void ofReadOnlyParameter<ParameterType,Friend>::setName(string name){
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
inline ofReadOnlyParameter<ParameterType,Friend> & ofReadOnlyParameter<ParameterType,Friend>::set(string name, ParameterType value){
	parameter.set(name,value);
	return *this;
}

template<typename ParameterType,typename Friend>
inline ofReadOnlyParameter<ParameterType,Friend> & ofReadOnlyParameter<ParameterType,Friend>::set(string name, ParameterType value, ParameterType min, ParameterType max){
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
inline void ofReadOnlyParameter<ParameterType,Friend>::fromString(string str){
	parameter.fromString(str);
}

template<typename ParameterType,typename Friend>
shared_ptr<ofAbstractParameter> ofReadOnlyParameter<ParameterType,Friend>::newReference() const{
	return shared_ptr<ofAbstractParameter>(new ofParameter<ParameterType>(*this));
}
