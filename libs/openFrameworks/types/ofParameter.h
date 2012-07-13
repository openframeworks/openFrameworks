#pragma once

#include "ofEvents.h"
#include "ofTypes.h"

template<typename ParameterType>
class ofParameter;

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

protected:
	void notifyParent();

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
	virtual ~ofParameter(){};

	ParameterType operator=(ParameterType v);
	operator const ParameterType & () const;
	operator ParameterType & ();

	virtual ParameterType getValue() const;
	virtual void setValue(ParameterType v);

	void setName(string name);
	string getName() const;

	void setMin(ParameterType min);
	ParameterType getMin();

	void setMax(ParameterType max);
	ParameterType getMax();

	ofParameter<ParameterType> & set(string name, ParameterType value);
	ofParameter<ParameterType> & set(string name, ParameterType value, ParameterType min, ParameterType max);

	string toString() const;
	void fromString(string str);

	template<class ListenerClass>
	void addListener(ListenerClass * listener, void ( ListenerClass::*method )(ParameterType&)){
		ofAddListener(obj->changedE,listener,method);
	}

	template<class ListenerClass>
	void removeListener(ListenerClass * listener, void ( ListenerClass::*method )(ParameterType&)){
		ofRemoveListener(obj->changedE,listener,method);
	}

	template<class ListenerClass>
	void addListener(ListenerClass * listener, void ( ListenerClass::*method )(const void*, ParameterType&)){
		ofAddListener(obj->changedE,listener,method);
	}

	template<class ListenerClass>
	void removeListener(ListenerClass * listener, void ( ListenerClass::*method )(const void*, ParameterType&)){
		ofRemoveListener(obj->changedE,listener,method);
	}

private:
	class Value{
	public:
		Value()
		:bInNotify(false){};

		Value(ParameterType v)
		:value(v)
		,bInNotify(false){}

		Value(string name, ParameterType v)
		:name(name)
		,value(v)
		,bInNotify(false){}

		Value(string name, ParameterType v, ParameterType min, ParameterType max)
		:name(name)
		,value(v)
		,min(min)
		,max(max)
		,bInNotify(false){};

		string name;
		ParameterType value;
		ParameterType min, max;
		ofEvent<ParameterType> changedE;
		bool bInNotify;
	};
	ofPtr<Value> obj;
};


template<typename ParameterType>
ofParameter<ParameterType>::ofParameter(){
	obj = ofPtr<Value>(new Value);
}

template<typename ParameterType>
ofParameter<ParameterType>::ofParameter(ParameterType v)
:obj(ofPtr<Value>(new Value(v))){}

template<typename ParameterType>
ofParameter<ParameterType>::ofParameter(string name, ParameterType v)
:obj(ofPtr<Value>(new Value(name, v))){}

template<typename ParameterType>
ofParameter<ParameterType>::ofParameter(string name, ParameterType v, ParameterType min, ParameterType max)
:obj(ofPtr<Value>(new Value(name, v, min, max))){}


template<typename ParameterType>
ParameterType ofParameter<ParameterType>::operator=(ParameterType v){
	setValue(v);
	return obj->value;
}

template<typename ParameterType>
ofParameter<ParameterType> & ofParameter<ParameterType>::set(string name, ParameterType value, ParameterType min, ParameterType max){
	setName(name);
	setValue(value);
	setMin(min);
	setMax(max);
	return *this;
}

template<typename ParameterType>
ofParameter<ParameterType> & ofParameter<ParameterType>::set(string name, ParameterType value){
	setName(name);
	setValue(value);
	return *this;
}

template<typename ParameterType>
ParameterType ofParameter<ParameterType>::getValue() const{
	return obj->value;
}

template<typename ParameterType>
void ofParameter<ParameterType>::setValue(ParameterType v){
	if(obj->bInNotify) return;
	obj->bInNotify = true;
	obj->value = v;
	ofNotifyEvent(obj->changedE,v,this);
	notifyParent();
	obj->bInNotify = false;
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
ofParameter<ParameterType>::operator ParameterType & (){
	return obj->value;
}

template<typename ParameterType>
ofParameter<ParameterType>::operator const ParameterType & () const{
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
	stringstream sstr;
	sstr.str() = str;
	sstr >> obj->value;
}
